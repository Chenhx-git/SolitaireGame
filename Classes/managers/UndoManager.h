#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include <stack>
#include <functional>

// 定义一个回退操作的数据结构
struct UndoAction {
    // 这是一个函数对象，存储"如何撤销"的具体逻辑
    // 这样做的好处是通用性极强，任何操作都可以存进来
    std::function<void()> restoreFunc;
};

class UndoManager {
public:
    UndoManager();
    ~UndoManager();

    // 记录一步操作
    void pushAction(const std::function<void()>& func);

    // 执行一次回退
    void undo();

    // 清空历史
    void clear();

    // 是否有历史
    bool hasHistory() const;

private:
    std::stack<UndoAction> _historyStack;
};

#endif // __UNDO_MANAGER_H__