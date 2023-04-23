#ifndef APP_QUERY_H
#define APP_QUERY_H

#include "app/sqlite/DB.h"

#include "sqlite3.h"
#include <vector>
#include <map>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>

namespace app {

    template<class T>
    class Model;

    /**
     * @brief Главный контроллер для запросов между пользователей, моделью и БД
     * @tparam T
     */
    template<class T>
    class Query : public DB {

    public:
        /**
         * @brief Конструктор по умолчанию
         * @param dbName
         */
        explicit Query(const char* dbName = "database.sqlite") : DB(dbName) { }

        /**
         * @brief Деструктор по умолчанию
         */
        ~Query() = default;

        /**
         * @brief Составление или обновление данных
         * @param model
         * @param fields
         * @return
         */
        bool upsert(Model<T>* model, const std::map<int, std::variant<int, int64_t, std::string>>& fields);

    };

    template<class T>
    bool Query<T>::upsert(Model<T>* model, const std::map<int, std::variant<int, int64_t, std::string>>& fields) {
        sqlite3_stmt *stmt;

        std::stringstream sql;

        {
            size_t count = fields.size();

            if (count == 0) {
                return "";
            }

            sql << "INSERT OR REPLACE INTO " << model->getTableName() << "(";

            {
                size_t i = 0;

                for (const auto& field : fields) {
                    if (i != 0) {
                        sql << ",";
                    }

                    sql << model->getFieldName(field.first);

                    i++;
                }
            }

            sql << ") VALUES (";

            for (size_t i = 0; i < count; i++) {
                if (i != 0) {
                    sql << ",";
                }

                sql << "?";
            }

            sql << ");";
        }

        int responseCode = sqlite3_prepare_v2(this->db, sql.str().c_str(), -1, &stmt, nullptr);

        if (responseCode != SQLITE_OK) {
            std::cerr << "Error in sqlite3_prepare_v2: " << sqlite3_errmsg(this->db) << std::endl;
            sqlite3_finalize(stmt);

            return false;
        }

        {
            size_t i = 0;

            for (const auto& field : fields) {
                i++;

                std::visit([&stmt, &i](auto&& value) {
                    using V = std::decay_t<decltype(value)>;

                    if constexpr (std::is_same_v<V, int>) {
                        sqlite3_bind_int(stmt, i, value);
                    } else if constexpr (std::is_same_v<V, int64_t>) {
                        sqlite3_bind_int64(stmt, i, value);
                    } else if constexpr (std::is_same_v<V, std::string>) {
                        sqlite3_bind_text(stmt, i, value.c_str(), static_cast<int>(value.size()), SQLITE_STATIC);
                    }
                }, field.second);
            }
        }

        responseCode = sqlite3_step(stmt);

        sqlite3_finalize(stmt);

        return responseCode == SQLITE_DONE;
    }

}

#endif
