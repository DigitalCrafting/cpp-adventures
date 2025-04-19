#include <unordered_map>
#include <string>
#include <chrono>

struct Connection {
    int socket_fd;
    std::string readBuffer;
    std::string writeBuffer;
    std::chrono::steady_clock::time_point lastActive;
};

class ConnectionManager {
public:
    ConnectionManager(int _server_fd): server_fd(_server_fd) {}
    void run();

private:
    void handleNewConnection() {}
    void handleClientRead(int client_fd) {}
    void handleClientWrite(int client_fd) {}
    void cleanUpDisconnected() {}

    int server_fd;
    std::unordered_map<int, Connection> connections;
    const std::chrono::seconds timeoutDuration = std::chrono::seconds(30);
};