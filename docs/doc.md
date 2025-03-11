# Oblivionis 技術文件

## **1. 專案概述**
這是一款 **終端版俄羅斯方塊 (Tetris)**，使用 C++ 開發，並支援：
- **動態難度調整**（共 10 關，每關 BGM 不同）
- **鍵盤操作**
- **音效與 BGM 支援**
- **終端繪製**
- **遊戲結束條件：方塊堆滿或完成 10 關**

---

## **2. 環境安裝與編譯**
### **(1) 安裝所需工具**
#### **Linux/macOS**
```bash
sudo apt update && sudo apt install g++ make mpg123 aplay -y  # Ubuntu/Debian
sudo pacman -S g++ make mpg123 alsa-utils  # Arch Linux
```

#### **Windows**
- **安裝 MinGW-w64**
- **安裝 `mpg123.exe`**（可下載並放入 `C:\Windows\System32`）

---

### **(2) 編譯**
#### **正式模式**
```bash
g++ -std=c++11 main.cpp Game.cpp Board.cpp Tetromino.cpp InputHandler.cpp Renderer.cpp ScoreManager.cpp AudioManager.cpp -o tetris
```

#### **測試模式**
（關卡通過條件降為 100 分）
```bash
g++ -std=c++11 -DTEST_MODE main.cpp Game.cpp Board.cpp Tetromino.cpp InputHandler.cpp Renderer.cpp ScoreManager.cpp AudioManager.cpp -o tetris_test
```

---

### **(3) 執行**
```bash
./tetris
```

---

## **3. 程式架構**
```
├── main.cpp
├── Game.cpp / Game.hpp
├── Board.cpp / Board.hpp
├── Tetromino.cpp / Tetromino.hpp
├── InputHandler.cpp / InputHandler.hpp
├── Renderer.cpp / Renderer.hpp
├── ScoreManager.cpp / ScoreManager.hpp
├── AudioManager.cpp / AudioManager.hpp
├── config.txt
```

---

## **4. 模組詳細說明**
### **(1) `Game` (遊戲管理)**
- **負責主迴圈 (`run()`)、關卡管理 (`nextLevel()`)、遊戲初始化 (`init()`)**
- **控制遊戲結束條件**
- **進入新關卡時自動播放對應 `BGM`**
- **使用 `countdownBeforeStart()` 進行倒數開始**

**主要函式**
```cpp
void init();  // 初始化遊戲
void run();   // 遊戲主迴圈
void cleanup();  // 釋放資源
void handleEvents();  // 處理玩家輸入
void update();  // 更新遊戲狀態
void render();  // 繪製畫面
void nextLevel();  // 進入下一關
void countdownBeforeStart();  // 倒數 3 秒開始
```

---

### **(2) `Board` (遊戲棋盤)**
- **維護 10x20 棋盤**
- **檢查方塊碰撞 (`checkCollision()`)**
- **消除方塊 (`clearLines()`)**
- **存放落地方塊 (`placeTetromino()`)**

**主要函式**
```cpp
bool checkCollision(const Tetromino& tetromino) const;
void placeTetromino(const Tetromino& tetromino);
int clearLines();
const std::vector<std::vector<int>>& getGrid() const;
```

---

### **(3) `Tetromino` (俄羅斯方塊)**
- **控制方塊的旋轉、移動**
- **使用 `getBlocks()` 獲取當前形狀**
- **支援 I, O, T, S, Z, J, L 七種形狀**

**主要函式**
```cpp
void moveLeft();
void moveRight();
void moveDown();
void moveUp();
void rotateLeft();
void rotateRight();
std::pair<int,int> getPosition() const;
std::vector<std::pair<int,int>> getBlocks() const;
```

---

### **(4) `InputHandler` (鍵盤輸入)**
- **非阻塞讀取鍵盤 (`processInput()`)**
- **支援方向鍵 / `a,d,s,q,e,x`**
- **使用 `termios` 在 Linux/macOS 讀取鍵盤**

**主要函式**
```cpp
void processInput();
bool isMoveLeft() const;
bool isMoveRight() const;
bool isRotateLeft() const;
bool isRotateRight() const;
bool isMoveDown() const;
bool isQuit() const;
```

---

### **(5) `Renderer` (畫面繪製)**
- **在終端顯示遊戲畫面**
- **使用 ANSI 轉義碼顯示不同顏色的方塊**
- **每行方塊使用 `[]` 繪製**

**主要函式**
```cpp
void draw(const Board& board, const Tetromino& tetromino, const ScoreManager& scoreManager, int level);
```

---

### **(6) `ScoreManager` (計分)**
- **記錄目前分數**
- **每消除 1 行加 100 分**

**主要函式**
```cpp
void addScore(int linesCleared);
int getScore() const;
```

---

### **(7) `AudioManager` (音效 & BGM)**
- **讀取 `config.txt`**
- **播放對應 `BGM` 與 `音效`**
- **在 `nextLevel()` 切換背景音樂**
- **使用 `mpg123` (Linux/macOS) 或 `PlaySound()` (Windows)**

**主要函式**
```cpp
void playLineClearSound();
void playMoveSound();
void playRotateSound();
void playMusic(int level);
void stopMusic();
```

---

## **5. 設定檔 (`config.txt`)**
- **儲存音效與 BGM 的路徑**
- **可以隨時修改 `config.txt`，無需改動程式碼**

**範例**
```
BGM_1=./BGM/1.mp3
BGM_2=./BGM/2.mp3
BGM_3=./BGM/3.mp3
...
BGM_10=./BGM/10.mp3

SOUND_MOVE=./soundeffect/move.wav
SOUND_ROTATE=./soundeffect/rotate.wav
SOUND_LINE_CLEAR=./soundeffect/line_clear.wav
```

---

## **6. 遊戲操作方式**
| 按鍵       | 動作           |
|------------|--------------|
| `←` / `a` | 左移         |
| `→` / `d` | 右移         |
| `↓` / `s` | 快速下落     |
| `↑` / `e` | 旋轉 (右)    |
| `q`        | 旋轉 (左)    |
| `x`        | 退出遊戲     |

---
