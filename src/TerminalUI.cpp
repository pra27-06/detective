#include "TerminalUI.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#endif

namespace UI {

    static bool s_colorEnabled = true;

    std::string Color::RESET         = "\033[0m";
    std::string Color::BOLD          = "\033[1m";
    std::string Color::RED           = "\033[31m";
    std::string Color::GREEN         = "\033[32m";
    std::string Color::YELLOW        = "\033[33m";
    std::string Color::BLUE          = "\033[34m";
    std::string Color::MAGENTA       = "\033[35m";
    std::string Color::CYAN          = "\033[36m";
    std::string Color::WHITE         = "\033[37m";
    std::string Color::BRIGHT_CYAN   = "\033[96m";
    std::string Color::BRIGHT_YELLOW = "\033[93m";
    std::string Color::BRIGHT_RED    = "\033[91m";
    std::string Color::BRIGHT_GREEN  = "\033[92m";
    std::string Color::DIM           = "\033[2m";

    void setColorEnabled(bool enabled) {
        s_colorEnabled = enabled;
        if (!enabled) {
            Color::RESET         = "";
            Color::BOLD          = "";
            Color::RED           = "";
            Color::GREEN         = "";
            Color::YELLOW        = "";
            Color::BLUE          = "";
            Color::MAGENTA       = "";
            Color::CYAN          = "";
            Color::WHITE         = "";
            Color::BRIGHT_CYAN   = "";
            Color::BRIGHT_YELLOW = "";
            Color::BRIGHT_RED    = "";
            Color::BRIGHT_GREEN  = "";
            Color::DIM           = "";
        } else {
            Color::RESET         = "\033[0m";
            Color::BOLD          = "\033[1m";
            Color::RED           = "\033[31m";
            Color::GREEN         = "\033[32m";
            Color::YELLOW        = "\033[33m";
            Color::BLUE          = "\033[34m";
            Color::MAGENTA       = "\033[35m";
            Color::CYAN          = "\033[36m";
            Color::WHITE         = "\033[37m";
            Color::BRIGHT_CYAN   = "\033[96m";
            Color::BRIGHT_YELLOW = "\033[93m";
            Color::BRIGHT_RED    = "\033[91m";
            Color::BRIGHT_GREEN  = "\033[92m";
            Color::DIM           = "\033[2m";
        }
    }

    bool isColorEnabled() {
        return s_colorEnabled;
    }

    void initTerminal() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    void clearScreen() {
        if (s_colorEnabled) {
            std::cout << "\033[2J\033[H";
        } else {
            std::cout << "\n\n========================================================================\n\n";
        }
        std::cout.flush();
    }

    void pause() {
        std::cout << "\n" << Color::DIM << ">> Press Enter to continue..." << Color::RESET;
        std::cin.clear();
        std::string dummy;
        if (!std::getline(std::cin, dummy)) {
            if (std::cin.eof()) {
                std::cout << "\n[Session terminated via EOF]\n";
                std::exit(0);
            }
        }
    }

    void printBanner() {
        std::cout << Color::BRIGHT_CYAN << Color::BOLD
                  << "  ========================================================================\n"
                  << "        ____    _    ____  _____   ____ _     ___  ____  _____ ____           \n"
                  << "       / ___|  / \\  / ___|| ____| / ___| |   / _ \\/ ___|| ____|  _ \\      \n"
                  << "      | |     / _ \\ \\___ \\|  _|  | |   | |  | | | \\___ \\|  _| | | | |     \n"
                  << "      | |___ / ___ \\ ___) | |___ | |___| |__| |_| |___) | |___| |_| |     \n"
                  << "       \\____/_/   \\_\\____/|_____| \\____|_____\\___/|____/|_____|____/      \n"
                  << "  ========================================================================\n"
                  << "       CASE CLOSED  ::  FORENSIC & DEDUCTIVE MYSTERY SIMULATOR           \n"
                  << "       College OOPs Capstone  |  Standard C++17 Standard Library Only     \n"
                  << "  ========================================================================\n"
                  << Color::RESET;
    }

    void printDivider(char symbol, int length) {
        std::cout << Color::DIM;
        for (int i = 0; i < length; ++i) {
            std::cout << symbol;
        }
        std::cout << Color::RESET << "\n";
    }

    void printDoubleDivider(int length) {
        printDivider('=', length);
    }

    void printHeader(const std::string& title) {
        std::cout << "\n" << Color::BRIGHT_YELLOW << Color::BOLD;
        std::cout << ">>> [ " << title << " ]" << Color::RESET << "\n";
        printDivider('-', 76);
    }

    void printSubHeader(const std::string& subtitle) {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "-- " << subtitle << " --" << Color::RESET << "\n";
    }

