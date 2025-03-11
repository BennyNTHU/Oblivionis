/* 
Compile command:
g++ -std=c++11 ./src/main.cpp\
    ./src/Game.cpp ./src/Board.cpp ./src/Tetromino.cpp ./src/InputHandler.cpp ./src/Renderer.cpp\
    ./src/ScoreManager.cpp ./src/AudioManager.cpp\
    -o oblivionis
    
test mode:
g++ -std=c++11 -DTEST_MODE ./src/main.cpp\
    ./src/Game.cpp ./src/Board.cpp ./src/Tetromino.cpp ./src/InputHandler.cpp ./src/Renderer.cpp\
    ./src/ScoreManager.cpp ./src/AudioManager.cpp\
    -o oblivionis
*/

#include "Game.hpp"
#include "AudioManager.hpp"

int main() 
{
    Game game;
    game.init();
    game.run();
    return 0;
}
