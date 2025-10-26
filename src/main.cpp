#include "binacpp.h"
#include "binacpp_websocket.h"

#include <json/json.h>
#include <iostream>
#include <string>
#include <chrono>

int main() {
    std::string api_key;
    std::string secret_key;

    BinaCPP::init(api_key, secret_key);

    Json::Value result;
    BinaCPP::get_klines( "BTCUSDT", "1s", 0, 0, 0, result );

    std::cout << result.size() << std::endl;
    
    return EXIT_SUCCESS;
}