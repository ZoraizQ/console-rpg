#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string>
#include <windows.h>
#include "RPG.h"
// Created by Zoraiz Qureshi

using namespace std;

void setConsoleColour(int colour)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush(); //To ensure that the active screen buffer is cleared and the characters are written to their destination.
    SetConsoleTextAttribute(hOut, colour);
    //Sets the console text color attribute to specified 'colour' value.
}

GameManager::GameManager()
{
    /*
    //LINKED by LINK function
    //Node is created when explored by player. So map initially starts with single node containing the player character
    //HEAD node - is the initially created node by the constructor, it is just like the head of a linked list, in this case the head of this map structure
    RANDOMLY determine position on map containing BOSS, PLAYER, HEALTH POTION, MINIONS 1-4, AXE, DIVINE SWORD PIECES 1-3
    SINGLE NODE IN THE BEGINNING - head node points to this, player node points to this
    ALL OTHER positions UNEXPLORED (0), while head node position 1
    There is only one node
    */
    int n = 7, i,j;
    Node* temp = new Node();
    Character* playerchar = new Character();
//7x7 or nxn map generated
    map = new string*[n];
    for (i = 0; i < n; i++)
    {
        map[i] = new string[n];
    }
//initialized to unexplored '0', NULL 'N' no character, '_' empty item
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            map[i][j] = "0/N/_";
        }
    }

    srand(time(NULL)); //use time(NULL) as seed for random generator
    int task = 0;
    while(task != 12)
    {
        i = rand()%7;
        j = rand()%7;
        //since tasks 0 to 5 are characters, they can stand over items as well, so from tasks 6 onwards we only check if there is an empty item node for the items except that of the Boss
        if ((task >= 0 && task <= 5 && map[i][j].substr(0,4) == "0/N/") || (task >= 6 && map[i][j][4] == '_' && map[i][j][2] != 'B' && map[i][j][2] != 'P'))  //separate conditions for character and item tasks
        {
            switch(task)
            {
            case 0: //task 0: assign player node random position
                map[i][j] = "1/P/_";
//create player node here, same as head initially
                playerchar->characterType = "Player";
                playerchar->HP = 100;
                playerchar->inventorySize = 6;
                playerchar->inventory = new Item[playerchar->inventorySize];
//Starting item 1: Sword
                playerchar->inventory[0].itemName = "Sword";
                playerchar->inventory[0].itemCategory = 1;
                playerchar->inventory[0].points = 20;
//Starting item 2: Health Potion
                playerchar->inventory[1].itemName = "HealingPotion";
                playerchar->inventory[1].itemCategory = 2;
                playerchar->inventory[1].points = 20;
//the rest are initialized to EMPTY by constructor in Item, inventory[2] to inventory[5]
                temp->c = playerchar;
                temp->nodePosition[0] = i;
                temp->nodePosition[1] = j;
                head = temp;
                playerNode = temp;
                break;
            case 1: //task 1: assign boss random position
                map[i][j] = "0/B/_";

                break;
            case 2:
            case 3:
            case 4:
            case 5: //task 2,3,4,5: assign 4 minions
                map[i][j] = "0/M/_";

                break;
            case 6:
            case 7: //task 6,7 assign HP potion 1,2
                map[i][j] =  map[i][j].substr(0,4) + "GreaterHealingPotion_2_30";

                break;
            case 8: //task 8: assign Axe
                map[i][j] =  map[i][j].substr(0,4) + "Axe_1_40";

                break;
            case 9:
            case 10:
            case 11://task 9,10,11: 3 DS fragments
                map[i][j] =  map[i][j].substr(0,4) + "DSFragment_1_0";

                break;
            }
            task++;
        }
    }
    minionNodes = NULL;
    minionNodesSize = 0;
}

