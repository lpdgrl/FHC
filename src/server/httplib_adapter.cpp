#include "../../include/server/httplib_adapter.hpp"

namespace fhc::server::http_adapter {
        
    void HttpLibAdapter::Init(HttpConfig config) {
        http_svr_ = std::make_unique<httplib::Server>();
        http_config_ = config;
    }
    

    void HttpLibAdapter::Post(const std::string&, RequestHandler handler) {

    }

    void HttpLibAdapter::Get(const std::string& path, StreamResponse::Handler handler,
                                   const std::string& content_type) 
    {
        // TODO: Add message if path is empty
        if (path.empty()) {
            return;
        }

        http_svr_->Get(path, [handler, content_type](const httplib::Request& request, httplib::Response& response) {
            std::unordered_map<std::string, std::string> params;
            for (const auto& param : request.params) {
                params[param.first] = param.second;
            }

            auto write = [&](const char* data, size_t size) {
                response.set_content(data, size, content_type);
            };

            handler(params, write);
        });
    }

    void HttpLibAdapter::SetMountPoint(const std::string& mount_point, const std::string& path_mount_point) {
        http_svr_->set_mount_point(mount_point, path_mount_point);
    }

    void HttpLibAdapter::Run() {
        http_svr_->listen(http_config_.ip, http_config_.port);
    }

    void HttpLibAdapter::Stop() {
        http_svr_->stop();
    }

    const std::string& HttpLibAdapter::GetPort() const {
        return http_config_.ip;
    }

    int HttpLibAdapter::GetIp() const {
        return http_config_.port;
    }
}