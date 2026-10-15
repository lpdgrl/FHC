#include "../../include/server/request_handler.hpp"

namespace fhc::server {

    RequestHandler::RequestHandler(std::shared_ptr<base::interface::IStorageAdapter> db_adapter,
        std::shared_ptr<base::sql_loader::SqlLoader> sql_loader) 
    : db_adapter_(db_adapter)
    , sql_loader_(sql_loader)
    {
        std::string api_key;
        std::string secret_key;
        binance_client_ = std::make_unique<base::API::BinanceClient>(api_key, secret_key);
    } 

    void RequestHandler::InitDataBase() const {
        auto query = sql_loader_->GetSQL("create_table_crypto_klines"s);
        db_adapter_->Execute(query.data());
    }

    bool RequestHandler::IsTableExists([[maybe_unused]] const std::string& table) const {
        RespFromQuery result;
        const auto query = sql_loader_->GetSQL("select_exists_crypto_klines"s);

        auto pqxx_ptr = dynamic_cast<base::interface::PqxxAdapter*>(db_adapter_.get());
        if ( pqxx_ptr != nullptr) {
            result = pqxx_ptr->Query(query.data());
        }

        return result[0][0] == "t";
    }
 
    void RequestHandler::ExecuteQuery(const std::string& query) const {
        db_adapter_->Execute(query);
    }

    std::vector<base::API::Kline> RequestHandler::GetKlines(const std::string& symbol, const std::string& interval, uint64_t start_time, uint64_t end_time) const{
        auto result = binance_client_->GetKlines(symbol, interval, start_time, end_time);
        return result;
    }

    std::uint64_t RequestHandler::GetTimeFromBinance() const {
        return StrToUint64(binance_client_->GetServerTime());
    }

    std::uint64_t RequestHandler::BinanceTimeToEpochUnixTime(std::uint64_t binance_time) const {
        return binance_time / 1000;
    }

    std::uint64_t RequestHandler::EpochUnixTimeToBinanceTime(std::uint64_t epoch_time) const {
        return epoch_time * 1000;
    }

    std::uint64_t RequestHandler::GetTimeSinceEpoch(TimeStamp timestamp) const {
        namespace krn = std::chrono;

        auto [year, month, day] = timestamp;
        krn::year_month_day ymd{krn::year{year}, krn::month{month}, krn::day{day}};
        auto days_since_epoch = krn::sys_days{ymd};

        auto time_point = days_since_epoch + 0h + 0min;
        return krn::duration_cast<krn::milliseconds>(time_point.time_since_epoch()).count();
    }

    krn::year_month_day RequestHandler::StrDateToEpochUnixTime(const std::string& str_date) const {
        krn::year_month_day ymd;
        std::istringstream iss(str_date);

        iss >> krn::parse("%F", ymd);
        return ymd;
    }

    std::uint64_t RequestHandler::StrStartDateToEpochUnixTime(const std::string& str_start_date) const {
        krn::year_month_day ymd = StrDateToEpochUnixTime(str_start_date);

        return krn::duration_cast<krn::milliseconds>(krn::time_point((krn::sys_days{ymd}) + 0h + 0min)
            .time_since_epoch()).count();
    }

    std::uint64_t RequestHandler::StrEndDateToEpochUnixTime(const std::string& str_end_date) const {
        krn::year_month_day ymd = StrDateToEpochUnixTime(str_end_date);

        return krn::duration_cast<krn::milliseconds>(krn::time_point((krn::sys_days{ymd}) + 23h + 59min + 59s)
            .time_since_epoch()).count();
    }

    std::uint64_t RequestHandler::StrToUint64(const std::string& str) const {
         std::istringstream s(str);
         return *std::istream_iterator<std::uint64_t>(s);
    }

