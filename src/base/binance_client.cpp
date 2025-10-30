#include "../../include/base/binance_client.hpp"

namespace fhc::base::API {
    BinanceClient::BinanceClient(const std::string& api_key, const std::string& secret_key) 
    : api_key_{api_key}
    , secret_key_{secret_key_} 
    {
        BinaCPP::init(api_key_, secret_key_);
    }

    std::vector<Kline> BinanceClient::GetKlines(const std::string& symbol, const std::string& interval, uint64_t start_time, uint64_t end_time) const {
        Json::Value parse_result;
        BinaCPP::get_klines(symbol.data(), interval.data(), 1000, start_time, end_time, parse_result);

        // TODO: Need process in future
        if (parse_result.empty()) {
            return {};
        }

        std::vector<Kline> result_klines;
        for (const auto& arr : parse_result) {
            if (arr.empty()) {
                // TODO: It is adding error message in future
                continue;
            }
            result_klines.push_back(ReturnKline(symbol, arr));
        }
        return result_klines;
    }

     std::string BinanceClient::GetServerTime() const {
        Json::Value result;
        BinaCPP::get_serverTime(result);

        return result["serverTime"].asString();
     }

    Kline BinanceClient::ReturnKline(const std::string& symbol, const Json::Value& value) const {
        Kline kline;
        
        // TODO: Scary expressions
        kline.symbol = symbol;
        kline.open_time = std::stoull(value[0].asString());
        kline.open = std::stod(value[1].asString());
        kline.high = std::stod(value[2].asString());
        kline.low = std::stod(value[3].asString());
        kline.close = std::stod(value[4].asString());
        kline.volume = std::stod(value[5].asString());
        kline.close_time = std::stoull(value[6].asString());
        kline.quote_asset_volume = std::stod(value[7].asString());
        kline.trades_count = std::stoi(value[8].asString());
        kline.taker_buy_base_volume = std::stod(value[9].asString());
        kline.taker_buy_quote_volume = std::stod(value[10].asString());

        return kline;
    }
}