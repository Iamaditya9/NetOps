#include "Device.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace netops {

std::string toString(DeviceStatus status) {
    switch (status) {
        case DeviceStatus::Online: return "ONLINE";
        case DeviceStatus::Degraded: return "DEGRADED";
        case DeviceStatus::Offline: return "OFFLINE";
    }
    return "UNKNOWN";
}

void Device::validateText(const std::string& value, const std::string& field) {
    if (value.empty()) throw std::invalid_argument(field + " cannot be empty");
}

Device::Device(std::string id, std::string hostname, std::string ipAddress)
    : id_(std::move(id)), hostname_(std::move(hostname)), ipAddress_(std::move(ipAddress)) {
    validateText(id_, "Device ID");
    validateText(hostname_, "Hostname");
    validateText(ipAddress_, "IP address");
}

const std::string& Device::id() const noexcept { return id_; }
const std::string& Device::hostname() const noexcept { return hostname_; }
const std::string& Device::ipAddress() const noexcept { return ipAddress_; }
DeviceStatus Device::status() const noexcept { return status_; }
double Device::latencyMs() const noexcept { return latencyMs_; }
double Device::cpuPercent() const noexcept { return cpuPercent_; }
double Device::memoryPercent() const noexcept { return memoryPercent_; }
unsigned long Device::uptimeSeconds() const noexcept { return uptimeSeconds_; }

void Device::setMetrics(double latencyMs, double cpuPercent, double memoryPercent) {
    if (latencyMs < 0.0 || cpuPercent < 0.0 || cpuPercent > 100.0 ||
        memoryPercent < 0.0 || memoryPercent > 100.0) {
        throw std::invalid_argument("Metrics out of range");
    }
    latencyMs_ = latencyMs;
    cpuPercent_ = cpuPercent;
    memoryPercent_ = memoryPercent;
}

void Device::setStatus(DeviceStatus status) noexcept { status_ = status; }
void Device::setUptimeSeconds(unsigned long seconds) noexcept { uptimeSeconds_ = seconds; }

std::string Device::summary() const {
    std::ostringstream out;
    out << id_ << " | " << hostname_ << " | " << ipAddress_ << " | " << type()
        << " | " << toString(status_) << " | latency " << std::fixed << std::setprecision(1)
        << latencyMs_ << " ms | CPU " << cpuPercent_ << "% | RAM " << memoryPercent_ << "%";
    return out.str();
}

} // namespace netops
