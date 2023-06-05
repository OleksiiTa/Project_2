#include "state.h"
#include "mapa.h"
#include "movimento.h"

void cast_light(STATE *s, float x, float y, float max_distance); 

void calculate_visible_cells(STATE *s, float max_distance, int rays);