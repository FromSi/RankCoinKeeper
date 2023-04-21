#!/bin/bash

su -c "mkdir -p $(pwd)/cmake-docker && cmake -S $(pwd) -B $(pwd)/cmake-docker && cd $(pwd)/cmake-docker && make -j4 && cd ../" user

env | grep RANK_COIN_KEEPER_TELEGRAM_BOT_TOKEN_API >> /etc/environment

cron -f
