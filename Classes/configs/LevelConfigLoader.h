#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "LevelConfig.h"
#include <string>

class LevelConfigLoader {
public:
    // 静态函数：给他一个关卡ID，他返回加载好的数据
    static LevelConfig loadLevel(int levelId);
};

#endif // __LEVEL_CONFIG_LOADER_H__#pragma once
