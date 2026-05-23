#pragma once

#include <string>

namespace nt219 {

enum class ErrorCode {
    ok = 0,
    invalid_argument = 1,
    file_error = 2,
    crypto_error = 3,
    unsupported = 4,
    authentication_failed = 5
};

std::string error_code_to_string(ErrorCode code);

}
