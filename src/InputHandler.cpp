#include "InputHandler.hpp"
#include <iostream>
#include <unistd.h>   // for read(), STDIN_FILENO
#include <termios.h>  // for struct termios, tcgetattr, tcsetattr
#include <fcntl.h>    // for fcntl, O_NONBLOCK
#include <errno.h>    // for EAGAIN, EWOULDBLOCK
#include <cstdio>     // for perror()

InputHandler::InputHandler()
: moveLeft(false),
  moveRight(false),
  rotateLeft(false),
  rotateRight(false),
  moveDown(false),
  quit(false),
  origFlags(-1)
{}

InputHandler::~InputHandler() 
{
    // 確保在物件被銷毀時，自動恢復終端設定
    restoreTerminal();
}

void InputHandler::initTerminal() 
{
    // 取得原先終端機設定
    if (tcgetattr(STDIN_FILENO, &origTermios) == -1) 
    {
        perror("tcgetattr");
        return;
    }

    // 設定新的模式
    struct termios newTermios = origTermios;

    // 關閉 canonical 模式與 echo
    newTermios.c_lflag &= ~(ICANON | ECHO);
    // 關閉輸入時的延遲 (VMIN, VTIME)
    newTermios.c_cc[VMIN]  = 0; 
    newTermios.c_cc[VTIME] = 0;

    // 套用新的設定
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newTermios) == -1) 
    {
        perror("tcsetattr");
        return;
    }

    // 取得原先 flags
    origFlags = fcntl(STDIN_FILENO, F_GETFL);
    if (origFlags == -1) 
    {
        perror("fcntl(F_GETFL)");
        return;
    }

    // 設為非阻塞
    if (fcntl(STDIN_FILENO, F_SETFL, origFlags | O_NONBLOCK) == -1) 
    {
        perror("fcntl(F_SETFL, O_NONBLOCK)");
        return;
    }
}

void InputHandler::restoreTerminal() 
{
    // 恢復原先的 flags
    if (origFlags != -1) {
        fcntl(STDIN_FILENO, F_SETFL, origFlags);
        origFlags = -1;
    }
    // 恢復原先的 termios 設定
    tcsetattr(STDIN_FILENO, TCSANOW, &origTermios);
}

void InputHandler::processInput() 
{
    // 重置所有動作狀態
    moveLeft = false;
    moveRight = false;
    rotateLeft = false;
    rotateRight = false;
    moveDown = false;
    quit = false;

    // 利用非阻塞 read() 讀取所有可用字元
    char buffer[16];
    int n = 0;

    // 連續讀取直到沒有資料可讀
    while (true) 
    {
        n = read(STDIN_FILENO, buffer, sizeof(buffer));

        if (n > 0) 
        {
            // 解析這些字元
            for (int i = 0; i < n; ++i) 
            {
                char c = buffer[i];

                if (c == '\x1B') 
                {
                    // 可能是 ESC or 方向鍵
                    // 檢查後面兩個字元是否為 '[' 與 A/B/C/D
                    // 這需要確保 buffer 中還有足夠字元可讀
                    if ((i + 2) < n && buffer[i+1] == '[') 
                    {
                        char arrow = buffer[i+2];
                        switch (arrow) 
                        {
                            case 'A': // Up arrow
                                rotateRight = true;
                                break;
                            case 'B': // Down arrow
                                moveDown = true;
                                break;
                            case 'C': // Right arrow
                                moveRight = true;
                                break;
                            case 'D': // Left arrow
                                moveLeft = true;
                                break;
                            default:
                                break;
                        }
                        // 跳過後面兩個字元
                        i += 2;
                    } 
                    else 
                    {
                        // 單純 ESC (27) 沒有後續，可能要退出或不處理
                        quit = true;
                    }
                }
                else 
                {
                    // 一般字元
                    switch (c) 
                    {
                        case 'a':
                            moveLeft = true;
                            break;
                        case 'd':
                            moveRight = true;
                            break;
                        case 'q':
                            rotateLeft = true;
                            break;
                        case 'e':
                            rotateRight = true;
                            break;
                        case 's':
                            moveDown = true;
                            break;
                        case 'x':
                            quit = true;
                            break;
                        default:
                            // 其他按鍵不處理
                            break;
                    }
                }
            }
        }
        else if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) 
        {
            // 沒有更多可讀資料，非阻塞狀態下直接跳出
            break;
        }
        else 
        {
            // 其他錯誤，或 EOF
            // 有時候 Ctrl+D (EOF) 也會進到這裡，可以視情況做處理
            break;
        }
    }
}

bool InputHandler::isMoveLeft() const 
{
    return moveLeft;
}

bool InputHandler::isMoveRight() const 
{
    return moveRight;
}

bool InputHandler::isRotateLeft() const 
{
    return rotateLeft;
}

bool InputHandler::isRotateRight() const 
{
    return rotateRight;
}

bool InputHandler::isMoveDown() const 
{
    return moveDown;
}

bool InputHandler::isQuit() const 
{
    return quit;
}
