#pragma once 

#include "../base/pqxx_adapter.hpp"
#include "../base/binance_client.hpp"

namespace fhc::server {
    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(std::shared_ptr<fhc::base::interface::IStorageAdapter> db_adapter);

        void HandledQuery(const std::string& query) const;
        void ExecuteQuery(const std::string& query) const;
        
        std::vector<fhc::base::API::Kline> GetKlines(const std::string& symbol, uint64_t start_time, uint64_t end_time) const;
        std::string GetTimeFromBinance() const;

        ~RequestHandler() = default;
    private:
        std::shared_ptr<fhc::base::interface::IStorageAdapter> db_adapter_;
        std::unique_ptr<fhc::base::API::BinanceClient> binance_client_;
    };
}