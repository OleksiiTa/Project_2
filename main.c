#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

#include "state.h"
#include "mapa.h"
#include "movimento.h"
#include "distancia.h"
#include "luz.h"
#include "weapons.h"


// Update - inputs of creation of bullets and movements 
void update(STATE *st, int *last_move, int *shot_type) {
	int key = getch();
	if(key == ERR) return;

	switch(key) {
		case KEY_UP:
		case '8': 
			moveAction(st, -1, +0); 
			*last_move=8;
			break;
		case KEY_LEFT:
		case '4': 
			moveAction(st, +0, -1);
			*last_move=4;
			break;
		case KEY_RIGHT:
		case '6': 
			moveAction(st, +0, +1); 
			*last_move=6;
			break;
		case KEY_DOWN:
		case '2': 
			moveAction(st, +1, +0);
			*last_move=2;
			break;
		
		case 'z':
			*shot_type = 1;
			creationBullets (st, last_move, shot_type);
			break;
		case 'x':
			*shot_type = 2;
			creationBullets (st, last_move, shot_type);
			break;
		case 'c':
			*shot_type = 3;
			creationBullets (st, last_move, shot_type);
			break;

		case 'q': 
			endwin(); 
			exit(0); 
			break;
	}
}
/* *shotTypes:
1 = explosive 
2 = 3 shurikens (diagonal & front)
3 = fire

direction:
8 = cima
6 = direita
2 = baixo
4 = esquerda
7 = esq cima
9 = dir cima 
1 = esq baixo
3 = dir baixo

example -> cima <- 82 -> shuriken   
*/
void draw_bullets(STATE *s) {
	int i,j;	
	for(i = 0; i< MAP_HEIGHT; i++){
		for(j = 0; j<MAP_WIDTH; j++){
			//if visible and if not a wall draw
            if (s->visible[i][j]) {
			if (!(is_wall(s,i,j))) {
                switch(s->bullets[i][j]) {
					case 81:
						attron(COLOR_PAIR(COLOR_BLUE));
						mvaddch(i, j, '^');
						attroff(COLOR_PAIR(COLOR_BLUE));
						break;
					case 61:
						attron(COLOR_PAIR(COLOR_BLUE));
						mvaddch(i, j, '>');
						attroff(COLOR_PAIR(COLOR_BLUE));
						break;
					case 21:
						attron(COLOR_PAIR(COLOR_BLUE));
						mvaddch(i, j, 'v');
						attroff(COLOR_PAIR(COLOR_BLUE));
						break;
					case 41:
						attron(COLOR_PAIR(COLOR_BLUE));
						mvaddch(i, j, '<');
						attroff(COLOR_PAIR(COLOR_BLUE));
						break;
					case 82:
					case 92:
					case 72:
					case 42:
					case 12:
					case 22:
					case 32:
					case 62:
						attron(COLOR_PAIR(COLOR_BLUE));
						mvaddch(i, j, '*');
						attroff(COLOR_PAIR(COLOR_BLUE));
						break;
					case 83:
					case 43:
					case 63:
					case 23:
						attron(COLOR_PAIR(COLOR_BLUE));
						mvaddch(i, j, 'w');
						attroff(COLOR_PAIR(COLOR_BLUE));
						break;
				}
            }
			}
		}
	}
}

//Map 
void draw_map(STATE *s){
	int i, j;
	for(i = 0; i< MAP_HEIGHT; i++){
    	for(j = 0; j<MAP_WIDTH; j++){
        	char c = s->visible[i][j] ? s->map[i][j] : ' ';
		 	if (s->map[i][j]=='#') {
				if (i<MAP_HEIGHT-1 && j<MAP_WIDTH-1 && j>1 && i>1) {
					attron(COLOR_PAIR(COLOR_GREEN));
					mvaddch(i, j, c);
					attroff(COLOR_PAIR(COLOR_GREEN));
				}
        		else {
					attron(COLOR_PAIR(COLOR_YELLOW));
					mvaddch(i, j, c);
					attroff(COLOR_PAIR(COLOR_YELLOW));
				}
			}
			else {
				attron(COLOR_PAIR(COLOR_WHITE));
				mvaddch(i, j, c);
				attroff(COLOR_PAIR(COLOR_WHITE));
			}
    	}
	}
}

//Player 
 void draw_player(STATE *s) {
	attron(COLOR_PAIR(COLOR_WHITE));
	mvaddch(s->playerX, s->playerY, '@' | A_BOLD);
	attroff(COLOR_PAIR(COLOR_WHITE));
}

//Distance<MAX_DIST 
void draw_distance (STATE *s) {
	for (int i=0;i<MAP_HEIGHT;i++) {
		for (int j= 0;j<MAP_WIDTH;j++) {
			//if distance[x][y]<0 its a wall or its a distance from player >20 
 			if (s->distance[i][j]<0) continue;
			char digit=((s->distance[i][j]) % 10) + '0';
 			attron(COLOR_PAIR(COLOR_BLUE));
 			mvaddch(i,j, digit);
 			attroff(COLOR_PAIR(COLOR_BLUE));
		}
	}
}

//Enemies
void draw_enemies(STATE *s) {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(s->enemies[i].x != -1 && s->enemies[i].y != -1 && s->visible[s->enemies[i].x][s->enemies[i].y]) {
            attron(COLOR_PAIR(COLOR_RED));
            mvaddch(s->enemies[i].x, s->enemies[i].y, 'E' | A_BOLD);
            attroff(COLOR_PAIR(COLOR_RED));
        }
    }
}

