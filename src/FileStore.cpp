#include "FileStore.h"
#include "Router.h"
#include "Server.h"
#include "Workstation.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>

namespace netops {
void FileStore::saveDevices(const NetworkManager& manager, const std::filesystem::path& path) {
    if (!path.parent_path().empty()) std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path);
    if (!out) throw std::runtime_error("Unable to write device data: " + path.string());
    out << "NETOPS_DEVICES_V1\n";
    for (const auto& entry : manager.devices()) {
        const Device& d = *entry.second;
        out << std::quoted(d.type()) << ' ' << std::quoted(d.id()) << ' '
            << std::quoted(d.hostname()) << ' ' << std::quoted(d.ipAddress()) << ' '
            << static_cast<int>(d.status()) << ' ' << std::setprecision(17)
            << d.latencyMs() << ' ' << d.cpuPercent() << ' ' << d.memoryPercent() << ' '
            << d.uptimeSeconds() << '\n';
    }
    if (!out) throw std::runtime_error("Failed while writing device data: " + path.string());
}

void FileStore::loadDevices(NetworkManager& manager, const std::filesystem::path& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Unable to read device data: " + path.string());
    std::string header;
    std::getline(in, header);
    if (header != "NETOPS_DEVICES_V1") throw std::runtime_error("Unsupported or invalid device data file");

    manager.clearDevices();
    std::string type, id, hostname, ip;
    int status = 0;
    double latency = 0, cpu = 0, memory = 0;
    unsigned long uptime = 0;
    while (in >> std::quoted(type) >> std::quoted(id) >> std::quoted(hostname)
              >> std::quoted(ip) >> status >> latency >> cpu >> memory >> uptime) {
        std::unique_ptr<Device> device;
        if (type == "Server") device = std::make_unique<Server>(id, hostname, ip);
        else if (type == "Router") device = std::make_unique<Router>(id, hostname, ip);
        else if (type == "Workstation") device = std::make_unique<Workstation>(id, hostname, ip);
        else throw std::runtime_error("Unknown device type in data file: " + type);
        if (status < 0 || status > 2) throw std::runtime_error("Invalid device status in data file");
        device->setMetrics(latency, cpu, memory);
        device->setStatus(static_cast<DeviceStatus>(status));
        device->setUptimeSeconds(uptime);
        manager.addDevice(std::move(device));
    }
    if (!in.eof()) throw std::runtime_error("Malformed device record in data file");
}
}