GameManager::~GameManager()
{
    cout << "\nDestructor running!\n";
    int pos[2];
    //delete all existing nodes and their contents by finding their address using searchnode
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (map[i][j][0] == '1')
            {
                pos[0] = i;
                pos[1] = j;
                Node* delnode;
                searchNode(delnode, head, pos, 0);
                if (delnode != NULL)
                {
                    //delnode now contains the node to be deleted
                    if (delnode->c != NULL)
                    {
                        if (delnode->c->inventory != NULL)
                        {
                            delete delnode->c->inventory;
                            delnode->c->inventory = NULL;
                        }
                        delete delnode->c;
                        delnode->c = NULL;
                    }
                    delete delnode;
                    cout << "Node and its contents deleted at " << i << " " << j << endl;
                }
            }
        }
    }

    head = NULL;
    //delete head;
    delete minionNodes;
    minionNodes = NULL;
    minionNodesSize = 0;
    playerNode = NULL;
    for (int i = 0; i < 7; i++)
    {
        delete map[i];
    }
    delete map;
    map = NULL;

}

void GameManager::showEnemy (Node* enemyNode)
{
    cout << "\n[Enemy Status]" << "\nEnemy: " << enemyNode->c->characterType << "\nHP: "<< enemyNode->c->HP << "\nInventory: \n";
    for (int i = 0; i < enemyNode->c->inventorySize; i++)
    {
        Item currItem =  enemyNode->c->inventory[i];
        cout << i << ". ";
        if (currItem.itemCategory == 1) cout << "[Weapon] ";
        else cout << "[HealthPotion] ";
        cout << currItem.itemName << " (" << currItem.points << ") \n";
    }
}


void GameManager::displayMap()
{
    //Map Displayed
    setConsoleColour(1);
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            setConsoleColour(1);
            if (map[i][j][0] == '1')
            {
                if (map[i][j][2] == 'P') setConsoleColour(10);
                cout << '1';
                if (map[i][j][4] == '_') cout << "/-";//if empty - no item present
                else cout << "/I";
            }
            else cout << " _ ";
            cout << ' ';
        }
        cout << endl;
    }
    setConsoleColour(15);

    /*
    cout << "\nTESTING FUNCTION - PRINTING ALL MAP STRINGS\n";
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            setConsoleColour(8);
            if (map[i][j][0] == '1') setConsoleColour(2);
            if (map[i][j][2] == 'P') setConsoleColour(10);
            if (map[i][j][2] == 'M') setConsoleColour(4);
            if (map[i][j][2] == 'B') setConsoleColour(12);
            cout << map[i][j].substr(0,5) << "     ";
        }
        cout << endl;
    }
    setConsoleColour(15);
    */
}

void GameManager::dropItem()
{
    int choice;
    int x = playerNode->nodePosition[0], y = playerNode->nodePosition[1];

    if (playerNode->item.itemName != "Empty")
    {
        cout << "\nThere is already an item present in this spot. Cannot drop another item.\n";
        return;
    } //else there is space

    cout << "Which item would you like to drop? (0-5)\n";
//display player status
    showPlayerStatus();

    cout << "\nYour choice: ";
    cin >> choice;
    if (choice < 0 || choice > 5)
    {
        cout << "Invalid choice.\n";
        return;
    }
    Item chosenItem = playerNode->c->inventory[choice];
    if (chosenItem.itemName == "Empty")
    {
        cout << "There is no item present in this slot.\n";
        return;
    }
    else
    {
        playerNode->item = chosenItem; //place item in current node's item spot
//remove from inventory
        playerNode->c->inventory[choice].itemName = "Empty";
        playerNode->c->inventory[choice].itemCategory = 0;
        playerNode->c->inventory[choice].points = 0;
//where map[x][y].substr(0,4) should be equal to "1/P/"
        string itemCatStr, itemPointsStr;
        if (chosenItem.itemCategory == 1) itemCatStr = "1";
        else itemCatStr = "2";
        stringstream ss;
        ss << chosenItem.points;
        itemPointsStr = ss.str();
        map[x][y] = map[x][y].substr(0,4) + chosenItem.itemName + '_' + itemCatStr + '_' + itemPointsStr;
        cout << "The item " << chosenItem.itemName << " was dropped from the Player's inventory.\n";
    }
}

