#include "AudioManager.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
#endif

std::mutex soundMutex;
std::queue<std::string> soundQueue;
std::condition_variable soundCV;
std::thread soundThread;
std::atomic<bool> isRunning(true);
bool isPlayingSound = false;

AudioManager::AudioManager() 
{
    loadConfig();
    lastSoundTime.clear();

    // 只啟動一次執行緒
    if (!soundThread.joinable()) 
    {
        soundThread = std::thread(&AudioManager::processSoundQueue, this);
    }
}

AudioManager::~AudioManager() 
{
    isRunning = false;
    soundCV.notify_all();

    if (soundThread.joinable()) 
    {
        soundThread.join(); 
    }

    // 移除 stopSoundEffect() 的呼叫，避免不必要地 kill aplay
    // stopSoundEffect();
}

void AudioManager::loadConfig() 
{
    std::ifstream configFile("./src/config.txt");
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

    auto now = std::chrono::steady_clock::now();
    if (lastSoundTime.find(key) != lastSoundTime.end()) 
    {
        double elapsed = std::chrono::duration<double>(now - lastSoundTime[key]).count();
        if (elapsed < 0.5) return;  // **冷卻時間 0.1 秒，防止過度疊加**
    }
    lastSoundTime[key] = now;

    std::string filePath = configMap[key];
    std::cout << "[Audio] 播放音效: " << filePath << "\n";

#ifdef _WIN32
    PlaySound(TEXT(filePath.c_str()), NULL, SND_FILENAME | SND_ASYNC);
#else
    std::thread([filePath]() {
        std::string command = "aplay " + filePath;
        system(command.c_str());
    }).detach();
#endif
}

void AudioManager::processSoundQueue() 
{
    while (isRunning) 
    {
        std::unique_lock<std::mutex> lock(soundMutex);
        soundCV.wait(lock, []{ return !soundQueue.empty() || !isRunning; });

        if (!isRunning) break;

        if (!soundQueue.empty()) 
        {
            isPlayingSound = true;
            auto soundFile = soundQueue.front();
            soundQueue.pop();
            lock.unlock();

#ifdef _WIN32
            // Windows 用 PlaySound
            PlaySound(TEXT(soundFile.c_str()), NULL, SND_FILENAME | SND_SYNC);
#else
            // Linux/macOS 用 aplay
            std::string command = "aplay " + soundFile;
            int ret = system(command.c_str());
#endif
            isPlayingSound = false;
        }
    }
}

// 停音效 => 可以只在程式結束時呼叫, 避免遊戲時期殺死 aplay
void AudioManager::stopSoundEffect() 
{
#ifndef _WIN32
    system("pkill -9 aplay");
#endif
}

// ---- 其它函式 (playLineClearSound, playMoveSound...) 保留 ----

void AudioManager::playLineClearSound() 
{
    playSoundEffect("SOUND_LINE_CLEAR");
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

    // **如果 BGM 未變更，則不重新播放**
    if (bgmFile == currentBGM) 
    {
        return;
    }

    stopMusic();  // **確保舊 BGM 先停止**
    currentBGM = bgmFile;

    std::cout << "[Audio] 播放 BGM: " << bgmFile << "\n";

#ifdef _WIN32
    PlaySound(TEXT(bgmFile.c_str()), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    std::string command = "nohup mpg123 --loop -1 -q " + bgmFile + " &";
    system(command.c_str());
#endif
}

void AudioManager::stopMusic() 
{
    std::cout << "[Audio] 停止 BGM.\n";
#ifdef _WIN32
    PlaySound(NULL, NULL, 0);
#else
    system("pkill -9 mpg123");  // **強制停止所有 mpg123 進程**
#endif
}

std::string AudioManager::getCurrentBGM() 
{
    return currentBGM;
}
