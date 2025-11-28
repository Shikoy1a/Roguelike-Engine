#pragma once
#include <vector>
#include <string>
#include "entity.hpp"
#include "pathfinding.hpp"

struct InventoryItem {
    std::string name;
    int healAmount;
};

class Game {
public:
    Game();

    void render() const;                 // 渲染
    void handleInput(char command, bool& running); // 处理玩家输入
    void updateMonsters(bool& running);  // 更新怪物 

    const std::vector<InventoryItem>& getInventory() const { return inventory;}

    //给图形化提供接口
    const std::vector<std::string>& getMap() const {return map;}
    const std::vector<Entity>& getEntities() const {return entities;}
    const std::vector<std::vector<bool>>& getVisible() const {return visible;}
    const std::vector<std::vector<bool>>& getExplored() const {return explored;}
    const std::vector<std::string>& getLog() const {return logLines;}

    void stepPlayerMove(int dx, int dy, bool& running);

private:
    std::vector<std::string> map;    // 地图
    int width = 0;
    int height = 0;

    std::vector<Entity> entities;   

    // 视野与探索
    std::vector<std::vector<bool>> visible;
    std::vector<std::vector<bool>> explored;
    int fovRadius = 8;

    std::vector<InventoryItem> inventory;

    // 简单日志系统
    std::vector<std::string> logLines;

    void init();                     // 初始化整个游戏（调用地牢生成等）
    void generateDungeon();          // 程序化地牢生成
    void updateFov();                // 计算 FoV
    void addLog(const std::string&); // 向日志里添加一条信息

    void pickUp();
    void useFirstItem();
};
