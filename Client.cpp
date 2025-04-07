#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[2048] = { 0 };

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed\n";
        return 1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 2048);
        if (valread <= 0) break;

        std::string received = buffer;
        std::cout << received;

        if (received.find("Your final score") != std::string::npos) {
            break;
        }

        std::string answer;
        if (received.find("Q. ") != std::string::npos)
        {
            std::cout << "Your answer: ";
            std::cin >> answer;
        }

        send(sock, answer.c_str(), answer.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 2048);
        std::cout << buffer << "\n";
    }

    close(sock);
    return 0;
}
