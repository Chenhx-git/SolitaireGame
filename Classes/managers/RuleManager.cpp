#include "RuleManager.h"
#include <cmath> // 用于 abs 函数

// 判断规则：点数相差 1 (不限花色)
bool RuleManager::canStack(CardView* fromCard, CardView* targetCard) {
    if (!fromCard || !targetCard) return false;

    int face1 = fromCard->getFace();
    int face2 = targetCard->getFace();

    // 计算差值
    int diff = std::abs(face1 - face2);

    // 规则A: 正常的差 1 (比如 3 和 4)
    if (diff == 1) {
        return true;
    }

    // 规则B: K(13) 和 A(1) 的衔接 (循环接龙)
    if ((face1 == 1 && face2 == 13) || (face1 == 13 && face2 == 1)) {
        return true;
    }

    return false;
}