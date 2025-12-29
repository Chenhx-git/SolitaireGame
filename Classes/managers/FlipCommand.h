#ifndef __FLIP_COMMAND_H__
#define __FLIP_COMMAND_H__

#include "Command.h"
#include "../views/CardView.h"

class FlipCommand : public Command {
public:
    FlipCommand(CardView* card);
    virtual ~FlipCommand();

    virtual void execute() override;
    virtual void undo() override;

private:
    CardView* _card;        // 被翻的那张牌
    CardView* _prevTopCard; // 记录翻牌前的旧底牌
    cocos2d::Vec2 _startPos; // 新增
    cocos2d::Vec2 _endPos;   // 新增
};

#endif // __FLIP_COMMAND_H__
