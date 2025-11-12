#pragma once

#include "../base/database.hpp"
#include "../base/sql_loader.hpp"
#include "../server/request_handler.hpp"
#include "../server/utils.hpp"

#include <httplib.h>

namespace fhc::server {
    class Server final {
    public:
        Server() = default;

        void Init();
        void Run();
        void Stop();
        bool IsRun() const;

        ~Server() = default;
    private:
        void InitializationDatabase();

        std::unique_ptr<base::Database> db_;
        std::unique_ptr<RequestHandler> handler_;
        std::unique_ptr<httplib::Server> http_svr_;
        std::unique_ptr<base::sql_loader::SqlLoader> sql_loader_;
        
        bool running_ = false;
    };
}
