#include <stdio.h>
#include <vector>
using namespace std;

// Define map rows and columns
#define ROWS 20
#define COLS 20

// Go straight price and Move diagonally price
#define STRAIGHT_PRICE 10
#define DIAGONAL_PRICE 14

// Direction enum
enum direct
{
    p_up,
    p_down,
    p_left,
    p_right,
    p_leftUp,
    p_leftDown,
    p_rightUp,
    p_rightDown
};

// Structure of point
struct MyPoint
{
    /* x y position, total price val (f), taken price(g), tradeoff(w)*/
    int xPos;
    int yPos;
    int f;
    int g;
    int h;
};

// Tree structure
struct treeNode
{
    /* point position, its parent, its childrens */
    MyPoint pos;
    treeNode *pointParent;
    vector<treeNode *> childs;

    treeNode() {}
    treeNode(MyPoint pos)
    {
        this->pos.xPos = pos.xPos;
        this->pos.yPos = pos.yPos;
        this->pointParent = NULL;
    }
};

void printTitle();
void printMap(int map[ROWS][COLS]);
int getH(MyPoint point, MyPoint endPoint);

int main()
{
    int map[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    // for tracking all points if is walked or not.
    bool pathMap[ROWS][COLS] = {0};

    printTitle();
    printMap(map);

    MyPoint startPoint = {1, 1}; // (1, 1)
    MyPoint endPoint = {18, 18}; // (18, 18)

    // Store the starting point to the tree.
    treeNode *pRoot = NULL;
    pRoot = new treeNode(startPoint);

    // Set the starting point to 1.
    pathMap[startPoint.yPos][startPoint.xPos] = true;

    // Current position.
    treeNode *current = pRoot;

    // Buff array for saving points h val
    vector<treeNode *> buff;

    // Vector iterator
    vector<treeNode *>::iterator it;
    vector<treeNode *>::iterator itMin;

    // Bool
    bool isReachDestination = false;

    while (1)
    {
        // 1 point will have 8 adjacent points.
        for (int i = 0; i < 8; i++)
        {
            treeNode *pTemp = new treeNode(current->pos);

            /**
             *  up: 0, down: 1, left: 2, right: 3
             *  leftUp: 4, leftDown: 5, rightUp: 6,  rightDown: 7
             */

            switch (i)
            {
            case p_up:
                pTemp->pos.yPos--;
                pTemp->pos.g += STRAIGHT_PRICE;
                break;
            case p_down:
                pTemp->pos.yPos++;
                pTemp->pos.g += STRAIGHT_PRICE;
                break;
            case p_left:
                pTemp->pos.xPos--;
                pTemp->pos.g += STRAIGHT_PRICE;
                break;
            case p_right:
                pTemp->pos.xPos++;
                pTemp->pos.g += STRAIGHT_PRICE;
                break;
            case p_leftUp:
                pTemp->pos.xPos--;
                pTemp->pos.yPos--;
                pTemp->pos.g += DIAGONAL_PRICE;
                break;
            case p_leftDown:
                pTemp->pos.xPos--;
                pTemp->pos.yPos++;
                pTemp->pos.g += DIAGONAL_PRICE;
                break;
            case p_rightUp:
                pTemp->pos.xPos++;
                pTemp->pos.yPos--;
                pTemp->pos.g += DIAGONAL_PRICE;
                break;
            case p_rightDown:
                pTemp->pos.xPos++;
                pTemp->pos.yPos++;
                pTemp->pos.g += DIAGONAL_PRICE;
                break;
            } // end of switch

            // checking if the point accessable.
            if (
                pathMap[pTemp->pos.yPos][pTemp->pos.xPos] == false &&
                map[pTemp->pos.yPos][pTemp->pos.xPos] != 1 &&
                pTemp->pos.yPos < ROWS && pTemp->pos.yPos >= 0 &&
                pTemp->pos.xPos < COLS && pTemp->pos.xPos >= 0)
            {
                // Set the bool to true
                pathMap[pTemp->pos.yPos][pTemp->pos.xPos] = true;

                // Computing h val.
                pTemp->pos.h = getH(pTemp->pos, endPoint);

                // Computing f val,
                pTemp->pos.f = pTemp->pos.h + pTemp->pos.g;

                // Adding to tree.
                current->childs.push_back(pTemp); // Set the pTemp be the current's children.
                pTemp->pointParent = current;     // Set current be the parent of pTemp.

                // Saving to buff array.
                buff.push_back(pTemp);
            }
            else
            {
                delete pTemp;
            }

        } // end of for

        // Select the minimum val of f from the buff array.
        itMin = buff.begin(); // assume the first h val is the smallest.
        for (it = buff.begin(); it != buff.end(); it++)
        {
            itMin = ((*itMin)->pos.f < (*it)->pos.f) ? itMin : it;
        }

        // move to next
        current = *itMin;

        // remove the walked one.
        buff.erase(itMin);

        if (current->pos.xPos == endPoint.xPos && current->pos.yPos == endPoint.yPos) // when reach the destination.
        {
            isReachDestination = true;
            break;
        }
        if (buff.empty())
        {
            break;
        }
    } // end of while

    if (isReachDestination)
    {
        printf("Found the destination, the shortest path is: \n");

        // print the shortest path.
        while (current->pointParent)
        {
            printf("(%d, %d)", current->pos.xPos, current->pos.yPos);
            map[current->pos.yPos][current->pos.xPos] = 2;
            current = current->pointParent;
        }
        printf("\n");
    }

    printf("\n");
    printMap(map);

    return 0;
}

void printTitle()
{
    printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
    printf("■■■■■■■■■■■■                                            ■■■■■■■■■■■■■\n");
    printf("■■■■■■■■■■■■        PATH FINDER BY A* ALGORITHM         ■■■■■■■■■■■■■\n");
    printf("■■■■■■■■■■■■                                            ■■■■■■■■■■■■■\n");
    printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
    printf("\n\n");
}

/**
 * @brief function of print the map
 *
 * @param map
 */
void printMap(int map[ROWS][COLS])
{
    printf("---------- Map -----------\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (map[i][j] == 0)
                printf("⬜");
            else if (map[i][j] == 1)
                printf("⬛");
            else if (map[i][j] == 2)
                printf("🟥");
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

int getH(MyPoint point, MyPoint endPoint)
{
    int x = (point.xPos > endPoint.xPos) ? (point.xPos - endPoint.xPos) : (endPoint.xPos - point.xPos);
    int y = (point.yPos > endPoint.yPos) ? (point.yPos - endPoint.yPos) : (endPoint.yPos - point.yPos);

    return (x + y) * 10;
}