#ifndef TEROMINO
#define TEROMINO

#pragma once
#include <vector>
#include <utility>

enum class TetrominoType 
{
    I, O, T, S, Z, J, L
};

class Tetromino 
{
    private:
        TetrominoType type;
        std::pair<int,int> position;          // (row, col)
        int rotationIndex;
        // 隨機顏色編號，非 0
        int color;                           

        // 形狀在不同旋轉狀態下的區塊座標
        std::vector<std::vector<std::pair<int,int>>> shapes;
        // 目前旋轉索引對應的區塊 (快取)
        std::vector<std::pair<int,int>> currentBlocks;

        void setShape(TetrominoType t);
        void updateBlocks();

    public:
        Tetromino();
        ~Tetromino();

        // 指定形狀重置位置與旋轉狀態，同時也重新決定顏色
        void reset(TetrominoType type);

        // 移動、旋轉操作
        void moveLeft();
        void moveRight();
        void moveDown();
        void moveUp();
        void rotateLeft();
        void rotateRight();

        // 取得當前方塊在棋盤的絕對位置 (row, col)
        std::pair<int,int> getPosition() const;
        // 取得此形狀目前旋轉狀態下，相對於 (row, col) 的各個區塊偏移量
        std::vector<std::pair<int,int>> getBlocks() const;

        // 取得現在的形狀
        TetrominoType getType() const;

        // 取得方塊顏色
        int getColor() const;
};

#endif