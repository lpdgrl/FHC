#include "../../include/server/server.hpp"

namespace fhc::server {
    using namespace std::literals;

    void Server::Init() {
        utils::Config config = utils::ReadConfig();
        std::string options(config());
        
        db_ = std::make_unique<base::Database>(options);

        http_svr_ = std::make_unique<http_adapter::HttpLibAdapter>();
        sql_loader_ = std::make_shared<fhc::base::sql_loader::SqlLoader>();

        request_handler_ = std::make_unique<server::RequestHandler>(db_->GetAdapter(), sql_loader_);

        sql_loader_->LoadSQL("create_table_crypto_klines"s);
        sql_loader_->LoadSQL("select_klines"s);
        sql_loader_->LoadSQL("insert_kline"s);
        sql_loader_->LoadSQL("select_last_kline"s);
        sql_loader_->LoadSQL("select_exists_crypto_klines"s);

        // TODO: Read settings from config
        http_svr_->Init({config.http_host, config.http_port});

        AddHandlerToHttpSvr();
    }

    void Server::SetMountPoint(const std::string& root, const std::string& path_to_files) const {
        http_svr_->SetMountPoint(root, path_to_files);
    }

    void Server::Run() {
        // TODO: Add throw and catch exceptions if db_ is nullptr?
        if (db_ != nullptr) {
            db_->Connect();
        }

        if (!request_handler_->IsTableExists("crypto_klines"s)) {
            request_handler_->InitDataBase();
            request_handler_->PopulateDatabase({2025, 8, 1});
        }

        http_svr_->Run();

        // TODO: Some need in future
        running_ = true;
        
    }

    void Server::Stop() { 
        // TODO: Add throw and catch exceptions if db_ is nullptr
        if (db_ != nullptr) {
            db_->Disconnect();
        }
        
        if (http_svr_ != nullptr) {
            http_svr_->Stop();
        }

        running_ = false;
    }

    bool Server::IsRun() const {
        return running_;
    }

    flatbuffers::FlatBufferBuilder Server::GetKlinesFromStorage(const std::string& start_date, const std::string& end_date) const {
        return request_handler_->GetKlinesFromStorage(start_date, end_date);
    }

    std::string Server::UpdateKlinesInStorage() const {
        return request_handler_->UpdateKlinesInStorage();
    }

    void Server::SetHandler(const std::string& path, HTTPHandler http_handler) {
        handlers_[path] = http_handler;
    }

    void Server::RemoveHandler(const std::string& path) {
        // TODO: Needs add substitution handler with response 404 when deleted handler in Server hash-map
        if (auto it_find_path = handlers_.find(path); it_find_path != handlers_.end()) {
            handlers_.erase(path);
        }
    }

    void Server::AddHandlerToHttpSvr() const {
        if (!handlers_.empty()) {
            for (const auto& [path, http_handler] : handlers_) {
                switch (http_handler.http_method) {
                    case HTTP_METHOD::GET:
                        http_svr_->Get(path, http_handler.handler, http_handler.content_type);
                        break;
                }
            }
        }
    }
}
