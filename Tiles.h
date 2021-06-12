#pragma once

#include "Image.h"

enum ETile {
    EMPTY_T,
    WALL_T,
    FLOOR_T,
    DOOR_T,
    GOLD_T,
    TORCH_T,
    EXIT_T,

    PLAYER_T,
    PLAYER_IDLE_T,
    PLAYER_RUN_T,
    PLAYER_IDLE_DAMAGED_T,
    PLAYER_RUN_DAMAGED_T,
    PLAYER_DEAD_T,

    ENEMY_T,
    ENEMY_IDLE_T,
    ENEMY_RUN_T,
    ENEMY_IDLE_DAMAGED_T,
    ENEMY_RUN_DAMAGED_T,
    ENEMY_DIED_T,

    BLACK_SCREEN_T,
    WIN_SCREEN_T,
    LOSE_SCREEN_T,
    HEALTH_T,

    SPLASH_T
};


struct TTile {
    static uint64_t Frame;
    const static uint64_t MaxFrame = 512;

    static const ETile CharToTile(char c);
    static const Image& ImgTile(ETile tile, int frame = 0);
    static void NextFrame();
};
