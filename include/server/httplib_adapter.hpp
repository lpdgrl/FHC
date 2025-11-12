#pragma once

#include <memory>

#include <httplib.h>

#include "ihttp_server.hpp"

namespace fhc::server::http_adapter {
    class HttpLibAdapter : public IHttpServer {
    public:
        HttpLibAdapter() = default;

        void Init(HttpConfig config) override;
        void Run() override;
        void Stop() override;

        void Post(const std::string&, RequestHandler handler) override;

        void SetMountPoint(const std::string& mount_point, const std::string& path_mount_point) override;

        void Get(const std::string& path, StreamResponse::Handler handler,
                       const std::string& content_type) override;

        const std::string& GetPort() const override;
        int GetIp() const override;

        ~HttpLibAdapter() = default;
    private:
        std::unique_ptr<httplib::Server> http_svr_;
        HttpConfig http_config_;
    };
}
