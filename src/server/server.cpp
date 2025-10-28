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

        // std::string create_table = R"(
        //     CREATE TABLE crypto_klines (
        //         id SERIAL PRIMARY KEY,
        //         symbol VARCHAR(10) NOT NULL,            -- торговая пара, например BTCUSDT
        //         open_time BIGINT NOT NULL,              -- время открытия свечи (в мс)
        //         open NUMERIC(18,8) NOT NULL,            -- цена открытия
        //         high NUMERIC(18,8) NOT NULL,            -- максимальная цена
        //         low NUMERIC(18,8) NOT NULL,             -- минимальная цена
        //         close NUMERIC(18,8) NOT NULL,           -- цена закрытия
        //         volume NUMERIC(24,10) NOT NULL,         -- объём торгов
        //         close_time BIGINT NOT NULL,             -- время закрытия свечи (в мс)
        //         quote_asset_volume NUMERIC(24,10) NOT NULL, -- объём в котируемом активе
        //         trades_count INTEGER NOT NULL,          -- количество сделок
        //         taker_buy_base_volume NUMERIC(24,10) NOT NULL, -- объём покупок у тейкеров (в базовом активе)
        //         taker_buy_quote_volume NUMERIC(24,10) NOT NULL, -- объём покупок у тейкеров (в котируемом активе)
        //         UNIQUE (symbol, open_time)              -- уникальность по паре и времени открытия
        //     );
        // )";
        // handler_->HandledQuery(create_table);
        handler_->GetKlines("BTCUSDT"s, 1502942428000, 1502943428000);
        handler_->HandledQuery("SELECT * FROM crypto_klines"s);
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
