#pragma once

#include <string>
#include <functional>
#include <unordered_map>

namespace fhc::server::http_adapter {
    struct HttpConfig {
        std::string ip;
        int port;
    };

    struct Request {
        std::string path;
        std::unordered_map<std::string, std::string> params;
    };

    struct StreamResponse {
        using WriteFunc = std::function<void(const char* data, size_t length)>;
        using Handler = std::function<void(const std::unordered_map<std::string, std::string>, WriteFunc write)>;
    };

    class IHttpServer {
    public:
        using RequestHandler = std::function<void(const std::string& path,
                                                   const std::unordered_map<std::string, std::string>& params,
                                                   std::string& response_body,
                                                   std::string& content_type)>;

        virtual void Init(HttpConfig) = 0;
        virtual void Run() = 0;
        virtual void Stop() = 0;

        virtual void Get(const std::string& path, StreamResponse::Handler handler,
                               const std::string& content_type) = 0;
        virtual void Post(const std::string&, RequestHandler) = 0;

        virtual void SetMountPoint(const std::string&, const std::string&) = 0;

        virtual const std::string& GetPort() const = 0;
        virtual int GetIp() const = 0;

        virtual ~IHttpServer() = default;
    };
}
