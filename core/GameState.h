//
// Created by SyperOlao on 27.09.2025.
//

#ifndef HAMMURABI_CLONE_GAMESTATE_H
#define HAMMURABI_CLONE_GAMESTATE_H


struct GameState {
    int population = 100;
    int wheat = 2800;
    int harvest_yield = 0; // урожайность с акра
    int destroyed_wheat = 0;
    int sow_wheat_land = 0;
    int land = 1000;
    int land_price = 0;
    int death_souls_price = 0;
    int years = 0;
    int deaths = 0;
    int death_from_starvation = 0;
    int immigrants = 0;
    int death_souls = 0;
    int death_for_the_last_round = 0;
    bool plague = false;
    bool inspector = false;


};


struct InputState {
    int land_for_buy = 0;
    int land_for_sell = 0;
    int wheat_for_food = 0;
    int wheat_for_sow = 0;
    int buy_death_souls = 0;
    bool give_bribe = false;
};


struct ResultGameStatistic {
    int average_death_percent = 0;
    int lend_for_person = 0;
};

#endif //HAMMURABI_CLONE_GAMESTATE_H
