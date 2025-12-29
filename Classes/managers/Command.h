#ifndef __COMMAND_H__
#define __COMMAND_H__

class Command {
public:
    virtual ~Command() {}

    // 执行操作
    virtual void execute() = 0;

    // 撤销操作 (回退)
    virtual void undo() = 0;
};

#endif // __COMMAND_H__#pragma once
