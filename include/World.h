#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Chunk.h"
#include "Constants.h"

class World {
public:
    std::unordered_map<ChunkKey, Chunk *, ChunkKeyHash, ChunkKeyEq> chunks;

    ~World() {
        for (auto &p : chunks)
            delete p.second;
    }

    Chunk *getChunk(int cx, int cy, bool create = true) {
        ChunkKey key{cx, cy};
        auto it = chunks.find(key);
        if (it != chunks.end())
            return it->second;
        if (!create)
            return nullptr;
        Chunk *c = new Chunk(cx, cy);
        chunks.emplace(key, c);
        return c;
    }

    void placeBlockAt(int bx, int by, Color c) {
        int cx = bx >= 0 ? bx / Constants::CHUNK_SIZE : (bx - Constants::CHUNK_SIZE + 1) / Constants::CHUNK_SIZE;
        int cy = by >= 0 ? by / Constants::CHUNK_SIZE : (by - Constants::CHUNK_SIZE + 1) / Constants::CHUNK_SIZE;
        int lx = (bx % Constants::CHUNK_SIZE + Constants::CHUNK_SIZE) % Constants::CHUNK_SIZE;
        int ly = (by % Constants::CHUNK_SIZE + Constants::CHUNK_SIZE) % Constants::CHUNK_SIZE;
        Chunk *chunk = getChunk(cx, cy);
        Block &block = chunk->blocks[lx][ly];
        if (!block.active) {
            chunk->activeBlockCount++;
        }
        block.active = true;
        block.color = c;
    }

    void destroyBlockAt(int bx, int by) {
        int cx = bx >= 0 ? bx / Constants::CHUNK_SIZE : (bx - Constants::CHUNK_SIZE + 1) / Constants::CHUNK_SIZE;
        int cy = by >= 0 ? by / Constants::CHUNK_SIZE : (by - Constants::CHUNK_SIZE + 1) / Constants::CHUNK_SIZE;
        Chunk *chunk = getChunk(cx, cy, false);
        if (!chunk)
            return;
        int lx = (bx % Constants::CHUNK_SIZE + Constants::CHUNK_SIZE) % Constants::CHUNK_SIZE;
        int ly = (by % Constants::CHUNK_SIZE + Constants::CHUNK_SIZE) % Constants::CHUNK_SIZE;
        Block &block = chunk->blocks[lx][ly];
        if (block.active) {
            block.active = false;
            chunk->activeBlockCount--;
            if (chunk->activeBlockCount == 0) {
                // Remove and delete chunk
                ChunkKey key{cx, cy};
                auto it = chunks.find(key);
                if (it != chunks.end()) {
                    delete it->second;
                    chunks.erase(it);
                }
            }
        }
    }

    void draw(Camera2D &cam) {
        BeginMode2D(cam);
        for (auto &p : chunks) {
            Chunk *c = p.second;
            c->Draw();
        }
        EndMode2D();
    }

    bool save(const std::string &filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out)
            return false;
        size_t count = chunks.size();
        out.write((char *)&count, sizeof(count));
        for (auto &p : chunks) {
            Chunk *c = p.second;
            out.write((char *)&c->x, sizeof(c->x));
            out.write((char *)&c->y, sizeof(c->y));
            for (int i = 0; i < Chunk::SIZE; ++i)
                for (int j = 0; j < Chunk::SIZE; ++j) {
                    Block &b = c->blocks[i][j];
                    out.write((char *)&b.active, sizeof(b.active));
                    if (b.active)
                        out.write((char *)&b.color, sizeof(b.color));
                }
        }
        return true;
    }

    bool load(const std::string &filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in)
            return false;
        for (auto &p : chunks)
            delete p.second;
        chunks.clear();
        size_t count;
        in.read((char *)&count, sizeof(count));
        for (size_t k = 0; k < count; ++k) {
            int cx, cy;
            in.read((char *)&cx, sizeof(cx));
            in.read((char *)&cy, sizeof(cy));
            Chunk *c = new Chunk(cx, cy);
            int activeCount = 0;
            for (int i = 0; i < Chunk::SIZE; ++i)
                for (int j = 0; j < Chunk::SIZE; ++j) {
                    in.read((char *)&c->blocks[i][j].active, sizeof(bool));
                    if (c->blocks[i][j].active) {
                        in.read((char *)&c->blocks[i][j].color, sizeof(Color));
                        activeCount++;
                    }
                }
            c->activeBlockCount = activeCount;
            chunks.emplace(ChunkKey{cx, cy}, c);
        }
        return true;
    }
};
