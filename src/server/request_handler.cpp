#include "../../include/server/request_handler.hpp"

namespace fhc::server {

    RequestHandler::RequestHandler(std::shared_ptr<fhc::base::interface::IStorageAdapter> db_adapter) : db_adapter_(db_adapter) {
        std::string api_key;
        std::string secret_key;
        binance_client_ = std::make_unique<fhc::base::API::BinanceClient>(api_key, secret_key);
    } 

    void RequestHandler::ExecuteQuery(const std::string& query) const {
        db_adapter_->Execute(query);
    }

    std::vector<fhc::base::API::Kline> RequestHandler::GetKlines(const std::string& symbol, const std::string& interval, uint64_t start_time, uint64_t end_time) const{
        auto result = binance_client_->GetKlines(symbol, interval, start_time, end_time);
        return result;
    }

    std::uint64_t RequestHandler::GetTimeFromBinance() const {
        std::uint64_t server_time = 0;

        std::istringstream in_str(binance_client_->GetServerTime());
        
        in_str >> server_time;

        return server_time;
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

        auto time_point = days_since_epoch + 0h + 0min + 0s;
        return krn::duration_cast<krn::seconds>(time_point.time_since_epoch()).count();
    }

    void RequestHandler::SendToFront(TimeStamp timestamp_start, TimeStamp timestamp_end) const {
        auto start_time = EpochUnixTimeToBinanceTime(GetTimeSinceEpoch(timestamp_start));
        auto end_time = EpochUnixTimeToBinanceTime(GetTimeSinceEpoch(timestamp_end));

        // TODO: In futuring need use pqxx::esc for blocking sql injection
        std::string query = ("SELECT * FROM crypto_klines "
                             "WHERE open_time >= " + std::to_string(start_time) +
                             " AND close_time <= " + std::to_string(end_time));
        auto result = db_adapter_->Query(query);

        // Starting serialization data
        flatbuffers::FlatBufferBuilder builder(1024);
        std::vector<flatbuffers::Offset<marketdata::Kline>> kline_offsets;
        
        for (const auto& row : result) {
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

        // Ready to save in the file or send over network
        uint8_t* buf = builder.GetBufferPointer();
        int size = builder.GetSize();
    }

    void RequestHandler::PopulateDatabase() const {
        std::string symbol_crypto{"BTCUSDT"};
        std::string interval{"1m"};

        auto since_begins_year_2025 = EpochUnixTimeToBinanceTime(GetTimeSinceEpoch({2025, 1, 1}));
        auto current_milliseconds = GetTimeFromBinance();

        for (long long unsigned int start_time = since_begins_year_2025; start_time < current_milliseconds; start_time += 59999999) {
            auto end_time = start_time + 60000000;
            auto result = GetKlines(symbol_crypto, interval, start_time, end_time);

            for (const auto& kline : result) {
            // TODO: Switch this to prepared statemens (libpqxx). 
            //       Looks it INSERT INTO crypto_klines (..., ...) VALUES ($1, $2, $3, $...)
            std::string query =
                "INSERT INTO crypto_klines "
                "(symbol, open_time, open, high, low, close, volume, close_time, "
                "quote_asset_volume, trades_count, taker_buy_base_volume, taker_buy_quote_volume) "
                "VALUES ('" + kline.symbol + "'," +
                std::to_string(kline.open_time) + "," +
                std::to_string(kline.open) + "," +
                std::to_string(kline.high) + "," +
                std::to_string(kline.low) + "," +
                std::to_string(kline.close) + "," +
                std::to_string(kline.volume) + "," +
                std::to_string(kline.close_time) + "," +
                std::to_string(kline.quote_asset_volume) + "," +
                std::to_string(kline.trades_count) + "," +
                std::to_string(kline.taker_buy_base_volume) + "," +
                std::to_string(kline.taker_buy_quote_volume) +
                ");";
                std::cout << query << std::endl;
                ExecuteQuery(query);
            }
        }
    }
}