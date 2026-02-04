#include <iostream>
#include <Windows.h>
#include <cmath>
using namespace std;
void gotoxy(short int x,short int y) 
{ 
   static HANDLE h = NULL;   
   if(!h) 
      h = GetStdHandle(STD_OUTPUT_HANDLE); 
   COORD c = { x, y };   
   SetConsoleCursorPosition(h,c); 
} 
struct Enemy
{
    int x;
    int y;
    bool alive;
};
void map(Enemy enemies[], int enemyCount, int walls[], int wallHeight[], int wallCount)
{
    system("cls");
    gotoxy(5, 25);
    cout << 'P';

    for (int i = 0; i < wallCount; i++)
        for (int y = 25; y > 25 - wallHeight[i]; y--)
        {
            gotoxy(walls[i], y);
            cout << '#';
        }

    for (int i = 0; i < enemyCount; i++)
        if (enemies[i].alive)
        {
            gotoxy(enemies[i].x, enemies[i].y);
            cout << 'E';
        }
}
void calculate(float &x, float &y, int step, int power, float angle)
{
    if (step <= 5)
    {
        x = x + (float)(power * cos(angle) * 0.1);
        y = y - (float)(power * sin(angle) * 0.1);
    }
    else
    {
        x = x + (float)(power * cos(angle) * 0.1);
        y = y + (float)(power * sin(angle) * 0.1);
    }
}

bool playLevel(Enemy enemies[], int enemyCount, int walls[], int wallHeight[], int wallCount, int &stars)
{
    float X , Y ;
    int power;
    float angle;

    for (int tekrar = 0; tekrar < 5; tekrar++)
    {
        bool endoflevel = true;
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i].alive) 
                endoflevel = false;
        }
        if (endoflevel)
        {
            if ( tekrar <= 2)
                stars = 3;
            else if ( tekrar <= 4 )
                stars = 2;
            else
                stars = 1;
            return true;
        }

        X  = 6;
        Y  = 24;
        bool hitWall = false;

        map(enemies, enemyCount, walls, wallHeight, wallCount);
        gotoxy((int)X , (int)Y );
        cout << '*';
        Sleep(3000);
        system("cls");

        cout << "Enter Power: ";
        cin >> power;
        cout << "Enter Angle: ";
        cin >> angle;
        angle = angle * 3.14159265 / 180.0;

        for (int step = 0; step < 20; step++)
        {
            map(enemies, enemyCount, walls, wallHeight, wallCount);
            gotoxy((int)X , (int)Y );
            cout << '*';

            for (int i = 0; i < enemyCount; i++)
                if (enemies[i].alive &&
                    abs((int)X  - enemies[i].x) <= 1 &&
                    abs((int)Y  - enemies[i].y) <= 1)
                    enemies[i].alive = false;

            for (int i = 0; i < wallCount; i++)
                for (int y = 25; y > 25 - wallHeight[i]; y--)
                    if (abs((int)X  - walls[i]) <= 1 &&
                        abs((int)Y  - y) <= 1)
                        hitWall = true;

            if (hitWall)
                break;

            calculate(X , Y , step, power, angle);

            if (X  > 100 || Y  > 25)
                break;
            Sleep(200);
        }
    }

    return false;
}

void resetEnemies(Enemy enemies[], int enemyCount)
{
    for (int i = 0; i < enemyCount; i++)
        enemies[i].alive = true;
}

int main()
{
    bool running = true;
    int menuChoice, levelSelect;

    int stars1 = 0, stars2 = 0, stars3 = 0;
    bool finished1 = false, finished2 = false, finished3 = false;

    int walls1[1] = {80}, wallHeight1[1] = {5};
    Enemy level1[1] = {{80, 25 - 5, true}};

    int walls2[2] = {65, 90}, wallHeight2[2] = {6, 4};
    Enemy level2[2] = {
        {65, 25 - 6, true},
        {90, 25 - 4, true}
    };

    int walls3[2] = {55, 95}, wallHeight3[2] = {9, 2};
    Enemy level3[3] = {
        {55, 25 - 9, true},
        {95, 25 - 2, true},
        {75, 15, true}
    };

    while (running)
    {
        system("cls");
        cout << "=====================\n";
        cout << "   CONSOLE GAME\n";
        cout << "=====================\n";
        cout << "Level 1 Stars: " << stars1 << "/3\n";
        cout << "Level 2 Stars: " << stars2 << "/3\n";
        cout << "Level 3 Stars: " << stars3 << "/3\n";

        if (!finished1) cout << "1) Start\n";
        else if (finished3) cout << "1) Select Level\n";
        else cout << "1) Continue\n";

        cout << "2) How to Play\n";
        cout << "3) Reset Progress\n";
        cout << "0) Exit\n> ";
        cin >> menuChoice;

        switch (menuChoice)
        {
        case 1:
            if (!finished1) finished1 = playLevel(level1, 1, walls1, wallHeight1, 1, stars1);
            else if (!finished2) finished2 = playLevel(level2, 2, walls2, wallHeight2, 2, stars2);
            else if (!finished3) finished3 = playLevel(level3, 3, walls3, wallHeight3, 2, stars3);
            else
            {
                cout << "Select Level (1-3): ";
                cin >> levelSelect;
                if (levelSelect == 1) 
                    playLevel(level1, 1, walls1, wallHeight1, 1, stars1);
                if (levelSelect == 2)
                    playLevel(level2, 2, walls2, wallHeight2, 2, stars2);
                if (levelSelect == 3)
                    playLevel(level3, 3, walls3, wallHeight3, 2, stars3);
            }
            break;

        case 2:
            system("cls");
            cout << "How to Play:\n";
            cout << "1. Enter Power and Angle to shoot\n";
            cout << "2. Hit all enemies to win\n";
            cout << "3. 5 tries per level\n";
            system("pause");
            break;

        case 3:
            finished1 = finished2 = finished3 = false;
            stars1 = stars2 = stars3 = 0;
            resetEnemies(level1, 1);
            resetEnemies(level2, 2);
            resetEnemies(level3, 3);
            break;

        case 0:
            running = false;
            break;
        }
    }

    return 0;
}