    std::string RequestHandler::UpdateKlinesInStorage() const {
        RespFromQuery result;
        const auto query = sql_loader_->GetSQL("select_last_kline"s);

        auto pqxx_ptr = dynamic_cast<base::interface::PqxxAdapter*>(db_adapter_.get());
        if ( pqxx_ptr != nullptr) {
            result = pqxx_ptr->Query(query.data());
        }

        // TODO: Add check if result is empty
        uint64_t storage_last_open_time = StrToUint64(result[0][0]);
        
        uint64_t binance_server_time = GetTimeFromBinance();

        if ((binance_server_time - storage_last_open_time) <= 60000 ) {
            return {"Storage and Binance has equal data. Doesn't need updates."};
        }

        int count_insert_klines = 0;
        std::string symbol_crypto{"BTCUSDT"s};
        std::string interval{"1m"s};

        // TODO: Replace all magic number on the constant
        for (auto start_time = storage_last_open_time + 60000; // + 60000 to avoid taking the current kline again in the request
            start_time < binance_server_time; 
            start_time += 59999999) {
            
            using namespace std;

            // TODO: Replace all magic number on the constant
            auto end_time = start_time + 60000000;
            auto result = GetKlines(symbol_crypto, interval, start_time, end_time);

            for (const auto& kline : result) {
                // TODO: Switch this to prepared statemens (libpqxx). 
                //       Looks it INSERT INTO crypto_klines (..., ...) VALUES ($1, $2, $3, $...)
                auto insert_query = sql_loader_->GetSQL("insert_kline"s);
                try {
                    pqxx_ptr->ExecuteWithParams(insert_query.data(), 
                    kline.symbol,
                    to_string(kline.open_time),
                    to_string(kline.open),
                    to_string(kline.high),
                    to_string(kline.low),
                    to_string(kline.close),
                    to_string(kline.volume),
                    to_string(kline.close_time),
                    to_string(kline.quote_asset_volume),
                    to_string(kline.trades_count),
                    to_string(kline.taker_buy_base_volume),
                    to_string(kline.taker_buy_quote_volume));
                } catch (pqxx::failure& e) {
                    std::cout << e.what() << std::endl;
                }

                count_insert_klines++;

                std::cout << "Insert " << count_insert_klines << " " <<
                    kline.symbol << " " <<
                    to_string(kline.open_time) << " " <<
                    to_string(kline.open) << " " << 
                    to_string(kline.high) << " " <<
                    to_string(kline.low) << " " <<
                    to_string(kline.close) << " " <<
                    to_string(kline.volume) << " " <<
                    to_string(kline.close_time) << " " <<
                    to_string(kline.quote_asset_volume) << " " <<
                    to_string(kline.trades_count) << " " <<
                    to_string(kline.taker_buy_base_volume) << " " <<
                    to_string(kline.taker_buy_quote_volume) << std::endl;
            }
        }

        return {"Insert klines " + std::to_string(count_insert_klines)};
    }

    flatbuffers::FlatBufferBuilder RequestHandler::GetKlinesFromStorage(const std::string& timestamp_start, const std::string& timestamp_end) const {
        // TODO: What's going on here?
        auto start_time = EpochUnixTimeToBinanceTime(BinanceTimeToEpochUnixTime(StrStartDateToEpochUnixTime(timestamp_start)));
        auto end_time = EpochUnixTimeToBinanceTime(BinanceTimeToEpochUnixTime(StrEndDateToEpochUnixTime(timestamp_end)));

        // TODO: In futuring need use pqxx::esc for blocking sql injection
        const auto query = sql_loader_->GetSQL("select_klines"s);
        RespFromQuery result;

        // TODO: Remove dynamic_cast
        if (auto pqxx_ptr = dynamic_cast<base::interface::PqxxAdapter*>(db_adapter_.get()); pqxx_ptr != nullptr) {
            result = pqxx_ptr->Query(query.data(), std::to_string(start_time), std::to_string(end_time));
        }

        return CreateFlatBuffer(result);
    }

