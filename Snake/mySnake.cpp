/*
+----------------------------------------------------------
*
* @authors: 风之凌殇 <fzls.zju@gmail.com>
* @FILE NAME:    mySnake.cpp
* @version:      v1.0
* @Time:         2016-04-03 22:34:22
* @Description:  implement The Greedy Snake game
*
+----------------------------------------------------------
*/
#define _CRT_SECURE_NO_DEPRECATE
#pragma comment(linker, "/STACK:66777216")

#include <algorithm>
#include <chrono>
#include <list>
#include <iomanip>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <random>
#include <string>
#include <graphics.h>
#include <ege/fps.h>

#include <conio.h>
#include <queue>
using namespace std;
#pragma region DebugSetting
#define DEBUG

#ifdef DEBUG
    #define debug(format, ...) printf("[line:%d:@%s] "format, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define debug(...)
#endif
#pragma endregion

const int INF = 0x7FFFFFFF;


class Snake {
    typedef int DIRECTION;
    typedef int COORDINATE;
  public:
    COORDINATE pos(int x, int y) {
        return x + width * y;
    }
    Snake(int height = 30, int width = 80);
    list<DIRECTION> path;
    int score = 0;
    map<char, DIRECTION> c2d;
    void move() {
        for (auto dir : path) {
            //行进到下一点
            snake.push_front(std::move(Body(head().pos + dir, head().dir)));
            bool getFood = false;

            //检测是否有事件发生
            if (board[head().pos] == FOOD) {
                ++score;
                getFood = true;
                generateFood();
            } else if (board[head().pos] != FREE) {
                running = false;
            }

            //更新棋盘
            board[head().pos] = SNAKE;

            if(!getFood) {
                board[tail().pos] = FREE;
                //去掉旧蛇尾
                snake.pop_back();
            }

            //绘制新图形
            print();
            delay_fps(moveSpeed);

            if(!is_run()) {
                running = false;
            }

            if (!running) { return; }
        }
    }

    void printFinalScore() {
        string result = "@echo your final score is " + to_string(score);
        setcolor(EGERGB(0, 0, 0xff));
        outtextxy(0, (height + 2) * 16, result.c_str());
    }

    void play() {
        print();

        while (running) {
            if(isAi) {
                findPath();
            } else {
                chooseDirection();
            }

            move();
        }

        printFinalScore();
    }
  private:
    /*body node of the snake*/
    struct Body {
        COORDINATE pos;
        DIRECTION dir;

        Body(COORDINATE pos, DIRECTION dir)
            : pos{ pos },
              dir{ dir } {
        }
    };
    /*constant for the objects*/
    enum Object {
        WALL_LR,
        WALL_BT,
        FOOD,
        SNAKE,
        FREE
    };


    int height, width, moveSpeed;
    list<Body> snake;
    vector<Object> board;
    const int UP, DOWN, LEFT, RIGHT;
    vector<DIRECTION> dirs;
    COORDINATE food;
    bool running = true;

    void generateFood();

    Body head() {
        return snake.front();
    }

    Body tail() {
        return snake.back();
    }

    void print() {
        setcolor(EGERGB(0xFF, 0x0, 0x0));
        setfont(-16, 0, "Consolas");
        cleardevice();
        static string line;
        line.resize(width);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                switch (board[pos(x, y)]) {
                    case Snake::WALL_LR:
                        line[x] = '|';
                        break;

                    case Snake::WALL_BT:
                        line[x] = '-';
                        break;

                    case Snake::FOOD:
                        line[x] = '$';
                        break;

                    case Snake::SNAKE:
                        if (pos(x, y) == snake.front().pos) { line[x] = '@'; }
                        else if (pos(x, y) == snake.back().pos) { line[x] = '&'; }
                        else { line[x] = '*'; }

                        break;

                    case Snake::FREE:
                        line[x] = ' ';
                        break;

                    default:
                        break;
                }
            }

