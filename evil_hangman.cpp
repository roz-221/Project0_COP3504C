#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <cctype>

class Evil_Hangman {
public:
    std::vector<std::string> load_words(const std::string& filename) {
        std::ifstream dict_file(filename);
        std::vector<std::string> words;
        std::string word_line;

        if (!dict_file.is_open()) {
            std::cerr << "Error: could not open file " << filename << std::endl;
            return {};
        }

        while (std::getline(dict_file, word_line)) {
            // Convert to lowercase
            std::transform(word_line.begin(), word_line.end(), word_line.begin(), ::tolower);
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

    void menu(int lives, const std::string& dashedWord) {
        std::cout << "\n--- Current State ---" << std::endl;
        std::cout << "You have " << lives << " lives left." << std::endl;
        std::cout << "Word: " << dashedWord << std::endl;
    }

    std::vector<std::string> getWordLength(const std::vector<std::string>& dict) {
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

    std::string getPattern(const std::string& word, char guess, const std::string& currentPattern) {
        std::string pattern = currentPattern;
        for (size_t i = 0; i < word.size(); i++) {
            if (word[i] == guess) {
                pattern[i] = guess;
            }
        }
        return pattern;
    }

    std::string selectLargestFamily(const std::vector<std::string>& words, char guess, const std::string& currentPattern, std::vector<std::string>& updatedWords) {
        std::unordered_map<std::string, std::vector<std::string>> families;

        for (const std::string& word : words) {
            std::string pattern = getPattern(word, guess, currentPattern);
            families[pattern].push_back(word);
        }

        size_t maxSize = 0;
        std::string bestPattern;

        for (const auto& [pattern, group] : families) {
            if (group.size() > maxSize) {
                maxSize = group.size();
                bestPattern = pattern;
                updatedWords = group;
            }
        }
        return bestPattern;
    }

    void run() {
        std::vector<std::string> dict = load_words("dictionary.txt");
        std::vector<std::string> possibleWords = getWordLength(dict);

        int wordLength = possibleWords[0].length();
        std::string dashedWord(wordLength, '-');
        std::vector<char> guessedLetters;
        
        // Double lives because this algorithm is truly evil
        int lives = 10;

        while (lives > 0 && dashedWord.find('-') != std::string::npos) {
            menu(lives, dashedWord);

            char guess;
            while (true) {
                std::cout << "\nWhat is your next guess? ";
                std::cin >> guess;
                guess = tolower(guess);
                if (!isalpha(guess)) {
                    std::cout << "Invalid input. Please enter a letter.\n";
                } 
                else if (alreadyGuessed(guess, guessedLetters)) {
                    std::cout << "You already guessed that letter.\n";
                } 
                else {
                    break;
                }
            }
            guessedLetters.push_back(guess);
            std::vector<std::string> newWords;
            std::string newPattern = selectLargestFamily(possibleWords, guess, dashedWord, newWords);

            if (newPattern == dashedWord) {
                std::cout << "The letter '" << guess << "' is not in the word!\n";
                lives--;
            } else {
                std::cout << "The letter '" << guess << "' is in the word!\n";
            }

            dashedWord = newPattern;
            possibleWords = newWords;
        }

        if (dashedWord.find('-') == std::string::npos) {
            menu(lives, dashedWord);
            std::cout << "\nCongratulations! You win!" << std::endl;
        } 
        else {
            std::string actualWord = possibleWords[0];
            std::cout << "\nYou used up your last guess and lost!\nThe word was: " << actualWord << "." << std::endl;
        }
    }
};