#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

namespace fhc::server::utils {
    struct Config {
        std::string host;
        std::string name;
        std::string user;
        std::string password;
        std::string http_host;
        int http_port;

        std::string operator()() const {
            return {"host=" + host + " dbname=" + name + " user=" + user + " password=" + password};
        }
    };

    static const std::string CONFIG_FILE(".nfile");

    static const char SIGN_EQUAL = '=';

    Config ReadConfig();
};