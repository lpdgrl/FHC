#pragma once

#include <unordered_map>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fhc::base::sql_loader {
    // static const std::string SQL_DIR_PATH("/sql/");
    static const std::string SQL_FILE_SUFFIX(".sql");

    class SqlLoader {
    public:
        SqlLoader() = default;
        void LoadSQL(const std::string& file_name);
        std::string_view GetSQL(const std::string& sql_query_name) const; 

    private:
        std::unordered_map<std::string, std::string> queries_;
    };
}
