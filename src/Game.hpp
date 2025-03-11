#ifndef GAME
#define GAME

#pragma once

#include "Board.hpp"
#include "Tetromino.hpp"
#include "InputHandler.hpp"
#include "Renderer.hpp"
#include "ScoreManager.hpp"
#include "AudioManager.hpp"

class Game
{
    private:
        int frameCount;          // 計數器
        int framesPerDrop;       // 多少「幀」執行一次 moveDown
        bool running;

        int level; // 當前關卡

        #ifdef TEST_MODE
        int levelThresholds[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100}; // 測試模式：每 100 分升級
        #else
        int levelThresholds[10] = {1000, 2500, 5000, 8000, 12000, 16000, 20000, 25000, 30000, 40000}; // 正式模式
        #endif

        Board board;
        Tetromino currentTetromino;
        InputHandler inputHandler;
        Renderer renderer;
        ScoreManager scoreManager;
        AudioManager audioManager;

        // 處理輸入事件
        void handleEvents();

        // 更新遊戲邏輯
        void update();

        // 繪製畫面
        void render();

        // 進入下一關
        void nextLevel(); 

        // 每一關開始前倒數三秒
        void countdownBeforeStart();

    public:
        Game();
        ~Game();

        // 初始化遊戲（資源、變數、物件）
        void init();
        
        // 進入主迴圈
        void run();

        // 遊戲結束後釋放資源
        void cleanup();
};

#endif