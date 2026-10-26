#include "NetworkManager.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace netops {
NetworkManager::NetworkManager(Logger logger) : logger_(std::move(logger)) {}

void NetworkManager::addDevice(std::unique_ptr<Device> device) {
    if (!device) throw std::invalid_argument("Cannot add a null device");
    const auto id = device->id();
    if (devices_.find(id) != devices_.end()) throw std::invalid_argument("Duplicate device ID: " + id);
    logger_.log("INFO", "Registered device " + id + " (" + device->hostname() + ")");
    devices_.emplace(id, std::move(device));
}

void NetworkManager::clearDevices() noexcept { devices_.clear(); }

bool NetworkManager::removeDevice(const std::string& id) {
    const auto removed = devices_.erase(id) > 0;
    if (removed) logger_.log("INFO", "Removed device " + id);
    return removed;
}

Device* NetworkManager::findDevice(const std::string& id) noexcept {
    const auto it = devices_.find(id);
    return it == devices_.end() ? nullptr : it->second.get();
}
const Device* NetworkManager::findDevice(const std::string& id) const noexcept {
    const auto it = devices_.find(id);
    return it == devices_.end() ? nullptr : it->second.get();
}

std::vector<std::string> NetworkManager::deviceSummaries() const {
    std::vector<std::string> result;
    result.reserve(devices_.size());
    for (const auto& item : devices_) result.push_back(item.second->summary());
    return result;
}

std::vector<Alert> NetworkManager::monitorAll() {
    std::vector<Alert> generated;
    for (auto& item : devices_) {
        auto found = monitor_.inspect(*item.second);
        for (auto& alert : found) {
            alert.id = "ALT-" + std::to_string(nextAlert_++);
            logger_.log(alert.severity == Severity::Critical ? "CRITICAL" : "WARNING",
                        alert.id + " device=" + alert.deviceId + " " + alert.message);
            alerts_.push_back(alert);
            generated.push_back(alert);
        }
    }
    return generated;
}

const std::vector<Alert>& NetworkManager::alerts() const noexcept { return alerts_; }
const std::vector<Incident>& NetworkManager::incidents() const noexcept { return incidents_; }

bool NetworkManager::acknowledgeAlert(const std::string& alertId) {
    auto it = std::find_if(alerts_.begin(), alerts_.end(),
                           [&](const Alert& alert) { return alert.id == alertId; });
    if (it == alerts_.end()) return false;
    it->acknowledged = true;
    logger_.log("INFO", "Acknowledged alert " + alertId);
    return true;
}

std::string NetworkManager::createIncident(const std::string& alertId) {
    const auto it = std::find_if(alerts_.begin(), alerts_.end(),
                                 [&](const Alert& alert) { return alert.id == alertId; });
    if (it == alerts_.end()) throw std::invalid_argument("Alert not found: " + alertId);
    const std::string id = "INC-" + std::to_string(nextIncident_++);
    incidents_.push_back(Incident{id, it->id, it->deviceId, it->message, IncidentStatus::Open});
    logger_.log("INFO", "Created incident " + id + " from alert " + alertId);
    return id;
}

bool NetworkManager::updateIncident(const std::string& incidentId, IncidentStatus status) {
    auto it = std::find_if(incidents_.begin(), incidents_.end(),
                           [&](const Incident& incident) { return incident.id == incidentId; });
    if (it == incidents_.end()) return false;
    it->status = status;
    logger_.log("INFO", "Incident " + incidentId + " changed to " + toString(status));
    return true;
}

std::size_t NetworkManager::deviceCount() const noexcept { return devices_.size(); }
const std::map<std::string, std::unique_ptr<Device>>& NetworkManager::devices() const noexcept { return devices_; }
}
