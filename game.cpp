#include "game.hpp"
#include "entity.hpp"
#include <iostream>
#include <cstdlib>
#include <sstream>

static void clear_screen() {
    system("cls");
}

Game::Game() {
    init();
}

void Game::setMessage(const std::string& msg) {
    lastMessage = msg;
}

void Game::init() {
    // 初始化地图
    map = {
        "##########",
        "#........#",
        "#..#.##..#",
        "#........#",
        "#..##.#..#",
        "#........#",
        "#..#..#..#",
        "#..#.##..#",
        "#........#",
        "##########"
    };

    entities.clear();

    // 玩家
    Entity player;
    player.x = 1;
    player.y = 1;
    player.glyph    = '@';
    player.blocks   = true;
    player.isPlayer = true;
    player.maxHp    = 20;
    player.hp       = 20;
    player.attack   = 5;
    entities.push_back(player);

    // 怪物1
    Entity monster1;
    monster1.x = 1;
    monster1.y = 8;
    monster1.glyph    = 'g';
    monster1.blocks   = true;
    monster1.isPlayer = false;
    monster1.maxHp    = 10;
    monster1.hp       = 10;
    monster1.attack   = 3;
    entities.push_back(monster1);

    // 怪物2
    Entity monster2;
    monster2.x = 7;
    monster2.y = 5;
    monster2.glyph    = 'o';
    monster2.blocks   = true;
    monster2.isPlayer = false;
    monster2.maxHp    = 12;
    monster2.hp       = 12;
    monster2.attack   = 4;
    entities.push_back(monster2);
}

// 渲染地图 + 实体 + 状态
void Game::render() const {
    clear_screen();

    int height = static_cast<int>(map.size());
    int width  = static_cast<int>(map[0].size());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char ch = map[y][x];

            // 有实体覆盖
            for (const auto& e : entities) {
                if (e.x == x && e.y == y) {
                    ch = e.glyph;
                    break;
                }
            }
            std::cout << ch;
        }
        std::cout << '\n';
    }

    const Entity& player = entities[0];
    std::cout << "\nPlayer (@) at (" << player.x << ", " << player.y << ")  "
              << "HP: " << player.hp << " / " << player.maxHp << "\n";

    for (std::size_t i = 1; i < entities.size(); ++i) {
        const auto& m = entities[i];
        std::cout << "Monster " << m.glyph << " at (" << m.x << ", " << m.y
                  << ") HP: " << m.hp << " / " << m.maxHp << "\n";
    }

    std::cout << "\nControls: WASD move, Q quit.\n";

    std::cout << "Message: ";
    if (lastMessage.empty()) {
        std::cout << "(none)";
    } else {
        std::cout << lastMessage;
    }
    std::cout << "\n";
}

void Game::handleInput(char command, bool& running) {
    setMessage("");
    // 转小写
    if (command >= 'A' && command <= 'Z') {
        command = command - 'A' + 'a';
    }

    Entity& player = entities[0];
    int dx = 0, dy = 0;

    if (command == 'w') dy = -1;
    else if (command == 's') dy = 1;
    else if (command == 'a') dx = -1;
    else if (command == 'd') dx = 1;
    else if (command == 'q') {
        running = false;
    }

    if (running && (dx != 0 || dy != 0)) {
        int targetX = player.x + dx;
        int targetY = player.y + dy;

        int monsterIndex = find_monster_at(entities, targetX, targetY);
        if (monsterIndex != -1) {

            Entity& m = entities[monsterIndex];
            m.hp -= player.attack;

            std::ostringstream oss;
            oss << "You hit " << m.glyph << " for "
                << player.attack << " damage! Monster HP = " << m.hp;
            setMessage(oss.str());

            if (m.hp <= 0) { 
                m.blocks = false;
                m.glyph  = '.'; 
                std::ostringstream deadMsg;
                deadMsg << "Monster " << m.glyph << " dies!";
                setMessage(oss.str()); 
            }
        } else {
            try_move_entity(player, map, entities, dx, dy);
        }
    }
}

void Game::updateMonsters(bool & running) {
    Entity& player = entities[0];

    int dx = 0;
    int dy = 0;

    for (std::size_t i = 1; i < entities.size(); ++i) {
        Entity& monster = entities[i];

        std::cout << "Updating monster index " << i
              << " glyph " << monster.glyph
              << " hp " << monster.hp << "\n";

        if (monster.hp <= 0) continue;

        if (monster.x < player.x) dx = 1;
        else if (monster.x > player.x) dx = -1;

        if (monster.y < player.y) dy = 1;
        else if (monster.y > player.y) dy = -1;

        int targetX = monster.x + dx;
        int targetY = monster.y + dy;

        if (targetX == player.x && targetY == player.y) {
            player.hp -= monster.attack;
            
            std::ostringstream oss;
            oss << "Monster " << monster.glyph << " hits you for "
                << monster.attack << " damage! Your HP = " << player.hp;
            setMessage(oss.str());

            if (player.hp <= 0) {
                setMessage("You died!");
                running = false;
                return;
            }
        } else {
            if (dx != 0) {
                try_move_entity(monster, map, entities, dx, 0);
            }

            if (dy != 0) {
                try_move_entity(monster, map, entities, 0, dy);
            }
        }
        
    }
}