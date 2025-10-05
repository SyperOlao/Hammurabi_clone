//
// Created by SyperOlao on 27.09.2025.
//

#ifndef HAMMURABI_CLONE_CONSTANTS_H
#define HAMMURABI_CLONE_CONSTANTS_H


namespace GameConsts {
    static constexpr int kMinPrice = 17;
    static constexpr int kMaxPrice = 26;
    static constexpr int kLandMaxProcess = 10;
    static constexpr int kMaxGame = 10;
    static constexpr int kConsumptionOfWheat = 20;
    static constexpr float kRatsLossMin = 0.0f;
    static constexpr float kRatsLossMax = 0.07f;
    static constexpr int kWheatMinProcess = 1;
    static constexpr int kWheatMaxProcess = 6;
    static constexpr int kLooseDeathPopulation = 45;
    static constexpr int kImmigrationDivider = 600;
    static constexpr int kMaxImmigrantAdd = 50;
    static constexpr int kPlagueChanceMax = 15;
    static constexpr int kPlagueDeathPercent = 50;
    static constexpr int kWheatConsumptionForLand = 2;
};

namespace GameSounds {
    static constexpr auto kStartGame = L"assets\\sound_start_pcm.wav";
    static constexpr auto kNextRound = L"assets\\sound_next_pcm.wav";
    static constexpr auto kGameLoose = L"assets\\sound_loose_pcm.wav";
    static constexpr auto kGameWin = L"assets\\sound_win_pcm.wav";
    static constexpr auto kPlague = L"assets\\sound_chuma_pcm.wav";
}

enum GameMarkResults {
    Fail,
    Satisfactorily,
    Good,
    Excellent,
};
#endif //HAMMURABI_CLONE_CONSTANTS_H
