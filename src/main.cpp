#include <raylib.h>

#include "Terminal.hpp"
#include "Shell.hpp"

#include "Path.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void newProfile(const std::string& name);

int main(int argc, char** argv) {

    // Loading configuration file
    std::ifstream config(configFilePath);
    json data = json::parse(config);

    Terminal terminal;
    terminal.title = "ExternalTerminal - ";

    // CLI
    if (argc == 1) {
        terminal.profile.loadProfile(data["profile"]);
    }
    else if (argc >= 3 && std::string(argv[1]) == "--profile") {
        terminal.profile.loadProfile(argv[2]);
    }
    else if (argc >= 3 && std::string(argv[1]) == "--new-profile") {
        newProfile(argv[2]);
        return 0;
    }
    
    InitWindow(800, 600, terminal.title.c_str());
    SetTargetFPS(60);


    // Set title
    terminal.title += terminal.profile.title;
    SetWindowTitle(terminal.title.c_str());

    // Load font
    terminal.profile.loadFont();
    
    // Welcome screen
    terminal.print("ExternalTerminal " + terminal.version);
    terminal.print("Loaded Shell: " + terminal.profile.title);
    
    // Load shell
    std::filesystem::path shellPath = shellsPath / terminal.profile.shell / (terminal.profile.shell + ".lua");
    Shell shell(shellPath, terminal);
    terminal.setShell(&shell);

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(terminal.profile.background);
            terminal.handleShortcuts();
            terminal.handleInput();
            terminal.displayInputDialog();
            terminal.renderBuffer();
        EndDrawing();
    }

    CloseWindow();
}

void newProfile(const std::string &name)
{
    std::filesystem::path shellFolder = "resources/shells/" + name;
    std::filesystem::create_directories(shellFolder);

    std::ofstream profile("resources/profiles/" + name + ".json");
    profile << "{\n"
            << "    \"title\": \"" << name << "\",\n"
            << "    \"shell\": \"" << name << "\",\n"
            << "    \"backgroundColor\": [20, 20, 20, 255],\n"
            << "    \"hostColor\": [250, 230, 80, 255],\n"
            << "    \"normalColor\": [255, 255, 255, 255],\n"
            << "    \"font\": \"DejaVuSansMono.ttf\"\n"
            << "}\n";
}
