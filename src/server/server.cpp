#include "../../include/server/server.hpp"

namespace fhc::server {
    using namespace std::literals;

    void Server::Init() {
        std::string options = "host=localhost dbname=fhc user=postgres password=kis123Oits"s;
        db_ = std::make_unique<base::Database>(options);
        handler_ = std::make_unique<server::RequestHandler>(db_->GetAdapter());
    }
    
    void Server::Run() {
        // TODO: Add throw and catch exceptions if db_ is nullptr
        if (db_ != nullptr) {
            db_->Connect();
        }

        std::string create_table = R"(
            CREATE TABLE crypto_klines (
                id SERIAL PRIMARY KEY,
                symbol VARCHAR(10) NOT NULL,            -- торговая пара, например BTCUSDT
                open_time BIGINT NOT NULL,              -- время открытия свечи (в мс)
                open NUMERIC(18,8) NOT NULL,            -- цена открытия
                high NUMERIC(18,8) NOT NULL,            -- максимальная цена
                low NUMERIC(18,8) NOT NULL,             -- минимальная цена
                close NUMERIC(18,8) NOT NULL,           -- цена закрытия
                volume NUMERIC(24,10) NOT NULL,         -- объём торгов
                close_time BIGINT NOT NULL,             -- время закрытия свечи (в мс)
                quote_asset_volume NUMERIC(24,10) NOT NULL, -- объём в котируемом активе
                trades_count INTEGER NOT NULL,          -- количество сделок
                taker_buy_base_volume NUMERIC(24,10) NOT NULL, -- объём покупок у тейкеров (в базовом активе)
                taker_buy_quote_volume NUMERIC(24,10) NOT NULL, -- объём покупок у тейкеров (в котируемом активе)
                UNIQUE (symbol, open_time)              -- уникальность по паре и времени открытия
            );
        )";
        handler_->ExecuteQuery(create_table);
        std::string symbol_crypto;

        auto duration_since_epoch = std::chrono::steady_clock::now().time_since_epoch();
        auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();

        std::cout << milliseconds_since_epoch << std::endl;
        auto current_milliseconds = std::stoull(handler_->GetTimeFromBinance());
        for (long long unsigned int start_time = 1502942428000; start_time < current_milliseconds; start_time += 1000000) {
            auto end_time = start_time + 1000000;
            auto result = handler_->GetKlines("BTCUSDT"s, start_time, end_time);
            for (const auto& kline : result) {
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
                handler_->ExecuteQuery(query);
            }
            std::cout << result.size() << std::endl;
        }
       
        // handler_->HandledQuery("SELECT * FROM crypto_klines"s);
    }

    void Server::Stop() {
        // TODO: Add throw and catch exceptions if db_ is nullptr
        if (db_ != nullptr) {
            db_->Disconnect();
        }
    }

    bool Server::IsRun() const {
        return running_;
    }
}
