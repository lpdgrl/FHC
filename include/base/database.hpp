#pragma once

#include <pqxx/pqxx>

#include "pqxx_adapter.hpp"

namespace fhc::base {
    /*
    CREATE TABLE crypto_klines (
        id SERIAL PRIMARY KEY,
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
        taker_buy_quote_volume NUMERIC(24,10) NOT NULL  -- объём покупок у тейкеров (в котируемом активе)
    );
    */

    class Database final {
    public:
        Database() = delete;

        explicit Database(const std::string& options);

        void Connect();
        void Disconnect();
        bool isConnected() const;
        std::shared_ptr<interface::IStorageAdapter> GetAdapter();
        
    private:
        std::string connect_options_;
        std::shared_ptr<interface::IStorageAdapter> interface_;
    };
}