void GameManager::pickItem()
{
    int x = playerNode->nodePosition[0], y = playerNode->nodePosition[1];
    Item currNodeItem = playerNode->item;
    int nextEmptyIndex;
    bool freeSpace = false;

    if (currNodeItem.itemName == "Empty")  //if there no item to pick up
    {
        cout << "\nThere is nothing to pick up in this spot.";
        return;
    }

//check for next free space in inventory
    for (int i = 0; i < playerNode->c->inventorySize; i++)
    {
        if (playerNode->c->inventory[i].itemName == "Empty")
        {
            nextEmptyIndex = i;
            freeSpace = true;
            break;
        }
    }

    if (freeSpace)
    {
        playerNode->c->inventory[nextEmptyIndex] = currNodeItem;
//where map[x][y].substr(0,4) should be equal to "1/P/"
        playerNode->item.itemName = "Empty";
        playerNode->item.itemCategory = 0;
        playerNode->item.points = 0;
        map[x][y] = map[x][y].substr(0,4) + '_';
        cout << "\nThe item " << currNodeItem.itemName << " was picked up.\n";
    }
    else
    {
        cout << "\nInventory is full. Cannot pick up item.\n";
        return;
    }
}

bool GameManager::isPlayerAlive()
{
    if (playerNode->c->HP <= 0) return false;
    else return true;
}

void GameManager::showPlayerStatus()
{
    cout << "\n[Player Status]" << "\nHP: " << playerNode->c->HP << "\nInventory: \n";
    for (int i = 0; i < playerNode->c->inventorySize; i++)
    {
        Item currItem =  playerNode->c->inventory[i];
        cout << i << ". ";
        if (currItem.itemCategory == 1) cout << "[Weapon]";
        else if (currItem.itemCategory == 2) cout << "[HealthPotion] ";
        cout << currItem.itemName << " (" << currItem.points << ") \n";
    }
}

bool Node::linkNode(Node* n, int direction)
{
    if (n != NULL)
    {
        switch(direction)
        {
        case 1:
            North = n;
            //n->South = this;
            return true;
            break;
        case 2:
            West = n;
            //n->East = this;
            return true;
            break;
        case 3:
            East = n;
            //n->West = this;
            return true;
            break;
        case 4:
            South = n;
            //n->North = this;
            return true;
            break;
        }
    }
    else
    {
//if n is NULL
//cout << "Node to be linked with does not exist.\n";
        return false;
    }
}

