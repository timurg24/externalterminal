#include "Terminal.hpp"
#include "Shell.hpp"
#include "Utils.hpp"

#include <raylib.h>
#include <cstring>

/// @brief Adds a message to the buffer
/// @param message Message to add
void Terminal::print(const std::string &message)
{
    buffer.emplace_back(message);
}

void Terminal::setShell(Shell *shellPtr)
{
    shell = shellPtr;
}

/// @brief Handles terminal related shortcuts
void Terminal::handleShortcuts()
{

    scroll += GetMouseWheelMove();
    scrollOffset = scroll * 10;

    bool ctrl = IsKeyDown(KEY_LEFT_CONTROL);

    // *FONT SIZE*
    bool didFontSizeChange = false;
    if(ctrl && IsKeyPressed(KEY_EQUAL)) {
        profile.fontSize += 1.0f;
        didFontSizeChange = true;
    }
    if(ctrl && IsKeyPressed(KEY_MINUS)) {
        profile.fontSize -= 1.0f;
        didFontSizeChange = true;
    }

    // *CLEAR BUFFER*
    if(ctrl && IsKeyPressed(KEY_Q)) clearBuffer();

    // Display current font size
    if(didFontSizeChange) {

        const char* text = std::to_string((int)profile.fontSize).c_str();

        Vector2 size = MeasureTextEx(profile.font, text, 25, 2);

        float x = GetScreenWidth() - size.x - 10;
        float y = GetScreenHeight() - size.y - 10;

        DrawTextEx(profile.font, text, {x, y}, 25, 2, WHITE);
    }
}

/// @brief Handles the user input
void Terminal::handleInput()
{
    std::string dialog = profile.shell + "> ";

    if(!movedCursor) cursor = (int)inputBuffer.size();

    KeyboardKey key = static_cast<KeyboardKey>(GetKeyPressed());
    // Handle arrows
    switch(key){
    case(KEY_LEFT):
        if(cursor>0) {
            movedCursor = true;
            cursor--;
        }
        break;
    case(KEY_RIGHT):
        if(cursor<inputBuffer.size()) {
            movedCursor = true;
            cursor++;
        }
        break;
    case(KEY_BACKSPACE):
        if(cursor>0) {
            inputBuffer.erase(cursor - 1, 1);
            cursor--;
        }
        break;
    case(KEY_ENTER):
            print(dialog + inputBuffer);
            shell->handleCommands(splitString(inputBuffer, ' '));
            inputBuffer.clear();
        break;
    }

    int keyChar = GetCharPressed();
    if(keyChar > 0) {
        inputBuffer.insert(cursor, 1, static_cast<char>(keyChar));
        cursor++;
        keyChar = GetCharPressed();
    }
}

/// @brief Renders dialog and user input
void Terminal::displayInputDialog()
{
    // display user input
    int lineOffset = (buffer.size()) * (int)profile.fontSize + scrollOffset;
    // dialog
    std::string dialog = profile.shell + "> ";
    Vector2 dialogSize = MeasureTextEx(profile.font, dialog.c_str(), profile.fontSize, 2);
    DrawTextEx(
        profile.font,
        dialog.c_str(),
        Vector2(10, lineOffset),
        profile.fontSize,
        2,
        profile.host
    );

    // user input
    float inputOffset = 10 + dialogSize.x;
    DrawTextEx(
        profile.font,
        inputBuffer.c_str(),
        Vector2(inputOffset, lineOffset),
        profile.fontSize,
        2,
        profile.normal
    );

    // cursor
    std::string cutInput = inputBuffer.substr(0, cursor);
    Vector2 cutSize = MeasureTextEx(profile.font, cutInput.c_str(), profile.fontSize, 2);
    DrawRectangle(
        inputOffset + cutSize.x,
        lineOffset,
        1,
        profile.fontSize,
        RAYWHITE
    );
}

/// @brief Renders buffer to screen
void Terminal::renderBuffer()
{

    
    int lineNum = 0;
    for(auto& line : buffer) {

        int lineOffset = lineNum * (int)profile.fontSize + scrollOffset;

        DrawTextEx(
            profile.font,
            line.c_str(),
            Vector2(10, lineOffset),
            profile.fontSize,
            2,
            profile.normal
        );
        lineNum++;
    }
}

void Terminal::clearBuffer()
{
    buffer.clear();
}