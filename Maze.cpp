#include "Maze.h"

#include <fstream>
#include <string>
#include <iostream>


TMaze::TMaze(const std::string path) {

    std::ifstream fin;
    fin.open(path);
    fin >> MazeWidth >> MazeHeight >> RoomWidth >> RoomHeight;

    RoomTypes.assign(MazeHeight, std::vector<char>(MazeWidth));
    Rooms.assign(MazeHeight, std::vector<TRoom *>(MazeWidth, nullptr));

    char c;
    fin.get(c);
    for (int y = 0; y < MazeHeight; ++y) {
        for (int x = 0; x < MazeWidth; ++x) {
            fin.get(c);
            RoomTypes[y][x] = c;
        }
        fin.get(c);
    }
    fin.close();

    for (int y = 0; y < MazeHeight; ++y) {
        for (int x = 0; x < MazeWidth; ++x) {
            if (RoomTypes[y][x] != '.') {
                std::string str = "../resources/";
                str += RoomTypes[y][x];
                str += ".txt";
                Rooms[y][x] = new TRoom(str, RoomWidth, RoomHeight);
                PlaceDoors(x, y);
            } else {
                Rooms[y][x] = nullptr;
            }
        }
    }

    Rooms[MazeHeight - 1][MazeWidth - 1]->PlaceTile(RoomWidth / 2, RoomHeight / 2, EXIT_T);

    CurrentRoom = Rooms[0][0];
    CurrentX = CurrentY = 0;
    Plr.GetCoords() = {RoomWidth * tileSize / 2, RoomHeight * tileSize / 2};

    Healthbar = new Image(80, 16, 4);
    UpdateHealthbar();
}


void TMaze::ProcessGame() {
    if (Fading) {
        FadeOut();
        return;
    }

    if (FrameShaking % 6 < 3) {
        Screen->PutImage(0, 0, GetCurrentRoom().GetImage());
    } else {
        Screen->PutImage(-1, -1, GetCurrentRoom().GetImage());
    }

    if (!GameOver) {
        ProcessPlayer();
    }

    if (!Fading) {
        ProcessTorchAnim();
        ProcessEnemies();
        ProcessInterface();
        Screen->PutImage(0, 0, GetCurrentRoom().GetShadow());
    }

    if (GameOver) {
        Screen->PutImage(RoomWidth * tileSize / 2 - TTile::ImgTile(LOSE_SCREEN_T).Width() / 2,
                RoomHeight * tileSize / 4 - TTile::ImgTile(LOSE_SCREEN_T).Height() / 2, TTile::ImgTile(LOSE_SCREEN_T));
    }

    if (Win) {
        Screen->PutImage(RoomWidth * tileSize / 2 - TTile::ImgTile(WIN_SCREEN_T).Width() / 2,
         RoomHeight * tileSize / 4 - TTile::ImgTile(WIN_SCREEN_T).Height() / 2, TTile::ImgTile(WIN_SCREEN_T));
    }
}


void TMaze::PlaceDoors(int x, int y) {
    if (x > 0 && RoomTypes[y][x - 1] != '.' && RoomTypes[y][x] != '.') {
        Rooms[y][x]->PlaceTile(0, RoomHeight / 2, DOOR_T);
    }
    if (x < MazeWidth - 1 && RoomTypes[y][x + 1] != '.' && RoomTypes[y][x] != '.') {
        Rooms[y][x]->PlaceTile(RoomWidth - 1, RoomHeight / 2, DOOR_T);
    }
    if (y > 0 && RoomTypes[y - 1][x] != '.' && RoomTypes[y][x] != '.') {
        Rooms[y][x]->PlaceTile(RoomWidth / 2, 0, DOOR_T);
    }
    if (y < MazeHeight - 1 && RoomTypes[y + 1][x] != '.' && RoomTypes[y][x] != '.') {
        Rooms[y][x]->PlaceTile(RoomWidth / 2, RoomHeight - 1, DOOR_T);
    }
}


bool TMaze::CheckTile(int x, int y, ETile tile) {
    if (x < 0 || x >= RoomWidth || y < 0 || y >= RoomHeight) {
        return false;
    }
    return CurrentRoom->GetTile(x, y) == tile;
}


