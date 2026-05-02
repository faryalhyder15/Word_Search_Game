#include <iostream>
#include "header.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>

using namespace std;

// WordGame class implementation
WordGame::WordGame() : chances(5), mode(0), currentLevel(1), validator(new WordValidator()), grid(nullptr)
{
    for (int i = 0; i < 5; i++) topScores[i] = 0; // Initialize  5 topScores=0
}

WordGame::~WordGame()
{
    delete validator;
    if (grid) delete grid;
}

void WordGame::saveTopScores(int newScore)
{
    int scores[6] = { 0 }; // Array to store up to 5 scores and 1 for new score
    int count = 0; // Number of existing scores

    // Read existing scores from the file
    ifstream inputFile("top_scores.txt");
    if (inputFile)
    {
        while (inputFile >> scores[count] && count < 5)
        {
            count++;
        }
        inputFile.close();
    }

    // Add the new score
    scores[count++] = newScore;

    // Sort the scores in descending order
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (scores[i] < scores[j])
            {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Write only the top 5 scores back to the file
    ofstream outputFile("top_scores.txt");
    if (outputFile)
    {
        for (int i = 0; i < 5 && i < count; i++)
        {
            outputFile << scores[i] << endl;
        }
        outputFile.close();
    }

    // Display the updated top 5 scores
    cout << "Updated Top 5 Scores:\n";
    for (int i = 0; i < 5 && i < count; i++)
    {
        cout << scores[i] << endl;
    }
}
// display menu 
void WordGame::displayMenu()
{
    cout << "\033[34m" << "\n===== Word Guessing Game =====\n" << "\033[0m";
    while (true)
    {
        try {
            cout << "1. Start Game\n";
            cout << "2. Instructions\n";
            cout << "3. About Us\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;

            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                throw runtime_error("Invalid input! Please enter an integer.");
            }

            switch (choice) {
            case 1:
                startGame();
                break;
            case 2:
                showInstructions();
                break;
            case 3:
                aboutUs();
                break;
            case 4:
                cout << "Thank you for playing!\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

void WordGame::startGame()
{
    try
    {
        chances = 5;
        // Game mode selection
        cout << "\nChoose Mode:\n1. Easy\n2. Hard\nEnter your choice: ";
        cin >> mode;
        const string filename = (mode == 1) ? "easy.txt" : "hard.txt";

        if (mode != 1 && mode != 2) {
            throw std::invalid_argument("Invalid mode selected. Choose 1 for easy mode and 2 for hard mode.");
        }

        if (validator->loadWords(filename, mode) == 0) {
            cout << "No words loaded. Exiting game.\n";
            return;
        }

        int totalScore = 0;

        // Loop through levels
        for (currentLevel = 1; currentLevel <= 3; ++currentLevel)
        {
            if (chances <= 0) {
                cout << "Game Over! You ran out of chances.\n";
                saveTopScores(totalScore);
                displayMenu(); // Return to the main menu after the game ends
                return; // Exit the startGame function
            }
            playLevel(); // Continue with the level if chances are available
        }

        // If all levels completed
        if (chances > 0) {
            cout << "\nCongratulations! You completed all levels.\n";
        }

        saveTopScores(totalScore); // Save score if game completed

    }
    catch (const std::invalid_argument& e)
    {
        cout << "Error: " << e.what() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}


void WordGame::playLevel()
{


    try
    {
        int gridSize = (currentLevel == 1) ? 10 : (currentLevel == 2) ? 15 : 20;
        int wordsToGuess = (currentLevel == 1) ? 3 : (currentLevel == 2) ? 5 : 7;
        string* words = validator->getWords();
        int wordCount = validator->getWordCount();
        grid = new WordGrid(gridSize, words, wordCount);
        grid->placeWords();
        grid->fillGrid();
        int currentScore = 0;

        cout << "\nGenerated Word Grid (Level " << currentLevel << ", Grid Size: " << gridSize << "x" << gridSize << "):\n";
        grid->displayGrid();
        int correctGuesses = 0;

        while (chances > 0 && correctGuesses < wordsToGuess)
        {
            cout << "\nChances remaining: " << chances << endl;
            cout << "Current Score: " << currentScore << endl;
            cout << "Construct a word by selecting characters from the grid.\n";
            string selectedWord = "";
            int numCharsToSelect = 0;
            int minLength = (mode == 1) ? 2 : 5;
            int maxLength = (mode == 1) ? 4 : 7;
            while (true) // Loop until valid input is provided
            {
                cout << "Enter the number of characters to select (" << minLength << "-" << maxLength << "): ";
                cin >> numCharsToSelect;

                if (cin.fail()) // Check for invalid input
                {
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid input! Please enter an integer.\n";
                    continue; // Retry input
                }

                if (numCharsToSelect < minLength || numCharsToSelect > maxLength) // Check range
                {
                    cout << "Invalid number of characters. It must be between " << minLength << " and " << maxLength << ".\n";
                    continue; // Retry input
                }

                break; // Exit loop when valid input is received
            }



            for (int i = 0; i < numCharsToSelect; i++)
            {
                int row, col;
                while (true) // Loop for row input
                {
                    cout << "Input row (0-" << gridSize - 1 << "): ";
                    cin >> row;

                    if (cin.fail()) // Invalid input
                    {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << "Invalid input! Please enter a valid integer.\n";
                        continue; // Retry input
                    }

                    if (row < 0 || row >= gridSize) // Out of range
                    {
                        cout << "Row out of bounds! Please enter a value between 0 and " << gridSize - 1 << ".\n";
                        continue; // Retry input
                    }
                    break; // Exit loop when input is valid
                }

                while (true) // Loop for column input
                {
                    cout << "Input column (0-" << gridSize - 1 << "): ";
                    cin >> col;

                    if (cin.fail()) // Invalid input
                    {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << "Invalid input! Please enter a valid integer.\n";
                        continue; // Retry input
                    }

                    if (col < 0 || col >= gridSize) // Out of range
                    {
                        cout << "Column out of bounds! Please enter a value between 0 and " << gridSize - 1 << ".\n";
                        continue; // Retry input
                    }
                    break; // Exit loop when input is valid
                }


                if (row >= 0 && row < gridSize && col >= 0 && col < gridSize)
                {
                    char selectedChar = grid->getCharacter(row, col);
                    selectedWord += selectedChar;
                    cout << "Selected character at position [" << row << "][" << col << "] = " << selectedChar << endl;
                }
                else
                {
                    cout << "Invalid indices. Try again.\n";
                    i--;
                }
            }

            cout << "\033[31m" << endl << "You constructed the word: " << selectedWord << endl << "\033[0m" << endl;
            if (grid->isWordInGrid(selectedWord))
            {
                cout << "\033[33mCorrect! You guessed a word.\033[0m\n";
                correctGuesses++;
                currentScore += 10;
                cout << "Remaining words to guess: " << (wordsToGuess - correctGuesses) << endl;
            }
            else
            {
                cout << "Incorrect word.\n";
                chances--;
            }
        }

        if (correctGuesses >= wordsToGuess)
        {
            cout << "Level " << currentLevel << " complete! Moving to the next level.\n";
            saveTopScores(currentScore);
        }
        else
        {
            cout << "Game Over! You ran out of chances.\n";
            saveTopScores(currentScore);
        }

        delete grid;
        grid = nullptr;
        cout << "Final Score for Level " << currentLevel << ": " << currentScore << endl;
    }
    catch (const runtime_error& e)
    {
        cout << "Error: " << e.what() << endl;
        cout << "Returning to Level 1 due to an error.\n";
        // Clean up
        delete grid;
        grid = nullptr;
    }

}
void WordGame::showInstructions()
{
    cout << "\nInstructions:\n";
    cout << "1. Guess words hidden in the grid.The words are place horizontally,diagonally and vertically in the grid\n";
    cout << "2. Use row and column indices to select characters.\n";
    cout << "3. Complete all levels to win!\n";
    cout << "===============================================" << endl;
}

void WordGame::aboutUs()
{
    cout << "\nAbout Us:\n";
    cout << "This game was developed by a Rida Mehmood and Faryal Jafferi \n";
    cout << "===============================================" << endl;
}

// WordGrid class implementation
WordGrid::WordGrid(int size, string* wordList, int count) : gridSize(size), words(wordList), wordCount(count)
{
    grid = new char* [gridSize];
    for (int i = 0; i < gridSize; i++)
    {
        grid[i] = new char[gridSize];
        for (int j = 0; j < gridSize; j++)
        {
            grid[i][j] = '.'; // Initialize grid with empty cells
        }
    }
}
// deallocate the grid
WordGrid::~WordGrid()
{
    for (int i = 0; i < gridSize; i++)
    {
        delete[] grid[i];
    }
    delete[] grid;
}
// fill the remaining spaces in the grid by random words 
void WordGrid::fillGrid()
{
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            if (grid[i][j] == '.')
            {
                grid[i][j] = 'a' + rand() % 26; // Fill empty cells with random letters
            }
        }
    }
}
// shows which words are not load in grid 
void WordGrid::placeWords()
{
    for (int i = 0; i < wordCount; i++)
    {
        if (!placeWord(words[i]))
        {
            // Handle word placement failure
        }
    }
}
// place words horizontally,vertically and diagonally in the grid
bool WordGrid::placeWord(const string& word)
{
    int len = word.length();
    int orientation = rand() % 3; // 0: Horizontal, 1: Vertical, 2: Diagonal
    int row, col;
    for (int attempt = 0; attempt < 50; attempt++)
    {
        row = rand() % gridSize;
        col = rand() % gridSize;
        if (orientation == 0 && col + len <= gridSize)
        { // Horizontal
            bool canPlace = true;
            for (int i = 0; i < len; i++)
            {
                if (grid[row][col + i] != '.' && grid[row][col + i] != word[i])
                {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace)
            {
                for (int i = 0; i < len; i++)
                {
                    grid[row][col + i] = word[i];
                }
                return true;
            }
        }
        else if (orientation == 1 && row + len <= gridSize)
        { // Vertical
            bool canPlace = true;
            for (int i = 0; i < len; i++)
            {
                if (grid[row + i][col] != '.' && grid[row + i][col] != word[i])
                {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace)
            {
                for (int i = 0; i < len; i++)
                {
                    grid[row + i][col] = word[i];
                }
                return true;
            }
        }
        else if (orientation == 2 && row + len <= gridSize && col + len <= gridSize)
        { // Diagonal
            bool canPlace = true;
            for (int i = 0; i < len; i++)
            {
                if (grid[row + i][col + i] != '.' && grid[row + i][col + i] != word[i])
                {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace)
            {
                for (int i = 0; i < len; i++)
                {
                    grid[row + i][col + i] = word[i];
                }
                return true;
            }
        }
    }
    return false;
}
// this function display the grid
void WordGrid::displayGrid() const
{
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}
// this function get character from the specific row and column that we input
char WordGrid::getCharacter(int row, int col) const
{
    if (row >= 0 && row < gridSize && col >= 0 && col < gridSize)
    {
        return grid[row][col];
    }
    return '\0';
}
// this check either the word is present in grid or not
bool WordGrid::isWordInGrid(const string& word) const
{
    for (int i = 0; i < wordCount; i++)
    {
        if (words[i] == word)
        {
            return true;
        }
    }
    return false;
}
// WordValidator class implementation
WordValidator::WordValidator() : wordCount(0) {}

int WordValidator::loadWords(const string& filename, int mode)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return 0;
    }
    string line;
    int count = 0;
    while (getline(file, line) && count < 50)
    {
        int len = line.length();
        if (mode == 1 && (len == 2 || len == 3 || len == 4))
        { // Easy mode: 2, 3, or 4 character words
            words[count++] = line;
        }
        else if (mode == 2 && (len == 5 || len == 6 || len == 7))
        { // Hard mode: 5, 6, or 7 character words
            words[count++] = line;
        }
    }
    file.close();
    wordCount = count;
    return count;
}

string* WordValidator::getWords()
{
    return words;
}

int WordValidator::getWordCount() const
{
    return wordCount;
}
