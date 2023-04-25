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
         * @return
         */
        std::string getTableName() const override;

        /**
         * @return
         */
        std::map<int, std::string>& getFields() override;

    };

}

#endif
