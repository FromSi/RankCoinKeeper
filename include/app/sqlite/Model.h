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
         * @brief Получить название поля
         * @param field
         * @return
         */
        std::string getFieldName(int field);

        /**
         * @brief Создание или обновление данных
         * @param model
         * @param fields
         * @return
         */
        bool createOrUpdate(const std::map<int, std::variant<int, int64_t, std::string>>& fields);

    protected:
        /**
         * @brief экземпляр класса для запросов
         */
        Query<T> query;

    };

    template<class T>
    bool Model<T>::createOrUpdate(const std::map<int, std::variant<int, int64_t, std::string>>& fields) {
        return this->query.upsert(this, fields);
    }

    template<class T>
    std::string Model<T>::getFieldName(int field) {
        const std::map<int, std::string>& fields = getFields();
        auto it = fields.find(field);

        if (it != fields.end()) {
            return it->second;
        }

        return "UnknownField";
    }

}

#endif
