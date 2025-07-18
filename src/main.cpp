#include "raylib.h"
#include "raymath.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Block.h"
#include "Chunk.h"
#include "Constants.h"
#include "States.h"
#include "World.h"
#include "Theme.h"
#include "Hud.h"

int main() {
    InitWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, Constants::WINDOW_TITLE);

    Camera2D camera = {{0, 0}, {0, 0}, 0.0f, 1.0f};
    SetTargetFPS(Constants::TARGET_FPS);

    World world;
    world.load(Constants::WORLD_FILE);

    Hud hud;
    Sidebar sidebar;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Camera movement
        if (IsKeyDown(KEY_A))
            camera.target.x -= Constants::SPEED * dt;
        if (IsKeyDown(KEY_D))
            camera.target.x += Constants::SPEED * dt;
        if (IsKeyDown(KEY_W))
            camera.target.y -= Constants::SPEED * dt;
        if (IsKeyDown(KEY_S))
            camera.target.y += Constants::SPEED * dt;

        if (IsKeyPressed(KEY_O))
            State::IS_DEBUG = !State::IS_DEBUG;

        // Mouse input
        Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
        int bx = (int)floor(mouse.x / Constants::BLOCK_SIZE);
        int by = (int)floor(mouse.y / Constants::BLOCK_SIZE);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            world.placeBlockAt(bx, by, DARKGREEN);
        } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            world.destroyBlockAt(bx, by);
        }

        BeginDrawing();
            ClearBackground(Theme::BACKGROUND);
            world.draw(camera);

            hud.draw(bx, by, mouse, camera, &sidebar);
        EndDrawing();
    }

    world.save(Constants::WORLD_FILE);
    CloseWindow();
}
