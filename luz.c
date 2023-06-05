#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "luz.h"

void cast_light(STATE *s, float x, float y, float max_distance) {
    for (float distance = 0; distance < max_distance; distance += 0.1) {
        int i = (int)(s->playerX + y * distance);
        int j = (int)(s->playerY + x * distance);

        if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
            // if (s->distance[i][j]!=(-1)) {    
                s->visible[i][j] = true;
                if (s->map[i][j] == '#') {
                    break;
                }
            // }
        }
    }
}

void calculate_visible_cells(STATE *s, float max_distance, int rays) {
    // Clear the array with the cells we can see 
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            s->visible[i][j] = false;
        }
    }
    // Cast the rays to find the cells we can see
    for (int r = 0; r < rays; r++) {
        float angle = 2 * M_PI * r / rays;
        float x = cos(angle);
        float y = sin(angle);
        cast_light(s, x, y, max_distance);
    }
}