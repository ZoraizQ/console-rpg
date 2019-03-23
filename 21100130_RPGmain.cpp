#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "RPG.cpp"
using namespace std;
// Created by Zoraiz Qureshi

int main( )
{
    GameManager GM;
    int dir, choice;

    setConsoleColour(15);

    cout << "Would you like to load your game from a previous save file?\n1. Yes\n2. No\n";
    cin >> choice;
    if (choice == 1)
    {
        if (GM.loadGame("rpg-save.txt"))
        {
            cout << "\nLoaded game successfully.\n";
        }
        else
        {
            cout << "\nFailed to load previous game.\n";
        }
    }
    else if (choice == 2) cout << "Starting a new game.\n";

    while(true)
    {
        cout << "1. Move\n2. Show Player Status\n3. Drop an Item\n4. Pick up Item\n5. Craft\n6. Use Item\n7. Save Game\n";
        GM.displayMap();

        if (!GM.isPlayerAlive())
        {
            cout << "Player is dead. Game Over.\n";
            return 0;
        }
        cout << "What would you like to do?\n";
        cin >> choice;
        switch(choice)
        {
        case 1: //move
            cout << "Enter direction to move: ";
            cin >> dir;
            if (!GM.playerMove(dir))
            {
                cout << "Was unable to move to that spot.\n";
            }
            break;
        case 2: //showPlayerStatus
            GM.showPlayerStatus();
            break;
        case 3: //dropItem
            GM.dropItem();
            break;
        case 4: //pickItem
            GM.pickItem();
            break;
        case 5: //craft
            GM.craft();
            break;
        case 6: //use item outside fight
            int itemchoice;
            GM.showPlayerStatus();
            cout << "\nWhich item would you like to use?: ";
            cin >> itemchoice;
            GM.useItemOutsideFight(itemchoice);
            break;
        case 7:
            if (GM.saveGame("rpg-save.txt"))
            {
                cout << "\nSave successful.\n";
            }
            break;
        default:
            cout << "\nInvalid choice.\n";
        }
        cout << '\n';
        GM.minionMove();
        system("pause");
        system("cls");
    }
}
