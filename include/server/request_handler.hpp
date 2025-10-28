#pragma once 

#include "../base/pqxx_adaptor.hpp"
#include "../base/binance_client.hpp"

namespace fhc::server {
    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(std::shared_ptr<fhc::base::interface::PQXXAdaptor> db_adapter);

        void HandledQuery(const std::string& query) const;
        void GetKlines(const std::string& symbol, uint64_t start_time, uint64_t end_time) const;

        ~RequestHandler() = default;
    private:
        std::shared_ptr<fhc::base::interface::PQXXAdaptor> db_adapter_;
        std::unique_ptr<fhc::base::API::BinanceClient> binance_client_;
    };
}