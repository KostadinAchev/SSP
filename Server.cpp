#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

struct Question {
    std::string questionText;
    std::vector<std::string> options;
    std::string correctAnswer; // like "d"
};

std::vector<Question> loadQuestions() {
    std::vector<Question> questions;

    questions.push_back({
        "Q. When was Bulgaria founded?",
        {"a. 215", "b. 321", "c. 354", "d. 681"},
        "d"
        });

    questions.push_back({
        "Q. When was Bulgaria freed from the Ottomans?",
        {"a. 681", "b. 1024", "c. 1878", "d. 225"},
        "c"
        });

    questions.push_back({
        "Q. When was I born?",
        {"a. 2001", "b. 2002", "c. 2003", "d. 2004"},
        "c"
        });

    return questions;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    std::vector<Question> questions = loadQuestions();
    int score = 0;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server ready. Waiting for client...\n";

    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    std::cout << "Client connected.\n";

    for (size_t i = 0; i < questions.size(); ++i) {
        std::stringstream ss;
        ss << questions[i].questionText << "\n";
        for (const auto& option : questions[i].options) {
            ss << option << "\n";
        }

        std::string questionText = ss.str();
        send(new_socket, questionText.c_str(), questionText.length(), 0);

        memset(buffer, 0, sizeof(buffer));
        int readBytes = read(new_socket, buffer, sizeof(buffer));
        if (readBytes <= 0) {
            std::cerr << "Client disconnected or read error.\n";
            break;
        }

        std::string clientAnswer = buffer;
        clientAnswer.erase(clientAnswer.find_last_not_of(" \n\r\t") + 1);

        std::string feedback = (clientAnswer == questions[i].correctAnswer) ? "\nNext Question\n" : "\nNext Question\n";
        if (clientAnswer == questions[i].correctAnswer) {
            score++;
        }

        send(new_socket, feedback.c_str(), feedback.length(), 0);
    }

    std::string result = "Final score: " + std::to_string(score) + "/" + std::to_string(questions.size()) + "\n";
    send(new_socket, result.c_str(), result.length(), 0);

    // Close connection
    close(new_socket);
    close(server_fd);

    std::cout << "Client disconnected. Quiz finished.\n";
    return 0;
}
