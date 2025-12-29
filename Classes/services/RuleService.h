#ifndef __RULE_SERVICE_H__
#define __RULE_SERVICE_H__

#include "../models/GameModel.h" // 引用刚刚创建的 Model

class RuleService {
public:
    // 静态方法：判断两张牌的数据是否匹配
    static bool canMatch(const CardData& cardA, const CardData& cardB);
};

#endif // __RULE_SERVICE_H__
