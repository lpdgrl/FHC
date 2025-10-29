#pragma once 

#include "binacpp.h"
#include "binacpp_websocket.h"
#include <json/json.h>

#include <cstdio>

namespace fhc::base::API {
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

    struct Kline {
        std::string symbol;                // торговая пара, например BTCUSDT
        uint64_t open_time;                // время открытия свечи (в мс)
        double open;                       // цена открытия
        double high;                       // максимальная цена
        double low;                        // минимальная цена
        double close;                      // цена закрытия
        double volume;                     // объём торгов
        uint64_t close_time;               // время закрытия свечи (в мс)
        double quote_asset_volume;         // объём в котируемом активе
        int trades_count;                  // количество сделок
        double taker_buy_base_volume;      // объём покупок у тейкеров (в базовом активе)
        double taker_buy_quote_volume;     // объём покупок у тейкеров (в котируемом активе)
    };

    class BinanceClient {
    public:
        BinanceClient() = default;
        explicit BinanceClient(const std::string& api_key, const std::string& secret_key);

        std::vector<Kline> GetKlines(const std::string& symbol, uint64_t start_time, uint64_t end_time) const;
        std::string GetServerTime() const;

        ~BinanceClient() = default;
    private:
        Kline ReturnKline(const std::string& symbol, const Json::Value& value) const;
        std::string api_key_;
        std::string secret_key_;
    };
}