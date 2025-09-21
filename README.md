# SimpleTelegramBotConstruct

## build:
```bash
g++ main.cpp -obot -lcurl
```

## launch:
1) add the config file: token.cfg
2) paste token into token.cfg
3) run ./bot

## description:

### BotApi.h:
low-level api with bot

### BotLogic.h
bot business logic - in this case message processing and work with BotApi

### IBotObserver.h
Observer for BotLogic 

### main.cpp
entry point to the program

### json.hpp
header for work with json
