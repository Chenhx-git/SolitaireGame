#ifndef __MATCH_COMMAND_H__
#define __MATCH_COMMAND_H__

#include "Command.h"
#include "../views/CardView.h"
#include "cocos2d.h"

class MatchCommand : public Command {
public:
    MatchCommand(CardView* card, cocos2d::Vec2 startPos, cocos2d::Vec2 endPos, int startZ, int endZ);
    // 析构函数里记得释放引用
    virtual ~MatchCommand();

    virtual void execute() override;
    virtual void undo() override;

private:
    CardView* _card;         // 操作的牌 (变成新底牌的那张)
    CardView* _prevTopCard;  // 【新增】记录操作前的旧底牌

    cocos2d::Vec2 _startPos;
    cocos2d::Vec2 _endPos;
    int _startZOrder;
    int _endZOrder;
};

#endif // __MATCH_COMMAND_H__