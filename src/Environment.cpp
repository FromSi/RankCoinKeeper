#include "app/Environment.h"

namespace app {

    std::string Environment::getSqliteFilePath() {
        return "database.sqlite";
    }

    bool Environment::hasSqliteFilePath() {
        return !getSqliteFilePath().empty();
    }

    std::string Environment::getTelegramBotTokenApi() {
        const char* envVar = std::getenv(TELEGRAM_BOT_TOKEN_API.c_str());

        return (envVar != nullptr) ? envVar : "";
    }

    bool Environment::hasTelegramBotTokenApi() {
        return !getTelegramBotTokenApi().empty();
    }

}
