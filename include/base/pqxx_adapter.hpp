#pragma once

#include "istorage_adapter.hpp"

namespace fhc::base::interface {
    class  PqxxAdapter final : public IStorageAdapter {
    public:
        PqxxAdapter() = default;

        void Connect(const std::string& options) override;
        void Disconnect() override;
        
        void Execute(const std::string& query) const override;

        template<typename... Args>
        void ExecuteWithParams(const std::string& query, Args&&... args) const;

        template <typename... Args>
        std::vector<std::vector<std::string>> Query(const std::string& query, Args&&... args) const;


        bool IsConnected() const override;

        ~PqxxAdapter() = default;
    private:
        template <typename... Args>
        pqxx::params MakeParams(Args&&... args) const;
        std::vector<std::vector<std::string>> PqxxResultToVectorStr(pqxx::result& result) const; 

        std::unique_ptr<pqxx::connection> connection_;
    };

    template <typename... Args>
    pqxx::params PqxxAdapter::MakeParams(Args&&... args) const {
        pqxx::params p;
        (p.append(std::forward<Args>(args)), ...);

        return p;
    }

    template<typename... Args>
    void PqxxAdapter::ExecuteWithParams(const std::string& query, Args&&... args) const {
        pqxx::work txn(*connection_);
        pqxx::result r = txn.exec(query, MakeParams(std::forward<Args>(args)...));
        txn.commit();
    }

    template<typename... Args>
    std::vector<std::vector<std::string>> PqxxAdapter::Query(const std::string& query, Args&&... args) const {
        // TODO: Switch to nontransaction object

        pqxx::work txn(*connection_);
        pqxx::result r = txn.exec(query, MakeParams(std::forward<Args>(args)...));
        txn.commit();

        std::vector<std::vector<std::string>> result(std::move(PqxxResultToVectorStr(r)));
        return result;
    }
}
