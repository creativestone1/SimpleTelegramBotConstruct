#include "BotLogic.h"
#include <fstream>
int main() {
	std::ifstream inCfg;
	inCfg.open("token.cfg");
	std::string token;
	std::getline(inCfg, token);

    BotApi api(token);
    MyBotLogic logic(api);
	api.setObserver(&logic);

	std::cout<< "bot inited\n";

    while (true) {
		logic.loop();
    }
}
