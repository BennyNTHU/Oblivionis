#include "Renderer.hpp"
#include <iostream>
#include <cstdlib>  // for system("clear") / system("cls")

static const char* COLOR_CODES[] = 
{
    "\033[0m",   // 0: RESET
    "\033[31m",  // 1: RED
    "\033[32m",  // 2: GREEN
    "\033[33m",  // 3: YELLOW
    "\033[34m",  // 4: BLUE
    "\033[35m",  // 5: MAGENTA
    "\033[36m",  // 6: CYAN
    "\033[37m",  // 7: WHITE
};

#define RESET "\033[0m"

// 將 color 限制在 1~7，超出以取模對應
inline const char* getColorCode(int color) 
{
    if (color <= 0) 
        return RESET; 
    int idx = color % 8; 

    if (idx == 0) 
        idx = 1;
    
    return COLOR_CODES[idx];
}

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::draw(const Board& board, const Tetromino& tetromino, const ScoreManager& scoreManager, int level)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // 你想要的水平縮排量（可自行調整）
    const int offset = 20;  

    // 取得棋盤狀態 (儲存顏色編號)
    auto grid = board.getGrid();
    // 暫時複製一份
    auto displayGrid = grid;

    // 疊加正在操作的方塊
    auto blocks = tetromino.getBlocks();
    auto pos = tetromino.getPosition();
    int activeColor = tetromino.getColor();

    for (auto &block : blocks) 
    {
        int row = pos.first + block.first;
        int col = pos.second + block.second;

        if (row >= 0 && row < Board::HEIGHT && col >= 0 && col < Board::WIDTH) 
        {
            displayGrid[row][col] = activeColor;
        }
    }

    // 顯示關卡
    const int boxWidth = 20; // 設定「Level」框的內部寬度

    // 計算 Level 佔的字元數
    std::string levelText = "Level: " + std::to_string(level);
    int levelTextLength = levelText.length();
    int padding = boxWidth - levelTextLength; // 算出剩餘空間
    int leftPadding = padding / 2;  // 左邊空格
    int rightPadding = padding - leftPadding;  // 右邊空格

    // 打印上框
    std::cout << std::string(offset, ' ') << "  +";
    std::cout << std::string(boxWidth, '-') << "+\n";

    // 打印 Level 內容，確保置中對齊
    std::cout << std::string(offset, ' ') << "  |" 
              << std::string(leftPadding, ' ') << levelText 
              << std::string(rightPadding, ' ') << "|\n";

    // 打印下框
    std::cout << std::string(offset, ' ') << "  +";
    std::cout << std::string(boxWidth, '-') << "+\n";

    // --------------------------
    // (1) 在遊戲盤面上方顯示分數，並用邊框框起來
    // --------------------------
    // 棋盤顯示寬度（不含左邊框兩空格和那個'|'），
    // 因為我們一格要印2字元，所以是 WIDTH * 2。
    // 這裡只示範簡單的做法，讓分數邊框跟棋盤寬度一樣。
    int boardContentWidth = Board::WIDTH * 2;

    // 印分數上邊框
    std::cout << std::string(offset, ' ') << "  +";
    for (int i = 0; i < boardContentWidth; ++i) 
    {
        std::cout << "-";
    }
    std::cout << "+\n";

    // 印分數內容 (此處簡單印「 Score: xxx 」，可以自行加 padding)
    // 若想更嚴謹，可計算字串長度，再補空白以對齊邊框長度。
    std::cout << std::string(offset, ' ') << "  | Score: " 
              << scoreManager.getScore();
    // 如果想要在分數後面補足空白對齊，可再計算:
    int used = 8 + std::to_string(scoreManager.getScore()).size(); // e.g. " Score: " + 分數長度
    int remain = boardContentWidth - used;
    for (int i = 0; i < remain; i++) std::cout << " ";
    std::cout << "|\n";

    // 印分數下邊框
    std::cout << std::string(offset, ' ') << "  +";
    for (int i = 0; i < boardContentWidth; ++i) 
    {
        std::cout << "-";
    }
    std::cout << "+\n";

    // --------------------------
    // (2) 開始印「遊戲盤面」
    // --------------------------
    // 上邊框 (跟原本的方式一樣, 只是加上 offset)
    std::cout << std::string(offset, ' ') << "  +";
    for (int c = 0; c < boardContentWidth; ++c) 
    {
        std::cout << "-";
    }
    std::cout << "+\n";

    // 顯示內容
    for (int r = 0; r < Board::HEIGHT; ++r) 
    {
        // 左邊框
        std::cout << std::string(offset, ' ') << "  |";
        for (int c = 0; c < Board::WIDTH; ++c) 
        {
            int cellColor = displayGrid[r][c];
            if (cellColor == 0) 
            {
                // 空白兩格
                std::cout << "  ";
            } 
            else 
            {
                // 以顏色代碼 + "[]" 來顯示
                std::cout << getColorCode(cellColor) << "██" << RESET;
            }
        }
        // 右邊框
        std::cout << "|\n";
    }

    // 下邊框
    std::cout << std::string(offset, ' ') << "  +";
    for (int c = 0; c < boardContentWidth; ++c) 
    {
        std::cout << "-";
    }
    std::cout << "+\n";

    // 控制提示 (不加入 offset)
    std::cout << "Controls: [Left/Right=Move], [Up=Rotate], [Down=Drop], [x=Exit]\n";
}