void spawn_enemies(STATE *s, int count) {
    srand(time(NULL)); // Seed the random number generator
	if(count > MAX_ENEMIES){
		count = MAX_ENEMIES;
	}
    int spawned = 0;
    while(spawned < count) {
        for(int i = 0; i < MAX_ENEMIES; i++) {
            // find an empty slot in the enemies array
            if(s->enemies[i].x == -1 && s->enemies[i].y == -1) {
                // randomly choose a starting point
                int x = (rand() % (MAP_HEIGHT - 3)) + 2;
                int y = (rand() % (MAP_WIDTH - 3)) + 2;
                
                // use find_non_wall to find a non-wall position around the randomly chosen point
                int nx, ny;
                find_non_wall(s, x, y, &nx, &ny);
                if(nx != -1 && ny != -1 && s->distance[x][y]==-1){
                    s->enemies[i].x = nx;
                    s->enemies[i].y = ny;
                    spawned++;
                    if(spawned >= count)
                        return;
                }
            }
        }
    }
}

void update_enemies(STATE *s) {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(s->enemies[i].x != -1 && s->enemies[i].y != -1) {
            // Check if the enemy is hit by a bullet
            if(s->bullets[s->enemies[i].x][s->enemies[i].y] != 0) {
                // If hit, remove the enemy
                s->enemies[i].x = -1;
                s->enemies[i].y = -1;
            }
        }
    }
}

void update_shoots_enemies(STATE *s) {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(s->enemies[i].x != -1 && s->enemies[i].y != -1) {
            if(s->visible[s->enemies[i].x][s->enemies[i].y]) {
                // If the enemy is visible, shoot at the player
                shoot_at_player(s, s->enemies[i].x, s->enemies[i].y);
            }
        }
    }
}

void update_player(STATE *s) {
    // Check if the player is hit by a bullet
    if(s->bullets[s->playerX][s->playerY] != 0) {
        // If hit, end the game
        endwin();
        printf("Game Over\n");
        exit(0);
    }
}


// time_t lastBulletUpdate = 0;
struct timeval lastBulletUpdate;
struct timeval lastBulletUpdate2;

int main() {

	WINDOW *wnd = initscr();
	int ncols, nrows;
	getmaxyx(wnd,nrows,ncols);
	
	STATE st = {
		.playerX = (MAP_HEIGHT/2),
		.playerY = (MAP_WIDTH/2),
		.distance = {{0}},
		.map = {{0}},
		.visible = {{0}},
		.bullets = {{0}}
	};

	for(int i = 0; i<MAX_ENEMIES; i++){
		st.enemies[i].x = -1;
		st.enemies[i].y = -1;
	}

	srand48(time(NULL));
	start_color();

	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, false);
	keypad(stdscr, true);
	nodelay(stdscr, TRUE);

	init_pair(COLOR_GREEN,COLOR_GREEN,COLOR_BLACK);
	init_pair(COLOR_RED,COLOR_RED,COLOR_BLACK);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);

	gerar(&st);

	init_bullets(&st); //init bullets matrix**
	int lastMove=6; //player starts looking right
	int shotType=0; 
	gettimeofday(&lastBulletUpdate, NULL);

	while(1) {
		calculate_visible_cells(&st, MAX_VIEW_DISTANCE, FOV);

		init_distance(&st);
		distanceFromPlayer(&st,0,0,0);
		
		draw_map(&st);
		//draw_distance(&st);
		draw_bullets(&st);
		draw_player(&st);
		draw_enemies(&st);

		struct timeval currentTime;
		gettimeofday(&currentTime, NULL);

		double timeDiff = (currentTime.tv_sec - lastBulletUpdate.tv_sec) + (currentTime.tv_usec - lastBulletUpdate.tv_usec)/1e6;
		double timeDiff2 = (currentTime.tv_sec - lastBulletUpdate2.tv_sec) + (currentTime.tv_usec - lastBulletUpdate2.tv_usec)/1e6;
		if(timeDiff2 >= 1.5){
			update_shoots_enemies(&st);
			// move_enemies(&st);
			lastBulletUpdate2 = currentTime;
		} 

		if(timeDiff >= 0.2){
			update_player(&st);
			update_enemies(&st);
			moveBullets(&st);
			// move_enemies(&st);
			lastBulletUpdate = currentTime;
		}
		// time_t currentTime = time(NULL);
		// if(difftime(currentTime, lastBulletUpdate) >= 0.3){
		// 	moveBullets (&st);
		// 	lastBulletUpdate = currentTime;
		// }



		move(MAP_HEIGHT, 0);
		attron(COLOR_PAIR(COLOR_BLUE));
		printw("(%d, %d) %d %d", st.playerX, st.playerY, ncols, nrows);
		attroff(COLOR_PAIR(COLOR_BLUE));
		move(st.playerX, st.playerY);
		update(&st, &lastMove, &shotType);


		bool all_enemies_dead = true;
		for(int i = 0; i<MAX_ENEMIES; i++){
			if(st.enemies[i].x != -1 || st.enemies[i].y != -1){
				all_enemies_dead = false;
				break;
			}
		}
		if(all_enemies_dead) {
			spawn_enemies(&st, MAX_ENEMIES);
		}
	}

	return 0;
}
