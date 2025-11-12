#include "../../include/server/utils.hpp"
#include <iostream>

namespace fhc::server::utils {
    Config ReadConfig() {
        std::unordered_map<std::string, std::string> result;

        auto current_dir = std::filesystem::current_path();
        auto parent_dir = current_dir.parent_path();

        std::ifstream file_config(parent_dir.c_str() + CONFIG_PATH, std::ios::in);
        
        if (!file_config.is_open()) {
            std::cerr << "File " << CONFIG_PATH << " doesn't opened!" << std::endl;
        }
        
        std::string line;
        while (std::getline(file_config, line)) {
            result[line.substr(0, line.find_first_of(SIGN_EQUAL))] = line.substr(line.find_first_of(SIGN_EQUAL) + 1, line.size() - 1);
        }
        
        Config config{
            .host = result["DB_HOST"],
            .name = result["DB_NAME"],
            .user = result["DB_USER"],
            .password = result["DB_PASSWORD"]
        };
        
        return config;
    }
}