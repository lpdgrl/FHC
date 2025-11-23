#include "../../include/base/pqxx_adapter.hpp"

namespace fhc::base::interface {
        void PqxxAdapter::Connect(const std::string& options) {
            // TODO: Add throw and catch exceptions when options empty
            if (options.empty()) {
                return;
            }

            // TODO: Add throw and catch exceptions when unique_ptr is nullptr
            // if (connection_ == nullptr) {
            //     return;
            // }

            try {
                connection_ = std::make_unique<pqxx::connection>(options);
                if (connection_->is_open()) {
                    std::cout << "Connected to database successfully \n";
                }
             } catch (const std::exception &e) {
                std::cerr << "Connection failed: " << e.what() << "\n";
            }
        }

        void PqxxAdapter::Disconnect() {
            // TODO: Add throw and catch exceptions when unique_ptr is nullptr
            if (connection_ == nullptr) {
                return;
            }

            // TODO: Add throw and catch exceptions when connection is closed
            connection_->close();
            if (connection_ == nullptr) {
                std::cerr << "Connection to database is closing successfully \n";
            }
        }

        void PqxxAdapter::Execute(const std::string& query) const {
            pqxx::work txn(*connection_);

            pqxx::result r = txn.exec(query);
            txn.commit();
        }

        bool PqxxAdapter::IsConnected() const {
            return connection_ && connection_->is_open();
        }

        std::vector<std::vector<std::string>> PqxxAdapter::PqxxResultToVectorStr(pqxx::result& result) const {
            std::vector<std::vector<std::string>> res;

            if (result.size() > 0) {
                res.reserve(result.size());
            }
            
            for (size_t i = 0; i < result.size(); ++i) {
                std::vector<std::string> row;
                for (size_t j = 0; j < result[i].size(); ++j) {
                   row.emplace_back(result[i][j].as<std::string>());
                }
                res.push_back(std::move(row));
            }
            return res;
        }
}