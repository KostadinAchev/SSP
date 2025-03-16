#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Question {
    std::string questionText;
    std::vector<std::string> options;
};

static std::vector<Question> getData()
{
    std::string Text;
    std::vector<Question> Questions;
    std::ifstream readFile("test.txt");

    if (!readFile) {
        std::cerr << "Error opening file!" << std::endl;
        return Questions;
    }

    Question currentQuestion;
    while (getline(readFile, Text)) {
        if (Text.find("Q. ") != std::string::npos) { // New question
            if (!currentQuestion.questionText.empty()) {
                Questions.push_back(currentQuestion); // Add previous question
            }
            currentQuestion = Question();
            currentQuestion.questionText = Text;
        }
        else if (Text.find("a.") != std::string::npos || Text.find("b.") != std::string::npos ||
            Text.find("c.") != std::string::npos || Text.find("d.") != std::string::npos) {
            currentQuestion.options.push_back(Text);
        }

        if (Text.find("d.") != std::string::npos) {
            Questions.push_back(currentQuestion);  // Add the current question to the list
            currentQuestion = Question();  // Reset for the next question
        }
    }

    readFile.close();
    return Questions;
}

static int checkAnswers(std::string answer,int i)
{
    int result = 0;
    std::vector<std::string> Answers_str;
    std::string Text;
    std::ifstream Answers("Answers.txt");

    

    while (getline(Answers, Text)) {
        Answers_str.push_back(Text);
    }

   


    for (i; i <= Answers_str.size();) {
        if (answer.compare(Answers_str[i])==0) {
            
            return 1;
        }
        else
        {
            
            return 0;
        }

    }

}

int main() {
    std::string answer;
    int result=0;
    int loop = 0;

    std::vector<Question> Questions = getData();

    

    for (int i = 0; i < Questions.size(); i++) {
        std::cout << Questions[i].questionText << '\n';
        for (int j = 0; j < Questions[i].options.size(); j++) {
            std::cout << Questions[i].options[j] << '\n';
        }

        std::cout << "Answer: ";
        std::cin >> answer;

        result += checkAnswers(answer,i);
        loop = i;
    }
    std::cout << "Correct answers: " << result << "/" << loop + 1 << '\n';
}
