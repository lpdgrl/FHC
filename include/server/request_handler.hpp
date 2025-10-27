#pragma once 

#include "../base/pqxx_adaptor.hpp"

namespace fhc::server {
    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(std::shared_ptr<fhc::base::interface::PQXXAdaptor> db_adapter);

        void HandledQuery(const std::string& query) const;

        ~RequestHandler() = default;
    private:
        std::shared_ptr<fhc::base::interface::PQXXAdaptor> db_adapter_;
        // std::unique_ptr<fhc::base::API::Binance> binance_;
    };
};