            setcolor(EGERGB(0x88, 0x88, 0x88));
            outtextxy(0, y * 16, line.c_str());
        }

        setfont(-24, 0, "Consolas");
        //当前分数
        string result = "your current score is " + to_string(score);
        setcolor(EGERGB(0xff, 0x00, 0x00));
        outtextxy(0, height * 16, result.c_str());
        //当前帧率
        string FPS = "fps: " + to_string(moveSpeed);
        setcolor(EGERGB(0x00, 0xff, 0x00));
        outtextxy(0, (height + 2) * 16, FPS.c_str());
    }

    void changeDirection(DIRECTION dir) {
        snake.begin()->dir = dir;
    }

    bool isAi;

    void changeSetting(char cmd) {
        switch (cmd) {
            case '='://namely the + key
                moveSpeed++;
                break;

            case '-':
                moveSpeed--;
                moveSpeed = max(1, moveSpeed);//at least 1 step per second
                break;

            case 'p':
                system("pause");
                break;

            case 'i':
                isAi = !isAi;
                break;

            case '.':
                moveSpeed *= 2;
                break;

            case '/':
                moveSpeed /= 2;
                moveSpeed = max(1, moveSpeed);//at least 1 step per second
                break;

            case '0':
                moveSpeed = 5 ;
                break;

            default:
                break;
        }
    }

    bool isMoveAndNotReverseWithHead(char cmd) {
        if(isMove(cmd)) {
            return c2d[cmd] + head().dir != 0;
        } else {//may be setting
            changeSetting(cmd);
            return false;//means is not move
        }
    }

    void chooseDirection() {
        path.clear();
        char cmd;
        DIRECTION dir;

        if (ege::kbhit() && isMoveAndNotReverseWithHead(cmd = ege::getch())) {
            dir = c2d[cmd];
            path.push_back(dir);
            changeDirection(dir);
        } else {
            path.push_back(head().dir);
        }
    }

    char isMove(char cmd) {
        return cmd == 'w' || cmd == 's' || cmd == 'a' || cmd == 'd';
    }

    void findPath() {
        if(ege::kbhit()) {
            changeSetting(ege::getch());
        }

        //TODO ai version
        path.clear();
    }








};

Snake::Snake(int height, int width) : height{ height },
    width{ width },
    board(height * width, FREE),
    UP{ -width },
    DOWN{ width },
    LEFT{ -1 },
    RIGHT{ 1 },
    moveSpeed{ 5 },
    isAi{ false } {
    /*初始化方向集*/
    dirs.push_back(UP);
    dirs.push_back(DOWN);
    dirs.push_back(LEFT);
    dirs.push_back(RIGHT);
    /*初始化字符转方向集*/
    c2d['w'] = UP;
    c2d['d'] = RIGHT;
    c2d['s'] = DOWN;
    c2d['a'] = LEFT;

    /*上下*/
    for (int x = 0; x < width; ++x) {
        board[x + width * 0] = WALL_BT;
        board[x + width * (height - 1)] = WALL_BT;
    }

    /*左右*/
    for (int y = 0; y < height; ++y) {
        board[0 + width * y] = WALL_LR;
        board[width - 1 + width * y] = WALL_LR;
    }

    /*确定蛇头初始位置与方向*/
    snake.push_back(Body(pos((width - 1) / 2, (height - 1) / 2), UP));
    board[snake.front().pos] = SNAKE;
    /*放置食物*/
    generateFood();
}

void Snake::generateFood() {
    /*获取随机数生成器*/
    static clock_t _time = clock();
    static default_random_engine _e(_time);

    /*在空地随机生成一个食物*/
    do {
        food = _e() % (height * width);
    } while (board[food] != FREE);

    //将食物放置到该点
    board[food] = FOOD;
}

void prepareGraph() {
    initgraph(960, 720, ege::INIT_ANIMATION);
    setbkcolor(EGERGB(0xF5, 0xF5, 0xDC));
}

int main() {
    prepareGraph();
    Snake game;
    game.play();
    system("pause");
    closegraph();
    return 0;
}