bool GameManager::playerMove(int direction)
{
    int px, py, nx, ny; //px,py position coordinates of playerNode, nx,ny position coordinates of nextNode
    Node* nextNode = NULL;
    Node** minionNodesCopy = NULL;
    Character* nextChar = NULL;
    px = playerNode->nodePosition[0];
    py = playerNode->nodePosition[1];
    switch(direction)
    {
    case 1: //North
        //(3,4) is north of (4,4)
        cout << "Moving North.\n";
        nx = px-1;
        ny = py;
        break;
    case 2: //West
        //(4,3) is west of (4,4)
        cout << "Moving West.\n";
        nx = px;
        ny = py-1;
        break;
    case 3: //East
        cout << "Moving East.\n";
        nx = px;
        ny = py+1;
        break;
    case 4: //South
        cout << "Moving South.\n";
        nx = px+1;
        ny = py;
        break;
    default:
        cout << "\nInvalid direction. Cannot move.\n";
        return false;
        break;
    }

//if the position generated (nx/ny) any of them are out of bounds of the map, we cannot move to this location
    if (nx > 6 || nx < 0 || ny > 6 || ny < 0)
    {
        cout << "Cannot move outside the map.\n";
        return false;
    }

//map string for the nextNode's position
    string currMapStr = map[nx][ny];

    if (currMapStr[0] == '0')
    {
        //create a node if it does not exists
        map[nx][ny][0] = '1'; //updated map string to explored
        nextNode = new Node;
        nextNode->nodePosition[0] = nx;
        nextNode->nodePosition[1] = ny;

        //spawn Character
        switch(currMapStr[2])
        {
        case 'N':
            //no character here, proceed to move
            nextChar = NULL;
            break;
        case 'M':
            //creating a Minion Character
            minionNodesSize++;
            minionNodesCopy = new Node*[minionNodesSize];
            for (int i = 0; i < minionNodesSize-1; i++)
            {
                minionNodesCopy[i] = minionNodes[i];
            }
            minionNodesCopy[minionNodesSize-1] = nextNode;
            delete minionNodes;
            minionNodes = minionNodesCopy;

            nextChar = new Character;
            nextChar->characterType = "Minion";
            nextChar->HP = 100;
            nextChar->inventorySize = 1;
            nextChar->inventory = new Item[1];
            nextChar->inventory[0].itemName = "Dagger";
            nextChar->inventory[0].itemCategory = 1;
            nextChar->inventory[0].points = 15;
            break;
        case 'B':
            //creating a Boss Character
            nextChar = new Character;
            nextChar->characterType = "Boss";
            nextChar->HP = 100;
            nextChar->inventorySize = 1;
            nextChar->inventory = new Item[1];
            nextChar->inventory[0].itemName = "Scythe";
            nextChar->inventory[0].itemCategory = 1;
            nextChar->inventory[0].points = 85;
            break;
        }
        //insert this character into the nextNode's character spot
        nextNode->c = nextChar;

        //if there IS an item to be generated
        if(currMapStr[4] != '_')
        {
            string itemNameStr = "", pointsStr = "";
            int category = 0, points = 0;
            int i = 4;
            while (currMapStr[i] != '_')
            {
                itemNameStr += currMapStr[i];
                i++;
            }
            i++;
            category = atoi(currMapStr.substr(i,1).c_str());
            i+=2;
            while (currMapStr[i] != '\0')
            {
                pointsStr += currMapStr[i];
                i++;
            }
            points = atoi(pointsStr.c_str());
            nextNode->item.itemName = itemNameStr;
            nextNode->item.itemCategory = category;
            nextNode->item.points = points;
        }


        nextNode->linkNode(nextNode->North, 1);

        nextNode->linkNode(nextNode->East, 3);

        nextNode->linkNode(nextNode->West, 2);

        nextNode->linkNode(nextNode->South, 4);

        if (!playerNode->linkNode(nextNode,direction))
        {
            cout << "Failed to link the player node to the new node.\n";
            return false;
        }
    }
    else
    {
//Node exists
        int pos[2];
        pos[0] = nx;
        pos[1] = ny;
        searchNode(nextNode,head,pos,direction);
        if (nextNode == NULL)
        {
            cout << "Node not found\n";
            return false;
        }
//nextNode now contains the node we're headed towards as it exists
    }
//the node should now exist in either cases

    //moving part comes here
    if (map[nx][ny][2] == 'N')  //no character stands in front of player (on next node position)
    {
        map[playerNode->nodePosition[0]][playerNode->nodePosition[1]][2] = 'N';
        map[nx][ny][2] = 'P';
        nextNode->c = playerNode->c; //copy player character pointer to nextNode's character pointer
        playerNode->c = NULL;
        //player has moved to the next node, so playerNode now points to the next node
        playerNode = nextNode;
        return true;
    }
    else
    {
        int choice;
        cout << "The shadow of an enemy looms over you.\n";
        showEnemy(nextNode);
        cout << "\nWould you like to initiate combat or flee?\n1. Fight\n2. Flee\n";
        cin >> choice;
        if (choice == 1)
        {
            if (fight(nextNode) == true)
            {
                cout << "You won the fight!\n";
                map[playerNode->nodePosition[0]][playerNode->nodePosition[1]][2] = 'N';
                map[nx][ny][2] = 'P';
                delete nextNode->c; //clear previous character
                nextNode->c = playerNode->c;
                playerNode = nextNode;
                return true;
            }
            else
            {
                cout << "You lost the fight.\n";
                return false;
            }
        }
        else
        {
            cout << "Fled from the battle safely!\n";
            return false;
        }
    }
}

