#include "RuleService.h"
#include <cmath> // std::abs

bool RuleService::canMatch(const CardData& cardA, const CardData& cardB) {
    int face1 = cardA.face;
    int face2 = cardB.face;

    // 逻辑和之前一样：差值为 1
    int diff = std::abs(face1 - face2);
    if (diff == 1) return true;

    // K(13) 和 A(1) 循环衔接
    if ((face1 == 1 && face2 == 13) || (face1 == 13 && face2 == 1)) {
        return true;
    }

    return false;
}