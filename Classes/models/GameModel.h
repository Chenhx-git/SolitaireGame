#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include <vector>

// 1. 定义卡牌的纯数据结构
struct CardData {
    int id;         // 唯一ID
    int face;       // 点数
    int suit;       // 花色
    bool isFaceUp;  // 是否正面朝上

    // 【关键修复】添加默认构造函数
    CardData() : id(-1), face(0), suit(0), isFaceUp(false) {}

    // 【关键修复】添加带参数的构造函数 (修复报错的核心)
    CardData(int _id, int _face, int _suit)
        : id(_id), face(_face), suit(_suit), isFaceUp(false) {
    }
};

// 2. 游戏整体数据模型
class GameModel {
public:
    GameModel() : _score(0) {}
    ~GameModel() {}

    // 桌面上的牌数据
    std::vector<CardData> playFieldCards;

    // 备用牌堆数据
    std::vector<CardData> stackCards;

    // 当前底牌数据
    CardData topCard;

    // 分数
    int getScore() const { return _score; }
    void addScore(int value) { _score += value; }

private:
    int _score;
};

#endif // __GAME_MODEL_H__