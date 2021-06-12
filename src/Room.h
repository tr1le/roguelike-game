#pragma once
#include "Image.h"
#include "Player.h"
#include "Tiles.h"

#include <vector>
#include <string>

class TRoom {
public:
    TRoom(const std::string path, int width, int height);

    Image& GetImage() { return Room; };
    Image& GetShadow() { return Shadow; };
    ETile& GetTile(int x, int y) { return Tiles[y][x]; };
    std::vector<Player *>& GetEnemies() { return Enemies; };
    std::vector<Point>& GetTorches() { return Torches; };

    void PlaceTile(int x, int y, ETile tile);
    void PlaceTorch(int x, int y, double radius = 3 * tileSize, int brightness = 25);
    void UpdateTorches();

    ~TRoom();

private:
    std::vector<std::vector<ETile>> Tiles;
    Image Room;
    Image Shadow;
    std::vector<Player *> Enemies;
    std::vector<Point> Torches;
    int Width;
    int Height;
    int Shade = 0;
};