bool GameManager::fight(Node* enemyNode)
{
    system("cls");
    cout << "An enemy " << enemyNode->c->characterType << " has appeared!\n";
    int playerchoice,enemychoice, itemchoice;

    while (true)
    {
        showEnemy(enemyNode);
        showPlayerStatus();
        if (enemyNode->c->HP <= 0)
        {
            cout << "The enemy " << enemyNode->c->characterType << " was defeated!\n";
            if (enemyNode->c->characterType == "Boss")
            {
                cout << "\nCONGRATULATIONS! You defeated the Boss! YOU WIN.\n";
                return 0;
            }
        }
        else if (!isPlayerAlive())
        {
            return false;
        }
//player's attack phase, enemy's defense phase
RETRY:
        cout << "Prepare to attack!\n1. Attack\n2. Self-cast Potion\n3. Skip Turn\n";
        cin >> playerchoice;
        enemychoice = rand() % 2 + 1; //1 to 2 random choice to dodge or defend
        switch(playerchoice)
        {
        case 1:
            cout << "\nSelect item to attack with from your inventory: ";
            cin >> itemchoice;
            useItem(playerNode->c->inventory[itemchoice],enemyNode, enemychoice);
            break;
        case 2:
            cout << "\nSelect item to self-cast from your inventory: ";
            cin >> itemchoice;
            useItem(playerNode->c->inventory[itemchoice],playerNode, 0); //0 for self-cast
            break;
        case 3:
            cout << "\nYou chose not to attack this turn.\n";
            break;
        default:
            cout << "\nInvalid choice.\n";
            goto RETRY;
            break;
        }

        system("pause");
        system("cls");
        showEnemy(enemyNode);
        showPlayerStatus();
        if (enemyNode->c->HP <= 0)
        {
            cout << "The enemy " << enemyNode->c->characterType << " was defeated!\n";
            if (enemyNode->c->characterType == "Boss")
            {
                cout << "\nCONGRATULATIONS! You defeated the Boss! YOU WIN.\n";
//exit game
            }
            return true;
        }
        else if (!isPlayerAlive())
        {
            return false;
        }

RETRY2:
        cout << "Enemy's turn to attack! Get ready.\n1. Dodge \n2. Defend\n";
        cin >> playerchoice;
        if (playerchoice == 1 || playerchoice == 2) useItem(enemyNode->c->inventory[0],playerNode, playerchoice); //enemy weapon
        else
        {
            cout << "\nInvalid choice.";
            goto RETRY2;
        }
        system("pause");
        system("cls");
    }
}
/*
    Node* find, (initially NULL)
    Traverse structure from head temp to ALL possible nodes RECURSIVELY, if a temp was found in the specific position (1 on the map) then return the temp address (temp), else return NULL in base case if temp not found
    e.g. the address of the temp at (1,2) position would be returned to the temp find if
    find.nodePosition[0] = 1, find.nodePosition[1] = 2---- if a temp exists there(edited)

    if (temp == NULL){
    return NULL;
    }
    */
void GameManager::searchNode(Node*& temp, Node* calling, int position[2], int direction)
{
    if (calling==NULL)
    {
        return;
    }
    else if (calling->isVisited==1)
    {
        return;
    }

    if(calling->nodePosition[0]==position[0] && calling->nodePosition[1]==position[1])
    {
        temp = calling;
    }
    else
    {
        calling->isVisited = 1;
        searchNode(temp,calling->North,position,direction);
        searchNode(temp,calling->West,position,direction);
        searchNode(temp,calling->East,position,direction);
        searchNode(temp,calling->South,position,direction);
    }

    calling->isVisited = 0;
}

