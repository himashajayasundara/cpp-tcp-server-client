#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    std::string message;
    char buffer[BUFFER_SIZE];

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "\nInvalid address/ Address not supported \n";
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // 2. Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection Failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "[+] Connected to the server! Type your messages below (type 'exit' to quit):\n";

    // 3. Send messages in a loop
    while (true) {
        std::cout << "You: ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        message += "\n";
        send(sock, message.c_str(), (int)message.length(), 0);

        // Read the echo response back from the server
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            std::cout << "[Server Echo]: " << buffer;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}