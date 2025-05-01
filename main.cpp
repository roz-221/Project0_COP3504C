#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <fstream>
#include <random>
#include "evil_hangman.cpp"
#include "hangman.cpp"

int main() {
    std::cout << "Welcome to COP 3504 C's Hangman!" << std::endl;
    
    int option;
    std::cout << "Would you like to play normal Hangman (1) or Evil Hangman (2)? ";
    std::cin >> option;
    
    if (option == 1) {
        Hangman game;
        game.run();
    }
    else if (option == 2) {
        Evil_Hangman evil_game;
        evil_game.run();
    }
    else {
        std::cerr << "Invalid input." << std::endl;
    }
    return 0;
}