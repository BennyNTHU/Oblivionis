#ifndef INPUTHANDLER
#define INPUTHANDLER

#include <termios.h>  // 需要包含這個，才用得到 struct termios

#pragma once

class InputHandler 
{
    private:
        bool moveLeft;
        bool moveRight;
        bool rotateLeft;
        bool rotateRight;
        bool moveDown;
        bool quit;

        // 用來保存原先的 termios 設定，方便離開遊戲時恢復
        int origFlags;
        struct termios origTermios;

    public:
        InputHandler();
        ~InputHandler();

        // 切換到原始模式(非阻塞)
        void initTerminal();
        // 恢復終端模式
        void restoreTerminal();

        // 主要用來偵測按鍵狀態
        void processInput();

        bool isMoveLeft() const;
        bool isMoveRight() const;
        bool isRotateLeft() const;
        bool isRotateRight() const;
        bool isMoveDown() const;
        bool isQuit() const;
};

#endif