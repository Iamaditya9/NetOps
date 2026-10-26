# NetOps — Network Operations & System Diagnostics

**NetOps** is a modular C++17 command-line application that simulates a small network environment. It lets an operator register devices, inspect simulated health metrics, detect threshold violations, acknowledge alerts, create incidents, and persist device state to a local file.

> **Scope note:** This version simulates monitoring data. It does not probe real hosts, collect operating-system metrics, or perform live network monitoring.

## Features

- Register and remove server, router, and workstation devices.
- Use polymorphic device types behind a common `Device` interface.
- Inspect availability, latency, CPU, and memory metrics.
- Generate warning and critical alerts using configurable thresholds.
- Acknowledge alerts and create/update incident records.
- Persist and restore device configuration and metrics.
- Append operational events to a log file.
- Validate metric ranges and report invalid operations.
- Build with CMake and run dependency-free automated tests with CTest.

## Technology

C++17 · STL · CMake · File I/O · Exception handling · CTest

## Project structure

```text
NetOps/
├── include/                 # Public class declarations (.h)
├── src/                     # Implementations and application entry point
├── tests/
│   └── test_netops.cpp
├── docs/
│   └── architecture.md
├── data/                    # Runtime data/logs are created here
├── CMakeLists.txt
└── README.md
```

## Build and run

### Prerequisites
A C++17 compiler (GCC, Clang, or MSVC) and CMake 3.16 or newer.

### Linux / macOS / WSL
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/netops
```

### Windows (PowerShell, CMake and a C++ compiler installed)
```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
.\build\Release\netops.exe
```
Depending on the generator, the Windows executable may instead be placed directly inside `build/`.

## Quick walkthrough

1. Choose **1** to list sample devices.
2. Choose **2** to run health checks. The sample server has elevated simulated metrics, so it generates alerts.
3. Choose **5** and enter an alert ID (for example, `ALT-1`) to acknowledge it.
4. Choose **6** with an alert ID to create an incident.
5. Choose **7** to review alerts and incidents.
6. Choose **8** to save device state to `data/devices.txt`.
7. Choose **0** to exit.

Running a health check repeatedly can create additional alerts for the same persistent condition.

## Design overview

- **`Device`** encapsulates device identity, status, uptime, and simulated metrics.
- **`Server`, `Router`, and `Workstation`** implement concrete device types through virtual dispatch.
- **`NetworkManager`** coordinates device registration, monitoring, alerts, incidents, and logging.
- **`MonitoringEngine`** evaluates metrics against warning and critical thresholds.
- **`FileStore`** saves and loads device state in a versioned local format.
- **`Logger`** appends timestamped operational events.

## Current limitations and next steps

- Monitoring is simulated; no real sockets, ICMP probes, or SNMP collection are implemented.
- Alert and incident records currently live in memory for the current session; device records can be saved and loaded. Loading replaces the current device list.
- IP addresses are stored as identifiers but are not yet validated as IPv4/IPv6 addresses.
- Future improvements could include incident persistence, service-level checks, filtering, and integration tests.

## Team

**Dynamic Duo** — Aditya Yadav and Nipun Wadyalkar

## License

No license has been selected yet. Add a license after deciding how you want the project to be reused.
