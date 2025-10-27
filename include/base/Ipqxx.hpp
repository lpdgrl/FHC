#pragma once

#include <pqxx/pqxx>

#include <string>
#include <memory>
#include <vector>

#include <iostream>

namespace fhc::base::interface {
    class IPQXX {
    public:
        virtual void ConnectPQXX(const std::string& options) = 0;
        virtual void DisconnectPQXX() = 0;
        virtual ~IPQXX() = default;
    };
}
