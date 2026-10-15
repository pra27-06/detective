#include "GameEngine.h"
#include "TerminalUI.h"
#include <iostream>
#include <exception>

int main() {
    try {
        GameEngine engine;
        engine.run();
    } catch (const std::exception& ex) {
        UI::printError(std::string("Fatal Engine Exception: ") + ex.what());
        return 1;
    } catch (...) {
        UI::printError("An unknown fatal runtime error occurred.");
        return 1;
    }
    return 0;
}
