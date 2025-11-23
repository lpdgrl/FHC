#include "../include/server/server.hpp"

#include <iostream>

void RunServer();

int main(int argc, const char** argv) {
    RunServer();

    return EXIT_SUCCESS;
}

void RunServer() {
    using namespace fhc;
    server::Server server;
    
    auto api_klines = [&](const std::unordered_map<std::string, std::string>& params, auto write) {
            std::string start_date;
            std::string end_date;

            if (auto it_find_start_time = params.find("StartTime"); it_find_start_time != params.end()) {
                start_date = it_find_start_time->second;
            }
            
            if (auto it_find_end_time = params.find("EndTime"); it_find_end_time != params.end()) {
                end_date = it_find_end_time->second;
            }
            
            if (start_date.empty() || end_date.empty()) {
                std::cerr << "StartTime or EndTime is empty!" << std::endl;
            }

            flatbuffers::FlatBufferBuilder builder = server.GetKlinesFromStorage(start_date, end_date);

            auto buf = std::make_shared<std::vector<uint8_t>>(
                builder.GetBufferPointer(),
                builder.GetBufferPointer() + builder.GetSize()
            );

            write(reinterpret_cast<const char*>(buf->data()), buf->size());
    };
    server.SetHandler("/api/klines", {server::HTTP_METHOD::GET, api_klines, "application/octet-stream"});

    auto api_klines_update = [&](const std::unordered_map<std::string, std::string>& params, auto write) {
        std::string result = server.UpdateKlinesInStorage();
        
        write(reinterpret_cast<const char*>(result.data()), result.size());
    };

    server.SetHandler("/api/update", {server::HTTP_METHOD::GET, api_klines_update, "text/plain"});

    server.Init();
    server.SetMountPoint("/", CMAKE_PATH_TO_WEB_FILES);
    std::cout << "Start server!" << std::endl;
    server.Run();
}
