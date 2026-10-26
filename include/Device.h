#pragma once

#include <string>

namespace netops {

enum class DeviceStatus { Online, Degraded, Offline };
std::string toString(DeviceStatus status);

class Device {
public:
    Device(std::string id, std::string hostname, std::string ipAddress);
    virtual ~Device() = default;

    const std::string& id() const noexcept;
    const std::string& hostname() const noexcept;
    const std::string& ipAddress() const noexcept;
    DeviceStatus status() const noexcept;
    double latencyMs() const noexcept;
    double cpuPercent() const noexcept;
    double memoryPercent() const noexcept;
    unsigned long uptimeSeconds() const noexcept;

    void setMetrics(double latencyMs, double cpuPercent, double memoryPercent);
    void setStatus(DeviceStatus status) noexcept;
    void setUptimeSeconds(unsigned long seconds) noexcept;
    virtual std::string type() const = 0;
    virtual std::string summary() const;

protected:
    static void validateText(const std::string& value, const std::string& field);

private:
    std::string id_;
    std::string hostname_;
    std::string ipAddress_;
    DeviceStatus status_{DeviceStatus::Online};
    double latencyMs_{1.0};
    double cpuPercent_{10.0};
    double memoryPercent_{20.0};
    unsigned long uptimeSeconds_{0};
};

} // namespace netops
