# Embedded & Quant Finance Project Roadmap

Projects are grouped by category, from basic to highly advanced. Each project builds skills that can stack for more complex designs.

---

## 1. Basic Hardware & IoT Builds

### 1.1 DIY Power Bank Using Reclaimed Laptop Cells
**Summary:** Salvage 18650 cells from an old laptop battery to build a custom high-capacity power bank.
**Upgrades:** Add USB-C PD (Power Delivery) fast-charging module for modern devices.

### 1.2 Wi-Fi File Server with OTA Updates
**Summary:** Secure local Wi-Fi file server with encryption and over-the-air firmware updates.
**Upgrades:** Add Zero Trust authentication and offline-first sync.

---

## 2. Tracking, Logging & Cloud Dashboards

### 2.1 Swarm-Capable GPS Tracker
**Summary:** GPS/GSM/LoRa tracker nodes that share data for multi-device tracking.
**Upgrades:** Integrate mesh networking so trackers communicate without internet.

### 2.2 ESP32 Quant Data Logger + Cloud Dashboard
**Summary:** Log time-series data (financial, environmental) and visualize with live dashboards.
**Upgrades:** Add real-time anomaly detection alerts.

### 2.3 Edge Computing for IoT
**Summary:** On-device ML filtering to send only important events to the cloud.
**Upgrades:** Include federated learning for privacy-preserving model updates.

---

## 3. AI/ML on Edge Devices

### 3.1 TinyML Gesture & Voice Recognition
**Summary:** Detect gestures/voice commands on-device with ESP32/Teensy.
**Upgrades:** Multi-modal recognition combining both gesture and voice.

### 3.2 AI-Powered Image Recognition
**Summary:** On-device object detection (MobileNet, YOLO) with Raspberry Pi or Jetson Nano.
**Upgrades:** Low-light optimization for night-time detection.

### 3.3 Live OCR for Financial Data
**Summary:** ESP32S3 + camera extracts printed/digital stock tick data in real-time.
**Upgrades:** Add API push to a quant backtesting engine.

---

## 4. FPGA & High-Performance Embedded Systems

### 4.1 FPGA-Based Signal Processor
**Summary:** Implement DSP algorithms (FFT, filtering) on FPGA for real-time analysis.
**Upgrades:** Extend to ML inference acceleration.

### 4.2 FPGA + MCU Hybrid for High-Speed Quant Tasks
**Summary:** FPGA handles heavy calculations, MCU handles control & networking.
**Upgrades:** Use PCIe-connected FPGA for sub-millisecond response.

---

## 5. Finance & Quant-Focused Devices

### 5.1 Edge-Based Market Sentiment Tracker
**Summary:** Collect news/Twitter feeds, run sentiment analysis, backtest market impact.
**Upgrades:** Add reinforcement learning to improve strategies.

### 5.2 Smart Portfolio Rebalancer
**Summary:** Fetch live stock data, optimize allocation, send rebalance alerts.
**Upgrades:** Include risk-parity optimization.

### 5.3 On-Device AI Trading Signal Tester
**Summary:** Predict price moves and test strategies locally.
**Upgrades:** Add adaptive learning from recent performance.

---

## 6. Advanced Experimental Projects

### 6.1 **Autonomous Visual & Volumetric (AVV) Drone for Aerial Data Collection**
**Summary:** A drone equipped with multi-spectral camera, LiDAR, and thermal sensors to collect environmental, agricultural, and infrastructure data. Can feed aerial data into finance models (e.g., crop yield predictions, commodity price forecasting).
**Key Features:**
- Autonomous waypoint navigation
- Multi-sensor data fusion (RGB, IR, LiDAR)
- Real-time image processing for object detection (TinyYOLO/EdgeTPU)
- Secure wireless data uplink to quant dashboard
- Option to run on solar-assisted battery system for longer missions

### 6.2 IoT + Blockchain Secure Data Logger
**Summary:** Log aerial or ground sensor data to blockchain for tamper-proof storage.

### 6.3 FPGA Neural Network Accelerator
**Summary:** Implement CNN layers in hardware for ultra-low-latency AI.

---

**Note:** These projects are meant to be modular — skills and hardware from earlier builds can be reused in advanced ones.
