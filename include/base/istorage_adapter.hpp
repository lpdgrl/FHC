#pragma once

#include <pqxx/pqxx>

#include <string>
#include <memory>
#include <vector>

#include <iostream>

namespace fhc::base::interface {
    class IStorageAdapter {
    public:
        virtual void Connect(const std::string& options) = 0;
        virtual void Disconnect() = 0;
        virtual bool IsConnected() const = 0;

        virtual void Execute(const std::string& query) const = 0;
        // virtual std::vector<std::vector<std::string>> Query(const std::string& query, Args&& args) const = 0;
        // virtual std::vector<std::vector<std::string>> Query(const std::string& query) const = 0;
        virtual ~IStorageAdapter() = default;
    };
}
