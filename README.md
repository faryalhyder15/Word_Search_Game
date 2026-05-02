
# 📖 Word Search Game (C++)

## 🎮 Overview

The **Word Search Game** is a console-based C++ game where players search for hidden words inside a randomly generated grid. The game includes multiple levels, difficulty modes, scoring, and a leaderboard system.

Players select characters using row and column indices to construct words and match them with hidden words in the grid.

---

## ✨ Features

* 🔤 Random word grid generation (10x10, 15x15, 20x20)
* 🎯 Multiple difficulty modes:

  * Easy (short words)
  * Hard (long words)
* 📈 Scoring system
* ❤️ Limited chances (lives system)
* 🏆 Top 5 High Scores saved in file
* 📊 Multiple levels with increasing difficulty
* 🧠 Word validation system
* 🎲 Random word placement (Horizontal, Vertical, Diagonal)

---

## 🧩 Game Structure

### 1. **WordGame Class**

Handles:

* Menu system
* Game flow
* Levels & scoring
* High score management

### 2. **WordGrid Class**

Responsible for:

* Creating grid
* Placing words randomly
* Filling empty cells
* Displaying grid

### 3. **WordValidator Class**

Manages:

* Loading words from files
* Filtering words based on difficulty
* Providing word list to the game

---

## 🕹️ How to Play

1. Run the program
2. Select from menu:

   * Start Game
   * Instructions
   * About Us
3. Choose difficulty:

   * Easy → Short words (2–4 letters)
   * Hard → Longer words (5–7 letters)
4. For each level:

   * A grid will be displayed
   * Select number of characters
   * Enter row & column indices
   * Form a word
5. If correct:

   * Score increases
6. If wrong:

   * Chances decrease
7. Complete all levels to win 🎉

---

## 📊 Levels

| Level | Grid Size | Words to Guess |
| ----- | --------- | -------------- |
| 1     | 10x10     | 3 words        |
| 2     | 15x15     | 5 words        |
| 3     | 20x20     | 7 words        |

---

## 📂 File Structure

```
📁 Project Folder
│── main.cpp
│── header.h
│── easy.txt          # Easy mode words
│── hard.txt          # Hard mode words
│── implementation.cpp 
│── README.md
```

---

## 💾 High Score System

* Stores top 5 scores in `top_scores.txt`
* Automatically updates after each game
* Scores sorted in descending order

---

## ⚙️ Compilation & Run

### Using g++:

```bash
g++ main.cpp -o game
./game
```

### Using Visual Studio:

* Create a new C++ project
* Add all `.cpp` and `.h` files
* Run the project

---

## ⚠️ Requirements

* C++ compiler (g++, MSVC)
* Standard libraries:

  * `<iostream>`
  * `<fstream>`
  * `<cstdlib>`
  * `<ctime>`
  * `<string>`

---

## 🚀 Future Improvements

* GUI version (using SFML or Qt)
* Timer-based gameplay ⏱️
* Hint system 💡
* Multiplayer mode 👥
* Better word placement algorithm

---

## 📌 Notes

* Words are randomly placed each time → different experience 🎲
* Input validation is included to avoid crashes
* Game automatically handles errors and invalid input

