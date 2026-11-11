#include "stage-2.h"

//perguntas da fase 2, divididas em 2 linhas para caber no card
//(sem acento porque a fonte do allegro nao mostra)
char perguntasLinha1[TOTAL_PERGUNTAS][40] = {
    "Capture o grafico de",
    "Capture a funcao constante",
    "Capture o grafico de",
    "Capture a funcao crescente",
    "Capture a parabola virada"
};
char perguntasLinha2[TOTAL_PERGUNTAS][40] = {
    "f(x) = x^2",
    "f(x) = 2",
    "f(x) = -x + 1",
    "que passa pelo ponto (0, 1)",
    "para baixo: f(x) = -x^2"
};

//qual funcao (inimigo) e a resposta certa de cada pergunta
int respostas[TOTAL_PERGUNTAS] = { 2, 4, 1, 0, 3 };

int respostaCerta(int perguntaAtual) {
    return respostas[perguntaAtual];
}

//calcula o valor da funcao "tipo" no ponto x
float valorFuncao(int tipo, float x) {
    if (tipo == 0)
        return x + 1;
    if (tipo == 1)
        return -x + 1;
    if (tipo == 2)
        return x * x;
    if (tipo == 3)
        return -(x * x);
    return 2; //tipo 4: funcao constante
}

//desenha o grafico da funcao dentro de um quadrado de 80x80 com centro em (cx, cy)
void desenhaGrafico(int tipo, float cx, float cy) {
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR gray = al_map_rgb(170, 170, 170);
    ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR darkred = al_map_rgb(150, 2, 2);

    //fundo e borda do "inimigo"
    al_draw_filled_rectangle(cx - 40, cy - 40, cx + 40, cy + 40, white);
    al_draw_rectangle(cx - 40, cy - 40, cx + 40, cy + 40, darkred, 4);

    //eixos x e y
    al_draw_line(cx - 36, cy, cx + 36, cy, gray, 1);
    al_draw_line(cx, cy - 36, cx, cy + 36, gray, 1);

    //desenha a curva ligando varios pontinhos, com x indo de -2 ate 2
    //cada unidade de x vale 16 pixels e cada unidade de y vale 8 pixels
    float x = -2;
    while (x < 2) {
        float x2 = x + 0.2;
        float y1 = valorFuncao(tipo, x);
        float y2 = valorFuncao(tipo, x2);

        float px1 = cx + x * 16;
        float py1 = cy - y1 * 8;    //no allegro o y cresce para baixo, por isso o "-"
        float px2 = cx + x2 * 16;
        float py2 = cy - y2 * 8;

        //so desenha se o pedaco estiver dentro do quadrado
        if (py1 > cy - 38 && py1 < cy + 38 && py2 > cy - 38 && py2 < cy + 38)
            al_draw_line(px1, py1, px2, py2, blue, 3);

        x = x2;
    }
}

void resetaInimigosFuncao(Inimigo* inimigos[]) {
    for (int i = 0; i < 5; i++) {
        (*inimigos[i]).x = 150 + i * 110;
        (*inimigos[i]).y = 120;
        (*inimigos[i]).vida = 100;
    }
}

void fase_2(Inimigo* inimigos[], int perguntaAtual, int tempoErro, ALLEGRO_FONT* font, bool condicao, Player p1, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* card) {
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR darkred = al_map_rgb(150, 2, 2);
    ALLEGRO_COLOR yellow = al_map_rgb(227, 176, 36);
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    //"Porta" (abre quando todas as perguntas forem respondidas)
    if (!condicao)
        al_draw_filled_rectangle(1200, 10, 1280, 720, yellow);

    //card com a pergunta (desenhado antes dos inimigos, para eles passarem por cima)
    if (perguntaAtual < TOTAL_PERGUNTAS) {
        char titulo[40];
        snprintf(titulo, sizeof(titulo), "Funcoes - Pergunta %d/%d", perguntaAtual + 1, TOTAL_PERGUNTAS);
        desenhaCard(card, font, titulo, perguntasLinha1[perguntaAtual], perguntasLinha2[perguntaAtual], "Botao direito no grafico certo");
    }
    else {
        desenhaCard(card, font, "Funcoes", "Voce acertou tudo!", "A porta abriu.", "Siga para a direita");
    }

    //inimigos (graficos)
    for (int i = 0; i < 5; i++) {
        if ((*inimigos[i]).vida > 0)
            desenhaGrafico((*inimigos[i]).letra, (*inimigos[i]).x, (*inimigos[i]).y);
    }

    if (tempoErro > 0)
        desenhaTextoGrande(font, red, 640, 640, 2, "Grafico errado! -20 de vida");

    //barra de vida (igual a da fase 1)
    char vida[20];
    snprintf(vida, sizeof(vida), "%d", p1.life);
    if (p1.life > 0) {
        float lifebarfim = 50 + (p1.life / 100.0f) * 250.0f;
        al_draw_filled_rectangle(50, 50, 300, 70, darkred);
        al_draw_filled_rectangle(50, 50, lifebarfim, 70, red);
    }
    al_draw_text(font, white, 175, 55, ALLEGRO_ALIGN_CENTER, vida);
    al_draw_text(font, black, 40, 55, ALLEGRO_ALIGN_CENTER, "HP: ");

    al_draw_bitmap(image, p1.x - 50, p1.y - 50, 0);
}
