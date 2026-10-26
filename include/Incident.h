#pragma once
#include <string>

namespace netops {
enum class IncidentStatus { Open, Investigating, Resolved };
std::string toString(IncidentStatus status);

struct Incident {
    std::string id;
    std::string alertId;
    std::string deviceId;
    std::string summary;
    IncidentStatus status{IncidentStatus::Open};
};
}
