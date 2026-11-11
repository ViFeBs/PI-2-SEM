
#include "stage-1.h"
#include "utils.h"
#include <string.h>

void fase_1(Inimigo* inimigos[], ALLEGRO_FONT* font, bool* condicao, Player p1, ALLEGRO_BITMAP* image, bool* inventarioCheio, int lifebar, ALLEGRO_BITMAP* spriteInimigos, ALLEGRO_BITMAP* card) {
    
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR yellow = al_map_rgb(227, 176, 36);
    ALLEGRO_COLOR green = al_map_rgba(22, 255, 20, 100);
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

    //colisão interação com a porta
    al_draw_filled_rectangle(900, 200, 1200, 720, green);

    //"Porta"
    if (!*condicao)
        al_draw_filled_rectangle(1200, 10, 1280, 720, yellow);


    //card com a pergunta do puzzle de ingles
    if (collide(p1.x - 45, p1.y + 47, p1.x + 40, p1.y - 44, 900, 200, 1280, 720))
        desenhaCard(card, font, "Puzzle de Ingles", "How __ I open this door?", "", "Aperte E para responder");
    else
        desenhaCard(card, font, "Puzzle de Ingles", "How __ I open this door?", "Capture as letras certas", "e aperte E na area verde");

    if (*inventarioCheio) {
        desenhaTextoGrande(font, red, 640, 640, 2, "Inventario Cheio");
    }

    ALLEGRO_COLOR darkred = al_map_rgb(150, 2, 2);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
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
    //o al_flip_display() fica so no main.c (senao a tela pisca e as balas somem)
}