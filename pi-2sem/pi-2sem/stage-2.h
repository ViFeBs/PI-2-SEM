#ifndef H_STAGE_2
#define H_STAGE_2

#include "utils.h"

//numero de perguntas do puzzle de funcoes
#define TOTAL_PERGUNTAS 5

//cada inimigo da fase 2 e o grafico de uma funcao.
//o campo "letra" do inimigo guarda qual funcao ele e (0 a 4):
//0: f(x) = x + 1
//1: f(x) = -x + 1
//2: f(x) = x^2
//3: f(x) = -x^2
//4: f(x) = 2

void fase_2(Inimigo* inimigos[], int perguntaAtual, int tempoErro, ALLEGRO_FONT* font, bool condicao, Player p1, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* card);
int respostaCerta(int perguntaAtual);
void resetaInimigosFuncao(Inimigo* inimigos[]);

#endif
