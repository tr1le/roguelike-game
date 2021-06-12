#include "Room.h"

#include <fstream>
#include <iostream>
#include <cmath>


TRoom::TRoom(const std::string path, int width, int height) 
    : Tiles(height, std::vector<ETile>(width))
    , Room(width * tileSize, height * tileSize, 3)
    , Width(width)
    , Height(height)
    , Shadow(width * tileSize, height * tileSize, 4)
{
    std::ifstream fin;
    fin.open(path);
    char c;
    fin >> Shade;
    fin.get(c);

    for (int i = 0; i < Shadow.Width() * Shadow.Height(); ++i) {
        Shadow.Data()[i] = {0, 0, 0, (unsigned char)Shade};
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            PlaceTile(x, y, FLOOR_T);
            fin.get(c);
            if (c == 'E') {
                static int i = 0;
                Enemies.push_back(new Player({x * tileSize, y * tileSize}, ENEMY_T, 1));
            } else {
                PlaceTile(x, y, TTile::CharToTile(c));
            }
        }
        fin.get(c);
    }
    fin.close();
}

void TRoom::PlaceTile(int x, int y, ETile tile) {
    Tiles[y][x] = tile;
    if (tile == TORCH_T) {
        PlaceTorch(x, y);
    } else {
        Room.PutImage(x * tileSize, y * tileSize, TTile::ImgTile(Tiles[y][x]));
    }
}

void TRoom::PlaceTorch(int x, int y, double r, int bright) {
    PlaceTile(x, y, WALL_T);
    Torches.push_back({x, y});
    Room.PutImage(x * tileSize, y * tileSize, TTile::ImgTile(TORCH_T));

    x = x * tileSize + tileSize / 2;
    y = y * tileSize + tileSize / 2;

    for (int yy = y - r; yy < y + r; ++yy) {
        for (int xx = x - r; xx < x + r; ++xx) {
            double d = (xx - x) * (xx - x) + (yy - y) * (yy - y);
            int pxl = yy * Width * tileSize + xx;
            double coef = (r * r - d) / (r * r);
            coef *= coef;
            if (pxl >= 0 && pxl < Width * Height * tileSize * tileSize && d <= r * r) {
                Shadow.Data()[pxl] = {
                    (unsigned char)(coef * 255), 
                    (unsigned char)(coef * 91), 
                    (unsigned char)(coef * 0), 
                    (unsigned char)(d / (r * r) * bright + (Shade - bright))
                };
            }
        }
    }
}


void TRoom::UpdateTorches() {
    for (auto& pos : Torches) {
        Room.PutImage(pos.x * tileSize, pos.y * tileSize, TTile::ImgTile(WALL_T));
        Room.PutImage(pos.x * tileSize, pos.y * tileSize, TTile::ImgTile(TORCH_T));
    }
}


TRoom::~TRoom() {
    for (auto& enem : Enemies) {
        if (enem != nullptr)
            delete enem;
    }
}