void TMaze::ProcessPlayer() {
    int x = Plr.GetCoords().x / tileSize;
    int y = Plr.GetCoords().y / tileSize;
    if (Plr.Moved()) {
        CheckCollision(Plr, WALL_T);
    }
    if (CheckTile(x, y, DOOR_T)) {
        NextLevel();
    }
    if (CheckTile(x, y, EMPTY_T)) {
        GameOver = true;
        Health = 0;
        UpdateHealthbar();
    }
    if (CheckTile(x, y, EXIT_T)) {
        Win = true;
    }
    if (Fading) {
        return;
    }
    if (Invincible) {
        DamagePlayer();
    }

    GetPlayer().Draw(*Screen);

    if (Melee) {
        ProcessMeleeAttack();
    }
    if (CheckTile(x, y, GOLD_T)) {
        Plr.UpGold();
        CurrentRoom->PlaceTile(x, y, FLOOR_T);
    }
}


void TMaze::ProcessEnemies() {
    for (auto& enem : GetCurrentRoom().GetEnemies()) {
        if (enem == nullptr) {
            continue;
        }
        if (enem->GetDied()) {
            if (enem->ProcessDied(*Screen)) {
                delete enem;
                enem = nullptr;
            }
            continue;
        }
        if (!GameOver) {
            enem->GoTo(Plr.GetCoords());
            int x = enem->GetCoords().x;
            int y = enem->GetCoords().y;
            if ((x - Plr.GetCoords().x) * (x - Plr.GetCoords().x) +
                    (y - Plr.GetCoords().y) * (y - Plr.GetCoords().y) < tileSize * tileSize / 6) {
                enem->GetCoords() = enem->GetOldCoords();
                if (!Invincible) {
                    DamagePlayer(enem->GetCoords());
                }
            }
        }
        if (enem->Moved()) {
            CheckCollision(*enem, WALL_T);
            CheckCollision(*enem, EMPTY_T);
        }

        enem->Draw(*Screen);
    }
}


void TMaze::CheckCollision(Player& plr, ETile tile) {
    int x = plr.GetCoords().x;
    int y = plr.GetCoords().y;

    if (CheckBox(x, y, tile)) {
        if (!CheckBox(x, plr.GetOldCoords().y, tile)) {
            plr.GetCoords().y = plr.GetOldCoords().y;
        } else if (!CheckBox(plr.GetOldCoords().x, y, tile)) {
            plr.GetCoords().x = plr.GetOldCoords().x;
        } else {
            plr.GetCoords() = plr.GetOldCoords();
        }
    }
}


bool TMaze::CheckBox(int x, int y, ETile tile, int wide) {
    x = x - wide / 2;
    y = y - wide / 2;
    return CheckTile(x / tileSize, y / tileSize, tile) ||
           CheckTile(x / tileSize, (y + wide) / tileSize, tile) ||
           CheckTile((x + wide) / tileSize, y / tileSize, tile) ||
           CheckTile((x + wide) / tileSize, (y + wide) / tileSize, tile);
}


void TMaze::NextLevel() {
    int x = Plr.GetCoords().x / tileSize;
    int y = Plr.GetCoords().y / tileSize;
    if (x == 0) {
        CurrentX--;
        Plr.GetCoords() = {RoomWidth * tileSize - 2 * tileSize, RoomHeight * tileSize / 2};
    } else if (x == RoomWidth - 1) {
        CurrentX++;
        Plr.GetCoords() = {2 * tileSize, RoomHeight * tileSize / 2};
    } else if (y == 0) {
        CurrentY--;
        Plr.GetCoords() = {RoomWidth * tileSize / 2, RoomHeight * tileSize - 2 * tileSize};
    } else if (y == RoomHeight - 1) {
        CurrentY++;
        Plr.GetCoords() = {RoomWidth * tileSize / 2, 2 * tileSize};
    }
    auto prevRoom = CurrentRoom;
    CurrentRoom = Rooms[CurrentY][CurrentX];
    FadeOut(prevRoom);
}


