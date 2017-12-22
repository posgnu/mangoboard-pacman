#include "Enemy.h"
#include "main.h"
#include <stdlib.h>

#define Up_STAT 1
#define Down_STAT 2
#define Left_STAT 3
#define Right_STAT 4

extern block map[][28];
extern pos enemy[4];

int enemy_stat[4] = {Up_STAT, Up_STAT, Up_STAT, Up_STAT};
int if_blocked[4] = {0, 0, 0 ,0};
int priot[4][4] = {{Up_STAT, Down_STAT, Left_STAT, Right_STAT},
                 {Down_STAT, Left_STAT, Up_STAT, Right_STAT},
                 {Right_STAT, Up_STAT, Left_STAT, Down_STAT},
                 {Left_STAT, Right_STAT, Down_STAT, Up_STAT}};

void enemy_move(){
    int i;
    pos enemy_backup;
    block overlap_state;

    for(i = 0; i < 4; i++){
        enemy_backup = enemy[i];
        switch (enemy_stat[i]){
            case Up_STAT:
                enemy[i].y++;
            case Down_STAT:
                enemy[i].y--;
            case Left_STAT:
                enemy[i].x--;
            case Right_STAT:
                enemy[i].x++;
        }
        overlap_state = map[enemy[i].x][enemy[i].y];
        if (overlap_state.block_type == WALL){
            enemy[i] = enemy_backup;   
            if_blocked[i] = 1;
        }
                
    }
}

void enemy_stat_modify(){
    int i, rv;
    for (i = 0; i < 4; i++){
        if(if_blocked[i]){
            get_possible_direction(enemy[i], priot[i], i);
        }
        else{
            rv = rand() % 100;
            if(rv < 30)
                get_possible_direction(enemy[i], priot[i], i);
        }
    }
    return;
}

void get_possible_direction(pos Current_pos, int priot_m[], int monster_num){
    int i;
    for(i = 0; i < 4; i++){
        switch (priot_m[i]){
            case Up_STAT:
                if (map[Current_pos.x][Current_pos.y + 1].block_type == BACK)
                    enemy_stat[i] = Up_STAT;
                break;
            case Down_STAT:
                if (map[Current_pos.x][Current_pos.y - 1].block_type == BACK)
                    enemy_stat[i] = Down_STAT;
                break;
            case Left_STAT:
                if (map[Current_pos.x - 1][Current_pos.y].block_type == BACK)
                    enemy_stat[i] = Left_STAT;
                break;
            case Right_STAT:
                if (map[Current_pos.x + 1][Current_pos.y].block_type == BACK)
                    enemy_stat[i] = Right_STAT;
                break;
        }
    }
    return;
}