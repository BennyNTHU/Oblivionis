#include "Tetromino.hpp"
#include <cstdlib> // for rand()

Tetromino::Tetromino()
: type(TetrominoType::I),
  position({0, 4}),
  rotationIndex(0),
  color(1) // 預設給一個顏色 (例如 1)
{
    setShape(type);
    updateBlocks();
}

Tetromino::~Tetromino() {}

void Tetromino::reset(TetrominoType t) 
{
    type = t;
    position = {0, 4};
    rotationIndex = 0;

    // 隨機決定顏色 (1~7)
    color = (std::rand() % 7) + 1;

    setShape(type);
    updateBlocks();
}

int Tetromino::getColor() const 
{
    return color;
}

void Tetromino::moveLeft() 
{
    position.second--;
}

void Tetromino::moveRight() 
{
    position.second++;
}

void Tetromino::moveDown() 
{
    position.first++;
}

void Tetromino::moveUp() 
{
    position.first--;
}

void Tetromino::rotateLeft() 
{
    rotationIndex = (rotationIndex + 3) % 4; // 相當於 -1 (mod 4)
    updateBlocks();
}

void Tetromino::rotateRight() 
{
    rotationIndex = (rotationIndex + 1) % 4; // 相當於 +1 (mod 4)
    updateBlocks();
}

std::pair<int,int> Tetromino::getPosition() const 
{
    return position;
}

std::vector<std::pair<int,int>> Tetromino::getBlocks() const 
{
    return currentBlocks;
}

TetrominoType Tetromino::getType() const 
{
    return type;
}

// 根據不同行狀，事先定義四種旋轉狀態下的每個區塊偏移位置
void Tetromino::setShape(TetrominoType t) 
{
    shapes.clear();

    switch(t) 
    {
        case TetrominoType::I:
            // 直條 I：0度、90度、180度、270度
            // 0度和180度類似，1和270度類似，這裡簡單實作
            shapes.push_back({{0,0},{0,1},{0,2},{0,3}}); // 水平
            shapes.push_back({{0,0},{1,0},{2,0},{3,0}}); // 垂直
            shapes.push_back({{0,3},{0,2},{0,1},{0,0}}); // 水平反向(可視為180度)
            shapes.push_back({{3,0},{2,0},{1,0},{0,0}}); // 垂直反向(可視為270度)
            break;
        case TetrominoType::O:
            // 方塊 O 不會改變形狀
            shapes.push_back({{0,0},{0,1},{1,0},{1,1}});
            shapes.push_back({{0,0},{0,1},{1,0},{1,1}});
            shapes.push_back({{0,0},{0,1},{1,0},{1,1}});
            shapes.push_back({{0,0},{0,1},{1,0},{1,1}});
            break;
        case TetrominoType::T:
            shapes.push_back({{0,0},{0,1},{0,2},{1,1}});
            shapes.push_back({{0,1},{1,0},{1,1},{2,1}});
            shapes.push_back({{1,0},{1,1},{1,2},{0,1}});
            shapes.push_back({{0,0},{1,0},{2,0},{1,1}});
            break;
        case TetrominoType::S:
            shapes.push_back({{0,1},{0,2},{1,0},{1,1}});
            shapes.push_back({{0,0},{1,0},{1,1},{2,1}});
            shapes.push_back({{1,1},{1,2},{2,0},{2,1}});
            shapes.push_back({{0,0},{1,0},{1,1},{2,1}});
            break;
        case TetrominoType::Z:
            shapes.push_back({{0,0},{0,1},{1,1},{1,2}});
            shapes.push_back({{0,1},{1,0},{1,1},{2,0}});
            shapes.push_back({{0,0},{0,1},{1,1},{1,2}});
            shapes.push_back({{0,1},{1,0},{1,1},{2,0}});
            break;
        case TetrominoType::J:
            shapes.push_back({{0,0},{0,1},{0,2},{1,0}});
            shapes.push_back({{0,0},{1,0},{2,0},{2,1}});
            shapes.push_back({{1,2},{1,1},{1,0},{0,2}});
            shapes.push_back({{0,0},{0,1},{1,1},{2,1}});
            break;
        case TetrominoType::L:
            shapes.push_back({{0,0},{0,1},{0,2},{1,2}});
            shapes.push_back({{0,0},{1,0},{2,0},{0,1}});
            shapes.push_back({{1,0},{1,1},{1,2},{0,0}});
            shapes.push_back({{0,0},{1,0},{2,0},{2,-1}}); // 依實際需求微調
            break;
    }
}

void Tetromino::updateBlocks() 
{
    currentBlocks = shapes[rotationIndex];
}
