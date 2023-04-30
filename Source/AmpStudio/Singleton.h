#pragma once
#include <AudioEngine.h>

namespace AmpStudio {
    class Singleton {
    public:
        static Singleton& getInstance() {
            static Singleton instance;
            return instance;
        }

        inline AmpProcessing::AudioEngine* GetAudio() { return m_AudioEngine.get(); };
    private:
        Singleton() : m_AudioEngine(std::make_unique<AmpProcessing::AudioEngine>()) {}; // private constructor to prevent external instantiation
        Singleton(const Singleton&) = delete; // delete copy constructor to prevent copying
        Singleton& operator=(const Singleton&) = delete; // delete assignment operator to prevent assignment

    private:
        std::unique_ptr<AmpProcessing::AudioEngine> m_AudioEngine;
    };
}