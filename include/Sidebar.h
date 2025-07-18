#pragma once
#include "raylib.h"

class Sidebar {
public:
    void draw() const {
        DrawRectangle(Constants::SCREEN_WIDTH - Constants::SIDEBAR_WIDTH, 0, Constants::SIDEBAR_WIDTH, Constants::SCREEN_HEIGHT, {0, 0, 0, 100});
    }
};