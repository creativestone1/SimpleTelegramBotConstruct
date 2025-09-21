#pragma once
#include "BotApi.h"
#include <thread>
#include <chrono>
class MyBotLogic : public IBotObserver {
public:
    MyBotLogic(BotApi& api) : _api(api) {}
	
	int loop(){	
        _api.receiveMessages();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return 0;
	}

    void onMessage(const std::string& chatId, const std::string& text) override {
        //std::cout << "Logic received: " << text << std::endl;

        if (text == "/start") {
            _api.sendMessage(chatId, "Привет! Я бот.");
        } else {
            _api.sendMessage(chatId, "Вы сказали: " + text);
        }
    }

private:
    BotApi& _api;
};
