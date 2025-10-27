#pragma once

#include "Ipqxx.hpp"

namespace fhc::base::interface {
    class  PQXXAdaptor final : public IPQXX  {
    public:
        PQXXAdaptor() = default;

        void ConnectPQXX(const std::string& options) override;
        void DisconnectPQXX() override;

        void Execute(const std::string& query);

        std::vector<std::vector<std::string>> Query(const std::string& query);

        bool IsConnected() const;

        // raw connection
        pqxx::connection& GetConnection();

        ~PQXXAdaptor() = default;
    private:
        std::unique_ptr<pqxx::connection> connection_;
    };
}
