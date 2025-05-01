#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <fstream>
#include <random>

class Hangman {
public:
    std::vector<std::string> load_words(const std::string& path) {
        std::ifstream dict_file(path);
    
        if (!dict_file.is_open()) {
            std::cerr << "Error: could not open file " << path << std::endl;
            return {};
        }
    
        std::vector<std::string> words;
        std::string word_line;
        while (std::getline(dict_file, word_line)) {
            words.push_back(word_line);
        }
        return words;
    }
    
    std::vector<std::string> getWordsOfLength(int length, const std::vector<std::string>& dictionary) {
        std::vector<std::string> filtered;
        for (const std::string& word : dictionary) {
            if (word.length() == length) {
                filtered.push_back(word);
            }
        }
        return filtered;
    }
    
    bool alreadyGuessed(char guess, const std::vector<char>& guessedLetters) {
        return std::find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end();
    }
    
    std::string getRandomWord(std::mt19937 generator, const std::vector<std::string>& words) {
        while (true) {
            int num = generator();
            if (num < words.size()) {
                return words.at(num);
                break;
            }
        }
    }
    
    void menu(int lives, std::string dashedWord) {
        std::cout << "\n--- Current State ---" << std::endl;
        std::cout << "You have " << std::to_string(lives) << " lives left." << std::endl;
        std::cout << "Word: " << dashedWord << std::endl;
    }
    
    // Get valid word length
    std::vector<std::string> getWordLength(std::vector<std::string> dict) {
        int length;
        std::vector<std::string> possibleWords;
        while (possibleWords.empty()) {
            std::cout << "Enter desired word length: ";
            std::cin >> length;
            possibleWords = getWordsOfLength(length, dict);
            if (possibleWords.empty()) {
                std::cout << "No words of that length. Try again.\n";
            }
        }
        return possibleWords;
    }
    
    void run() {
        std::vector<std::string> dict = load_words("dictionary.txt");
        std::vector<std::string> possibleWords = getWordLength(dict);
        
        // Pick random word
        std::mt19937 number_generator(3504);
        std::string word = getRandomWord(number_generator, possibleWords);
        std::string dashedWord;
        for (int i = 0; i < word.length(); ++i) {
            dashedWord += "- ";
        }
        
        std::vector<char> guessedLetters;
        int lives = 5;
        int correct_letters = 0;
        
        while (lives > 0 && correct_letters < word.length()) {
            menu(lives, dashedWord);
            
            // Get user guess
            char guess;
            while (true) {
                std::cout << "\nWhat is your next guess? ";
                std::cin >> guess;
                guess = tolower(guess);
                if (!isalpha(guess)) {
                    std::cout << "Invalid input. Please enter a letter.\n";
                } else if (alreadyGuessed(guess, guessedLetters)) {
                    std::cout << "You already guessed that letter.\n";
                } else {
                    break;
                }
            }
    
            guessedLetters.push_back(guess);
    
            // Process guess
            bool correct = false;
            for (size_t i = 0; i < word.length(); i++) {
                if (word[i] == guess) {
                    dashedWord[2*i] = guess;
                    correct = true;
                    correct_letters++;
                }
            }
            if (correct) {
                std::cout << "The letter '"<< guess<< "' is in the word!\n";
            } 
            else {
                std::cout << "The letter '" << guess << "' is not in the word!\n";
                lives--;
            }
        }

        if (correct_letters == word.length()) {
            menu(lives, dashedWord);
            std::cout << "\nCongratulations! You win!" << std::endl;
        } 
        else {
            std::cout << "\nYou used up your last guess and lost!\nThe word was: " << word << "." << std::endl;
        }
    }
};