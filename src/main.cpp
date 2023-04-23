#include <iostream>
#include <string>
#include <map>
#include "tgbot/tgbot.h"
#include "app/app.h"

std::string getHelp();
void handlerUpdates();

/**
 * @brief Основная точка входа
 * @param argc Количество аргументов
 * @param argv Аргументы для работы с программой
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error: No arguments" << std::endl;
        std::cout << getHelp() << std::endl;

        return EXIT_FAILURE;
    }

    char *firstArgument = argv[1];

    try {
        if (std::strcmp(firstArgument, "-h") == 0 || std::strcmp(firstArgument, "--help") == 0) {
            std::cout << getHelp() << std::endl;
        } else if (std::strcmp(firstArgument, "-u") == 0 || std::strcmp(firstArgument, "--updates") == 0) {
            handlerUpdates();
        } else {
            std::cout << "Error: " << argv[1] << " argument not supported" << std::endl;
            std::cout << getHelp() << std::endl;

            return EXIT_FAILURE;
        }
    } catch (const TgBot::TgException& exception) {
        std::cout << "ErrorTG: " << exception.what();

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * Справочная информация по проекту
 *
 * @return
 */
std::string getHelp() {
    return std::string("Support arguments:\n")
        + "\t-h --help - Information on the project";
}

/**
 * Обработчик аргумента: -u --updates
 */
void handlerUpdates() {
    if (!app::Environment::hasTelegramBotTokenApi() || !app::Environment::hasSqliteFilePath()) {
        std::cout << "Error: empty ENVs" << std::endl;

        return;
    }

    TgBot::Bot tgBot(app::Environment::getTelegramBotTokenApi());

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
                                         {app::ChatModel::FIELD_CHAT_ID,   chatId},
                                         {app::ChatModel::FIELD_NAME,      chatName},
                                         {app::ChatModel::FIELD_USERNAME,  chatUsername},
                                         {app::ChatModel::FIELD_IS_ACTIVE, isActive},
                                         {app::ChatModel::FIELD_IS_ADMIN,  isAdmin},
                                 });
    });

    while (true) {
        tgLongPoll.start();
    }
}
