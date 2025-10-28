#include "../../include/server/request_handler.hpp"

namespace fhc::server {

    RequestHandler::RequestHandler(std::shared_ptr<fhc::base::interface::PQXXAdaptor> db_adapter) : db_adapter_(db_adapter) {
        std::string api_key;
        std::string secret_key;
        binance_client_ = std::make_unique<fhc::base::API::BinanceClient>(api_key, secret_key);
    } 

    void RequestHandler::HandledQuery(const std::string& query) const {
        auto result = db_adapter_->Query(query);

        for (const auto& row : result) {
            for (const auto& field : row) {
                std::cout << "Field: " << field << std::endl;
            }
        }
    }

    void RequestHandler::GetKlines(const std::string& symbol, uint64_t start_time, uint64_t end_time) const{
        auto result = binance_client_->GetKlines(symbol, start_time, end_time);
    }

}