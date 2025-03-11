#ifndef RENDERER
#define RENDERER

#pragma once

#include "Board.hpp"
#include "Tetromino.hpp"
#include "ScoreManager.hpp"

class Renderer 
{
    public:
        Renderer();
        ~Renderer();

        void draw(const Board& board, const Tetromino& tetromino, const ScoreManager& scoreManager, int level);
};


#endif