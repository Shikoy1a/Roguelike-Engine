#include "raylib.h"
#include "game.hpp"
#include <ctime>
#include <cstdlib>

const int TILE_SIZE = 32;

Color TileColor(char tile, bool visible, bool explored) {
    if (!visible && !explored) {
        return BLACK;
    }

    if (!visible && explored) {
        switch(tile) {
            case '#': return (Color) {40, 40, 40, 255};
            case '.': return (Color) {30, 30, 30, 255};
            default: return (Color) {60, 60, 60, 255};
        }
    }

    switch (tile) {
        case '#': return (Color){ 130, 130, 130, 255 }; 
        case '.': return (Color){ 60, 60, 60, 255 };     
        default:  return (Color){ 100, 100, 100, 255 };
    }
}

// 实体颜色
Color EntityColor(const Entity& e, bool visible) {
    if (!visible) {
        return (Color){ 80, 80, 80, 255 };
    }

    if (e.isPlayer) {
        return GREEN;
    } else if (e.hp > 0) {
        return RED;
    } else {
        // 尸体
        return PURPLE;
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Game game;

    const auto& map = game.getMap();
    int mapHeight = (int)map.size();
    int mapWidth = mapHeight > 0 ? (int)map[0].size() : 0;

    int screenWidth = mapWidth * TILE_SIZE;
    int screenHeight = mapHeight * TILE_SIZE + 100;

    InitWindow(screenWidth, screenHeight, "ROGUE ENGINE");
    SetTargetFPS(60);

    bool running = true;

    while (!WindowShouldClose() && running) {

        int dx = 0, dy = 0;

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    dy = -1;
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  dy = 1;
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  dx = -1;
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;

        if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
            running = false;
        }

        if (dx != 0 || dy != 0) {
            game.stepPlayerMove(dx, dy, running);
            if (!running) break;

            game.updateMonsters(running);
            if (!running) break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const auto& mapRef      = game.getMap();
        const auto& entities    = game.getEntities();
        const auto& visibleGrid = game.getVisible();
        const auto& exploredGrid= game.getExplored();

        // 画地图 
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                bool vis  = visibleGrid.empty()  ? true : visibleGrid[y][x];
                bool expl = exploredGrid.empty() ? true : exploredGrid[y][x];

                Color c = TileColor(mapRef[y][x], vis, expl);
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, c);
            }
        }

        // 画实体
        for (std::size_t i = 0; i < entities.size(); ++i) {
            const Entity& e = entities[i];
            if (e.hp <= 0 && e.glyph != 'x') continue;

            int x = e.x;
            int y = e.y;

            bool vis = visibleGrid.empty() ? true : visibleGrid[y][x];
            Color c = EntityColor(e, vis);

            int cx = x * TILE_SIZE + TILE_SIZE / 4;
            int cy = y * TILE_SIZE + TILE_SIZE / 4;
            int size = TILE_SIZE / 2;

            DrawRectangle(cx, cy, size, size, c);
        }

        const auto& ents = game.getEntities();
        if (!ents.empty()) {
            const Entity& player = ents[0];
            DrawText(TextFormat("HP: %d / %d", player.hp, player.maxHp),
                     10, mapHeight * TILE_SIZE + 10, 20, RAYWHITE);
        }

        int logY = mapHeight * TILE_SIZE + 40;
        const auto& logs = game.getLog();
        int maxLinesToShow = 4;
        int start = (int)logs.size() - maxLinesToShow;
        if (start < 0) start = 0;
        int line = 0;
        for (int i = start; i < (int)logs.size(); ++i) {
            DrawText(logs[i].c_str(), 10, logY + line * 20, 18, GRAY);
            line++;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}