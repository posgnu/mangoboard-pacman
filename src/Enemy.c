#include "Enemy.h"
#include "main.h"
#include <stdlib.h>

#define Up_STAT 1
#define Down_STAT 2
#define Left_STAT 3
#define Right_STAT 4

extern pos pacman;
extern block map[][28];
extern pos enemy[4];

int enemy_stat[4] = {Up_STAT, Up_STAT, Up_STAT, Up_STAT};
int if_blocked[4] = {0, 0, 0 ,0};
int priot[4][4] = {{Up_STAT, Left_STAT, Down_STAT, Right_STAT},
                 {Down_STAT, Right_STAT, Up_STAT, Left_STAT},
                 {Up_STAT, Left_STAT, Down_STAT, Right_STAT},
                 {Down_STAT, Right_STAT, Up_STAT, Left_STAT}};

int enemy_move(){
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
        if (enemy[i].x == pacman.x && enemy[i].y == pacman.y)
            return -1;
        if (overlap_state.block_type == WALL){
            enemy[i] = enemy_backup;   
            if_blocked[i] = 1;
        }
                
    }
    return 0;
}

void enemy_stat_modify(){
    int i, rv;
    int temp;
    for (i = 0; i < 4; i++){
        if(if_blocked[i]){
            get_possible_direction(enemy[i], priot, i);
            if_blocked[i] = 0;
        }
        else{
            rv = rand() % 100;
            if(rv < 50){
                if (rv < 30){
                    temp = priot[i][0];
                    priot[i][0] = priot[i][3];
                    priot[i][3] = temp;
                }
                temp = priot[i][0]; 
                priot[i][0] = priot[i][2];
                priot[i][2] = temp;

                temp = priot[i][1];
                priot[i][1] = priot[i][3];
                priot[i][3] = temp;
                
                get_possible_direction(enemy[i], priot, i);
            }    
        }
    }
    return;
}

void enemy_stat_modify_scary(){
    int i, rv;
    int diffx, diffy;

    for (i = 0; i < 4; i++){
        diffx = pacman.x - enemy[i].x;
        diffy = pacman.y - enemy[i].y;

        if(diffy < 0){
             priot[i][3] = Left_STAT;
             priot[i][0] = Right_STAT;
        }
        else{
            priot[i][3] = Right_STAT;
            priot[i][0] = Left_STAT;
        } 

        if(diffx < 0){
            priot[i][3] = Up_STAT;
            priot[i][0] = Down_STAT;
        }
        else{
            priot[i][3] = Down_STAT;
            priot[i][0] = Up_STAT;
        }
           
    }
    for (i = 0; i < 4; i++){
        if(if_blocked[i]){
            get_possible_direction(enemy[i], priot, i);
            if_blocked[i] = 0;
        }
        else{
            rv = rand() % 100;
            if(rv < 50){
                get_possible_direction(enemy[i], priot, i);
            }    
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
