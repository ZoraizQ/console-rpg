#ifndef RPG_H
#define RPG_H
#include <iostream>
#include <string>

using namespace std;

struct Item
{
    string itemName; // The name of item
    int itemCategory; // Items can be of two types. Weapons or HealthPotions. Weapons = 1. HealthPotions = 2.
    int points; // Points determine the effectiveness of the an item. If points of a weapon is 20 it would inflict a damage of 20 to the opposing character if used.
    // If points of a HealthPotion is 20 then it would increase the HP by 20 of the character using it.
    Item()
    {
        itemName = "Empty";
        itemCategory = 0;
        int points = 0;
    }
};

struct Character
{
    string characterType; // A string that would be representing the character. A character could be a "Player", a "Minion" or a "Boss".
    Item* inventory; // A player can collect item in his inventory. Minion and Boss characters have fixed weapons in inventory with no healthPortions.
    int HP; // HP of a character. A character dies if HP goes to 0. HP can't be more than 100.
    int inventorySize; // Size of inventory.
    Character()
    {
        characterType = "None";
        inventory = NULL;
        HP = 100;
        inventorySize = 0;
    }
};

struct Node
{
    Character *c; // Can host a character.
    Item item; // Can have an item which a player could pick up if the node is explored.
    int nodePosition[2]; // Position of node on the map

    int isVisited = 0; // Could either be a 0 or a 1. This could be helpful when writing the search algorithm.

    Node* North; // a pointer to adjacent node on north.
    Node* West;  // a pointer to adjacent node on west.
    Node* East;  // a pointer to adjacent node on east.
    Node* South; // a pointer to adjacent node on south.

    Node()
    {
        c = NULL;
        nodePosition[0] = -1;
        nodePosition[1] = -1;
        North = NULL;
        West = NULL;
        East = NULL;
        South = NULL;
    }

    // Links two nodes. 1 for North. 2 for West. 3 for East. 4 for South.
    // Directions are for the node calling the function i.e. if direction is 1 then this means that n should be linked with North.
    bool linkNode(Node* n,int direction);
};


class GameManager
{

private:
    string** map; // A 2d array of strings to show what a new node will contain. format of string = Explored/Character/itemName_Category_Points
    // Explored can be 0 or 1. Character can be P (player), M (minion), B (boss), N (no character). "_" represents no item.
    // Explored basically keeps track of the nodes which have been created in the memory and using it you have to link it with the other adjacent nodes.
    Node* head; // Use this to find all the other nodes in the game.
    Node* playerNode; // A pointer to the node which contains the player.
    Node** minionNodes; // An array of pointers pointing to the nodes containing minion.
    int minionNodesSize; // Size of array

    //int visited; // Could either be a 0 or a 1. This could be helpful when writing the search algorithm.
public:

    GameManager(); // Starts the game with a Player character in the head node and initializes the positions for player, minions, boss and items on the map.

    ~GameManager(); // Deallocate all the dynamic memory.

    bool playerMove(int direction); // Moves the player in a certain direction and changes the characterNode and player position on map accordingly.
    // 1 for North. 2 for West. 3 for East. 4 for South
    // it is possible that the player encounters some minion/boss and refuses to move or dies while fighting. In any case a new node has been explored.
    void searchNode(Node*& temp, Node* calling, int position[2], int direction); // Write an algorithm to find the node with position equal to the input position. You have to write a recursive algorithm
    // for it and then save its address in the temp node. To be used in playerMove
    // direction from which it is being called.
    // 0 for head. 1 for North. 2 for West. 3 for East. 4 for South.
    void minionMove(); // Move the available minions in minionNodes to an adjacent node which is already explored by the player.
    // Minions cannot explore map. Minions can only move to "adjacent nodes" with no character. Only player can initiate fight in movePlayer.

    bool fight(Node* enemyNode); // Use this function in playerMove to fight the boss or a minion. Returns true if the player has defeated the enemy (Make sure you NULL enemyNode.c if enemy character loses)
    // DO NOT NULL characterNode.c (Once a player has died the game would end and died player remains in that place)
    void showEnemy (Node* enemyNode); // Shows the health and inventory of opposing enemy. Use in fight function.

    void useItem(Item item, Node* character,int choice); // Uses the item. HealthPortions are self casting items and have to be removed from a players inventory.
    // Weapons can be used infinite times against the enemy.
    // Can be used inside or outside fight function.
    // choice represents the character has chosen. 0 for self cast. 1 for dodge. 2 for defend.
    void dropItem(); // Can only drop item if the current node has no item. Update the node and map accordingly. Use showPlayerStatus to look at your inventory.

    void pickItem(); // pick up the item in the current node. Update the node and map accordingly. Use showPlayerStatus to look at your inventory.

    void craft(); // Function for crafting items using items in Player's inventory. Use showPlayerStatus to look at your inventory.

    void displayMap(); // Displays the map discovered by the player. Only shows player and items if not picked up by player.
    // Call this function everytime playerMove is called just before returning from the function.
    bool saveGame(string fileName); // saves the game to be loaded later. fileName.c_str() might be helpful.

    bool loadGame(string fileName); // loads the saved game. fileName.c_str() might be helpful.

    void showPlayerStatus(); // Displays the player health and inventory. Can be used in fight.

    bool isPlayerAlive(); // Returns if player is alive or not

    void useItemOutsideFight(int ichoice);
};

#endif
