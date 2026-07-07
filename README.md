# Smart Saline Monitoring System

An IoT-enabled healthcare solution that continuously monitors IV saline bottle levels using a dual-sensor architecture (Load Cell + HX711 and IR Sensor) connected to ESP32 nodes. Sensor data is transmitted over a LoRa network to a central gateway, where a web-based dashboard provides real-time monitoring and alerts to hospital staff.

---

## Key Features

- Dual-sensor validation to reduce false alarms
- Real-time saline level monitoring
- Drip detection using IR sensor
- LoRa-based long-range communication
- ESP32 edge devices
- Centralized monitoring dashboard
- In-app and desktop notifications
- Historical data logging
- Offline-capable local deployment
- Docker support

---

## System Architecture

```text
 IV Bottle
     │
     ▼
 Load Cell + HX711
     │
 IR Drip Sensor
     │
     ▼
     ESP32
     │
   LoRa Radio
     │
     ▼
 LoRa Gateway
     │
 ASP.NET Core API
     │
 SQL Server
     │
 React Dashboard
     │
 Hospital Staff
```

---

## Repository Structure

```text
backend/      ASP.NET Core Web API
dashboard/    React + TypeScript frontend
firmware/     ESP32 firmware
hardware/     Wiring, PCB, enclosure files
docs/         Architecture and documentation
docker/       Deployment files
scripts/      Utility scripts
```

---

## Planned Features

- Patient and bed management
- Sensor calibration
- Real-time alert engine
- Historical analytics
- Device health monitoring
- Role-based authentication
- Multi-ward support
- REST API
- Docker deployment

---

## Technology Stack

- ESP32
- HX711
- Load Cell
- IR Sensor
- LoRa
- ASP.NET Core (.NET 8)
- React
- TypeScript
- SQL Server
- Docker

---

## License

MIT
