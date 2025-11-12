#include "../../include/server/server.hpp"

namespace fhc::server {
    using namespace std::literals;

    void Server::Init() {
        utils::Config config = utils::ReadConfig();
        std::string options(config());
        db_ = std::make_unique<base::Database>(options);

        handler_ = std::make_unique<server::RequestHandler>(db_->GetAdapter());
        http_svr_ = std::make_unique<httplib::Server>();

        sql_loader_ = std::make_unique<base::sql_loader::SqlLoader>();
    }

    void Server::Run() {
        // TODO: Add throw and catch exceptions if db_ is nullptr?
        if (db_ != nullptr) {
            db_->Connect();
        }
        
        sql_loader_
        
        http_svr_->Get("/hi", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello World!", "text/plain");
            });

        http_svr_->listen("0.0.0.0", 8000);
         
        // TODO: Some need in future
        running_ = true;
        
        handler_->SendToFront({2025, 10, 01}, {2025, 10, 30});

        // TODO: Add select when this run
        // InitializationDatabase();
        // handler_->PopulateDatabase();
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
    void Server::InitializationDatabase() {
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
    }
}