void TMaze::FadeOut(TRoom *prev) {
    static int frame;
    static Image *img;
    int maxFrame = 30;
    if (!Fading) {
        Fading = true;
        frame = 0;
        img = new Image(Screen->Width(), Screen->Height(), 3);
        for (int i = 0; i < Screen->Width() * Screen->Height(); ++i) {
            img->Data()[i] = Screen->Data()[i];
        }
        img->PutImage(0, 0, prev->GetShadow());
        Screen->PutImage(0, 0, prev->GetShadow());
    } else if (frame < maxFrame) {
        frame++;
        for (int i = 0; i < Screen->Width() * Screen->Height(); ++i) {
            Pixel d = img->Data()[i];
            Screen->Data()[i] = d * ((double)(maxFrame - frame) / maxFrame);
        }
    } else if (frame == maxFrame) {
        frame++;
        for (int i = 0; i < Screen->Width() * Screen->Height(); ++i) {
            Screen->Data()[i] = {0, 0, 0, 255};
        }
        img->PutImage(0, 0, GetCurrentRoom().GetImage());
        img->PutImage(0, 0, GetCurrentRoom().GetShadow());
    } else if (frame < 2 * maxFrame) {
        frame++;
        for (int i = 0; i < Screen->Width() * Screen->Height(); ++i) {
            Pixel d = img->Data()[i];
            Screen->Data()[i] = d * ((double)(frame - maxFrame) / maxFrame);
        }
    } else {
        delete img;
        img = nullptr;
        frame = 0;
        Fading = false;
    }
}


void TMaze::ProcessInterface() {
    Screen->PutImage(RoomWidth * tileSize - Healthbar->Width() - tileSize / 2,
            tileSize / 2, *Healthbar);
}


void TMaze::UpdateHealthbar() {
    for (int y = 0; y < 16; ++y) {
        for (int x = tileSize; x < 79 ; ++x) {
            Healthbar->Data()[y * 80 + x] = {0, 0, 0, 0};
        }
    }
    for (int y = 0; y < 16; ++y) {
        for (int x = tileSize; x < tileSize + Health * tileSize - 1; ++x) {
            Healthbar->Data()[y * 80 + x] = {255, 0, 0, 255};
        }
    }
    Healthbar->PutImage(0, 0, TTile::ImgTile(HEALTH_T));
}

void TMaze::DamagePlayer(const Point& pos) {
    static const int invincibleFrames = 40;
    static int frame;
    if (!Invincible) {
        Invincible = true;
        frame = 0;
        Plr.GetDamaged() = true;
        FrameShaking = 15;
        Health--;
        UpdateHealthbar();
        if (!Health) {
            GameOver = true;
        }
        Plr.PushBack(pos);
        CheckCollision(Plr, WALL_T);
    } else if (frame >= invincibleFrames) {
        Invincible = 0;
        frame = 0;
        Plr.GetDamaged() = false;
    } else {
        frame++;
        if (FrameShaking) {
            FrameShaking--;
        }
    }
}


void TMaze::ProcessMeleeAttack() {
    static int frame = 0;
    Plr.DrawSplash(*Screen, frame);

    if (!frame) {
        Melee = true;
        frame++;
        CheckAttackedEnemies(Plr.GetCoords().x + (Plr.GetPos()? -tileSize: tileSize),
                Plr.GetCoords().y);
        
    } else if (frame == 5) {
        frame = 0;
        Melee = false;
    } else {
        frame++;
    }
}


void TMaze::CheckAttackedEnemies(int x, int y, int range) {
    for (auto& enem : GetCurrentRoom().GetEnemies()) {
        if (enem == nullptr) {
            continue;
        }
        if (enem->GetDied()) {
            continue;
        }
        int xx = enem->GetCoords().x, yy = enem->GetCoords().y;
        if (xx <= x + range && xx >= x - range && yy <= y + range / 2 && yy >= y - range / 2) {
            enem->DecreaseHealth();
        }
    }
}


void TMaze::ProcessTorchAnim() {
    CurrentRoom->UpdateTorches();
}


TMaze::~TMaze() {
    for (int i = 0; i < MazeHeight; ++i) {
        for (int j = 0; j < MazeWidth; ++j) {
            if (Rooms[i][j] != nullptr) {
                delete Rooms[i][j];
            }
        }
    }
    delete Healthbar;
}