#include "Board.hpp"
#include <algorithm>

Board::Board() 
{
    // 初始化：整個棋盤都是 0 (空)
    grid.resize(HEIGHT, std::vector<int>(WIDTH, 0));
}

Board::~Board() {}

bool Board::checkCollision(const Tetromino& tetromino) const 
{
    auto blocks = tetromino.getBlocks();
    auto pos = tetromino.getPosition();

    for (auto &block : blocks) 
    {
        int row = pos.first + block.first;
        int col = pos.second + block.second;

        // 超出邊界
        if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) 
        {
            return true;
        }

        // 該格子已經有方塊(顏色 != 0)
        if (grid[row][col] != 0) 
        {
            return true;
        }
    }
    return false;
}

void Board::placeTetromino(const Tetromino& tetromino) 
{
    auto blocks = tetromino.getBlocks();
    auto pos = tetromino.getPosition();
    // 取得該方塊的顏色
    int color = tetromino.getColor(); 

    for (auto &block : blocks) 
    {
        int row = pos.first + block.first;
        int col = pos.second + block.second;

        if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH) 
        {
            // 放入該方塊的顏色
            grid[row][col] = color;
        }
    }
}

int Board::clearLines() 
{
    int linesCleared = 0;

    for (int r = 0; r < HEIGHT; ++r) 
    {
        bool isFull = true;

        for (int c = 0; c < WIDTH; ++c) 
        {
            if (grid[r][c] == 0) 
            {
                isFull = false;
                break;
            }
        }

        if (isFull) 
        {
            linesCleared++;

            // 把該行上方的內容往下搬
            for (int rowToMove = r; rowToMove > 0; --rowToMove) 
            {
                grid[rowToMove] = grid[rowToMove - 1];
            }

            // 最上面那行清空
            grid[0] = std::vector<int>(WIDTH, 0);
        }
    }

    return linesCleared;
}

const std::vector<std::vector<int>>& Board::getGrid() const 
{
    return grid;
}
