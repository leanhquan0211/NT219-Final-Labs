#include "common/cli.hpp"

#include <iostream>

namespace nt219 {

CliParser::CliParser(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.rfind("--", 0) == 0) {
            std::string key = arg.substr(2);
            std::string value = "true";

            if (i + 1 < argc) {
                std::string next = argv[i + 1];
                if (next.rfind("--", 0) != 0) {
                    value = next;
                    ++i;
                }
            }

            options_[key] = value;
        } else {
            positional_.push_back(arg);
        }
    }
}

bool CliParser::has_flag(const std::string& name) const {
    return options_.find(name) != options_.end();
}

std::string CliParser::get_option(const std::string& name, const std::string& default_value) const {
    auto it = options_.find(name);
    if (it == options_.end()) {
        return default_value;
    }
    return it->second;
}

std::string CliParser::command() const {
    if (positional_.empty()) {
        return "";
    }
    return positional_[0];
}

const std::vector<std::string>& CliParser::positional() const {
    return positional_;
}

void print_common_help(const std::string& tool_name, const std::string& description) {
    std::cout
        << tool_name << "\n"
        << description << "\n\n"
        << "Common CLI format:\n"
        << "  " << tool_name << " <command> [options]\n\n"
        << "Common options:\n"
        << "  --help\n"
        << "  --in INFILE\n"
        << "  --text TEXT\n"
        << "  --out OUTFILE\n"
        << "  --key KEYFILE\n"
        << "  --key-hex HEX\n"
        << "  --iv IV\n"
        << "  --nonce NONCE\n"
        << "  --mode MODE\n"
        << "  --encode hex|base64|raw\n"
        << "  --kat vectors.json\n"
        << "  --verbose\n";
}

}
