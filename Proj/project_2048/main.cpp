/*
 * File: main.cpp
 * Author: Tiffany Pan
 * Date: October 15, 2015
 * Project: 2048
 */

//System Libraries
#include <iostream>
#include <ctime>

using namespace std;

struct Tile {
    int max; //int value?    
};
struct Grid {
    Tile grid[4][4];
};

struct Game {
    int score;
    int plus;
    int win;
    int max;
    int response;
    int destroy;
    Grid grid;
    Grid bgrid;
    Input input;    
    int randomIndex(int);
    
    int pressEnter;    
    char control;
    char option;
    string name;
};

//Function Prototypes
void logicFlow(Game*);
void gameEndCheck(Game*);
void keyPress(Game*);
void startGrid();
void updateGrid();
void fillSpace();
void spawn();
void findGreatestTile();
void backupGrid();
void undo();
int full();
int blockMoves();

void displayGrid();
void displayHelpScreen();
void displayWinScreen();
void displayLoserScreen();
char displayTryAgainScreen(int);

//Execution begins here
int main(int argc, char** argv) 
{
    Game exec = {0, 0, 2048, 0, 0, 1};
    srand(time(NULL));
    startGrid();
    //not done here
    
    while(1)
    {
        displayGrid();
        keyPress(&exec);
        logicFlow(&exec);
        gameEndCheck(&exec);
    }
    return 0;
}

/* 
 * Definition of function.
 *
 */
void keyPress(Game *game)
{
    system("stty raw");
    cin >> game->control;
    system("stty cooked");
}

/* 
 * Definition of function.
 *
 */
void logicFlow(Game *execute)
{
    switch(control)
    {
        case 'w':
        case 'a':
        case 's':
        case 'd':
            backupGrid();
            fillSpace();
            updateGrid();
            fillSpace();
            findGreatestTile();
            displayGrid();
            sleep(1000*160);
            
            if(full() && execute->destroy)
            {
                execute->response = -1;
                break;
            }
            else if (blockMoves())
            {
                spawn();
                break;
            }
            else
            {
                execute->response = 0;
                break;
            }
        case 'u':
            if (blockMoves())
            {
                execute->score -= plus;
            }
            undo();
            break;
        case 'r':
            startGrid();
            execute->score = 0;
            execute->plus = 0;
            break;
        case 'q':
            execute->response = -1;
            break;
        case'h':
            displayHelpScreen();
            execute->pressEnter = getchar();
            displayGrid();
            break;
    }
}

/* 
 * Definition of function.
 *
 */
void gameEndCheck(Game *screen)
{
    if (max == screen->win)
    {
        displayWinScreen();
        
        if(displayTryAgainScreen(0) == 'n')
        {
            screen->response = -1;
        }
        else
            screen->win*=2;
    }
    else if (screen->response == -1)
    {
        displayLoserScreen();
        
        if(displayTryAgainScreen(1) == 'y')
        {
            startGrid();
            screen->response = 0;
        }
    }
    
    if (screen->response == -1)
    {
        cout << "\n\n\t\t > Thank you for playing. "
                << "\n\n\n\t\t\t\t\t\t DEVELOPED BY \n\n";
        screen->pressEnter = getchar();
        exit(0);
    }
}

/* 
 * Definition of function.
 *
 */
void startGrid(Game *game)
{
    int i, j;
    game->plus = 0;
    game->score = 0;
    max = 0;
    
    for (i= 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            grid[i][j]=0;
        }
    }
    i = randomIndex(4);
    j = randomIndex(4);
    
    grid[i][j] = 2;
    
    i = randomIndex(4);
    j = randomIndex(4);
    
    grid[i][j] = 2;
}

/* 
 * Definition of function.
 *
 */
