#ifndef APP_CHATMODEL_H
#define APP_CHATMODEL_H

#include "Model.h"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "sqlite3.h"

namespace app {

    /**
     * @brief Модель таблицы chats
     */
    class ChatModel : public Model<ChatModel> {

    public:
        /**
         * @brief Название таблицы для модели
         */
        inline static const std::string TABLE_NAME = "chats";

        /**
         * @brief Список полей из таблицы
         */
        enum Fields {
            /** @brief ID телеграм чата */
            FIELD_CHAT_ID,
            /** @brief Название чата */
            FIELD_NAME,
            /** @brief Username чата */
            FIELD_USERNAME,
            /** @brief Активность в чате */
            FIELD_IS_ACTIVE,
            /** @brief Права администратора */
            FIELD_IS_ADMIN,
        };

        /**
         * @brief Конструктор по умолчанию. Полезен при простом создании модели
         */
        explicit ChatModel() : Model() { }

        /**
         * @brief Деструктор по умолчанию
         */
        ~ChatModel() = default;

        /**
         * @return
         */
        std::map<int, std::string>& getFields() override;

    };

}

#endif
