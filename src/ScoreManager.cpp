#include "ScoreManager.hpp"

ScoreManager::ScoreManager(): score(0) {}

ScoreManager::~ScoreManager() {}

void ScoreManager::addScore(int linesCleared) 
{
    // 一個簡單演算法：每消一行給 100 分
    score += linesCleared * 100;
}

int ScoreManager::getScore() const 
{
    return score;
}
