#pragma once

#include <raylib.h>
#include <string>

struct Profile {

    std::string title = "No Title";

    Color background = {20, 20, 20, 255};
    Color host = {250, 230, 80, 255};
    Color normal = {0, 0, 0, 255};

    std::string fontPath = "resources/DejaVuSansMono.ttf";
    Font font;
    float fontSize = 15.0f;
    void loadFont();

    std::string shell = "No Shell";

    void loadProfile(const std::string& profileName);
};