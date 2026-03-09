#pragma once

#include "Profile.hpp"
#include <vector>
#include <string>

class Shell;

class Terminal {
public:
    std::string title;
    std::string version = "beta v1.0.0"; // vMAJOR.MINOR.PATCH
    Profile profile;

    Shell* shell = nullptr;

    void print(const std::string& message);
    void setShell(Shell* shellPtr);
    void handleShortcuts();
    void handleInput();
    void displayInputDialog();
    void renderBuffer();
    void clearBuffer();
private:
    std::vector<std::string> buffer; // buffer line by line

    float scroll = 1.0f;
    float scrollOffset = 1.0f;
    int cursor = 0;
    bool movedCursor = false;
    std::string inputBuffer;

    std::vector<std::string> history;
    bool movedHistory = false;
    int historyIndex = 0;
};