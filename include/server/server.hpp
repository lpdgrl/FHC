#pragma once

#include "../base/database.hpp"
#include "../base/sql_loader.hpp"
#include "../server/request_handler.hpp"
#include "../server/utils.hpp"
#include "../server/httplib_adapter.hpp"

#include <httplib.h>

namespace fhc::server {
    enum class HTTP_METHOD {
        GET,
        POST,
    };

    class Server final {
    public:
        using Handler = http_adapter::StreamResponse::Handler;

        struct HTTPHandler {
            HTTP_METHOD http_method;
            Handler handler;
            std::string content_type;
        };

        Server() = default;

        void Init();
        void Run();
        void Stop();
        bool IsRun() const;

        void SetHandler(const std::string& path, HTTPHandler http_handler);
        void RemoveHandler(const std::string& path);
        void SetMountPoint(const std::string& root, const std::string& path_to_files) const;

        flatbuffers::FlatBufferBuilder GetKlinesFromStorage(const std::string& start_date, const std::string& end_date) const;

        std::string UpdateKlinesInStorage() const;

        ~Server() = default;
    private:
        void AddHandlerToHttpSvr() const;

        std::unique_ptr<base::Database> db_;
        std::unique_ptr<RequestHandler> request_handler_;
        std::unique_ptr<http_adapter::IHttpServer> http_svr_;
        std::shared_ptr<base::sql_loader::SqlLoader> sql_loader_;

        bool running_ = false;
        std::unordered_map<std::string, HTTPHandler> handlers_;
    };
}
