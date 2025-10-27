#include "../../include/server/server.hpp"

namespace fhc::server {
    using namespace std::literals;

    void Server::Init() {
        std::string options = "host=localhost dbname=fhc user=postgres password=kis123Oits"s;
        db_ = std::make_unique<base::Database>(options);
        handler_ = std::make_unique<server::RequestHandler>(db_->GetAdapter());
    }
    
    void Server::Run() {
        // TODO: Add throw and catch exceptions if db_ is nullptr
        if (db_ != nullptr) {
            db_->Connect();
        }

        handler_->HandledQuery("SELECT * FROM crypto_klines"s);
    }

    void Server::Stop() {
        // TODO: Add throw and catch exceptions if db_ is nullptr
        if (db_ != nullptr) {
            db_->Disconnect();
        }
    }

    bool Server::IsRun() const {
        return running_;
    }
}
