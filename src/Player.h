#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Tiles.h"

struct Point
{
    int x;
    int y;
};

enum class MovementDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Player 
{
    explicit Player(Point pos = {.x = 10, .y = 10}, ETile type = PLAYER_T, int speed = 2, int health = 4)
    : coords(pos)
    , old_coords(coords)
    , Type(type)
    , move_speed(speed)
    , Health(health)
    , MaxHealth(health)
    , FrameWalk(rand() % MaxFrameWalk) {};

    bool Moved() const;
    void ProcessInput(MovementDir dir);

    void Draw(Image &screen);
    void DrawSplash(Image &screen, int frame);

    bool ProcessDied(Image &screen);

    Point& GetCoords() { return coords; };
    Point& GetOldCoords() { return old_coords; };
    bool& GetDamaged() { return Damaged; };
    bool& GetDied() { return Died; };
    bool& GetPos() { return pos; };

    void PushBack(Point pos, int length = tileSize / 2);
    void GoTo(Point pos);
    void UpGold() { Gold++; };
    void DecreaseHealth();
    

private:
    void SubDraw(Image &screen, ETile tile, int frame = 0);

private:
    Point coords {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    bool pos = true;
    int move_speed = 3;
    ETile Type;
    
    int Health;
    int MaxHealth;
    int Gold = 0;

    bool Damaged = false;
    bool Died = false;
    int FrameWalk;
    static const int MaxFrameWalk = 200;
    int FrameDamaged = 0;
    static const int MaxFrameDamaged = 20;
    int FrameDie = 0;
    static const int MaxFrameDie = 4;

    int PrevFrame = 0;
};

#endif //MAIN_PLAYER_H
