#include "app/sqlite/ChatModel.h"

namespace app {

    std::string ChatModel::getTableName() const {
        return "chats";
    }

    std::map<int, std::string>& ChatModel::getFields() {
        static std::map<int, std::string> fields {
            {ChatModel::Fields::FIELD_CHAT_ID, "chat_id"},
            {ChatModel::Fields::FIELD_NAME, "name"},
            {ChatModel::Fields::FIELD_USERNAME, "username"},
            {ChatModel::Fields::FIELD_IS_ACTIVE, "is_active"},
            {ChatModel::Fields::FIELD_IS_ADMIN, "is_admin"},
        };

        return fields;
    }

}

