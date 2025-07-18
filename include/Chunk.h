#pragma once

#include <functional>

#include "Block.h"
#include "Constants.h"

struct Chunk {
    static const int SIZE = Constants::CHUNK_SIZE;
    // 2D grid of blocks
    Block blocks[SIZE][SIZE];
    int x, y; // chunk coordinates
    int activeBlockCount = 0;  // count of active blocks

    Chunk(int cx, int cy) : x(cx), y(cy) {
        // initialize empty
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                blocks[i][j] = {false, WHITE};
    }

    void Draw() {
        for (int i = 0; i < Chunk::SIZE; ++i)
            for (int j = 0; j < Chunk::SIZE; ++j) {
                Block &b = blocks[i][j];
                if (b.active) {
                    int wx = (x * Constants::CHUNK_SIZE + i) * Constants::BLOCK_SIZE;
                    int wy = (y * Constants::CHUNK_SIZE + j) * Constants::BLOCK_SIZE;
                    b.draw(wx, wy);
                }
            }
            
        if (Constants::IS_DEBUG) {
            // Draw border around the entire chunk
            int wx = x * Constants::CHUNK_SIZE * Constants::BLOCK_SIZE;
            int wy = y * Constants::CHUNK_SIZE * Constants::BLOCK_SIZE;
            int borderSize = Constants::CHUNK_SIZE * Constants::BLOCK_SIZE;
            DrawRectangleLines(wx, wy, borderSize, borderSize, BLUE);
            // Draw the Chunk coordinate as text
            DrawText((std::to_string(x) + "," + std::to_string(y)).c_str(), wx + 5, wy + 5, Constants::FONT_SIZE, WHITE);
        }
    }
};

struct ChunkKey {
    int x, y;
};

struct ChunkKeyHash {
    size_t operator()(ChunkKey const &k) const noexcept {
        return std::hash<long long>()(((long long)k.x << 32) ^ (unsigned long long)k.y);
    }
};

struct ChunkKeyEq {
    bool operator()(ChunkKey const &a, ChunkKey const &b) const noexcept {
        return a.x == b.x && a.y == b.y;
    }
};