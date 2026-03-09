#include "Shell.hpp"
#include "Terminal.hpp"

#include <iostream>


Shell::Shell(const std::filesystem::path& path, Terminal& term): terminal(term)
{
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );

    lua.set_function("bprint", &Shell::fprint, this);
    lua.set_function("register", &Shell::fregister, this);
    lua.set_function("route", &Shell::froute, this);


    sol::load_result loaded = lua.load_file(path.string());
    if (!loaded.valid()) {
        sol::error err = loaded;
        terminal.print(std::string("Lua load error: ") + err.what());
        return;
    }

    sol::protected_function_result execResult = loaded();
    if (!execResult.valid()) {
        sol::error err = execResult;
        terminal.print(std::string("Lua runtime error: ") + err.what());
        return;
    }

    sol::protected_function start = lua["start"];
    if (!start.valid()) {
        terminal.print("No start() function found in Lua file");
        return;
    }

    sol::protected_function_result startResult = start();
    if (!startResult.valid()) {
        sol::error err = startResult;
        terminal.print(std::string("Lua start() error: ") + err.what());
    }

}

void Shell::handleCommands(const std::vector<std::string>& parameters)
{
    std::string inputCommand = parameters[0];

    // built in help memu
    if(inputCommand == "help") {
        terminal.print("Registerd commands:");
        for(auto& command : commands) {
            terminal.print("\t" + command.name + ": " + command.description);
        }
        return;
    }

    for(auto& command : commands) {
        if(inputCommand == command.name) {
            std::vector<std::string> params = parameters;
            params.erase(params.begin());
            lua[command.function](params);
            return;
        }
    }
    terminal.print("Command not registered: " + inputCommand);
}

void Shell::fprint(const std::string &message)
{
    terminal.print(message);
}

void Shell::fregister(const std::string &function, const std::string &name, const std::string &description)
{
    Command command = {
        function,
        name,
        description
    };
    commands.emplace_back(command);
}

void Shell::froute(const std::vector<std::string> &inputedParameters, const std::string &trigger, const std::string &functionName)
{
    if(inputedParameters[0] == trigger) {
        std::vector<std::string> cut = inputedParameters;
        cut.erase(cut.begin());
        lua[functionName](cut);
    }
}