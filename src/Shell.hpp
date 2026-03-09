#pragma once

#include <sol/sol.hpp>
#include <string>
#include <vector>
#include <filesystem>

#include "Command.hpp"

class Terminal;


class Shell {
public:
    Terminal& terminal;
    sol::state lua;
    Shell(const std::filesystem::path& path, Terminal& term);

    void handleCommands(const std::vector<std::string>& parameters);

    std::vector<Command> commands;

private:
    void fprint(const std::string& message);
    void fregister(const std::string& function, const std::string& name, const std::string& description);
    void froute(const std::vector<std::string>& inputedParameters, const std::string& trigger, const std::string& functionName);
};