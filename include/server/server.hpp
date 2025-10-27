#pragma once

#include "../base/database.hpp"
#include "../server/request_handler.hpp"

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
        std::unique_ptr<base::Database> db_;
        std::unique_ptr<RequestHandler> handler_;
        bool running_ = false;
    };
}
