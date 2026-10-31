#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>

void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
struct Point {
    int x;
    int y;
};
std::vector<Point> FreeSpace(std::vector<Point> snake, int WIDTH, int HEIGHT) {
    std::vector<Point> space;
    bool busy = false;
    for (int X = 1; X < WIDTH; X++) {
        for (int Y = 1; Y < HEIGHT; Y++) {
            for (int s = 0; s < snake.size(); s++) {
                if (snake[s].x == X && snake[s].y == Y) {
                    busy = true;
                    break;
                    }
                }
            if (busy == false) {
                space.push_back({X, Y});
            }
            busy = false;
        }
    }
    return space;
}
void DrawSnake(std::vector<Point> snake) {
    for (int i = 0; i < snake.size(); i++) {
        if (i == 0) {
            gotoxy(2*snake[0].x,snake[0].y);
            std::cout << "@";
        }
        else {
            gotoxy(2*snake[i].x,snake[i].y);
            std::cout << 'O';
        }
    }
}
void DrawScore(int score, int WIDTH, int HEIGHT) {
    gotoxy(2*WIDTH + 2,HEIGHT/2 );
    std::cout << "YOU'R SCORE: " << score;
}
bool Pressed(int x) {
    if ((GetAsyncKeyState(x) & 0x8000) == 0x8000) {
        return true;
    }
    else { 
        return false;
    }
}
bool HitSelf(Point newHead, std::vector<Point> snake) {
    for (int i = 0; i < snake.size()-1; i++) {
        if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
            return true;
            break;

        }
    }
    return false;
}
bool HitWall(Point newHead, int WIDTH, int HEIGHT) {
    if ( 1 > newHead.x || newHead.x > WIDTH-1 || 1 > newHead.y || newHead.y > HEIGHT-1) {
        return true;
    }
    return false;
}
int main() {

    srand(time(0));
    int  dirX=1, dirY=0, tick = 2, score = 0,  t = 16;
    bool  exis = false, GameOver = false;
    const int WIDTH = 30;
    const int HEIGHT = 15;
    Point newHead, apple = {0,0};
    std::vector<Point> spaces, snake = {{1,3}, {1,2}, {1,1}};

    system("cls");

    for (int i = 0; i <= HEIGHT; i++) {
        gotoxy(0, i);
        std::cout << '#';
        gotoxy(2*WIDTH, i);
        std::cout << '#';
    }
    for (int i = 0; i <= 2*WIDTH; i++) {
        gotoxy(i, 0);
        std::cout << "#";
        gotoxy(i, HEIGHT);
        std::cout << "#";
    }
    while (true) {
        if ((Pressed('W') || Pressed(VK_UP)) && (dirY != 1)) {
            dirX = 0;
            dirY = -1;
        }
        else if ((Pressed('S') || Pressed(VK_DOWN)) && (dirY != -1)) {
            dirX = 0;
            dirY = 1;
        }
        else if ((Pressed('A') || Pressed(VK_LEFT)) && (dirX != 1)) {
            dirX = -1;
            dirY = 0;
        }
        else if ((Pressed('D') || Pressed(VK_RIGHT)) && (dirX != -1)) {
            dirX = 1;
            dirY = 0;
        }

        tick--;
        
        if (tick <= 0) {
            newHead.x = snake[0].x + dirX;
            newHead.y = snake[0].y + dirY;
            if (HitWall(newHead, WIDTH, HEIGHT) || HitSelf(newHead, snake)) {
                GameOver = true;
                break;
            }
            snake.insert(snake.begin(), newHead);
            gotoxy(2*snake.back().x, snake.back().y);
            if (newHead.x == apple.x && newHead.y == apple.y){
                exis = 0;
                score++;
                if (score % 10==0 && t >= 6) {
                    t -= 2;
                }
            }
            else {
                snake.pop_back();
                std::cout << ' ';
            }
            DrawSnake(snake);
            tick = t;

        }
        
        if (exis != 1) {
            spaces = FreeSpace(snake, WIDTH, HEIGHT);
            DrawScore(score, WIDTH, HEIGHT);
            if (spaces.size() > 0) {
                apple = spaces[rand() % spaces.size()];
                gotoxy(2*apple.x, apple.y);
                std::cout << '$';
                exis = 1;
            }
            else {
                GameOver = false;
                break;
            }
        }
        Sleep(10);
    }
    system("cls");
    DrawScore(score, WIDTH, HEIGHT);
    gotoxy(2*WIDTH+2, HEIGHT/2 + 1);
    if (GameOver == true) {
        std::cout << "GAME OVER";
    }
    else {
        std::cout << "!!YOU ARE WINNER!!";
    }
    return 0;
}