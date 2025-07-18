#pragma once

#include "raylib.h"

#include "States.h"
#include "Constants.h"

struct Block {
    bool active;
    Color color;

    void draw(int wx, int wy) const {
        if (active) {
            DrawRectangle(wx, wy, Constants::BLOCK_SIZE, Constants::BLOCK_SIZE, color);
            if (State::IS_DEBUG)
                DrawText((std::to_string(wx / Constants::BLOCK_SIZE) + "," + std::to_string(wy / Constants::BLOCK_SIZE)).c_str(), wx + 5, wy + 5, Constants::FONT_SIZE, WHITE);
        }
    }
};