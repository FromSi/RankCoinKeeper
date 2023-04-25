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
         * @brief Ключи конструкций для составления запросов
         */
        enum StatementKeys {
            WHERE,
            LEFT_JOIN_FOR_WHERE,
            ORDER_BY,
            LIMIT,
            OFFSET,
        };

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
         * @brief Создание или обновление данных
         * @param model
         * @param fields
         * @return
         */
        bool upsert(Model<T>* model, const std::map<int, std::variant<int64_t, std::string>>& fields);

        /**
         * @brief Получить данные
         *
         * @param model
         * @param select
         * @param statementKeys
         * @return
         */
        std::vector<std::map<int, std::variant<int64_t, std::string>>> select(
                Model<T>* model,
                const std::vector<int>& select,
                const std::map<int, std::string>& statementKeys);
    };

    template<class T>
    std::vector<std::map<int, std::variant<int64_t, std::string>>> Query<T>::select(
            Model<T>* model,
            const std::vector<int>& select,
            const std::map<int, std::string>& statementKeys) {
        std::vector<T> result;

        if (!this->isConnection) {
            return result;
        }

        std::stringstream sql;

        {
            sql << "SELECT ";

            if (select.empty()) {
                sql << T::TABLE_NAME << ".* ";
            } else {
                for (size_t i = 0; i < select.size(); i++) {
                    sql << model->getFieldName(select[i]);

                    if (i != select.size() - 1) {
                        sql << ",";
                    }
                }

                sql << " ";
            }

            sql << "FROM " << T::TABLE_NAME << " ";

            if (statementKeys.find(LEFT_JOIN_FOR_WHERE) != statementKeys.end()) {
                sql << statementKeys[LEFT_JOIN_FOR_WHERE] << " ";
            }

            if (statementKeys.find(WHERE) != statementKeys.end()) {
                sql << "WHERE " << statementKeys[WHERE] << " ";
            }

            if (statementKeys.find(ORDER_BY) != statementKeys.end()) {
                sql << "ORDER BY " << statementKeys[ORDER_BY] << " ";
            }

            if (statementKeys.find(LIMIT) != statementKeys.end()) {
                sql << "LIMIT " << statementKeys[LIMIT] << " ";
            }

            if (statementKeys.find(OFFSET) != statementKeys.end()) {
                sql << "OFFSET " << statementKeys[OFFSET] << " ";
            }

            sql << ";";
        }

        sqlite3_stmt *stmt;

        int responseCode = sqlite3_prepare_v2(this->db, sql.str().c_str(), -1, &stmt, nullptr);

        if (responseCode != SQLITE_OK) {
            std::cerr << "Error in sqlite3_prepare_v2: " << sqlite3_errmsg(this->db) << std::endl;

            sqlite3_finalize(stmt);

            return result;
        }

        {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::map<int, std::variant<int64_t, std::string>> item;

                int numSelectColumns = sqlite3_column_count(stmt);

                for (int i = 0; i < numSelectColumns; ++i) {
                    int typeSelectColumn = sqlite3_column_type(stmt, i);
                    int fieldId = model->getFieldId(reinterpret_cast<const char*>(sqlite3_column_name(stmt, i)));

                    switch (typeSelectColumn) {
                        case SQLITE_INTEGER:
                            item[fieldId] = sqlite3_column_int64(stmt, i);
                            break;
                        case SQLITE_TEXT:
                            item[fieldId] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                            break;
                        default:
                            item[fieldId] = "NULL";
                    }
                }

                result.push_back(item);
            }

            sqlite3_finalize(stmt);
        }

        return result;
    }

    template<class T>
    bool Query<T>::upsert(Model<T>* model, const std::map<int, std::variant<int64_t, std::string>>& fields) {
        if (!this->isConnection) {
            return false;
        }

        std::stringstream sql;

        {
            size_t count = fields.size();

            if (count == 0) {
                return false;
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

        sqlite3_stmt *stmt;

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
