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

void fase_1(Inimigo i1, Inimigo i2, Inimigo i3, Inimigo i4, Inimigo i5, ALLEGRO_FONT* font, ALLEGRO_COLOR black, ALLEGRO_COLOR blue, ALLEGRO_COLOR red, ALLEGRO_COLOR green, ALLEGRO_COLOR yellow, bool condicao, Player p1,ALLEGRO_BITMAP* image, bool inventarioCheio);

#endif