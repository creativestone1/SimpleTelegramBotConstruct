#pragma once
#include <string>
class IBotObserver {
public:
    virtual ~IBotObserver() = default;
    virtual void onMessage(const std::string& chatId, const std::string& text) = 0;
};
