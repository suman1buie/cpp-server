#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int port = std::stoi(argv[1]);
    
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port " << port << "...\n";

    while(true){
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
        std::cout << "Client connected!\n";
        const char* msg = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>Hello from C++ Server!</h1></body></html>\r\n";
        send(client_fd, msg, strlen(msg), 0);
        close(client_fd);
    }
    close(server_fd);
    return 0;
}