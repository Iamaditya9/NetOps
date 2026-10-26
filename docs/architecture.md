# Architecture

```text
CLI (main.cpp)
     |
     v
NetworkManager ---------> Logger --------> data/netops.log
     |                       |
     +----> MonitoringEngine |
     |          |            |
     |          v            |
     |        Alert          |
     +----> Incident         |
     |
     +----> Device (abstract)
               |-- Server
               |-- Router
               `-- Workstation

FileStore <----> data/devices.txt
```

## Responsibilities

- `Device`: encapsulates device identity, status, uptime, and simulated metrics.
- Concrete device classes: identify device subtypes using virtual dispatch.
- `MonitoringEngine`: checks status and metric thresholds and returns alerts.
- `NetworkManager`: owns devices and coordinates alerts, incidents, and state changes.
- `FileStore`: persists device state in a versioned text format.
- `Logger`: appends timestamped operational events.

## Monitoring rules

Default thresholds are configured in `MonitoringEngine.h`. An offline device creates a critical alert. Otherwise latency, CPU, and memory are evaluated against warning and critical thresholds. A degraded device with no metric threshold violation receives a warning.
