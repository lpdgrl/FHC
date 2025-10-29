#include "../../include/base/database.hpp"

namespace fhc::base {
    Database::Database(const std::string& options) : connect_options_(options) {
        interface_ = std::make_shared<fhc::base::interface::PqxxAdapter>();
    }

    void Database::Connect() {
        interface_->Connect(connect_options_);
    }

    void Database::Disconnect() {
        interface_->Disconnect();
    }

    bool Database::isConnected() const {
        return interface_->IsConnected();
    }

    std::shared_ptr<interface::IStorageAdapter> Database::GetAdapter() {
        return interface_;
    }
 
}