void displayGrid()
{
    system("clear");
    cout << "\n :: [  THE 2048 PUZZLE  ]:: \t\t\t\tDeveloped by Tiffany\n\n\t";
    if (plus)
    {
        cout << "+" << plus << "!";
    }
    else
    {
        cout << "   ";
    }
    
    cout<<"\t\t\t\t\t\tSCORE::"<<score<<" \n\n\n\n";
    
    for(int i = 0; i < 4; i++)
    {
        cout << "\t\t    |";
        for (int j = 0; j < 4; j++)
        {
            if (grid[i][j])
            {
                printf("%4d    |",grid[i][j]);
            }
            else
            {
                printf("%4c    |",' ');
            }            
        }
        cout << endl << endl;
    }
    cout << "\n\n Controls (+ :: o)\t\t\t\tu - undo\tr - restart\n\n\tW\t\t     ^\t\t\th - help\tq - quit\n\t\t\t\t\t\t\t\t"
            << " \n   A    S    D\t\t<    v    >\t\t\t     ."
            << " \n\t\t\t\t                             ";
}

/* 
 * Definition of function.
 *
 */
int randomIndex(int x)
{
    int index;
    index = rand()%x + 0;
    return index;
}

/* 
 * Definition of function.
 *
 */
void backupGrid()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bgrid[i][j] = grid[i][j];
        }
    }
}

/* 
 * Definition of function.
 *
 */
void fillSpace()
{
    switch(control)
    {
        case 'w':
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!grid[i][j])
                    {
                        for (int k = j + 1; k < 4; k++)
                        {
                            if (grid[k][i])
                            {
                                grid[j][i] = grid [k][i];
                                grid[k][i] = 0;
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case 's': 
            for (int i = 0; i < 4; i++)
            {
                for (int j = 3; j >= 0; j--)
                {
                    if (!grid[j][i])
                    {
                        for (int k = j - 1; k>=0; k--)
                        {
                            if (grid[k][i])
                            {
                                grid[j][i] = grid[k][i];
                                grid[k][i] = 0;
                                break;
                            }
                        }
                    }
                }
            }
        case 'a': 
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (!grid[i][j])
                    {
                        for (int k = j + 1; k < 4; k++)
                        {
                            if (grid[i][k])
                            {
                                grid[i][j] = grid [i][k];
                                grid[i][k] = 0;
                                break;
                            }
                        }
                    }
                }
            }
        case 'd': 
            for (int i = 0; i < 4; i++)
            {
                for (int j = 3; j >= 0; j--)
                {
                    if (!grid[i][j])
                    {
                        for (int k = j - 1; k>=0; k--)
                        {
                            if (grid[i][k])
                            {
                                grid[i][j] = grid[i][k];
                                grid[i][k] = 0;
                                break;
                            }
                        }
                    }
                }
            }
    }
}

/* 
 * Definition of function.
 *
 */
void updateGrid()
{
    plus = 0;
    destroy = 1;
    switch(control)
    {
        case 'w': 
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (grid[j][i] && grid [j][i] == grid [j+1][i])
                    {
                        destroy = 0;
                        grid[j][i] += grid[j+1][i];
                        grid[j+1][i] = 0;
                        plus += (((log2(grid[j][i]))-1)*grid[j][i]);
                        score += (((log2(grid[j][i]))-1)*grid[j][i]); 
                    }
                }
            }
            break;
        case 's': 
            for (int i = 0; i < 4; i++)
            {
                for (int j = 3; j > 0; j--)
                {
                    if (grid[j][i] && grid [j][i] == grid [j-1][i])
                    {
                        destroy = 0;
                        grid[j][i] += grid[j-1][i];
                        grid[j-1][i] = 0;
                        plus += (((log2(grid[j][i]))-1)*grid[j][i]);
                        score += (((log2(grid[j][i]))-1)*grid[j][i]); 
                    }
                }
            }
            break;
        case 'a':
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (grid[i][j] && grid [i][j] == grid [i][j+1])
                    {
                        destroy = 0;
                        grid[i][j] += grid[i][j+1];
                        grid[i][j+1] = 0;
                        plus += (((log2(grid[i][j]))-1)*grid[i][j]);
                        score += (((log2(grid[i][j]))-1)*grid[i][j]); 
                    }
                }
            }
            break;
        case 'd':
            for (int i = 0; i < 4; i++)
            {
                for (int j = 3; j > 0; j--)
                {
                    if (grid[i][j] && grid[i][j] == grid [i][j-1])
                    {
                        destroy = 0;
                        grid[i][j] += grid[i][j-1];
                        grid[i][j-1] = 0;
                        plus += (((log2(grid[i][j]))-1)*grid[i][j]);
                        score += (((log2(grid[i][j]))-1)*grid[i][j]); 
                    }
                }
            }
            break;
    }
}

