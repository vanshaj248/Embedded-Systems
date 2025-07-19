# Embedded Systems Projects

## Microcontroller-Based Local File Managing Server (RTOS)

**Description:**  
A project implementing a local file management server on a microcontroller platform using a Real-Time Operating System (RTOS). The server allows users to upload, download, and organize files over a local network.

**Key Features:**
- RTOS-based multitasking for handling multiple client requests.
- Lightweight TCP/IP stack for network communication.
- File system integration (e.g., FAT32 on SD card).
- Web-based or command-line interface for file operations.
- User authentication and access control.

**Technologies Used:**
- ARM Cortex-M microcontroller (e.g., STM32)
- FreeRTOS
- lwIP (Lightweight IP stack)
- SD card module
- Embedded web server (e.g., Mongoose)

**Applications:**
- Local data storage for IoT devices
- Secure file sharing in embedded environments
- Remote firmware updates

**Repository:**  
[GitHub - Embedded File Server Example](https://github.com/example/embedded-file-server)