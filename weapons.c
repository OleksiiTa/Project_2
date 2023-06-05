#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "weapons.h"
/* *shotTypes:
1 = explosive 
2 = 3 bullets (diagonal & front)
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
*/
void init_bullets(STATE *s) {
	for (int i=0;i<MAP_HEIGHT;i++) {
		for (int j= 0;j<MAP_WIDTH;j++) {
			s->bullets[i][j]=(0);			
		}
	}
}

void creationBullets (STATE *s, int *lastMove, int *shotType) {
    //up
    if (*lastMove==8) {
        if (*shotType==1) s->bullets[s->playerX-1][s->playerY]=81;
        
        if (*shotType==2) {
            s->bullets[s->playerX-1][s->playerY]=82;
            s->bullets[s->playerX-1][s->playerY+1]=92;
            s->bullets[s->playerX-1][s->playerY-1]=72;
        }

        if (*shotType==3) {
            for(int i=0;i<3;i++) {
                    s->bullets[s->playerX-1][s->playerY-1+i]=83;
            }
        }
    }
    //down
    if (*lastMove==2) {
        if (*shotType==1) s->bullets[s->playerX+1][s->playerY]=21;

        if (*shotType==2) {
            s->bullets[s->playerX+1][s->playerY]=22;
            s->bullets[s->playerX+1][s->playerY+1]=32;
            s->bullets[s->playerX+1][s->playerY-1]=12;
        }

        if (*shotType==3) {
            for(int i=0;i<3;i++) {
                    s->bullets[s->playerX+1][s->playerY-1+i]=23;
            }
        }
    }
    //left
    if (*lastMove==4) {
        if (*shotType==1) s->bullets[s->playerX][s->playerY-1]=41;

        if (*shotType==2) {
            s->bullets[s->playerX][s->playerY-1]=42;
            s->bullets[s->playerX-1][s->playerY-1]=72;
            s->bullets[s->playerX+1][s->playerY-1]=12;
        }

        if (*shotType==3) {
            for(int i=0;i<3;i++) {
                    s->bullets[s->playerX-1+i][s->playerY-1]=43;
            }
        }
    }
    //Right
    if (*lastMove==6) {
        if (*shotType==1) s->bullets[s->playerX][s->playerY+1]=61;

        if (*shotType==2) {
            s->bullets[s->playerX][s->playerY+1]=62;
            s->bullets[s->playerX-1][s->playerY+1]=92;
            s->bullets[s->playerX+1][s->playerY+1]=32;
        }

        if (*shotType==3) {
            for(int i=0;i<3;i++) {
                    s->bullets[s->playerX-1+i][s->playerY+1]=63;
            }
        }
    }
}

void explosion (STATE *s,int x,int y, int n) {
    //if an wall is found, remove it
    if (is_wall(s, x, y) && n<3) {
        if (x>1 && y>1 && x<MAP_HEIGHT-1 && y<MAP_WIDTH-1) {
            s->map[x][y]='.';
            for(int i = 0; i < MAX_ENEMIES; i++) {
                if(s->enemies[i].x == x && s->enemies[i].y == y) {
                    // If an enemy is found, remove it
                    s->enemies[i].x = -1;
                    s->enemies[i].y = -1;
                    break; // exit the loop as we've found an enemy
                }
            }
            explosion (s, x-1,y,n+1);
            explosion (s,x+1,y,n+1);
            explosion (s,x,y+1,n+1);
            explosion (s,x, y-1, n+1);
        }
    }
}

