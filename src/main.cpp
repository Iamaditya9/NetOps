#include "FileStore.h"
#include "Router.h"
#include "Server.h"
#include "Workstation.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace netops;

namespace {
void printMenu() {
    std::cout << "\n=== NetOps | Network Operations & Diagnostics ===\n"
              << "1. List devices\n2. Run health checks\n3. Add a device\n"
              << "4. Update simulated metrics\n5. Acknowledge an alert\n"
              << "6. Create incident from alert\n7. List alerts and incidents\n"
              << "8. Save devices\n9. Load devices\n0. Exit\nSelect: ";
}
std::unique_ptr<Device> createDevice(const std::string& type, const std::string& id,
                                     const std::string& hostname, const std::string& ip) {
    if (type == "1") return std::make_unique<Server>(id, hostname, ip);
    if (type == "2") return std::make_unique<Router>(id, hostname, ip);
    if (type == "3") return std::make_unique<Workstation>(id, hostname, ip);
    throw std::invalid_argument("Choose 1 (Server), 2 (Router), or 3 (Workstation)");
}
}
int main() {
    try {
        NetworkManager manager(Logger("data/netops.log"));
        manager.addDevice(std::make_unique<Server>("srv-01", "api-server-01", "10.0.0.10"));
        manager.addDevice(std::make_unique<Router>("rtr-01", "edge-router-01", "10.0.0.1"));
        manager.addDevice(std::make_unique<Workstation>("ws-01", "ops-workstation-01", "10.0.0.21"));
        manager.findDevice("srv-01")->setMetrics(280.0, 94.0, 88.0);
        manager.findDevice("srv-01")->setStatus(DeviceStatus::Degraded);

        bool running = true;
        while (running) {
            printMenu();
            std::string choice;
            if (!std::getline(std::cin, choice)) break;
            try {
                if (choice == "0") running = false;
                else if (choice == "1") {
                    for (const auto& row : manager.deviceSummaries()) std::cout << row << '\n';
                } else if (choice == "2") {
                    const auto newAlerts = manager.monitorAll();
                    if (newAlerts.empty()) std::cout << "No threshold violations detected.\n";
                    for (const auto& a : newAlerts)
                        std::cout << a.id << " [" << toString(a.severity) << "] device="
                                  << a.deviceId << ": " << a.message << '\n';
                } else if (choice == "3") {
                    std::string type, id, hostname, ip;
                    std::cout << "Type (1 Server, 2 Router, 3 Workstation): "; std::getline(std::cin, type);
                    std::cout << "Device ID: "; std::getline(std::cin, id);
                    std::cout << "Hostname: "; std::getline(std::cin, hostname);
                    std::cout << "IP address: "; std::getline(std::cin, ip);
                    manager.addDevice(createDevice(type, id, hostname, ip));
                    std::cout << "Device registered.\n";
                } else if (choice == "4") {
                    std::string id, latencyText, cpuText, memoryText;
                    std::cout << "Device ID: "; std::getline(std::cin, id);
                    Device* d = manager.findDevice(id);
                    if (!d) { std::cout << "Device not found.\n"; continue; }
                    std::cout << "Latency ms: "; std::getline(std::cin, latencyText);
                    std::cout << "CPU percent: "; std::getline(std::cin, cpuText);
                    std::cout << "Memory percent: "; std::getline(std::cin, memoryText);
                    std::size_t used = 0;
                    const double latency = std::stod(latencyText, &used);
                    if (used != latencyText.size()) throw std::invalid_argument("Invalid latency value");
                    const double cpu = std::stod(cpuText, &used);
                    if (used != cpuText.size()) throw std::invalid_argument("Invalid CPU value");
                    const double memory = std::stod(memoryText, &used);
                    if (used != memoryText.size()) throw std::invalid_argument("Invalid memory value");
                    d->setMetrics(latency, cpu, memory);
                    d->setStatus(DeviceStatus::Online);
                    std::cout << "Simulated metrics updated.\n";
                } else if (choice == "5") {
                    std::string id; std::cout << "Alert ID: "; std::getline(std::cin, id);
                    std::cout << (manager.acknowledgeAlert(id) ? "Alert acknowledged.\n" : "Alert not found.\n");
                } else if (choice == "6") {
                    std::string id; std::cout << "Alert ID: "; std::getline(std::cin, id);
                    std::cout << "Created incident " << manager.createIncident(id) << '\n';
                } else if (choice == "7") {
                    for (const auto& a : manager.alerts())
                        std::cout << a.id << " [" << toString(a.severity) << "] "
                                  << (a.acknowledged ? "ACKNOWLEDGED" : "UNACKNOWLEDGED")
                                  << " device=" << a.deviceId << " " << a.message << '\n';
                    for (const auto& i : manager.incidents())
                        std::cout << i.id << " [" << toString(i.status) << "] "
                                  << i.summary << " device=" << i.deviceId << '\n';
                } else if (choice == "8") {
                    FileStore::saveDevices(manager, "data/devices.txt");
                    std::cout << "Saved devices to data/devices.txt\n";
                } else if (choice == "9") {
                    FileStore::loadDevices(manager, "data/devices.txt");
                    std::cout << "Loaded devices from data/devices.txt\n";
                } else std::cout << "Unknown menu option.\n";
            } catch (const std::exception& ex) {
                std::cerr << "Operation failed: " << ex.what() << '\n';
            }
        }
        std::cout << "NetOps session ended.\n";
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
