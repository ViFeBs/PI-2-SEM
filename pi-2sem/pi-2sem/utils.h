#ifndef H_UTILS
#define H_UTILS

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include <string.h>
#include "Structs.h"


float calcula_distancia(float dx, float dy);
void trajetoria_tiro(float y_cursor, float x_cursor, float y_bullet, float x_bullet, Bullet* b);
void mostraInimigos(char* arr);
void getInimigos(Inimigo inimigo, char* inimigosCapturados[], int* contador, bool* inventarioCheio);
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
bool verificaColisao(Inimigo* arr[], Inimigo i1, int qtd, int pos);
Inimigo moveInimigos(Inimigo i, Player p1, float v, Inimigo* arr, int qtd, int pos);
void exibeInventario(char* inimigos, ALLEGRO_BITMAP* alfabeto);
void verificaAtaque(Inimigo* arr[], Player* p1);
void resetaInimigos(Inimigo* i1, Inimigo* i2, Inimigo* i3, Inimigo* i4, Inimigo* i5);
void limpaInventario(ALLEGRO_BITMAP* alfabeto, char* inimigosCapturados[], bool* inventarioCheio);
void animaInimigos(Inimigo* arr[], ALLEGRO_BITMAP* alfabeto);
void reviveInimigos(char* inimigosPrioridade, Inimigo* arr[], int qtd);
bool checkar(Inimigo* arr[], char* inimigosCapturados[], char* inimigosPrioridade[], int* contador, bool* inventarioCheio);


#endif // !struct#pragma once
