#pragma once
#include <curl/curl.h>
#include <iostream>
#include "json.hpp"
#include "IBotObserver.h"
using json = nlohmann::json;

class BotApi {
public:
    BotApi(const std::string& token) : _token(token), _offset(0){
        _curl = curl_easy_init();
    }
	void setObserver(IBotObserver* observer){
		_observer = observer;
	}

    ~BotApi() {
        if (_curl) curl_easy_cleanup(_curl);
    }


    void sendMessage(const std::string& chatId, const std::string& text) {
        if (!_curl) 
			throw "sendMessage error! << curl not inited\n";

        curl_easy_reset(_curl);
        std::string url = "https://api.telegram.org/bot" + _token + "/sendMessage";
        std::string postFields = "chat_id=" + chatId + "&text=" + curl_easy_escape(_curl, text.c_str(), text.length());

        curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, postFields.c_str());

        CURLcode res = curl_easy_perform(_curl);
        if (res != CURLE_OK) {
            std::cerr << "Error curl: " << curl_easy_strerror(res) << std::endl;
			throw "sendMessage error! << curl is not ok\n";
        }
    }

    void receiveMessages() {
        if (!_curl || !_observer)
			throw "receiveMessages error! << curl | observer not inited\n";

        curl_easy_reset(_curl);
        std::string url = "https://api.telegram.org/bot" + _token + "/getUpdates?offset=" + std::to_string(_offset);

        std::string response;
        curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(_curl);
        if (res != CURLE_OK) {
            std::cerr << "Error getting updates: " << curl_easy_strerror(res) << std::endl;
			throw "receiveMessages error! << curl is not ok\n";
        }

        try {
            auto json = nlohmann::json::parse(response);
            for (const auto& update : json["result"]) {
                _offset = update["update_id"].get<int>() + 1;

                std::string chatId = std::to_string(update["message"]["chat"]["id"].get<int>());
                std::string text = update["message"]["text"].get<std::string>();
				
                _observer->onMessage(chatId, text); // делегируем обработку
            }
        } catch (...) {
            std::cerr << "Error parsing JSON\n";
			throw "Error parsing JSON\n";
        }
    }

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append((char*)contents, totalSize);
        return totalSize;
    }

private:
    CURL* _curl;
    std::string _token;
    int _offset;
    IBotObserver* _observer;
};