    flatbuffers::FlatBufferBuilder RequestHandler::CreateFlatBuffer(const RespFromQuery& resp) const {
             // Starting serialization data
        flatbuffers::FlatBufferBuilder builder(1024);
        std::vector<flatbuffers::Offset<marketdata::Kline>> kline_offsets;
        
        for (const auto& row : resp) {
            auto symbol = builder.CreateString(row[1]);
            auto kline = marketdata::CreateKline(
                builder,
                std::stoi(row[0]), // id
                symbol,
                std::stoll(row[2]), // open_time
                std::stod(row[3]), // open
                std::stod(row[4]), // high
                std::stod(row[5]), // low
                std::stod(row[6]), // close
                std::stod(row[7]), // volume
                std::stoll(row[8]), // close_time
                std::stod(row[9]), // quote asset volume
                std::stoi(row[10]), // trades count
                std::stod(row[11]), // taker buy base volume
                std::stod(row[12]) // taker buy quote volume
            );
            kline_offsets.push_back(kline);
        }

        auto klines_vector = builder.CreateVector(kline_offsets);

        auto kline_list = marketdata::CreateKlineList(builder, klines_vector);

        builder.Finish(kline_list);

        return builder;
    }

    void RequestHandler::PopulateDatabase(TimeStamp timestamp) const {
        // TODO: Separate it into a unique entity
        std::string symbol_crypto{"BTCUSDT"s};
        std::string interval{"1m"s};

        auto since_begins_year_2025 = GetTimeSinceEpoch(timestamp);
        auto current_milliseconds = GetTimeFromBinance();

        // TODO: Remove dynamic_cast
        auto pqxx_adapter_ptr = dynamic_cast<base::interface::PqxxAdapter*>(db_adapter_.get());
        if (pqxx_adapter_ptr == nullptr) {
            std::cerr << "Db adapter is not Pqxx Adapter!" << std::endl;
        }

        int count_insert_klines = 0;
        // TODO: Replace all magic number on the constant
        for (long long unsigned int start_time = since_begins_year_2025; start_time < current_milliseconds; start_time += 59999999) {
            using namespace std;

            // TODO: Replace all magic number on the constant
            auto end_time = start_time + 60000000;
            auto result = GetKlines(symbol_crypto, interval, start_time, end_time);

            for (const auto& kline : result) {
                // TODO: Switch this to prepared statemens (libpqxx). 
                //       Looks it INSERT INTO crypto_klines (..., ...) VALUES ($1, $2, $3, $...)
                auto query = sql_loader_->GetSQL("insert_kline"s);
                try {
                    pqxx_adapter_ptr->ExecuteWithParams(query.data(), 
                    kline.symbol,
                    to_string(kline.open_time),
                    to_string(kline.open),
                    to_string(kline.high),
                    to_string(kline.low),
                    to_string(kline.close),
                    to_string(kline.volume),
                    to_string(kline.close_time),
                    to_string(kline.quote_asset_volume),
                    to_string(kline.trades_count),
                    to_string(kline.taker_buy_base_volume),
                    to_string(kline.taker_buy_quote_volume)
                    );
                } catch (pqxx::failure& e) {
                    std::cout << e.what() << std::endl;
                }

                std::cout << "Insert " << count_insert_klines << " " <<
                kline.symbol << " " <<
                to_string(kline.open_time) << " " <<
                to_string(kline.open) << " " << 
                to_string(kline.high) << " " <<
                to_string(kline.low) << " " <<
                to_string(kline.close) << " " <<
                to_string(kline.volume) << " " <<
                to_string(kline.close_time) << " " <<
                to_string(kline.quote_asset_volume) << " " <<
                to_string(kline.trades_count) << " " <<
                to_string(kline.taker_buy_base_volume) << " " <<
                to_string(kline.taker_buy_quote_volume) << std::endl;
                
                count_insert_klines++;
            }
        }
    }
}