#include "../../include/server/request_handler.hpp"

namespace fhc::server {

    RequestHandler::RequestHandler(std::shared_ptr<fhc::base::interface::PQXXAdaptor> db_adapter) : db_adapter_(db_adapter) {} 

    void RequestHandler::HandledQuery(const std::string& query) const {
        auto result = db_adapter_->Query(query);

        for (const auto& row : result) {
            for (const auto& field : row) {
                std::cout << "Field: " << field << std::endl;
            }
        }
    }

}