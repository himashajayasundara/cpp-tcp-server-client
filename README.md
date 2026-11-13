# Multithreaded TCP Server & Client in C++

A lightweight, multithreaded TCP server and client application built in C++ using the Windows Sockets API (`Winsock2`). Designed to handle multiple concurrent client connections and provide reliable message transmission on Windows.

## Features

- **Concurrent Client Handling:** Uses multithreading (`std::thread` / `pthread`) to manage multiple simultaneous client connections without blocking.
- **Winsock2 Integration:** Built directly on `winsock2.h` and `ws2tcpip.h` for native Windows network communication.
- **Reliable TCP Communication:** Ensures ordered, error-checked data delivery between server and clients.
- **Clean Resource Management:** Properly closes sockets and cleans up Winsock instances upon disconnection or termination.

## Prerequisites

- **Operating System:** Windows 10/11
- **Compiler:** MinGW-w64 / MSYS2 (`g++` with C++11 or later support)
- **Libraries:** Windows Socket Library (`ws2_32`) and POSIX Threads (`pthread`)

## Project Structure

```text
cpp-tcp-server-client/
├── server.cpp      # Multithreaded TCP server implementation
├── client.cpp      # TCP client implementation
├── .gitignore      # Excludes compiled binaries and IDE files
└── README.md       # Project documentation