/* 
 * Definition of function.
 *
 */
void spawn()
{
    int i, j, k;
    do {
        i = randomIndex(4);
        j = randomIndex(4);
        k = randomIndex(10);
    } while (grid[i][j]);   
    
    if (k < 2)
    {
        grid[i][j] = 4;
    }
    else 
    {
        grid[i][j] = 2;
    }
}

/* 
 * Definition of function.
 *
 */
void findGreatestTile()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (grid[i][j] > max)
            {
                max = grid[i][j];
            }
        }
    }
}

/* 
 * Definition of function.
 *
 */
int full()
{
    int k = 1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if(!grid[i][j])
            {
                k = 0;
            }
        }
    }
    return k;
}

/* 
 * Definition of function.
 *
 */
void displayHelpScreen()
{
    system("clear");
    cout << endl << "  ::[  THE 2048 PUZZLE  ]::\t\t\t\tDeveloped by SCM [-_-]";
    char text[] = "> The game starts with 1 or 2 randomly placed number s in a 4x4 grid (16 cells).> Use the ";//???
    for (int i = 0; text[i]; i++)
    {
        if (text[i] == '>')
        {
            sleep(1000*500);
            cout << "\n\n\n";
        }
        cout << text[i];
    }    
}

/* 
 * Definition of function.
 *
 */
void displayWinScreen()
{
    system("clear");
    cout << endl << endl;
    cout << "\n\t\t\t  :: [  YOU MADE "<<win<<"!  ] ::"
            << "\n\n\t\t\t  :: [ YOU WON THE GAME ] ::"
            <<"\n\n\n\n\t\t\t TILE\t     SCORE\t    NAME";
    printf("\n\n\t\t\t %4d\t    %6d\t    ",max,score);
    cin >> name;
    
    cout << "\n\n\t\t> The maximum possible tile is 65,536 ! So go on :)";
}

/* 
 * Definition of function.
 *
 */
void displayLoserScreen()
{
    system("clear");
    
    cout << "\n\n\n\t\t\t  :: [ G A M E  O V E R ] ::"
         << "\n\n\n\n\t\t\t TILE\t     SCORE\t    NAME";
    printf("\n\n\t\t\t %4d\t    %6d\t    ",max,score);
    cin >> name;
    
    cout << "\n\n\t\t> The maximum possible score is 3,932,156 ! So close :p";
}

/* 
 * Definition of function.
 *
 */
char displayTryAgainScreen(int lose)
{
    if (lose)
    {
        cout << "\n\n\n\t    > Would you like to try again "<<name<<" (y/n) ? :: ";
    }
    else
    {
        cout << "\n\n\n\t    > Would you like to continue playing "<<name<<" (y/n) ? :: ";
    }
    
    system("stty raw");
    cin >> option;
    system("stty cooked");
    
    return option;
}

/* 
 * Definition of function.
 *
 */
void undo()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            grid[i][j] = bgrid[i][j];
        }
    }
}

/* 
 * Definition of function.
 *
 */
int blockMoves()
{
    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (bgrid[i][j] != grid[i][j])
            {
                k = 1;
                break;
            }
        }
    }
    return k;
}