#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

struct Connection {
    int socket_fd;
    std::string readBuffer;
    std::string writeBuffer;
    std::chrono::steady_clock::time_point lastActive;
};

class ConnectionManager {
public:
    ConnectionManager(int _server_fd): server_fd(_server_fd) {}
    void run() {
        while (true) {
            fd_set read_fds, write_fds;
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);

            FD_SET(server_fd, &read_fds);
            int max_fd = server_fd;

            for (auto &[fd, conn] : connections) {
                FD_SET(fd, &read_fds);
                if (!conn.writeBuffer.empty()) {
                    FD_SET(fd, &write_fds);
                }
                if (fd > max_fd) max_fd = fd;
            }

            struct timeval tv = {1, 0};
            int activity = select(max_fd + 1, &read_fds, &write_fds, nullptr, &tv);

            if (activity < 0) {
                perror("select");
                break;
            }

            if (FD_ISSET(server_fd, &read_fds)) {
                handleNewConnection();
            }

            std::vector<int> toRemove;
            for (auto &[fd, conn] : connections) {
                if (FD_ISSET(fd, &read_fds)) {
                    handleClientRead(fd);
                }
                if (FD_ISSET(fd, &write_fds)) {
                    handleClientWrite(fd);
                }
            }

            cleanUpDisconnected();
        }
    }

private:
    void handleNewConnection() {}
    void handleClientRead(int client_fd) {}
    void handleClientWrite(int client_fd) {}
    void cleanUpDisconnected() {}

    int server_fd;
    std::unordered_map<int, Connection> connections;
    const std::chrono::seconds timeoutDuration = std::chrono::seconds(30);
};