#pragma once
#include "Alert.h"
#include "Device.h"
#include "Incident.h"
#include "Logger.h"
#include "MonitoringEngine.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace netops {
class NetworkManager {
public:
    explicit NetworkManager(Logger logger = Logger("data/netops.log"));
    void addDevice(std::unique_ptr<Device> device);
    bool removeDevice(const std::string& id);
    void clearDevices() noexcept;
    Device* findDevice(const std::string& id) noexcept;
    const Device* findDevice(const std::string& id) const noexcept;
    std::vector<std::string> deviceSummaries() const;
    std::vector<Alert> monitorAll();
    const std::vector<Alert>& alerts() const noexcept;
    const std::vector<Incident>& incidents() const noexcept;
    bool acknowledgeAlert(const std::string& alertId);
    std::string createIncident(const std::string& alertId);
    bool updateIncident(const std::string& incidentId, IncidentStatus status);
    std::size_t deviceCount() const noexcept;
    const std::map<std::string, std::unique_ptr<Device>>& devices() const noexcept;
private:
    std::map<std::string, std::unique_ptr<Device>> devices_;
    std::vector<Alert> alerts_;
    std::vector<Incident> incidents_;
    MonitoringEngine monitor_;
    Logger logger_;
    unsigned long nextAlert_{1};
    unsigned long nextIncident_{1};
};
}
