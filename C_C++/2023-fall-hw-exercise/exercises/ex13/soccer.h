#ifndef SOCCER_H
#define SOCCER_H

#include <stdbool.h>

//TODO 1: DEFINE DATE, STAT AND PLAYER STRUCTS HERE
typedef struct {
    int num_of_goals;
    int num_of_assists;
    float pass_accuracy;
    int min_played;
    int num_of_shots;
    float shot_accuracy;
} Stat;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int age;
    int jersey_number;
    bool goalkeeper;
    Date *date;
    Stat *stat;
} Player;

void create_player (Player *p, bool goalie);
void create_team(Player team[], int size);
void print_team (Player team[], int size);

#endif
