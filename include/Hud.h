#pragma once

#include "raylib.h"
#include <sstream>
#include <string>

#include "Sidebar.h"
#include "Constants.h"

struct Cursor {
    void draw(int x, int y, int cameraX, int cameraY) const {
        DrawRectangleLinesEx(
            {
                (float)x * Constants::BLOCK_SIZE - cameraX,
                (float)y * Constants::BLOCK_SIZE - cameraY,
                (float)Constants::BLOCK_SIZE,
                (float)Constants::BLOCK_SIZE
            },
            Constants::CURSOR_OUTLINE_THICKNESS,
            Theme::CURSOR_OUTLINE
        );

        if (Constants::IS_DEBUG) {

            const char* text = (std::to_string(x) + "," + std::to_string(y)).c_str();
            int textWidth = MeasureText(text, Constants::FONT_SIZE);
            int textHeight = Constants::FONT_SIZE;

            DrawText(text, (float)x * Constants::BLOCK_SIZE - cameraX + Constants::BLOCK_SIZE - textWidth - 5, (float)y * Constants::BLOCK_SIZE - cameraY + Constants::BLOCK_SIZE - textHeight - 5, Constants::FONT_SIZE, WHITE);
        }
    }
};

class Hud {
public:
    void draw(int bx, int by, const Vector2 &mouse, const Camera2D &camera, const Sidebar* sidebar) const {
        sidebar->draw();
        cursor.draw(bx, by, camera.target.x, camera.target.y);
        if (!Constants::IS_DEBUG)
            return;

        int y = 10;
        const int lineHeight = 30;

        drawDebugText("Block: ", bx, by, 10, y);
        y += lineHeight;
        drawDebugText("Mouse: ", static_cast<int>(mouse.x), static_cast<int>(mouse.y), 10, y);
        y += lineHeight;
        drawDebugText("Camera Target: ", static_cast<int>(camera.target.x), static_cast<int>(camera.target.y), 10, y);
        y += lineHeight;
        drawDebugText("Camera Offset: ", static_cast<int>(camera.offset.x), static_cast<int>(camera.offset.y), 10, y);
    }

private:
    Cursor cursor;

    void drawDebugText(const std::string &label, int x, int y, int screenX, int screenY) const {
        std::ostringstream oss;
        oss << label << x << "," << y;
        DrawText(oss.str().c_str(), screenX, screenY, 20, WHITE);
    }
};