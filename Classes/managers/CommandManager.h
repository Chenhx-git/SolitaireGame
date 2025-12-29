#ifndef __COMMAND_MANAGER_H__
#define __COMMAND_MANAGER_H__

#include "Command.h"
#include <stack>
#include "cocos2d.h"

class CommandManager {
public:
    static CommandManager* getInstance();

    // 执行一条新命令，并压入历史栈
    void executeCommand(Command* cmd);

    // 执行回退
    void undoLastCommand();

    // 清空历史 (新游戏时用)
    void clear();

private:
    std::stack<Command*> _undoStack;
};

#endif // __COMMAND_MANAGER_H__#pragma once
