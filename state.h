#ifndef ___STATE_H___
#define ___STATE_H___
/**
 *
 * Falta muita coisa, incluindo por exemplo:
 * - o mapa
 * - os monstros
 * - a fila de prioridade para saber quem se move
 * - o que está em cada casa
 *
 */
#define MAP_WIDTH 203 
#define MAP_HEIGHT 48 
#define WALL_PERCENT 40

#define MAX_DIST 15

#define FOV 360
#define RAY_ANGLE_INCREMENT 1
#define MAX_VIEW_DISTANCE 20
#define M_PI 3.14159265358979323846

#define FPS 60
#define BULLET_UPDATE_INTERVAL 1

#define MAX_ENEMIES 15 

typedef struct {
	int x, y;
} ENEMY;


typedef struct state {
	int playerX;
	int playerY;
	int distance[MAP_HEIGHT][MAP_WIDTH];
	char map[MAP_HEIGHT][MAP_WIDTH];
	int visible[MAP_HEIGHT][MAP_WIDTH];
	int bullets[MAP_HEIGHT][MAP_WIDTH];
	ENEMY enemies[MAX_ENEMIES];
} STATE;

#endif