void moveBullets (STATE *s) {
    int i, j;

	for(i = 0; i< MAP_HEIGHT; i++){
		for(j = 0; j<MAP_WIDTH; j++){
            //exception - if shot == explosive bullet (1) explodes walls  
            if (is_wall(s,i,j) && s->bullets[i][j]!=0) {
                if (s->bullets[i][j] % 10==1) explosion (s,i,j,0);
                s->bullets[i][j]=0;
            }
			else {
                switch(s->bullets[i][j]) {
                    case 81:
                    if (s->bullets[i-1][j]!=s->bullets[i][j] && s->bullets[i-1][j] !=0) s->bullets[i-1][j]=0; 
                    else s->bullets[i-1][j]=81;
                    s->bullets[i][j]=0;
                    break;
                    case 41:
                    
                    if (s->bullets[i-1][j]!=s->bullets[i][j] && s->bullets[i-1][j] !=0) s->bullets[i][j-1]=0; 
                    else s->bullets[i][j-1]=41;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 82:
                    if (s->bullets[i-1][j]!=s->bullets[i][j] && s->bullets[i-1][j] !=0) s->bullets[i-1][j]=0; 
                    else s->bullets[i-1][j]=82;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 92:
                    if (s->bullets[i-1][j+1]!=s->bullets[i][j] && s->bullets[i-1][j+1] !=0) s->bullets[i-1][j+1]=0; 
                    else s->bullets[i-1][j+1]=92;
                    s->bullets[i][j]=0;
                    break;

                    case 72:
                    if (s->bullets[i-1][j-1]!=s->bullets[i][j] && s->bullets[i-1][j-1] !=0) s->bullets[i-1][j-1]=0; 
                    else s->bullets[i-1][j-1]=72;
                    s->bullets[i][j]=0;
                    break;

                    case 42:
                    if (s->bullets[i][j-1]!=s->bullets[i][j] && s->bullets[i][j-1] !=0) s->bullets[i][j-1]=0; 
                    else s->bullets[i][j-1]=42;
                    s->bullets[i][j]=0;
                    break;

                    case 83:
                    if (s->bullets[i-1][j]!=s->bullets[i][j] && s->bullets[i-1][j] !=0) s->bullets[i-1][j]=0; 
                    else s->bullets[i-1][j]=83;
                    s->bullets[i][j]=0;
                    break;

                    case 43:
                    if (s->bullets[i][j-1]!=s->bullets[i][j] && s->bullets[i][j-1] !=0) s->bullets[i][j-1]=0; 
                    else s->bullets[i][j-1]=43;
                    s->bullets[i][j]=0;
                    break;
                }
            
            }
		}
    }
        
    for(i = MAP_HEIGHT; i>0; i--){
		for(j = MAP_WIDTH; j>0; j--){

                switch(s->bullets[i][j]) {
                    case 21:
                    if (s->bullets[i+1][j]!=s->bullets[i][j] && s->bullets[i+1][j] !=0) s->bullets[i+1][j]=0; 
                    else s->bullets[i+1][j]=21;
                    s->bullets[i][j]=0;
                    break;

                    case 61:
                    if (s->bullets[i][j+1]!=s->bullets[i][j] && s->bullets[i][j+1] !=0) s->bullets[i][j+1]=0; 
                    else s->bullets[i][j+1]=61;
                    s->bullets[i][j]=0;
                    break;

                    case 62:
                    if (s->bullets[i][j+1]!=s->bullets[i][j] && s->bullets[i][j+1] !=0) s->bullets[i][j+1]=0; 
                    else s->bullets[i][j+1]=62;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 32:
                    if (s->bullets[i+1][j+1]!=s->bullets[i][j] && s->bullets[i+1][j+1] !=0) s->bullets[i+1][j+1]=0; 
                    else s->bullets[i+1][j+1]=32;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 22:
                    if (s->bullets[i+1][j]!=s->bullets[i][j] && s->bullets[i+1][j] !=0) s->bullets[i+1][j]=0; 
                    else s->bullets[i+1][j]=22;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 12:
                    if (s->bullets[i+1][j-1]!=s->bullets[i][j] && s->bullets[i+1][j-1] !=0) s->bullets[i+1][j-1]=0; 
                    else s->bullets[i+1][j-1]=12;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 63:
                    if (s->bullets[i][j+1]!=s->bullets[i][j] && s->bullets[i][j+1] !=0) s->bullets[i][j+1]=0; 
                    else s->bullets[i][j+1]=63;
                    s->bullets[i][j]=0;
                    break;
                    
                    case 23:
                    if (s->bullets[i+1][j]!=s->bullets[i][j] && s->bullets[i+1][j] !=0) s->bullets[i+1][j]=0; 
                    else s->bullets[i+1][j]=23;
                    s->bullets[i][j]=0;
                    break;
                }
            
            
		}
	}
}


void shoot_at_player(STATE *s, int enemy_x, int enemy_y) {
    // Calculate direction from enemy to player
    int dx = s->playerX - enemy_x;
    int dy = s->playerY - enemy_y;

    // Normalize direction
    if (abs(dx) > abs(dy)) {
        dx /= abs(dx);
        if (dy != 0)
            dy /= abs(dy);
    } else {
        dy /= abs(dy);
        if (dx != 0)
            dx /= abs(dx);
    }

    // Set bullet direction based on calculated direction
    if (dx == 1 && dy == 0) { // bottom
        s->bullets[enemy_x+1][enemy_y]=22;
    } else if (dx == -1 && dy == 0) { // top
        s->bullets[enemy_x-1][enemy_y]=82;
    } else if (dx == 0 && dy == 1) { // right
        s->bullets[enemy_x][enemy_y+1]=62;
    } 
    else if (dx == 0 && dy == -1) { // left
        s->bullets[enemy_x][enemy_y-1]=42;
    } 
    else if (dx == 1 && dy == 1) { // right bottom 
        s->bullets[enemy_x+1][enemy_y+1]=32;
    } 
    else if (dx == 1 && dy == -1) { // left bottom 
        s->bullets[enemy_x+1][enemy_y-1]=12;
    } 
    else if (dx == -1 && dy == 1) { // right top
        s->bullets[enemy_x-1][enemy_y+1]=92;
    } 
    else if (dx == -1 && dy == -1) { // left top 
        s->bullets[enemy_x-1][enemy_y-1]=72;
    }
}