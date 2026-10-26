#include "Alert.h"
namespace netops {
std::string toString(Severity severity) {
    switch (severity) {
        case Severity::Info: return "INFO";
        case Severity::Warning: return "WARNING";
        case Severity::Critical: return "CRITICAL";
    }
    return "UNKNOWN";
}
}
