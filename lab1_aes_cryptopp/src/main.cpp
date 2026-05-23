#include "common/cli.hpp"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    try {
        nt219::CliParser cli(argc, argv);

        const std::string tool_name = "aestool";
        const std::string description = "Lab 1 - Symmetric Encryption with Crypto++";

        if (argc == 1 || cli.has_flag("help")) {
            nt219::print_common_help(tool_name, description);
            return 0;
        }

        const std::string command = cli.command();

        if (command == "version") {
            std::cout << tool_name << " version 0.1.0\n";
            return 0;
        }

        if (command == "selftest") {
            std::cout << tool_name << " selftest: PASS\n";
            return 0;
        }

        std::cerr << "Unsupported command for skeleton stage: " << command << "\n";
        std::cerr << "Use --help to see common CLI format.\n";
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
}

