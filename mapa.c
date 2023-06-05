#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include "mapa.h"

void generate_map(STATE *s) {
    int i, j;

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            if (i < 2 || j < 2 || i >= MAP_HEIGHT - 2 || j >= MAP_WIDTH - 2) {
                s->map[i][j] = '#';
            } else if (rand() % 100 < WALL_PERCENT) {
                s->map[i][j] = '#';
            } else {
                s->map[i][j] = '.';
            }
        }
    }
}

int is_wall(STATE *s, int x, int y) {
    return (s->map[x][y] == '#');
}

int radius_count(STATE *s, int x, int y, int radius) {
    int count = 0;
    int i, j;

    for (i = -radius; i <= radius; i++) {
        for (j = -radius; j <= radius; j++) {
            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < MAP_HEIGHT && ny >= 0 && ny < MAP_WIDTH && is_wall(s, nx, ny)) {
                count++;
            }
        }
    }

    return count;
}

void smooth_map(STATE *s, int first_pass, int second_pass) {
    int i, j, pass;

    for (pass = 0; pass < first_pass; pass++) {
        STATE temp = *s;

        for (i = 0; i < MAP_HEIGHT; i++) {
            for (j = 0; j < MAP_WIDTH; j++) {
                if (i < 2 || j < 2 || i >= MAP_HEIGHT - 2 || j >= MAP_WIDTH - 2) {
                    temp.map[i][j] = s->map[i][j]; // Keep the border cells unchanged
                } else {
                    int adj_walls_1 = radius_count(s, i, j, 1);
                    int adj_walls_2 = radius_count(s, i, j, 2);
                    temp.map[i][j] = (adj_walls_1 >= 5 || adj_walls_2 <= 2) ? '#' : '.';
                }
            }
        }

        *s = temp;
    }

    for (pass = 0; pass < second_pass; pass++) {
        STATE temp = *s;

        for (i = 0; i < MAP_HEIGHT; i++) {
            for (j = 0; j < MAP_WIDTH; j++) {
                if (i < 2 || j < 2 || i >= MAP_HEIGHT - 2 || j >= MAP_WIDTH - 2) {
                    temp.map[i][j] = s->map[i][j]; // Keep the border cells unchanged
                } else {
                    int adj_walls_1 = radius_count(s, i, j, 1);
                    temp.map[i][j] = (adj_walls_1 >= 5) ? '#' : '.';
                }
            }
        }

        *s = temp;
    }
}

void gerar(STATE *s) {


    generate_map(s);
    smooth_map(s, 4, 3); // 4 passes of the first algorithm and 3 passes of the second algorithm
}

int is_spot_empty(STATE *s, int x, int y) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (s->enemies[i].x == x && s->enemies[i].y == y) {
            return 0;  // Spot is not empty
        }
    }
    return 1;  // Spot is empty
}

void find_non_wall(STATE *s, int x, int y, int *nx, int *ny) {
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    int visited[MAP_HEIGHT][MAP_WIDTH] = {0}; 

    typedef struct{
        int x, y;
    } Point;
    Point queue[MAP_HEIGHT * MAP_WIDTH];
    int front = 0, rear = 0;

    // mark the source cell as visited and enqueue it
    visited[x][y] = 1;
    queue[rear++] = (Point) {x, y};

    while (front < rear) {
        Point p = queue[front++];

        // check all four directions around the point
        for (int i = 0; i < 4; i++) {
            int next_x = p.x + dx[i];
            int next_y = p.y + dy[i];

            // check if the new point is inside the map and hasn't been visited yet
            if (next_x >= 0 && next_x < MAP_HEIGHT && next_y >= 0 && next_y < MAP_WIDTH && !visited[next_x][next_y]) {
                visited[next_x][next_y] = 1;

                // if we find a non-wall cell and spot is empty we return it
                if (s->map[next_x][next_y] != '#' && is_spot_empty(s, next_x, next_y)) {
                    *nx = next_x;
                    *ny = next_y;
                    return;
                }

                queue[rear++] = (Point) {next_x, next_y};
            }
        }
    }

    // if no non-wall cell is found set nx and ny to -1
    *nx = -1;
    *ny = -1;
}