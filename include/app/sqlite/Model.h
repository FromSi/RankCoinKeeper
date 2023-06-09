#ifndef APP_MODEL_H
#define APP_MODEL_H

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "sqlite3.h"
#include "Query.h"

namespace app {

    /**
     * @brief Абстракция для описания моделей. Модели "сериализуют" таблицы
     * @tparam T
     */
    template<class T>
    class Model {

    public:
        /**
         * @brief Конструктор по умолчанию
         */
        explicit Model() = default;

        /**
         * @brief Деструктор по умолчанию
         */
        ~Model() = default;

        /**
         * @brief Название таблицы для модели
         */
        virtual std::string getTableName() const = 0;

        /**
         * @brief Список полей таблицы
         * @return
         */
        virtual std::map<int, std::string>& getFields() = 0;

        /**
         * @brief Получить id поля
         * @param field
         * @return
         */
        int getFieldId(const std::string& fieldName);

        /**
         * @brief Получить название поля
         * @param field
         * @return
         */
        std::string getFieldName(const int& fieldId);

        /**
         * @brief Получить данные
         *
         * @param select
         * @param statementKeys
         * @return
         */
        std::vector<std::map<int, std::variant<int64_t, std::string>>> all(
                const std::vector<int>& select,
                const std::map<int, std::string>& statementKeys);

        /**
         * @brief Создание или обновление данных
         * @param model
         * @param fields
         * @return
         */
        bool createOrUpdate(const std::map<int, std::variant<int64_t, std::string>>& fields);

        /**
         * @brief Обновление данных
         * @param model
         * @param set
         * @param where
         * @return
         */
        bool update(const std::map<int, std::variant<int64_t, std::string>>& set, const std::string& where);

    protected:
        /**
         * @brief экземпляр класса для запросов
         */
        Query<T> query;

    };

    template<class T>
    std::vector<std::map<int, std::variant<int64_t, std::string>>> Model<T>::all(
            const std::vector<int>& select,
            const std::map<int, std::string>& statementKeys) {
        return this->query.select(this, select, statementKeys);
    }

    template<class T>
    bool Model<T>::update(const std::map<int, std::variant<int64_t, std::string>>& set, const std::string& where) {
        return this->query.update(this, set, where);
    }

    template<class T>
    bool Model<T>::createOrUpdate(const std::map<int, std::variant<int64_t, std::string>>& fields) {
        return this->query.upsert(this, fields);
    }

    template<class T>
    int Model<T>::getFieldId(const std::string& fieldName) {
        const std::map<int, std::string>& fields = getFields();

        for (const auto& field : fields) {
            if (field.second == fieldName) {
                return field.first;
            }
        }

        return -1;
    }

    template<class T>
    std::string Model<T>::getFieldName(const int& fieldId) {
        const std::map<int, std::string>& fields = getFields();
        auto it = fields.find(fieldId);

        if (it != fields.end()) {
            return it->second;
        }

        return "UnknownField";
    }

}

#endif
