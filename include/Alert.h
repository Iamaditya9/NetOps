#pragma once
#include <string>

namespace netops {
enum class Severity { Info, Warning, Critical };
std::string toString(Severity severity);

struct Alert {
    std::string id;
    std::string deviceId;
    Severity severity{Severity::Info};
    std::string message;
    bool acknowledged{false};
};
}
