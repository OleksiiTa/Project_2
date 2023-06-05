#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "movimento.h"

//Movimentação 

void moveAction (STATE *s, int dx, int dy) {
	int x= s->playerX + dx;
	int y= s->playerY + dy;
	//se for parede fica no mesmo sítio
	if (!(is_wall(s,x,y))) {
		s->playerX += dx;
		s->playerY += dy;
	}
}
