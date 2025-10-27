#include "binacpp.h"
#include "binacpp_websocket.h"
#include "pqxx/pqxx"

#include "../include/server/server.hpp"

#include <json/json.h>
#include <iostream>
#include <string>
#include <chrono>

int main() {
    // std::string create_table = R"(
    //     CREATE TABLE crypto_klines (
    //         id SERIAL PRIMARY KEY,
    //         symbol VARCHAR(10) NOT NULL,
    //         interval VARCHAR(5) NOT NULL,
    //         open_time BIGINT NOT NULL,
    //         open NUMERIC(18,8) NOT NULL,
    //         high NUMERIC(18,8) NOT NULL,
    //         low NUMERIC(18,8) NOT NULL,
    //         close NUMERIC(18,8) NOT NULL,
    //         volume NUMERIC(30,8) NOT NULL,
    //         close_time BIGINT NOT NULL,
    //         UNIQUE(symbol, interval, open_time)
    //     );
    // )";

    fhc::server::Server server;
    server.Init();
    server.Run();

    // std::cout << db.isConnected() << std::endl;  
    
    
    // try {
    //     auto& conn = db.GetConnection();
    //     pqxx::work txn(conn);
    //     pqxx::result r = txn.exec(create_table);
    //     txn.commit();
    //     // for (const auto& row : r) {
    //     //     for (size_t i = 0; i < row.size(); ++i) {
    //     //         std::cout << row[i].c_str() << std::endl;
    //     //     }
    //     // }
    // } catch (const std::exception& e) {
    //     std::cerr << "Query failed: " << e.what() << "\n";
    // }

    // std::string api_key;
    // std::string secret_key;

    // BinaCPP::init(api_key, secret_key);

    // Json::Value result;
    // BinaCPP::get_klines( "BTCUSDT", "1h", 500, 1603833517, 1761513517, result );

    // std::cout << result.size() << std::endl;
    
    return EXIT_SUCCESS;
}