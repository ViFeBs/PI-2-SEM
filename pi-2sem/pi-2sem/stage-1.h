#ifndef H_STAGE_1
#define H_STAGE_1

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include <string.h>
#include "Structs.h"

void fase_1(Inimigo* inimigos[], ALLEGRO_FONT* font, bool* condicao, Player p1, ALLEGRO_BITMAP* image, bool* inventarioCheio, int lifebar, ALLEGRO_BITMAP* spriteInimigos,Porta* porta);

#endif