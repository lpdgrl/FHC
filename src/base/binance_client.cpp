#include "../../include/base/binance_client.hpp"

namespace fhc::base::API {
    BinanceClient::BinanceClient(const std::string& api_key, const std::string& secret_key) 
    : api_key_{api_key}
    , secret_key_{secret_key_} 
    {
        BinaCPP::init(api_key_, secret_key_);
    }

    std::vector<Kline> BinanceClient::GetKlines(const std::string& symbol, uint64_t start_time, uint64_t end_time) const {
        Json::Value result;
        BinaCPP::get_klines(symbol.data(), "1s", 1000, start_time, end_time, result );
        // std::cout << result << std::endl;
        // std::cout << result.size() << std::endl;

        for (const auto& arr : result) {
            std::cout << arr[0] << std::endl;
        }
        return {};
    }
}