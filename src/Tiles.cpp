#include "Tiles.h"

#include <vector>
#include <iostream>

uint64_t TTile::Frame = 0;

const ETile TTile::CharToTile(char c) {
    switch (c) {
        case ' ':
            return EMPTY_T;
        case '#':
            return WALL_T;
        case 'E':
        case '.':
            return FLOOR_T;
        case 'G':
            return GOLD_T;
        case 'F':
            return TORCH_T;
        case 'x':
            return EXIT_T;
    }
    return EMPTY_T;
}

const Image& TTile::ImgTile(ETile tile, int frame) {
    static Image EMPTY_I("../src/resources/tiles/floor/floor_10.png");
    static Image WALL_I("../src/resources/tiles/wall/wall_1.png");
    static Image FLOOR_I("../src/resources/tiles/floor/floor_2.png");
    static Image GOLD_I("../src/resources/props_itens/bag_coins.png");
    static Image EXIT_I("../src/resources/props_itens/chest_closed_anim_f1.png");
    static std::vector<Image *> TORCH_I {
        new Image("../src/resources/props_itens/torch_anim_f0.png"),
        new Image("../src/resources/props_itens/torch_anim_f1.png"),
        new Image("../src/resources/props_itens/torch_anim_f2.png"),
        new Image("../src/resources/props_itens/torch_anim_f3.png"),
        new Image("../src/resources/props_itens/torch_anim_f4.png"),
        new Image("../src/resources/props_itens/torch_anim_f5.png"),
    };
    static std::vector<Image *> PLAYER_IDLE_I {
        new Image("../src/resources/heroes/knight/knight_idle_anim_f0.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f1.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f2.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f3.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f4.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f5.png"),
    };
    static std::vector<Image *> PLAYER_RUN_I {
        new Image("../src/resources/heroes/knight/knight_run_anim_f0.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f1.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f2.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f3.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f4.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f5.png"),
    };
    static std::vector<Image *> PLAYER_IDLE_DAMAGED_I {
        new Image("../src/resources/heroes/knight/knight_idle_damaged_f0.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f1.png"),
        new Image("../src/resources/heroes/knight/knight_idle_damaged_f2.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f3.png"),
        new Image("../src/resources/heroes/knight/knight_idle_damaged_f4.png"),
        new Image("../src/resources/heroes/knight/knight_idle_anim_f5.png"),
    };
    static std::vector<Image *> PLAYER_RUN_DAMAGED_I {
        new Image("../src/resources/heroes/knight/knight_run_damaged_f0.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f1.png"),
        new Image("../src/resources/heroes/knight/knight_run_damaged_f2.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f3.png"),
        new Image("../src/resources/heroes/knight/knight_run_damaged_f4.png"),
        new Image("../src/resources/heroes/knight/knight_run_anim_f5.png"),
    };
    static Image PLAYER_DEAD_I("../src/resources/heroes/knight/knight_dead.png");
    static std::vector<Image *> ENEMY_IDLE_I {
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f0.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f1.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f2.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f3.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f4.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f5.png"),
    };
    static std::vector<Image *> ENEMY_RUN_I {
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f0.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f1.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f2.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f3.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f4.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f5.png"),
    };
    static std::vector<Image *> ENEMY_IDLE_DAMAGED_I {
        new Image("../src/resources/enemies/goblin/goblin_idle_damaged_f0.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f1.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_damaged_f2.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f3.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_damaged_f4.png"),
        new Image("../src/resources/enemies/goblin/goblin_idle_anim_f5.png"),
    };
    static std::vector<Image *> ENEMY_RUN_DAMAGED_I {
        new Image("../src/resources/enemies/goblin/goblin_run_damaged_f0.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f1.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_damaged_f2.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f3.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_damaged_f4.png"),
        new Image("../src/resources/enemies/goblin/goblin_run_anim_f5.png"),
    };
    static Image HEALTH_I("../src/resources/ui (new)/health_ui.png");
    static std::vector<Image *> SPLASH_I {
        new Image("../src/resources/effects (new)/slash_effect_anim_f0.png"),
        new Image("../src/resources/effects (new)/slash_effect_anim_f0.png"),
        new Image("../src/resources/effects (new)/slash_effect_anim_f1.png"),
        new Image("../src/resources/effects (new)/slash_effect_anim_f1.png"),
        new Image("../src/resources/effects (new)/slash_effect_anim_f2.png"),
        new Image("../src/resources/effects (new)/slash_effect_anim_f2.png"),
    };
    static std::vector<Image *> ENEMY_DIED_I {
        new Image("../src/resources/effects (new)/enemy_afterdead_explosion_anim_f0.png"),
        new Image("../src/resources/effects (new)/enemy_afterdead_explosion_anim_f1.png"),
        new Image("../src/resources/effects (new)/enemy_afterdead_explosion_anim_f2.png"),
        new Image("../src/resources/effects (new)/enemy_afterdead_explosion_anim_f3.png"),
    };
    static Image WIN_SCREEN_I("../src/resources/win.png");
    static Image LOSE_SCREEN_I("../src/resources/gameover.png");

    switch (tile) {
    case EMPTY_T:
        return EMPTY_I;
    case WALL_T:
        return WALL_I;
    case DOOR_T:
    case FLOOR_T:
        return FLOOR_I;
    case GOLD_T:
        return GOLD_I;
    case TORCH_T:
        return *TORCH_I[Frame % 6];
    case PLAYER_IDLE_T:
        return *PLAYER_IDLE_I[Frame % 6];
    case PLAYER_RUN_T:
        return *PLAYER_RUN_I[Frame % 6];
    case PLAYER_IDLE_DAMAGED_T:
        return *PLAYER_IDLE_DAMAGED_I[Frame % 6];
    case PLAYER_RUN_DAMAGED_T:
        return *PLAYER_RUN_DAMAGED_I[Frame % 6];
    case PLAYER_DEAD_T:
        return PLAYER_DEAD_I;
    case ENEMY_IDLE_T:
        return *ENEMY_IDLE_I[(Frame + 3) % 6];
    case ENEMY_RUN_T:
        return *ENEMY_RUN_I[(Frame + 3) % 6];
    case ENEMY_IDLE_DAMAGED_T:
        return *ENEMY_IDLE_DAMAGED_I[Frame % 6];
    case ENEMY_RUN_DAMAGED_T:
        return *ENEMY_RUN_DAMAGED_I[Frame % 6];
    case EXIT_T:
        return EXIT_I;
    case HEALTH_T:
        return HEALTH_I;
    case SPLASH_T:
        return *SPLASH_I[frame];
    case ENEMY_DIED_T:
        return *ENEMY_DIED_I[frame];
    case WIN_SCREEN_T:
        return WIN_SCREEN_I;
    case LOSE_SCREEN_T:
        return LOSE_SCREEN_I;
    }
    return EMPTY_I;
}

void TTile::NextFrame() {
    Frame = (Frame + 1) % MaxFrame;
}