void GameManager::craft()
{
    showPlayerStatus();
    int choice;
    int NoOfPieces = 0;
    int s1,s2,s3;
    cout << "\nWhat would you like to craft? \n1. DivineSword\n";
    cin >> choice;

    if (choice == 1)
    {
//crafting the DS
//search for 3 pieces
        for(int i=0; i<6; i++)
        {
            if(playerNode->c->inventory[i].itemName=="DSFragment")
            {
                NoOfPieces++;
                if(NoOfPieces==1) s1 = i;
                else if(NoOfPieces==2) s2 = i;
                else if(NoOfPieces==3) s3 = i;
            }
        }
        if(NoOfPieces==3)
        {
            cout<<"The DivineSword was placed in your inventory in s "<<s1;
            playerNode->c->inventory[s1].itemName = "DivineSword";
            playerNode->c->inventory[s1].itemCategory = 1;
            playerNode->c->inventory[s1].points = 85;

            playerNode->c->inventory[s2].itemName = "Empty";
            playerNode->c->inventory[s2].itemCategory = 0;
            playerNode->c->inventory[s2].points = 0;

            playerNode->c->inventory[s3].itemName = "Empty";
            playerNode->c->inventory[s3].itemCategory = 0;
            playerNode->c->inventory[s3].points = 0;
        }
        else
        {
            cout<<"You do not have enough fragments to craft the DivineSword.";
        }
    }
    else
    {
        cout << "Invalid choice.\n";
        return;
    }
}


void GameManager::useItem(Item item, Node* character,int choice)
{
    /*
    3 choices: 0, 1, 2
    check the choice
    if 0 , that is for self-cast,
    character HP += item.points;
    remove item from inventory (find first item with the same name in inventory, empty it "", 0,0 and then break)(edited)
    //if 1,  50% chance (random) to dodge.
    int chance = rand()%2;
    if chance = 0 then dodged,
    else if chance = 1 then character->c->HP -= item.points;
    if 2, then defend, that is character gets half the damage
    character HP -= 0.5*item.points;
    */
    int chance, damage;
    switch(choice)
    {
    case 0: //self-cast
        if (item.itemCategory == 1)
        {
            cout << "\nCannot self-cast a weapon.\n";
            return;
        }
        else if (item.itemCategory == 0)
        {
            cout << "\nThere is no item present in this slot.\n";
            return;
        }
        //HP Potion used
        if (playerNode->c->HP == 100)
        {
            cout << "\nHP already full!\n";
            return;
        }
        cout << "\nPlayer used the item " << item.itemName << "\n";
        int hpRestored;
        if (item.points > (100-playerNode->c->HP)) hpRestored = 100-playerNode->c->HP; //we add up the difference only, to ensure it becomes 100
        else hpRestored = item.points; //else add the complete value
        playerNode->c->HP += hpRestored;
        cout << ".\nRestored " << hpRestored << " HP!\n";
        for (int i = 0; i < 6; i++)  //find first item with same properties in inventory, and remove it (empty its slot)
        {
            if (playerNode->c->inventory[i].itemName == item.itemName)
            {
                playerNode->c->inventory[i].itemName = "Empty";
                playerNode->c->inventory[i].itemCategory = 0;
                playerNode->c->inventory[i].points = 0;
                break;
            }
        }
        break;
    case 1: //dodge
        //Each player has a 50% chance of dodging the other players attack
        chance = rand()%2; //0 or 1
        if (item.itemCategory == 2)
        {
            cout << "\nCannot attack with an item of this category.\n";
            return;
        }
        damage = chance*item.points;
        if (chance == 1) cout << character->c->characterType << " was unable to evade the attack! Took " << damage << " damage.\n";
        else cout << character->c->characterType << " dodged the attack!\n";
        if (damage > character->c->HP) damage = character->c->HP;
        character->c->HP -= damage; //if chance 1, then full damage, else 0 deduction - points nullified
        break;
    case 2: //defend
        //Your chance of dodging the other players attack falls to 0% but you get half the damage of other players attack
        if (item.itemCategory == 2)
        {
            cout << "\nCannot attack with an item of this category.\n";
            return;
        }
        damage = 0.5*item.points;
        cout << character->c->characterType << " defended the attack, was inflicted with " << damage << " damage.\n";
        if (damage > character->c->HP) damage = character->c->HP;
        character->c->HP -= damage;
        break;
    }
}

