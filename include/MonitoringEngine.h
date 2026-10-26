#pragma once
#include "Alert.h"
#include "Device.h"
#include <vector>

namespace netops {
struct Thresholds {
    double warningLatencyMs{100.0};
    double criticalLatencyMs{250.0};
    double warningCpuPercent{75.0};
    double criticalCpuPercent{90.0};
    double warningMemoryPercent{80.0};
    double criticalMemoryPercent{95.0};
};

class MonitoringEngine {
public:
    explicit MonitoringEngine(Thresholds thresholds = {});
    std::vector<Alert> inspect(Device& device) const;
private:
    Thresholds thresholds_;
};
}
