#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
using namespace std;

// Forward Declarations
class WordGrid;
class WordValidator;

// Game class
class WordGame
{
private:
    int chances; // Remaining chances
    int mode; // Game mode: Easy or Hard
    int currentLevel;
    WordValidator* validator;
    WordGrid* grid;
    int topScores[5]; // Array to store top 5 scores

    void saveTopScores(int newScore);

public:
    WordGame();
    ~WordGame();
    void displayMenu();
    void showInstructions();
    void aboutUs();
    void playLevel();
    void startGame();
};

// WordGrid class manages the grid
class WordGrid
{
private:
    char** grid;
    int gridSize;
    string* words;
    int wordCount;

    bool placeWord(const string& word);

public:
    WordGrid(int size, string* wordList, int count);
    ~WordGrid();
    void fillGrid();
    void placeWords();
    void displayGrid() const;
    char getCharacter(int row, int col) const;
    bool isWordInGrid(const string& word) const;
};

// WordValidator class
class WordValidator
{
private:
    string words[50];
    int wordCount;

public:
    WordValidator();
    int loadWords(const string& filename, int mode);
    string* getWords();
    int getWordCount() const;
};
