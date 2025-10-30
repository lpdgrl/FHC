#include "../include/server/server.hpp"

int main() {


    fhc::server::Server server;
    server.Init();
    server.Run();

    return EXIT_SUCCESS;
}