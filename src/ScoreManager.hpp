#ifndef SCOREMANAGER
#define SCOREMANAGER

#pragma once

class ScoreManager 
{
    private:
        int score;

    public:
        ScoreManager();
        ~ScoreManager();

        // 依照消除行數增加分數
        void addScore(int linesCleared);

        // 取得目前分數
        int getScore() const;
};

#endif