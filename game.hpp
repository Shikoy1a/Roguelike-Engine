#pragma once

#include <vector>
#include <string>
#include "entity.hpp"

class Game {
public:
    Game();

    void render() const;               // 画地图 + 状态 + 消息
    void handleInput(char command, bool& running);
    void updateMonsters(bool& running);

private:
    std::vector<std::string> map;
    std::vector<Entity> entities;

    std::string lastMessage;

    void init();                       // 初始化地图和实体
    void setMessage(const std::string& msg);
};