void GameManager::useItemOutsideFight(int ichoice)
{
    useItem(playerNode->c->inventory[ichoice],playerNode,0);
    return;
}

void GameManager::minionMove()
{
    int nx, ny, px, py, randDir;
    Node* nextNode;
    for (int i = 0; i < minionNodesSize; i++)
    {
        randDir = rand() % 4 + 1;
        if (minionNodes[i]->c == NULL)  //this minion was eliminated by the player
        {
            continue;
        }
        px = minionNodes[i]->nodePosition[0];
        py = minionNodes[i]->nodePosition[1];
        switch(randDir)
        {
        case 1: //North
            nx = px-1;
            ny = py;
            nextNode = minionNodes[i]->North;
            break;
        case 2: //West
            nx = px;
            ny = py-1;
            nextNode = minionNodes[i]->West;
            break;
        case 3: //East
            nx = px;
            ny = py+1;
            nextNode = minionNodes[i]->East;
            break;
        case 4: //South
            nx = px+1;
            ny = py;
            nextNode = minionNodes[i]->South;
            break;
        }

        if (nx > 6 || nx < 0 || ny > 6 || ny < 0)
        {
//cannot move outside map
            continue; //to next minion iteration
        }

        if (map[nx][ny].substr(0,4) == "1/N/")
        {
//"1/N/" string found
            if (!minionNodes[i]->linkNode(nextNode,randDir))
            {
                continue;
            }
            map[px][py][2] = 'N';
            map[nx][ny][2] = 'M';
            nextNode->nodePosition[0] = nx;
            nextNode->nodePosition[1] = ny;
            nextNode->c = minionNodes[i]->c; //copy minion i character pointer to nextNode's character pointer
            //minion i has moved to the next node, so minionNodes[i] now points to the next node
            minionNodes[i] = nextNode;
            return;
        }
//else unexplored region, cannot move here or space already occupied by a character, minion i's turn to move skipped
    }
}


bool GameManager::saveGame(string fileName)
{
    int n = 7, i, j;
    ofstream outFile;
    outFile.open(fileName.c_str());
    if (outFile.fail())
    {
        cout << "Failed to write to the file. Save failed.\n";
        return false;
    }
//save head node coordinates
    outFile << head->nodePosition[0] << '\t' << head->nodePosition[1] << '\n';

//save player hp
    outFile << playerNode->c->HP << '\n';
//save player inventory
    for (i = 0; i < 6; i++)
    {
        outFile << playerNode->c->inventory[i].itemName << '\t' << playerNode->c->inventory[i].itemCategory << '\t' << playerNode->c->inventory[i].points << '\n';;
    }

//save all map strings
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            outFile << map[i][j] <<'\t';
        }
        outFile << '\n';
    }

    outFile.close();
    cout << "Your game progress was successfully saved in the file '" + fileName + "' in the program directory.\n";
    return true;
}

