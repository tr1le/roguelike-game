#include "Player.h"

#include <cmath>
#include <iostream>

bool Player::Moved() const
{
    if(coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

void Player::ProcessInput(MovementDir dir)
{
    int move_dist = move_speed * 1;
    switch(dir) {
    case MovementDir::UP:
        old_coords.y = coords.y;
        coords.y += move_dist;
        break;
    case MovementDir::DOWN:
        old_coords.y = coords.y;
        coords.y -= move_dist;
        break;
    case MovementDir::LEFT:
        old_coords.x = coords.x;
        coords.x -= move_dist;
        pos = true;
        break;
    case MovementDir::RIGHT:
        old_coords.x = coords.x;
        coords.x += move_dist;
        pos = false;
        break;
    default:
        break;
    }
}

void Player::Draw(Image &screen)
{
    switch (Type) {
    case PLAYER_T:
        if (!Died) {
            if(Moved()) {
                SubDraw(screen, Damaged ? PLAYER_RUN_DAMAGED_T : PLAYER_RUN_T);
                old_coords = coords;
            } else {
                SubDraw(screen, Damaged ? PLAYER_IDLE_DAMAGED_T : PLAYER_IDLE_T);
            }
        }
        break;
    case ENEMY_T:
        if(Moved()) {
            SubDraw(screen, Damaged ? ENEMY_RUN_DAMAGED_T : ENEMY_RUN_T, FrameDamaged);
            old_coords = coords;
        } else {
            SubDraw(screen, Damaged ? ENEMY_IDLE_DAMAGED_T : ENEMY_IDLE_T, FrameDamaged);
        }
        if (Damaged) {
            if (FrameDamaged == MaxFrameDamaged) {
                Damaged = false;
                FrameDamaged = 0;
            } else {
                FrameDamaged++;
            }
        }
        break;
    }
}

void Player::SubDraw(Image &screen, ETile tile, int frame) {
    if (!pos) {
        screen.PutImage(coords.x - tileSize / 2, coords.y - tileSize / 2, TTile::ImgTile(tile, frame));
    } else {
        screen.PutMirror(coords.x - tileSize / 2, coords.y - tileSize / 2, TTile::ImgTile(tile, frame));
    }
}

void Player::DrawSplash(Image &screen, int frame) {
    if (pos) {
        screen.PutMirror(coords.x - tileSize / 2 + (!pos ? 1 : -1) * 13, coords.y - tileSize / 2, TTile::ImgTile(SPLASH_T, frame));
    } else {
        screen.PutImage(coords.x - tileSize / 2 + (!pos ? 1 : -1) * 13, coords.y - tileSize / 2, TTile::ImgTile(SPLASH_T, frame));
    }
}

bool Player::ProcessDied(Image &screen) {
    if (PrevFrame == -1) {
        PrevFrame = TTile::Frame;
        FrameDie = 0;
    }
    SubDraw(screen, ENEMY_DIED_T, FrameDie / 2);
    if (FrameDie >= MaxFrameDie * 2 - 1) {
        PrevFrame = -1;
        FrameDie = 0;
        return true;
    }
    if (TTile::Frame != PrevFrame) {
        FrameDie++;
        PrevFrame = TTile::Frame;
    }
    return false;
}

void Player::GoTo(Point pnt) {
    const int walkFrames = 80;
    const int stopFrames = MaxFrameWalk - walkFrames;
    FrameWalk++;
    FrameWalk %= walkFrames + stopFrames;

    if (FrameWalk > walkFrames) {
        return;
    }
    int move_dist = move_speed * 1;

    if (coords.y < pnt.y) {
        old_coords.y = coords.y;
        coords.y += move_dist;
    } else if (coords.y > pnt.y) {
        old_coords.y = coords.y;
        coords.y -= move_dist;
    }

    if (coords.x < pnt.x) {
        old_coords.x = coords.x;
        coords.x += move_dist;
        pos = false;
    } else if (coords.x > pnt.x) {
        old_coords.x = coords.x;
        coords.x -= move_dist;
        pos = true;
    }
}

void Player::PushBack(Point pos, int length) {
    double x = coords.x - pos.x, y = coords.y - pos.y;
    double xx, yy;
    if (x * x + y * y == 0) {
        xx = 0;
        yy = length;
    } else {
        double d = sqrt(x * x + y * y);
        xx = length * x / d;
        yy = length * y / d;
    }
    old_coords = coords;
    coords = {int(coords.x + xx), int(coords.y + yy)};
}

void Player::DecreaseHealth() {
    Health--;
    if (Health <= 0) {
        Died = true;
    } else {
        Damaged = true;
    }
}