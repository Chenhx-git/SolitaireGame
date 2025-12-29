#include "CommandManager.h"

static CommandManager* _instance = nullptr;

CommandManager* CommandManager::getInstance() {
    if (!_instance) _instance = new CommandManager();
    return _instance;
}

void CommandManager::executeCommand(Command* cmd) {
    if (!cmd) return;

    cmd->execute();       // 执行
    _undoStack.push(cmd); // 记录
}

void CommandManager::undoLastCommand() {
    if (_undoStack.empty()) {
        cocos2d::log("没有可以回退的操作了");
        return;
    }

    Command* cmd = _undoStack.top();
    _undoStack.pop();

    cmd->undo(); // 执行反向操作

    delete cmd;  // 销毁命令对象
}

void CommandManager::clear() {
    while (!_undoStack.empty()) {
        Command* cmd = _undoStack.top();
        delete cmd; // 删除命令对象
        _undoStack.pop();
    }
}