#include "Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// 設定音效播放的最短間隔時間（秒）
#define SOUND_COOLDOWN 0.3

// 記錄上次播放音效的時間
std::chrono::steady_clock::time_point lastMoveSoundTime;
std::chrono::steady_clock::time_point lastRotateSoundTime;

Game::Game(): running(false), frameCount(0), framesPerDrop(30) {}

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

    audioManager.playMusic(level);

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

    // 停止 BGM
    audioManager.stopMusic();

    // 現在才停音效 => 讓程式在結束前將音效殺掉
    audioManager.stopSoundEffect();

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

    auto now = std::chrono::steady_clock::now();

    if (inputHandler.isMoveLeft()) 
    {
        currentTetromino.moveLeft();
        if (board.checkCollision(currentTetromino)) 
        {
            currentTetromino.moveRight();
        } 
        else 
        {
            if (std::chrono::duration<double>(now - lastMoveSoundTime).count() > SOUND_COOLDOWN) 
            {
                lastMoveSoundTime = now;
            }
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
            if (std::chrono::duration<double>(now - lastMoveSoundTime).count() > SOUND_COOLDOWN) 
            {
                lastMoveSoundTime = now;
            }
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
            if (std::chrono::duration<double>(now - lastRotateSoundTime).count() > SOUND_COOLDOWN) 
            {
                lastRotateSoundTime = now;
                audioManager.playRotateSound();
            }
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
            if (std::chrono::duration<double>(now - lastRotateSoundTime).count() > SOUND_COOLDOWN) 
            {
                lastRotateSoundTime = now;
                audioManager.playRotateSound();
            }
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
            if (std::chrono::duration<double>(now - lastMoveSoundTime).count() > SOUND_COOLDOWN) 
            {
                lastMoveSoundTime = now;
            }
        }
    }
}

void Game::update() {
    if (frameCount >= framesPerDrop) 
    {
        currentTetromino.moveDown();
        frameCount = 0;  

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

            if (level <= 10 && scoreManager.getScore() >= levelThresholds[level - 1]) 
            {
                nextLevel();
            }

            TetrominoType randomType = static_cast<TetrominoType>(std::rand() % 7);
            currentTetromino.reset(randomType);

            if (board.checkCollision(currentTetromino)) 
            {
                running = false;
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
        audioManager.stopMusic();
        return;
    }

    std::cout << "[Level Up] 進入關卡 " << level << "!\n";

    audioManager.stopMusic();  // 確保上一關的 BGM 停止
    countdownBeforeStart();
    audioManager.playMusic(level);  // 只會在新關卡時播放 BGM

#ifndef TEST_MODE
    if (framesPerDrop > 5) 
    {
        framesPerDrop -= 3;
    }
#endif
}
