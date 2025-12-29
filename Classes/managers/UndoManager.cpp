#include "UndoManager.h"

UndoManager::UndoManager() {}

UndoManager::~UndoManager() {
    clear();
}

void UndoManager::pushAction(const std::function<void()>& func) {
    UndoAction action;
    action.restoreFunc = func;
    _historyStack.push(action);
}

void UndoManager::undo() {
    if (_historyStack.empty()) return;

    // 取出最近的一次操作
    UndoAction action = _historyStack.top();
    _historyStack.pop();

    // 执行恢复逻辑
    if (action.restoreFunc) {
        action.restoreFunc();
    }
}

void UndoManager::clear() {
    while (!_historyStack.empty()) {
        _historyStack.pop();
    }
}

bool UndoManager::hasHistory() const {
    return !_historyStack.empty();
}