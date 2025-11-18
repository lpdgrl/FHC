#include "../../include/base/sql_loader.hpp"

namespace fhc::base::sql_loader {
    void SqlLoader::LoadSQL(const std::string& file_name) {
        auto parent_dir = std::filesystem::current_path().parent_path();

        std::ifstream sql_file(parent_dir.c_str() + SQL_DIR_PATH + file_name + SQL_FILE_SUFFIX, std::ios::in);

        if (!sql_file.is_open()) {
            std::cerr << "File " << file_name << " doesn't opened!" << std::endl;
        }
        
        std::string line;
        while (std::getline(sql_file, line)) {
            queries_[file_name] += line;
        }
    }

    std::string_view SqlLoader::GetSQL(const std::string& sql_query_name) const {
        if (auto it_sql = queries_.find(sql_query_name); it_sql != queries_.end()) {
            return it_sql->second;
        }
        return {};
    }
}
