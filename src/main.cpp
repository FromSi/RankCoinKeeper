#include <iostream>
#include <string>
#include <sstream>
#include "tgbot/tgbot.h"
#include "app/app.h"

std::string getHelp();
void handlerMemberCounts(TgBot::Bot&);
void handlerUpdates(TgBot::Bot&);

/**
 * @brief Основная точка входа
 * @param argc Количество аргументов
 * @param argv Аргументы для работы с программой
 * @return
 */
int main(int argc, char *argv[]) {
    if (!app::Environment::hasTelegramBotTokenApi() || !app::Environment::hasSqliteFilePath()) {
        std::cout << "Error: empty ENVs" << std::endl;

        return EXIT_FAILURE;
    }

    if (argc < 2) {
        std::cout << "Error: No arguments" << std::endl;
        std::cout << getHelp() << std::endl;

        return EXIT_FAILURE;
    }

    char *firstArgument = argv[1];

    TgBot::Bot tgBot(app::Environment::getTelegramBotTokenApi());

    try {
        if (std::strcmp(firstArgument, "-h") == 0 || std::strcmp(firstArgument, "--help") == 0) {
            std::cout << getHelp() << std::endl;
        } else if (std::strcmp(firstArgument, "-mc") == 0 || std::strcmp(firstArgument, "--member_counts") == 0) {
            handlerMemberCounts(tgBot);
        }  else if (std::strcmp(firstArgument, "-u") == 0 || std::strcmp(firstArgument, "--updates") == 0) {
            handlerUpdates(tgBot);
        } else {
            std::cout << "Error: " << argv[1] << " argument not supported" << std::endl;
            std::cout << getHelp() << std::endl;

            return EXIT_FAILURE;
        }
    } catch (const TgBot::TgException& exception) {
        std::cout << "ErrorTG: " << exception.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Справочная информация по проекту
 */
std::string getHelp() {
    return std::string("Support arguments:\n")
        + "\t-h --help - Information on the project\n"
        + "\t-mc --member_counts - Get the number of members in chats\n"
        + "\t-u --updates - Get new updates\n";
}

/**
 * @brief Обработчик аргумента: -mc --member_counts
 * @param tgBot Телеграм бот
 */
void handlerMemberCounts(TgBot::Bot& tgBot) {
    app::ChatModel chatModel;

    std::vector<std::map<int, std::variant<int64_t, std::string>>> chats = chatModel.all({
        app::ChatModel::Fields::FIELD_CHAT_ID,
    },{});

    for (const std::map<int, std::variant<int64_t, std::string>>& chat : chats) {
        std::visit([&tgBot, &chatModel](auto&& chatId) {
            int64_t memberCount = tgBot.getApi().getChatMemberCount(chatId);

            std::stringstream where;

            where << chatModel.getFieldName(app::ChatModel::Fields::FIELD_CHAT_ID) << " " << chatId;

            chatModel.update(
                {{app::ChatModel::Fields::FIELD_MEMBER_COUNT, memberCount}},
                where.str()
            );
        }, chat.at(app::ChatModel::Fields::FIELD_CHAT_ID));
    }
}

/**
 * Обработчик аргумента: -u --updates
 * @param tgBot Телеграм бот
 */
void handlerUpdates(TgBot::Bot& tgBot) {
    TgBot::TgLongPoll tgLongPoll(tgBot);

    tgBot.getEvents().onMyChatMember([&tgBot](const TgBot::ChatMemberUpdated::Ptr& message){
        if (message->chat->type != TgBot::Chat::Type::Supergroup) {
            return;
        }

        int64_t tgBotId = tgBot.getApi().getMe()->id;

        if (message->newChatMember->user->id != tgBotId) {
            return;
        }

        int64_t chatId = message->chat->id;

        std::string chatName = (!message->chat->title.empty()) ? message->chat->title : "empty";

        std::string chatUsername = (!message->chat->username.empty()) ? message->chat->username : "empty";

        bool isActive = message->newChatMember->status != TgBot::ChatMemberLeft::STATUS
                && message->newChatMember->status != TgBot::ChatMemberBanned::STATUS;

        bool isAdmin = message->newChatMember->status == TgBot::ChatMemberAdministrator::STATUS
                && dynamic_cast<TgBot::ChatMemberAdministrator*>(message->newChatMember.get())->canRestrictMembers;

        app::ChatModel chatModel;

        chatModel.createOrUpdate({
            {app::ChatModel::Fields::FIELD_CHAT_ID, chatId},
            {app::ChatModel::Fields::FIELD_NAME, chatName},
            {app::ChatModel::Fields::FIELD_USERNAME, chatUsername},
            {app::ChatModel::Fields::FIELD_IS_ACTIVE, isActive},
            {app::ChatModel::Fields::FIELD_IS_ADMIN, isAdmin},
        });
    });

    while (true) {
        tgLongPoll.start();
    }
}
