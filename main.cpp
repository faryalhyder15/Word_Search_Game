#include <iostream>
#include"Header.h"
// Main function
int main()
{
    srand(static_cast<unsigned int>(time(0)));
    WordGame game;
    game.displayMenu();
    system("pause");
    return 0;
}
