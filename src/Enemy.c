#include "Enemy.h"
#include "main.h"
#include <stdlib.h>

#define Up_STAT 1
#define Down_STAT 2
#define Left_STAT 3
#define Right_STAT 4

extern block map[][28];
extern pos enemy[4];

int enemy_stat[4] = {Left_STAT, Left_STAT, Left_STAT, Left_STAT};
int if_blocked[4] = {0, 0, 0 ,0};
int priot[4][4] = {{Up_STAT, Down_STAT, Right_STAT, Left_STAT},
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
                enemy[i].x--;
								break;
            case Down_STAT:
                enemy[i].x++;
								break;
            case Left_STAT:
                enemy[i].y--;
								break;
            case Right_STAT:
                enemy[i].y++;
								break;
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
            get_possible_direction(enemy[i], priot, i);
            if_blocked[i] = 0;
        }
        else{
            rv = rand() % 100;
            if(rv < 30)
                get_possible_direction(enemy[i], priot, i);
        }
    }
    return;
}

void get_possible_direction(pos Current_pos, int priot_m[][4], int monster_num){
    int i;
    for(i = 0; i < 4; i++){
        switch (priot_m[monster_num][i]){
            case Up_STAT:
                if (map[Current_pos.x - 1][Current_pos.y].block_type != WALL)
                    enemy_stat[monster_num] = Up_STAT;
                break;
            case Down_STAT:
                if (map[Current_pos.x + 1][Current_pos.y].block_type != WALL)
                    enemy_stat[monster_num] = Down_STAT;
                break;
            case Left_STAT:
                if (map[Current_pos.x][Current_pos.y - 1].block_type != WALL)
                    enemy_stat[monster_num] = Left_STAT;
                break;
            case Right_STAT:
                if (map[Current_pos.x][Current_pos.y + 1].block_type != WALL)
                    enemy_stat[monster_num] = Right_STAT;
                break;
        }
    }
    return;
}
