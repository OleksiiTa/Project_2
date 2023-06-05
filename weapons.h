#include "state.h"
#include "mapa.h"
#include "movimento.h"

void init_bullets(STATE *s);
void creationBullets (STATE *s, int *lastMove, int *shotType);
void moveBullets (STATE *s);
void shoot_at_player(STATE *s, int enemy_x, int enemy_y);