bool GameManager::loadGame(string fileName)
{
    int n = 7, i, j, headx, heady;
    Node* nextNode;
    Character* nextChar;
    Node** minionNodesCopy;
    ifstream inFile;
    Node* nodePtrArr[7][7] = {};
    inFile.open(fileName.c_str());
    if (inFile.fail())
    {
        cout << "Failed to write to the file. Save failed.\n";
        return false;
    }
    int tempHP;
    Item* tempInventory = new Item[6];

//head x and y coordinates
    inFile >> headx >> heady;

//load player hp
    inFile >> tempHP;

//load player inventory
    for (i = 0; i < 6; i++)
    {
        inFile >> tempInventory[i].itemName;
        inFile >> tempInventory[i].itemCategory;
        inFile >> tempInventory[i].points;
    }


//load all map strings
    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 7; j++)
        {
            inFile >> map[i][j];

            string currMapStr = map[i][j];

            if(currMapStr[0] == '1')
            {
                nextNode = new Node;
                nextChar = NULL;
                nextNode->nodePosition[0] = i;
                nextNode->nodePosition[1] = j;
                nodePtrArr[i][j] = nextNode;
                //spawn Character
                switch(currMapStr[2])
                {
                case 'P':
                    playerNode = nextNode;
                    nextChar = new Character;
                    nextChar->characterType = "Player";
                    nextChar->HP = tempHP;
                    nextChar->inventorySize = 6;
                    nextChar->inventory = tempInventory;
                    break;
                case 'M':
                    //creating a Minion Character
                    minionNodesSize++;
                    minionNodesCopy = new Node*[minionNodesSize];
                    for (int i = 0; i < minionNodesSize-1; i++)
                    {
                        minionNodesCopy[i] = minionNodes[i];
                    }
                    minionNodesCopy[minionNodesSize-1] = nextNode;
                    delete minionNodes;
                    minionNodes = minionNodesCopy;

                    nextChar = new Character;
                    nextChar->characterType = "Minion";
                    nextChar->HP = 100;
                    nextChar->inventorySize = 1;
                    nextChar->inventory = new Item[1];
                    nextChar->inventory[0].itemName = "Dagger";
                    nextChar->inventory[0].itemCategory = 1;
                    nextChar->inventory[0].points = 15;
                    //insert this character into the nextNode's character spot
                    break;
                case 'B':
                    //creating a Boss Character
                    nextChar = new Character;
                    nextChar->characterType = "Boss";
                    nextChar->HP = 100;
                    nextChar->inventorySize = 1;
                    nextChar->inventory = new Item[1];
                    nextChar->inventory[0].itemName = "Scythe";
                    nextChar->inventory[0].itemCategory = 1;
                    nextChar->inventory[0].points = 85;
                    //insert this character into the nextNode's character spot
                    break;
                }
                nextNode->c = nextChar;
                //if there IS an item to be generated
                if(currMapStr[4] != '_')
                {
                    string itemNameStr = "", pointsStr = "";
                    int category = 0, points = 0;
                    int i = 4;
                    while (currMapStr[i] != '_')
                    {
                        itemNameStr += currMapStr[i];
                        i++;
                    }
                    i++;
                    category = atoi(currMapStr.substr(i,1).c_str());
                    i+=2;
                    while (currMapStr[i] != '\0')
                    {
                        pointsStr += currMapStr[i];
                        i++;
                    }
                    points = atoi(pointsStr.c_str());
                    nextNode->item.itemName = itemNameStr;
                    nextNode->item.itemCategory = category;
                    nextNode->item.points = points;
                }

            }
        }
    }


    Node * tempHead;
    int hpos[2] = {headx,heady};
    searchNode(tempHead,head,hpos,0);
    head = tempHead;//head will now point to the head node position of the load file

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            Node* currNode = nodePtrArr[i][j];
            if(currNode)
            {
                head = currNode;
                for (int x = i - 1; x <= i + 1; x++)
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        if (x > 6 || x < 0 || y > 6 || y < 0)
                            continue;

                        if (nodePtrArr[x][y])
                        {
                            Node * exploredN = nodePtrArr[x][y];

                            if (x == i-1 && y == j) //N
                                currNode->linkNode(exploredN,1);
                            else if (x == i && y == j-1) //W
                                currNode->linkNode(exploredN,2);
                            else if (x == i && y == j+1) //E
                                currNode->linkNode(exploredN,3);
                            else if (x == i+1 && y == j) //S
                                currNode->linkNode(exploredN,4);

                        }

                    }
                }

            }
        }
    }

    inFile.close();
    cout << "Your game progress was successfully loaded from the file '" + fileName + "' in the program directory.\n";
    return true;
}
