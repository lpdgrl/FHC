#include "../../include/base/pqxx_adaptor.hpp"

namespace fhc::base::interface {
        void PQXXAdaptor::ConnectPQXX(const std::string& options) {
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

        void PQXXAdaptor::DisconnectPQXX() {
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

        void PQXXAdaptor::Execute(const std::string& query) {
            pqxx::work txn(*connection_);

            pqxx::result r = txn.exec(query);
            txn.commit();
        }

        std::vector<std::vector<std::string>> PQXXAdaptor::Query(const std::string& query) {
            // TODO: Switch to nontransaction object
            std::vector<std::vector<std::string>> result;
            pqxx::work txn(*connection_);

            pqxx::result r = txn.exec(query);
            txn.commit();

            if (r.size() > 0) {
                result.reserve(r.size());
            }

            
            for (size_t i = 0; i < r.size(); ++i) {
                std::vector<std::string> row;
                for (size_t j = 0; j < r[i].size(); ++j) {
                   row.emplace_back(r[i][j].c_str());
                }
                result.push_back(std::move(row));
            }

            return result;
        }

        // raw connection. This method is testing
        pqxx::connection& PQXXAdaptor::GetConnection() {
            return *connection_;
        }

        bool PQXXAdaptor::IsConnected() const {
            return connection_ && connection_->is_open();
        }
}