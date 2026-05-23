#pragma once

#include <map>
#include <string>
#include <vector>

namespace nt219 {

class CliParser {
public:
    explicit CliParser(int argc, char** argv);

    bool has_flag(const std::string& name) const;
    std::string get_option(const std::string& name, const std::string& default_value = "") const;
    std::string command() const;
    const std::vector<std::string>& positional() const;

private:
    std::vector<std::string> positional_;
    std::map<std::string, std::string> options_;
};

void print_common_help(const std::string& tool_name, const std::string& description);

}
