#include "Incident.h"
namespace netops {
std::string toString(IncidentStatus status) {
    switch (status) {
        case IncidentStatus::Open: return "OPEN";
        case IncidentStatus::Investigating: return "INVESTIGATING";
        case IncidentStatus::Resolved: return "RESOLVED";
    }
    return "UNKNOWN";
}
}
