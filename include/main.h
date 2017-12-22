#ifndef __MAIN_H
#define __MAIN_H

#define MAX_LIFE 3

typedef enum type_
{
  WALL,
  BACK,
  COIN
}type;

typedef struct pos_
{
  int x;
  int y;
}pos;

typedef struct block_
{
  type block_type;
}block;

typedef enum
{
	UNDEF,
	UP,
	DOWN,
	LEFT,
	RIGHT
}way;
#endif
