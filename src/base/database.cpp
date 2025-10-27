#include "../../include/base/database.hpp"

namespace fhc::base {
    Database::Database(const std::string& options) : connect_options_(options) {
        interface_ = std::make_shared<interface::PQXXAdaptor>();
    }

    void Database::Connect() {
        interface_->ConnectPQXX(connect_options_);
    }

    void Database::Disconnect() {
        interface_->DisconnectPQXX();
    }

    bool Database::isConnected() const {
        return interface_->IsConnected();
    }

    std::shared_ptr<interface::PQXXAdaptor> Database::GetAdapter() {
        return interface_;
    }
 
}