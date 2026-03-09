#include "Profile.hpp"

#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Profile::loadFont()
{
    font = LoadFont(fontPath.c_str());
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
}

Color loadColor(const json& j)
{
    return Color{
        j[0],
        j[1],
        j[2],
        j[3]
    };
}

void Profile::loadProfile(const std::string &profileName)
{
    std::ifstream configFile("resources/profiles/" + profileName + ".json");
    if (!configFile.is_open())
    {
        throw std::runtime_error("Failed to open profile: " + profileName);
    }
    json data = json::parse(configFile);


    title = data["title"];
    shell = data["shell"];

    background = loadColor(data["backgroundColor"]);
    host = loadColor(data["hostColor"]);
    normal = loadColor(data["normalColor"]);
    fontPath = std::string("resources/fonts/") + std::string(data["font"]);

}
