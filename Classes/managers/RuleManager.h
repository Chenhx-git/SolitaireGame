#ifndef __RULE_MANAGER_H__
#define __RULE_MANAGER_H__

#include "../views/CardView.h"

class RuleManager {
public:
    // 判断两张牌是否符合消除规则 (点数相差 1)
    // fromCard: 想要消除的那张牌 (通常是桌上的牌)
    // targetCard: 目标牌 (通常是手牌堆顶部的牌)
    static bool canStack(CardView* fromCard, CardView* targetCard);
};

#endif // __RULE_MANAGER_H__#pragma once
