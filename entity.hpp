#pragma once
#include <vector>
#include <string>

struct Entity {
    int x;
    int y;
    char glyph;
    bool blocks;
    bool isPlayer;

    int hp;
    int maxHp;
    int attack;
};

// 这些函数只声明一次，供 game.cpp / main.cpp 使用
bool in_bounds(const std::vector<std::string>& map, int x, int y);
bool is_walkable_tile(const std::vector<std::string>& map, int x, int y);
bool is_blocked(const std::vector<std::string>& map,
                const std::vector<Entity>& entities,
                int x, int y);

void try_move_entity(Entity& e,
                     const std::vector<std::string>& map,
                     const std::vector<Entity>& entities,
                     int dx, int dy);

int find_monster_at(const std::vector<Entity>& entities, int x, int y);
