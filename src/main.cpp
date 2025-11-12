#include "../include/server/server.hpp"

#include <iostream>

void RunServer();

int main(int argc, const char** argv) {
    RunServer();

    return EXIT_SUCCESS;
}

void RunServer() {
    fhc::server::Server server;
    server.Init();
    server.Run();
}
