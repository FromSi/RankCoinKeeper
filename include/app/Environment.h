#ifndef APP_ENVIRONMENT_H
#define APP_ENVIRONMENT_H

#include <cstdlib>
#include <string>
#include <cstring>

namespace app {

    /**
     * @brief Работа с переменными окружения
     */
    class Environment {

    public:
        /**
         * @brief Ключ переменной для телеграм API-токен бота
         */
        inline static const std::string TELEGRAM_BOT_TOKEN_API = "RANK_COIN_KEEPER_TELEGRAM_BOT_TOKEN_API";

        /**
         * @brief Конструктор по умолчанию
         */
        explicit Environment() = default;

        /**
         * @brief Деструктор по умолчанию
         */
        ~Environment() = default;

        /**
         * @brief Получение телеграм API-токен бота
         * @return
         */
        static std::string getTelegramBotTokenApi();

        /**
         * @brief Проверка на существование значения телеграм API-токен бота
         * @return
         */
        static bool hasTelegramBotTokenApi();

        /**
         * @brief Получение пути до sqlite3 файла
         * @return
         */
        static std::string getSqliteFilePath();

        /**
         * @brief Проверка на существование значения пути до sqlite3 файла
         * @return
         */
        static bool hasSqliteFilePath();

    };

}

#endif
