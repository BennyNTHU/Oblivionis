#ifndef BOARD
#define BOARD

#pragma once

#include <vector>
#include "Tetromino.hpp"

class Board 
{
    private:
        // 二維容器存放方塊狀態：0 表示空，非 0 表示有方塊 (可用作顏色/ID)
        std::vector<std::vector<int>> grid;

    public:
        Board();
        ~Board();

        static const int WIDTH = 10;   // 棋盤寬度
        static const int HEIGHT = 20;  // 棋盤高度

        // 檢查放置中的方塊是否碰撞到牆壁或其他方塊
        bool checkCollision(const Tetromino& tetromino) const;

        // 將方塊放置到棋盤上
        void placeTetromino(const Tetromino& tetromino);

        // 檢查並消除已填滿的一行，回傳消除的行數
        int clearLines();

        // 取得棋盤內部狀態，用於繪製或調試
        const std::vector<std::vector<int>>& getGrid() const;
};

# endif