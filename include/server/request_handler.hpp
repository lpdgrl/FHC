#pragma once 

#include "../base/pqxx_adapter.hpp"
#include "../base/binance_client.hpp"
#include "../flatbuffers/klines_generated.h"

namespace fhc::server {

    struct TimeStamp {
        int year;
        unsigned int month;
        unsigned int day;
    };

    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(std::shared_ptr<fhc::base::interface::IStorageAdapter> db_adapter);

        void ExecuteQuery(const std::string& query) const;
        
        std::vector<fhc::base::API::Kline> GetKlines(const std::string& symbol, const std::string& interval, uint64_t start_time, uint64_t end_time) const;

        void SendToFront(TimeStamp timestamp_start, TimeStamp timestamp_end) const;

        void PopulateDatabase() const;

        ~RequestHandler() = default;
    private:
        std::uint64_t GetTimeFromBinance() const;
        std::uint64_t BinanceTimeToEpochUnixTime(std::uint64_t binance_time) const;
        std::uint64_t EpochUnixTimeToBinanceTime(std::uint64_t unix_time) const;
        std::uint64_t GetTimeSinceEpoch(TimeStamp timestamp) const;
        
        std::shared_ptr<fhc::base::interface::IStorageAdapter> db_adapter_;
        std::unique_ptr<fhc::base::API::BinanceClient> binance_client_;
    };
}