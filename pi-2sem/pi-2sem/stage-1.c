
#include "stage-1.h"
#include <string.h>

void fase_1(Inimigo i1, Inimigo i2, Inimigo i3, Inimigo i4, Inimigo i5, ALLEGRO_FONT* font, ALLEGRO_COLOR black, ALLEGRO_COLOR blue, ALLEGRO_COLOR red, ALLEGRO_COLOR green, ALLEGRO_COLOR yellow, bool condicao, Player p1,ALLEGRO_BITMAP* image, bool inventarioCheio) {
    if (i1.vida > 0) {
        al_draw_filled_circle(i1.x, i1.y, 50, blue);
        //colisão
        al_draw_filled_rectangle(i1.x - 40, i1.y + 40, i1.x + 40, i1.y - 40, red);
        al_draw_text(font, black, i1.x, i1.y - 70, ALLEGRO_ALIGN_CENTER, "D");
    }
    if (i2.vida > 0) {
        al_draw_filled_circle(i2.x, i2.y, 50, blue);
        al_draw_filled_rectangle(i2.x - 40, i2.y + 40, i2.x + 40, i2.y - 40, red);
        al_draw_text(font, black, i2.x, i2.y - 70, ALLEGRO_ALIGN_CENTER, "O");
    }

    if (i3.vida > 0) {
        al_draw_filled_circle(i3.x, i3.y, 50, blue);
        al_draw_filled_rectangle(i3.x - 40, i3.y + 40, i3.x + 40, i3.y - 40, red);
        al_draw_text(font, black, i3.x, i3.y - 70, ALLEGRO_ALIGN_CENTER, "A");
    }

    if (i4.vida > 0) {
        al_draw_filled_circle(i4.x, i4.y, 50, blue);
        al_draw_filled_rectangle(i4.x - 40, i4.y + 40, i4.x + 40, i4.y - 40, red);
        al_draw_text(font, black, i4.x, i4.y - 70, ALLEGRO_ALIGN_CENTER, "B");
    }

    if (i5.vida > 0) {
        al_draw_filled_circle(i5.x, i5.y, 50, blue);
        al_draw_filled_rectangle(i5.x - 40, i5.y + 40, i5.x + 40, i5.y - 40, red);
        al_draw_text(font, black, i5.x, i5.y - 70, ALLEGRO_ALIGN_CENTER, "S");
    }

    //colisão interação com a porta
    al_draw_filled_rectangle(350, 200, 700, 720, green);

    //"Porta"
    if (!condicao)
        al_draw_filled_rectangle(700, 10, 780, 720, yellow);


    if (collide(p1.x - 45, p1.y + 47, p1.x + 40, p1.y - 44, 350, 200, 700, 720)) {
        al_draw_text(font, black, 640, 180, ALLEGRO_ALIGN_CENTER, "How __ i open this door?");
        al_draw_text(font, black, 640, 190, ALLEGRO_ALIGN_CENTER, "Aperte E para interagir");
    }

    if (inventarioCheio) {
        al_draw_text(font, red, 640, 640, ALLEGRO_ALIGN_CENTER, "Inventário Cheio");
    }

    char vida[20];
    snprintf(vida, sizeof(vida), "%d", p1.life);
    al_draw_text(font, red, 1200, 140, ALLEGRO_ALIGN_CENTER, vida);



    al_draw_bitmap(image, p1.x - 50, p1.y - 50, 0);
    al_flip_display();
}