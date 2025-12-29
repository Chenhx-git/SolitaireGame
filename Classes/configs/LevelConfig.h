#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include <vector>
#include "cocos2d.h"

// 1. 定义单张牌的配置数据
struct CardConfigData {
    int face; // 牌面数字 1-13 (A-K)
    int suit; // 花色 0-3 (黑红梅方)
    float x;  // 屏幕位置 X
    float y;  // 屏幕位置 Y
};

// 2. 定义整个关卡的配置数据
struct LevelConfig {
    int levelId;
    std::vector<CardConfigData> playfieldCards; // 主牌区的牌列表
    std::vector<CardConfigData> stackCards;     // 备用牌堆列表
};

#endif // __LEVEL_CONFIG_H__
