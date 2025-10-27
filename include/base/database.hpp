#pragma once

#include <pqxx/pqxx>

#include "pqxx_adaptor.hpp"

namespace fhc::base {
    /*
    CREATE TABLE crypto_klines (
        id SERIAL PRIMARY KEY,
        symbol VARCHAR(10) NOT NULL,
        interval VARCHAR(5) NOT NULL,
        open_time BIGINT NOT NULL,
        open NUMERIC(18,8) NOT NULL,
        high NUMERIC(18,8) NOT NULL,
        low NUMERIC(18,8) NOT NULL,
        close NUMERIC(18,8) NOT NULL,
        volume NUMERIC(30,8) NOT NULL,
        close_time BIGINT NOT NULL,
        UNIQUE(symbol, interval, open_time)
    );
    */

    class Database final {
    public:
        Database() = delete;

        explicit Database(const std::string& options);

        void Connect();
        void Disconnect();
        bool isConnected() const;
        std::shared_ptr<interface::PQXXAdaptor> GetAdapter();
        
    private:
        std::string connect_options_;
        std::shared_ptr<interface::PQXXAdaptor> interface_;
    };
}
