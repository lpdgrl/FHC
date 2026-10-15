#include "../../include/server/utils.hpp"
#include <iostream>

namespace fhc::server::utils {
    Config ReadConfig() {
        std::unordered_map<std::string, std::string> result;

        auto parent_dir = std::filesystem::current_path().parent_path().parent_path();
        
        
        std::ifstream file_config(CMAKE_CONFIG_PATH + CONFIG_FILE, std::ios::in);
        
        if (!file_config.is_open()) {
            std::cerr << "File " << CONFIG_FILE << " doesn't opened!" << std::endl;
            return {};
        }
        
        std::string line;
        while (std::getline(file_config, line)) {
            result[line.substr(0, line.find_first_of(SIGN_EQUAL))] = line.substr(line.find_first_of(SIGN_EQUAL) + 1, line.size() - 1);
        }
        
        Config config{
            .host = result["DB_HOST"],
            .name = result["DB_NAME"],
            .user = result["DB_USER"],
            .password = result["DB_PASSWORD"],
            .http_host = result["HTTP_HOST"],
            .http_port = std::stoi(result["HTTP_PORT"])
        };
        
        return config;
    }
}