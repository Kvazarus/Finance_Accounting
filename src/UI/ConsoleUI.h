#ifndef KPO2_CONSOLEUI_H
#define KPO2_CONSOLEUI_H

#include <iostream>
#include <limits>

class ConsoleUI {
private:
    static void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

public:
    static void printHeader(const std::string& title) {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "  " << title << "\n";
        std::cout << std::string(60, '=') << "\n";
    }

    static int getIntInput(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;
            if (std::cin.fail() || value < min || value > max) {
                std::cout << "Error: enter integer from " << min << " to " << max << "\n";
                clearInputBuffer();
            } else {
                clearInputBuffer();
                return value;
            }
        }
    }

    static double getDoubleInput(const std::string& prompt, double min = 0.0) {
        double value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;
            if (std::cin.fail() || value < min) {
                std::cout << "Error: enter number >= " << min << "\n";
                clearInputBuffer();
            } else {
                clearInputBuffer();
                return value;
            }
        }
    }

    static std::string getStringInput(const std::string& prompt, bool allowEmpty = false) {
        std::string value;
        while (true) {
            std::cout << prompt;
            getline(std::cin, value);
            if (!allowEmpty && value.empty()) {
                std::cout << "Error: field cannot be empty\n";
            } else {
                return value;
            }
        }
    }

    static void pressAnyKey() {
        std::cout << "\nPress Enter to continue...";
        clearInputBuffer();
    }

    static void showError(const std::string& error) {
        std::cout << "Error: " << error << "\n";
    }

    static void showSuccess(const std::string& message) {
        std::cout << "Success: " << message << "\n";
    }
};

#endif //KPO2_CONSOLEUI_H
