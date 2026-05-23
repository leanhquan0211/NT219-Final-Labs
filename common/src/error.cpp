#include "common/error.hpp"

namespace nt219 {

std::string error_code_to_string(ErrorCode code) {
    switch (code) {
        case ErrorCode::ok:
            return "ok";
        case ErrorCode::invalid_argument:
            return "invalid argument";
        case ErrorCode::file_error:
            return "file error";
        case ErrorCode::crypto_error:
            return "crypto error";
        case ErrorCode::unsupported:
            return "unsupported operation";
        case ErrorCode::authentication_failed:
            return "authentication failed";
        default:
            return "unknown error";
    }
}

}
