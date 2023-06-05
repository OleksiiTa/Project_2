#include "state.h"

void generate_map(STATE *s);

int is_wall(STATE *s, int x, int y);

void gerar(STATE *s);

void smooth_map(STATE *s, int first_pass, int second_pass);

void find_non_wall(STATE *s, int x, int y, int *nx, int *ny);

int radius_count(STATE *s, int x, int y, int radius);

int is_spot_empty(STATE *s, int x, int y);