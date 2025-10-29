#pragma once

#include "istorage_adapter.hpp"

namespace fhc::base::interface {
    class  PqxxAdapter final : public IStorageAdapter {
    public:
        PqxxAdapter() = default;

        void Connect(const std::string& options) override;
        void Disconnect() override;

        void Execute(const std::string& query) const override;

        std::vector<std::vector<std::string>> Query(const std::string& query) const;

        bool IsConnected() const override;

        ~PqxxAdapter() = default;
    private:
        std::unique_ptr<pqxx::connection> connection_;
    };
}