    void printBox(const std::string& title, const std::vector<std::string>& lines, int width) {
        std::cout << "\n" << Color::CYAN << "+";
        for (int i = 0; i < width - 2; ++i) std::cout << "-";
        std::cout << "+\n";

        // Title line
        std::string titleStr = " " + title + " ";
        int padLeft = 2;
        int padRight = width - 2 - padLeft - static_cast<int>(titleStr.length());
        if (padRight < 0) padRight = 0;

        std::cout << "|  " << Color::BOLD << Color::BRIGHT_YELLOW << titleStr << Color::RESET << Color::CYAN;
        for (int i = 0; i < padRight - 2; ++i) std::cout << " ";
        std::cout << "|\n";

        std::cout << "+";
        for (int i = 0; i < width - 2; ++i) std::cout << "-";
        std::cout << "+\n" << Color::RESET;

        // Content lines
        for (const auto& line : lines) {
            std::string wrapped = wrapText(line, width - 4);
            std::istringstream iss(wrapped);
            std::string subline;
            while (std::getline(iss, subline)) {
                std::cout << Color::CYAN << "| " << Color::RESET;
                std::cout << subline;
                int spaceCount = width - 4 - static_cast<int>(subline.length());
                if (spaceCount < 0) spaceCount = 0;
                for (int i = 0; i < spaceCount; ++i) std::cout << " ";
                std::cout << Color::CYAN << " |\n" << Color::RESET;
            }
        }

        std::cout << Color::CYAN << "+";
        for (int i = 0; i < width - 2; ++i) std::cout << "-";
        std::cout << "+\n" << Color::RESET;
    }

    void printSuccess(const std::string& msg) {
        std::cout << Color::BRIGHT_GREEN << Color::BOLD << "[SUCCESS] " << Color::RESET 
                  << Color::GREEN << msg << Color::RESET << "\n";
    }

    void printError(const std::string& msg) {
        std::cout << Color::BRIGHT_RED << Color::BOLD << "[ERROR] " << Color::RESET 
                  << Color::RED << msg << Color::RESET << "\n";
    }

    void printWarning(const std::string& msg) {
        std::cout << Color::BRIGHT_YELLOW << Color::BOLD << "[WARNING] " << Color::RESET 
                  << Color::YELLOW << msg << Color::RESET << "\n";
    }

    void printInfo(const std::string& msg) {
        std::cout << Color::BRIGHT_CYAN << Color::BOLD << "[INFO] " << Color::RESET 
                  << Color::WHITE << msg << Color::RESET << "\n";
    }

    void printHint(const std::string& msg) {
        std::cout << Color::MAGENTA << Color::BOLD << "[HINT] " << Color::RESET 
                  << Color::MAGENTA << msg << Color::RESET << "\n";
    }

    int promptInt(const std::string& prompt, int minVal, int maxVal) {
        while (true) {
            std::cout << Color::BOLD << Color::WHITE << prompt 
                      << " (" << minVal << "-" << maxVal << "): " << Color::RESET;
            std::string input;
            if (!std::getline(std::cin, input)) {
                if (std::cin.eof()) {
                    std::cout << "\n[Session terminated via EOF]\n";
                    std::exit(0);
                }
                std::cin.clear();
                return minVal;
            }
            input = trim(input);
            if (input.empty()) {
                printWarning("Input cannot be empty. Please enter a valid number.");
                continue;
            }

            try {
                size_t idx = 0;
                int val = std::stoi(input, &idx);
                if (idx == input.length() && val >= minVal && val <= maxVal) {
                    return val;
                }
            } catch (...) {
                // fall through
            }
            printWarning("Invalid choice. Please enter a number between " + 
                         std::to_string(minVal) + " and " + std::to_string(maxVal) + ".");
        }
    }

    std::string promptString(const std::string& prompt, bool allowEmpty) {
        while (true) {
            std::cout << Color::BOLD << Color::WHITE << prompt << ": " << Color::RESET;
            std::string input;
            if (!std::getline(std::cin, input)) {
                if (std::cin.eof()) {
                    std::cout << "\n[Session terminated via EOF]\n";
                    std::exit(0);
                }
                std::cin.clear();
                return "";
            }
            input = trim(input);
            if (input.empty() && !allowEmpty) {
                printWarning("Input cannot be blank. Please try again.");
                continue;
            }
            return input;
        }
    }

    bool promptConfirm(const std::string& prompt) {
        while (true) {
            std::cout << Color::BOLD << Color::WHITE << prompt << " (y/n): " << Color::RESET;
            std::string input;
            if (!std::getline(std::cin, input)) {
                if (std::cin.eof()) {
                    std::cout << "\n[Session terminated via EOF]\n";
                    std::exit(0);
                }
                std::cin.clear();
                return false;
            }
            input = trim(toUpper(input));
            if (input == "Y" || input == "YES") return true;
            if (input == "N" || input == "NO") return false;
            printWarning("Please enter 'y' for yes or 'n' for no.");
        }
    }

    std::string wrapText(const std::string& text, size_t maxWidth) {
        if (maxWidth <= 0) return text;
        std::istringstream words(text);
        std::ostringstream wrapped;
        std::string word;
        size_t currentLineLength = 0;

        while (words >> word) {
            if (currentLineLength + word.length() + (currentLineLength > 0 ? 1 : 0) > maxWidth) {
                wrapped << "\n" << word;
                currentLineLength = word.length();
            } else {
                if (currentLineLength > 0) {
                    wrapped << " ";
                    currentLineLength += 1;
                }
                wrapped << word;
                currentLineLength += word.length();
            }
        }
        return wrapped.str();
    }

    std::string toUpper(const std::string& str) {
        std::string res = str;
        std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
        return res;
    }

    std::string trim(const std::string& str) {
        auto start = str.begin();
        while (start != str.end() && std::isspace(*start)) {
            start++;
        }
        auto end = str.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
        return std::string(start, end + 1);
    }
}
