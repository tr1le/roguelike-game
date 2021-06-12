#pragma once

#include "Room.h"
#include "Player.h"

#include <string>
#include <vector>

class TMaze {
public:
    TMaze(const std::string path);

    void ProcessGame();

    Player& GetPlayer() { return Plr; };

    void SetScreen(Image& arg) { Screen = &arg; };
    void MeleeAttack() { Melee = true; };
    
    bool CheckFading() { return Fading; };
    bool CheckWin() { return Win; };
    bool CheckLose() { return GameOver; };

    ~TMaze();

private:
    TRoom& GetCurrentRoom() { return *CurrentRoom; };
    void NextLevel();
    void PlaceDoors(int x, int y);

    bool CheckTile(int x, int y, ETile tile);
    bool CheckBox(int x, int y, ETile tile, int wide = tileSize / 2);
    void CheckCollision(Player& player, ETile tile);

    void ProcessPlayer();
    void ProcessEnemies();
    void ProcessMeleeAttack();
    void ProcessTorchAnim();

    void DamagePlayer(const Point& pos = {0, 0});
    void CheckAttackedEnemies(int x, int y, int range = tileSize);

    void ProcessInterface();
    void UpdateHealthbar();

    void FadeOut(TRoom *prev = nullptr);

private:
    std::vector<std::vector<TRoom *>> Rooms;
    Player Plr;

    Image *Screen;
    Image *Healthbar;
    Image *Inventory;

    std::vector<std::vector<char>> RoomTypes;
    int MazeWidth, MazeHeight, RoomWidth, RoomHeight;
    TRoom *CurrentRoom;
    int CurrentX, CurrentY;

    int Health = 4;
    int MaxHealth = 4;

    bool Fading = false;
    bool Invincible = false;
    bool Melee = false;
    bool GameOver = false;
    bool Win = false;

    int FrameShaking = 0;
};