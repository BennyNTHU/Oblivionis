#include "Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// Game.cpp
Game::Game(): running(false), frameCount(0), framesPerDrop(30) {} // 假設 30 幀掉一次

Game::~Game() {}

void Game::init() 
{
    inputHandler.initTerminal();

    running = true;
    board = Board();
    currentTetromino = Tetromino();
    renderer = Renderer();
    scoreManager = ScoreManager();
    audioManager = AudioManager();

    level = 1;
    framesPerDrop = 30;

    audioManager.playMusic(level); // 播放第一關 BGM

    countdownBeforeStart();
    std::cout << "[Game] Initialized.\n";
}

void Game::countdownBeforeStart() 
{
    std::cout << "[Level " << level << "] 即將開始...\n";
    
    for (int i = 3; i > 0; --i) 
    {
        std::cout << "倒數 " << i << " 秒...\r" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));  // 延遲 1 秒
    }
    
    std::cout << "開始！        " << std::endl;  // 確保清除上一行
}

void Game::run() 
{
    // 主迴圈
    while (running) 
    {
        handleEvents();
        update();
        render();
    }

    cleanup();
}

void Game::cleanup() 
{
    inputHandler.restoreTerminal();
    audioManager.stopMusic(); // 停止背景音樂
    std::cout << "[Game] Cleanup and exit.\n";
}

void Game::handleEvents() 
{
    inputHandler.processInput();

    if (inputHandler.isQuit()) 
    {
        running = false;
        return;
    }

    if (inputHandler.isMoveLeft()) 
    {
        currentTetromino.moveLeft();
        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.moveRight();
        } 
        else 
        {
            audioManager.playMoveSound();
        }
    }

    if (inputHandler.isMoveRight()) 
    {
        currentTetromino.moveRight();
        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.moveLeft();
        } 
        else 
        {
            audioManager.playMoveSound();
        }
    }

    if (inputHandler.isRotateLeft()) 
    {
        currentTetromino.rotateLeft();
        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.rotateRight();
        } 
        else 
        {
            audioManager.playRotateSound();
        }
    }

    if (inputHandler.isRotateRight()) 
    {
        currentTetromino.rotateRight();
        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.rotateLeft();
        } 
        else 
        {
            audioManager.playRotateSound();
        }
    }

    if (inputHandler.isMoveDown()) 
    {
        currentTetromino.moveDown();
        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.moveUp(); 
        } 
        else 
        {
            audioManager.playMoveSound();
        }
    }
}

void Game::update() {
    // 只在累積到指定的幀數時才 moveDown
    if (frameCount >= framesPerDrop) 
    {
        currentTetromino.moveDown();
        frameCount = 0;  // 重置計數器

        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.moveUp();
            board.placeTetromino(currentTetromino);

            int linesCleared = board.clearLines();
            if (linesCleared > 0) 
            {
                scoreManager.addScore(linesCleared);
                audioManager.playLineClearSound();
            }

            // **檢查是否達到當前關卡門檻**
            if (level <= 10 && scoreManager.getScore() >= levelThresholds[level - 1]) 
            {
                nextLevel(); // 進入下一關
            }

            // 生成新方塊
            TetrominoType randomType = static_cast<TetrominoType>(std::rand() % 7);
            currentTetromino.reset(randomType);

            if (board.checkCollision(currentTetromino)) 
            {
                running = false; // Game Over
            }
        }
    } 
    else 
    {
        frameCount++;
    }
}

void Game::render() 
{
    if (level > 10) 
    {
        std::cout << "\n[Game Over] 你已完成所有關卡！感謝遊玩！\n";
        return;
    }

    renderer.draw(board, currentTetromino, scoreManager, level);
}

void Game::nextLevel() 
{
    level++;

    if (level > 10) 
    {
        std::cout << "[Game Over] 你已完成所有關卡！\n";
        running = false;
        audioManager.stopMusic(); // 停止 BGM
        return;
    }

    std::cout << "[Level Up] 進入關卡 " << level << "!\n";
    
    audioManager.playMusic(level); // 切換 BGM

    countdownBeforeStart();

#ifndef TEST_MODE
    if (framesPerDrop > 5) 
    {
        framesPerDrop -= 3;
    }
#endif
}
