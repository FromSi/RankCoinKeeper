#include <iostream>
#include <string>
#include "tgbot/tgbot.h"
#include "app/app.h"

std::string getHelp();

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

    if (std::strcmp(firstArgument, "-h") == 0 || std::strcmp(firstArgument, "--help") == 0) {
        std::cout << getHelp() << std::endl;
    } else {
        std::cout << "Error: " << argv[1] << " argument not supported" << std::endl;
        std::cout << getHelp() << std::endl;

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
