#ifndef __GAME_DEFINES_H__
#define __GAME_DEFINES_H__

// --- 1. 用户指定的枚举定义 ---

// 花色类型
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS = 0,      // 梅花 (对应资源 card_2_x)
    CST_DIAMONDS = 3,   // 方块 (对应资源 card_3_x)
    CST_HEARTS = 1,     // 红桃 (对应资源 card_1_x)
    CST_SPADES = 0,     // 黑桃 (对应资源 card_0_x)
    // 注意：具体 int 值需要和您的图片资源命名对应，这里暂定
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE = 1,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

// --- 2. 游戏区域标记 (用于判断牌在哪里) ---
enum class CardZone {
    PLAYFIELD,  // 主牌区 (桌面上待消除的牌)
    STOCK,      // 备用牌堆 (左下角待翻的牌)
    DISCARD     // 底牌堆/手牌区 (当前显示的顶牌)
};

// --- 3. 屏幕尺寸配置 ---
static const float DESIGN_WIDTH = 1080.0f;
static const float DESIGN_HEIGHT = 2080.0f;

#endif // __GAME_DEFINES_H__#pragma once
