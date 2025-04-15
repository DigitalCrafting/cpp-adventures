#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

constexpr int PORT = 8080;

int main() {
    std::cout << "Hello from server\n";
    std::cout << "Starting...";

    int server_fd, client_socket;
    sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
   
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int bind_res = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    if (bind_res < 0) {
        std::cout << "Failed to bind to a port\n";
        return 1;
    }


    listen(server_fd, 3);

    std::cout << "Started on port " << PORT << "\n";

    client_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*)&addrlen);

    read(client_socket, buffer, 1024);
    std::cout << "Received from client: " << buffer << "\n";

    const char* response = "HTTP/1.1 200 OK\nHello from server!";
    send(client_socket, response, strlen(response), 0);


    close(client_socket);
    close(server_fd);
    return 0;
}
