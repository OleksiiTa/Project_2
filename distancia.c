#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "distancia.h"

void init_distance(STATE *st) {
	for (int i=0;i<MAP_HEIGHT;i++) {
		for (int j= 0;j<MAP_WIDTH;j++) {
			st->distance[i][j]=(-1);			
		}
	}
}

//creates a matrix map with the distance from player of a certain coord
void distanceFromPlayer (STATE *s, int dx, int dy, int n) {
	int x= s->playerX + dx;
	int y= s->playerY + dy;

	if (n>MAX_DIST) return;
	if (is_wall(s,x,y)) {
		s->distance[x][y]=(-2);
		return;
	}
	if (n>=s->distance[x][y] && s->distance[x][y]>0) return;
        s->distance[x][y]=n;

        distanceFromPlayer (s, dx+1, dy, n+1);
        distanceFromPlayer (s, dx-1, dy, n+1);
        distanceFromPlayer (s, dx, dy+1, n+1);
        distanceFromPlayer (s, dx, dy-1, n+1);
}