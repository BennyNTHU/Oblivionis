#ifndef AUDIOMANAGER
#define AUDIOMANAGER

#pragma once
#include <string>
#include <unordered_map>

class AudioManager 
{
    private:
        std::unordered_map<std::string, std::string> configMap; // 存放讀取的 config 設定
        std::string currentBGM; // 記錄當前播放的 BGM

        void loadConfig(); // 讀取 config.txt
        void playSoundEffect(const std::string& key); // 播放音效

    public:
        AudioManager();
        ~AudioManager();

        void playLineClearSound();
        void playMoveSound();
        void playRotateSound();
        
        void playMusic(int level);
        void stopMusic();
};


# endif