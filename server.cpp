#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle communication with an individual client
void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_read <= 0) {
            std::cout << "[-] Client disconnected or error occurred.\n";
            break;
        }
        
        std::cout << "[Client Message]: " << buffer;
        
        // Echo the message back to the client
        send(client_socket, buffer, bytes_read, 0);
    }
    
    closesocket(client_socket);
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 1. Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 3. Start listening for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "[+] Server is listening on port " << PORT << "...\n";

    // 4. Accept connections in a loop
    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (int*)&addrlen);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
            continue;
        }

        std::cout << "[+] New client connected!\n";

        // Spawn a new thread for each connected client and detach it
        std::thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}