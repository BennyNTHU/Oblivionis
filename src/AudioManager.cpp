#include "AudioManager.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib> // for system()
#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
#endif

AudioManager::AudioManager() 
{
    loadConfig(); // 初始化時讀取 config.txt
}

AudioManager::~AudioManager() {}

void AudioManager::loadConfig() 
{
    std::ifstream configFile("config.txt");
    if (!configFile) 
    {
        std::cerr << "[Error] 無法讀取 config.txt\n";
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) 
    {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) 
        {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            configMap[key] = value;
        }
    }
    configFile.close();
}

void AudioManager::playSoundEffect(const std::string& key) 
{
    if (configMap.find(key) == configMap.end()) 
    {
        std::cerr << "[Error] 找不到音效設定: " << key << "\n";
        return;
    }

    std::string filePath = configMap[key];
    std::cout << "[Audio] 播放音效: " << filePath << "\n";

#ifdef _WIN32
    PlaySound(TEXT(filePath.c_str()), NULL, SND_FILENAME | SND_ASYNC);
#else
    std::string command = "aplay " + filePath + " &";
    system(command.c_str());
#endif
}
void AudioManager::playLineClearSound() 
{
    playSoundEffect("SOUND_LINE_CLEAR");
}

void AudioManager::playMoveSound() 
{
    playSoundEffect("SOUND_MOVE");
}

void AudioManager::playRotateSound() 
{
    playSoundEffect("SOUND_ROTATE");
}

void AudioManager::playMusic(int level) 
{
    std::string key = "BGM_" + std::to_string(level);
    if (configMap.find(key) == configMap.end()) 
    {
        std::cerr << "[Error] 找不到 BGM 設定: " << key << "\n";
        return;
    }

    std::string bgmFile = configMap[key];
    if (bgmFile == currentBGM) return; // 如果音樂未變更，不重新播放
    currentBGM = bgmFile;

    std::cout << "[Audio] 播放 BGM: " << bgmFile << "\n";
#ifdef _WIN32
    PlaySound(TEXT(bgmFile.c_str()), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    std::string command = "mpg123 -q " + bgmFile + " &";
    system(command.c_str());
#endif
}

void AudioManager::stopMusic() 
{
    std::cout << "[Audio] 停止 BGM.\n";
#ifdef _WIN32
    PlaySound(NULL, NULL, 0);
#else
    system("pkill mpg123");
#endif
}

