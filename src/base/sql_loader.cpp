#include "../../include/base/sql_loader.hpp"

namespace fhc::base::sql_loader {
    void SqlLoader::LoadSQL(const std::string& file_name) {
        auto parent_dir = std::filesystem::current_path().parent_path();

        std::ifstream sql_file(parent_dir.c_str() + SQL_DIR_PATH + file_name, std::ios::in);

        if (!sql_file.is_open()) {
            std::cerr << "File " << file_name << " doesn't opened!" << std::endl;
        }
        
        std::string line;
        while (std::getline(sql_file, line)) {
            std::cout << line << std::endl;
        }
        
    }

    std::string_view SqlLoader::GetSQL(const std::string& sql_query_name) const {

    }
}
