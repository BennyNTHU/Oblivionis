#ifndef AUDIOMANAGER
#define AUDIOMANAGER

#pragma once
#include <string>
#include <unordered_map>
#include <chrono> // **新增 chrono**

class AudioManager 
{
    private:
        std::unordered_map<std::string, std::string> configMap; // 存放讀取的 config 設定
        std::string currentBGM; // 記錄當前播放的 BGM

        void loadConfig(); // 讀取 config.txt
        
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastSoundTime; // 避免playMoveSound()在極短時間內被多次執行

    public:
        AudioManager();
        ~AudioManager();

        void playLineClearSound();
        void playRotateSound();
        void playSoundEffect(const std::string& key); // 播放音效
        void playMusic(int level);
        void stopMusic();
        void stopSoundEffect();
        void processSoundQueue();
        std::string getCurrentBGM(); // **新增函數來獲取當前 BGM**
};


# endif