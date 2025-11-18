#pragma once 

#include "../base/pqxx_adapter.hpp"
#include "../base/binance_client.hpp"
#include "../flatbuffers/klines_generated.h"
#include "../base/sql_loader.hpp"


namespace fhc::server {
    namespace krn = std::chrono;

    using RespFromQuery = std::vector<std::vector<std::string>>;

    struct TimeStamp {
        int year;
        unsigned int month;
        unsigned int day;
    };

    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(std::shared_ptr<base::interface::IStorageAdapter> db_adapter, 
            std::shared_ptr<base::sql_loader::SqlLoader> sql_loader);
        
        void InitDataBase() const;
        void ExecuteQuery(const std::string& query) const;
        bool IsTableExists(const std::string& table) const;
        
        flatbuffers::FlatBufferBuilder GetKlinesFromStorage(const std::string& timestamp_start, const std::string& timestamp_end) const;
        std::string UpdateKlinesInStorage() const;
        
        void PopulateDatabase(TimeStamp timestamp) const;

        ~RequestHandler() = default;
    private:
        std::uint64_t StrToUint64(const std::string& str) const;
        krn::year_month_day StrDateToEpochUnixTime(const std::string& str_date) const;
        std::uint64_t StrStartDateToEpochUnixTime(const std::string& str_start_date) const;
        std::uint64_t StrEndDateToEpochUnixTime(const std::string& str_end_date) const;

        std::vector<base::API::Kline> GetKlines(
            const std::string& symbol, 
            const std::string& interval, 
            uint64_t start_time, 
            uint64_t end_time) const;
            
        std::uint64_t GetTimeFromBinance() const;

        std::uint64_t BinanceTimeToEpochUnixTime(std::uint64_t binance_time) const;
        std::uint64_t EpochUnixTimeToBinanceTime(std::uint64_t unix_time) const;
        std::uint64_t GetTimeSinceEpoch(TimeStamp timestamp) const;

        flatbuffers::FlatBufferBuilder CreateFlatBuffer(const RespFromQuery& resp) const;

    private:
        std::shared_ptr<base::interface::IStorageAdapter> db_adapter_;
        std::shared_ptr<base::sql_loader::SqlLoader> sql_loader_;
        std::unique_ptr<base::API::BinanceClient> binance_client_;
    };
}