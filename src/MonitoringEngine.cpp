#include "MonitoringEngine.h"
#include <string>

namespace netops {
MonitoringEngine::MonitoringEngine(Thresholds thresholds) : thresholds_(thresholds) {}

std::vector<Alert> MonitoringEngine::inspect(Device& device) const {
    std::vector<Alert> alerts;
    const auto add = [&](Severity severity, const std::string& message) {
        alerts.push_back(Alert{"", device.id(), severity, message, false});
    };

    if (device.status() == DeviceStatus::Offline) {
        add(Severity::Critical, "Device is offline");
        return alerts;
    }

    if (device.latencyMs() >= thresholds_.criticalLatencyMs)
        add(Severity::Critical, "Critical network latency threshold exceeded");
    else if (device.latencyMs() >= thresholds_.warningLatencyMs)
        add(Severity::Warning, "Network latency is elevated");

    if (device.cpuPercent() >= thresholds_.criticalCpuPercent)
        add(Severity::Critical, "Critical CPU utilization threshold exceeded");
    else if (device.cpuPercent() >= thresholds_.warningCpuPercent)
        add(Severity::Warning, "CPU utilization is elevated");

    if (device.memoryPercent() >= thresholds_.criticalMemoryPercent)
        add(Severity::Critical, "Critical memory utilization threshold exceeded");
    else if (device.memoryPercent() >= thresholds_.warningMemoryPercent)
        add(Severity::Warning, "Memory utilization is elevated");

    if (alerts.empty() && device.status() == DeviceStatus::Degraded)
        add(Severity::Warning, "Device is marked degraded");

    return alerts;
}
}
