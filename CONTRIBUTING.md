# Contributing
Описание для GNU/Linux дистрибутивов имеющих [apt](https://salsa.debian.org/apt-team/apt). Например: Debian, Mint, Ubuntu, etc. 

## Установка
> Нужен [docker](https://docs.docker.com/get-started/) без использования [sudo](https://github.com/sudo-project/sudo).

Выполнить команду:
```
sudo apt update && sudo apt install git make
```

Выполнить команды в корне проекта:
```
sudo apt update 
sudo apt install g++ binutils cmake libboost-system-dev libssl-dev zlib1g-dev libcurl4-openssl-dev libsqlite3-dev
make dev_init && make dev_build
sudo rm -rf /var/lib/apt/lists/*
```

Добавить переменную окружения (допустим в `~/.bashrc`).
```
# API-токен бота
export RANK_COIN_KEEPER_TELEGRAM_BOT_TOKEN_API="token"
```

Для работы на CLion нужно настроить:
```
File | Settings | Build, Execution, Deployment | CMake
```

В `Build directory` введи директорию `cmake`. 

Зайди в файл CMakeLists.txt и загрузи его для IDE. 

В конфигурации запуска нужно указать ENV системы и аргументы программы. А так же рабочую директорию.

<p align="center">
  <img src="https://github.com/FromSi/RankCoinKeeper/blob/master/screenshots/run_config.jpg" height="400"/>
</p>

## Запуск
* Либо запускать через IDE 
* Либо запустить команду `make dev_build && ./cmake/app` + `[OPTIONS COMMANDS]`

## Все кейсы запуска программы
* `make dev_build && ./cmake/app` - запуск без аргументов (вызовется -h --help)
* `make dev_build && ./cmake/app -h` или `--help` - справочная информация по проекту
* `make dev_build && ./cmake/app -u` или `--updates` - обработка новых сообщений