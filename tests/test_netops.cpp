#include "FileStore.h"
#include "Router.h"
#include "Server.h"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
using namespace netops;
int main() {
    const auto temp = std::filesystem::temp_directory_path() / "netops_test_data.txt";
    const auto log = std::filesystem::temp_directory_path() / "netops_test.log";
    std::filesystem::remove(temp); std::filesystem::remove(log);
    NetworkManager manager{Logger(log)};
    manager.addDevice(std::make_unique<Server>("s1", "server-one", "10.0.0.2"));
    assert(manager.deviceCount() == 1);
    assert(manager.findDevice("s1") != nullptr && manager.findDevice("missing") == nullptr);
    bool duplicateRejected = false;
    try { manager.addDevice(std::make_unique<Router>("s1", "duplicate", "10.0.0.3")); }
    catch (const std::invalid_argument&) { duplicateRejected = true; }
    assert(duplicateRejected);
    Device* server = manager.findDevice("s1");
    server->setMetrics(300.0, 95.0, 96.0);
    auto alerts = manager.monitorAll();
    assert(alerts.size() == 3 && alerts.front().severity == Severity::Critical);
    assert(manager.acknowledgeAlert(alerts.front().id));
    assert(manager.alerts().front().acknowledged);
    const auto incidentId = manager.createIncident(alerts.front().id);
    assert(manager.incidents().size() == 1);
    assert(manager.updateIncident(incidentId, IncidentStatus::Resolved));
    assert(manager.incidents().front().status == IncidentStatus::Resolved);
    assert(!manager.updateIncident("INC-missing", IncidentStatus::Resolved));
    bool invalidMetricsRejected = false;
    try { server->setMetrics(1.0, 101.0, 20.0); }
    catch (const std::invalid_argument&) { invalidMetricsRejected = true; }
    assert(invalidMetricsRejected);
    manager.addDevice(std::make_unique<Router>("r1", "router-one", "10.0.0.1"));
    FileStore::saveDevices(manager, temp);
    NetworkManager restored{Logger(log)};
    FileStore::loadDevices(restored, temp);
    assert(restored.deviceCount() == 2);
    assert(restored.findDevice("s1") != nullptr);
    assert(restored.findDevice("s1")->type() == "Server");
    assert(restored.findDevice("s1")->cpuPercent() == 95.0);
    assert(manager.removeDevice("r1") && !manager.removeDevice("r1"));
    std::filesystem::remove(temp); std::filesystem::remove(log);
    std::cout << "All NetOps tests passed.\n";
    return 0;
}
