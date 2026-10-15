#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include <string>
#include <vector>

namespace UI {

    // Color mode state
    void setColorEnabled(bool enabled);
    bool isColorEnabled();

    // Terminal text formatting and colors
    struct Color {
        static std::string RESET;
        static std::string BOLD;
        static std::string RED;
        static std::string GREEN;
        static std::string YELLOW;
        static std::string BLUE;
        static std::string MAGENTA;
        static std::string CYAN;
        static std::string WHITE;
        static std::string BRIGHT_CYAN;
        static std::string BRIGHT_YELLOW;
        static std::string BRIGHT_RED;
        static std::string BRIGHT_GREEN;
        static std::string DIM;
    };

    // Initialize terminal (enables virtual terminal processing on Windows if available)
    void initTerminal();

    // Screen clearing and navigation
    void clearScreen();
    void pause();

    // Box and banner drawing (using 100% portable ASCII characters)
    void printBanner();
    void printHeader(const std::string& title);
    void printSubHeader(const std::string& subtitle);
    void printDivider(char symbol = '-', int length = 76);
    void printDoubleDivider(int length = 76);
    void printBox(const std::string& title, const std::vector<std::string>& lines, int width = 76);

    // Status notifications
    void printSuccess(const std::string& msg);
    void printError(const std::string& msg);
    void printWarning(const std::string& msg);
    void printInfo(const std::string& msg);
    void printHint(const std::string& msg);

    // Input handlers with validation
    int promptInt(const std::string& prompt, int minVal, int maxVal);
    std::string promptString(const std::string& prompt, bool allowEmpty = false);
    bool promptConfirm(const std::string& prompt);

    // Text formatting utilities
    std::string wrapText(const std::string& text, size_t maxWidth);
    std::string toUpper(const std::string& str);
    std::string trim(const std::string& str);
}

#endif // TERMINAL_UI_H
