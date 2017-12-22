#include"pac.h"
#include"main.h"

// block size = 20
// map size (20 * 28) blocks
// There is no 1 pixel move
extern block map[][28];
extern pos enemy[4];
extern int c_count;
extern pos pacman;

int mov_stat = Right_STAT;

static int pos_cmp(pos a, pos b)
{
	return (a.x == b.x) && (a.y == b.y);
}

int mov_check(){
    int if_dead;
    type overlap_state;
    pos pacman_backup = pacman;
    switch (mov_stat){
        case Stop_STAT:
            return 0;
        case Up_STAT:
            pacman.x += 1;
            break;
        case Down_STAT:
            pacman.x -= 1;
            break;
        case Left_STAT:
            pacman.y -= 1;
            break;
        case Right_STAT:
            pacman.y += 1;
            break;
    }

    if_dead = (pos_cmp(pacman,enemy[0]));
    if_dead = if_dead || (pos_cmp(pacman, enemy[1]));
    if_dead = if_dead || (pos_cmp(pacman, enemy[2]));
    if_dead = if_dead || (pos_cmp(pacman, enemy[3]));
    
    overlap_state = map[pacman.x][pacman.y].block_type;
    if(overlap_state == WALL){
        pacman = pacman_backup;
        return 1;
    }

    else if(overlap_state == COIN)
        c_count--;

    if(if_dead)
        return -1;

    return 0;
}
