#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include <string.h>
#include "Structs.h"
#include "stage-1.h"


//variavies globais
char inimigosCapturados[2];
int contador;
bool inventarioCheio;

#define KEY_SEEN     1
#define KEY_DOWN     2
#define MAX_BALAS 100

//objetos




//funções
float calcula_distancia(float dx, float dy) {
    return sqrt((dx * dx) + (dy * dy));
}

void trajetoria_tiro(float y_cursor, float x_cursor,
    float y_bullet, float x_bullet, Bullet* b) {

    float dx = x_cursor - x_bullet;
    float dy = y_cursor - y_bullet;

    float d = calcula_distancia(dx, dy);

    if (d > 0) {
        (*b).dx = dx / d;
        (*b).dy = dy / d;
        (*b).estado = 1;
    }
}

void mostraInimigos(char* arr) {
    int i;
    for (i = 0; i < sizeof(arr); i++)
        printf("%c", arr[i]);

}


static void getInimigos(Inimigo inimigo) {
    //printf("%c", inimigo);
    int i;
    for (i = 0; sizeof(inimigosCapturados) > i; i++) {
        if (inimigosCapturados[i] == 0) {
            inimigosCapturados[i] = inimigo.letra;
            contador++;
            break;
        }
    }
    if (contador == 2)
        inventarioCheio = true;
    mostraInimigos(inimigosCapturados);
}

//função de colisão
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if (ax1 > bx2) return false;
    if (ax2 < bx1) return false;
    if (ay1 > by2) return false;
    if (ay2 < by1) return false;

    return true;
}

//função checkar puzzle
bool checkar() {
    int corretos = 0;
    for (int i = 0; sizeof(inimigosCapturados) > i; i++) {
        if (inimigosCapturados[i] == 68) {
            corretos++;
        }
        else if (inimigosCapturados[i] == 79) {
            corretos++;
        }
        else {
            inimigosCapturados[i] = 0;
            contador--;
            inventarioCheio = false;
        }
    }
    if (corretos == 2)
        return true;

    return false;
}



int main()
{
    //intalar addons
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    

    //desenhar a tela
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);

    //inicia texto
    ALLEGRO_FONT* al_load_font(char const* filename, int size, int flags);
    ALLEGRO_FONT* font = al_create_builtin_font();

    //variavel de controle do game loop
    bool running = true;
    int fase = 0;

    //variaveis do puzzle
    bool condicao = false;

    //Variaveis do personagem
    struct Player p1 = { 150,500,100,10 };

    // Inicializando a bala e a velocidade
    
    Bullet bullet[MAX_BALAS] = {0};
    float velocidade_bala = 10.0f;

    //timer não é obrigatorio mas ele da um controle maior ao fps do game
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    //carregar imagem
    ALLEGRO_BITMAP* image = al_load_bitmap("cat.png");

    //carregar fila de eventos para observar os eventos do teclado
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());

    //movimentação do personagem

    
 

    //array que define quais teclas podem ser clicadas assim evitando que o pragram cheque teclas desnecessárias
    unsigned char key[ALLEGRO_KEY_MAX];
    //aqui fazemos com que a array definitivamente não tenha nenhum valor
    memset(key, 0, sizeof(key));

    struct Inimigo i1 = {150,120,100,'D'};
    struct Inimigo i2 = {260,120,100,'O'};
    struct Inimigo i3 = {370,120,100,'A'};
    struct Inimigo i4 = {480,120,100,'B'};
    struct Inimigo i5 = {590,120,100,'S'};

    float enemyX = 0;
    float enemyY = 0;
    //ativa o timer
    al_start_timer(timer);

    while (running) {

        ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
        ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
        ALLEGRO_COLOR yellow = al_map_rgb(227, 176, 36);
        ALLEGRO_COLOR green = al_map_rgba(22, 255, 20, 100);
        ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

        //ativa os eventos
        ALLEGRO_EVENT event;
        al_wait_for_event(fila, &event);

        //desenhar elementos na tela
        al_clear_to_color(al_map_rgb(255, 255, 255));

        //game loop

        //menu
        if (fase == 0) {
            al_draw_filled_rectangle(600, 180, 680, 200, red);
            al_draw_text(font, black, 640, 180, ALLEGRO_ALIGN_CENTER, "Novo Jogo");
            switch (event.type) {
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_LEFT) {
                        if (event.mouse.x > 600 && event.mouse.x < 680 && event.mouse.y > 180 && event.mouse.y < 200)
                            fase = 1;
                    };
                    break;
            }
            al_flip_display();
        }
        //fase 1
        if (fase == 1) {
            fase_1(i1, i2, i3, i4, i5, font, black, blue, red, green, yellow, condicao, p1, enemyX, enemyY, image, inventarioCheio);
        }
        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:

                //move inimigos
                enemyX = p1.x - i1.x;
                enemyY = p1.y - i1.y;
                float angulo = atan2(enemyY, enemyX);
                i1.x += cos(angulo) * 4;
                i1.y += sin(angulo) * 4;

                //aqui observamos a array de teclas para observar qual foi cliclada ao invés do estádo da tecla
                if (key[ALLEGRO_KEY_W])
                    if (p1.y > 50)
                        p1.y -= p1.speed;
                if (key[ALLEGRO_KEY_S])
                    if (p1.y < 670)
                        p1.y += p1.speed;
                if (key[ALLEGRO_KEY_A])
                    if (p1.x > 50)
                        p1.x -= p1.speed;
                if (key[ALLEGRO_KEY_D])
                    if (!condicao) {
                        if ((p1.x < 670))
                            p1.x += p1.speed;
                    }
                    else
                        p1.x += p1.speed;


                // movimentação de todas as balas ativas
                for (int i = 0; i < MAX_BALAS; i++) {

                    if (bullet[i].estado == 1) {

                        bullet[i].x_bullet +=
                            bullet[i].dx * velocidade_bala;

                        bullet[i].y_bullet +=
                            bullet[i].dy * velocidade_bala;

                        // verifica se a bala saiu da tela
                        if (bullet[i].x_bullet < 0 ||
                            bullet[i].x_bullet > 1280 ||
                            bullet[i].y_bullet < 0 ||
                            bullet[i].y_bullet > 720) {

                            bullet[i].estado = 0;
                        }
                    }
                }

                if (key[ALLEGRO_KEY_ESCAPE])
                    running = false;

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;
                break;


            case ALLEGRO_EVENT_MOUSE_AXES:
                al_draw_line(p1.x, p1.y, event.mouse.x, event.mouse.y, red, 5);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_RIGHT) {
                    if (event.mouse.x > (i1.x - 40) && event.mouse.x < (i1.x + 40) && event.mouse.y >(i1.y - 40) && event.mouse.y < (i1.y + 40)) {
                        if ((i1.vida > 0) && !inventarioCheio) {
                            getInimigos(i1);
                            i1.vida = 0;
                        }
                    }
                    if (event.mouse.x > (i2.x - 40) && event.mouse.x < (i2.x + 40) && event.mouse.y >(i2.y - 40) && event.mouse.y < (i2.y + 40)) {
                        if ((i2.vida > 0) && !inventarioCheio) {
                            getInimigos(i2);
                            i2.vida = 0;
                        }
                    }
                    if (event.mouse.x > (i3.x - 40) && event.mouse.x < (i3.x + 40) && event.mouse.y >(i3.y - 40) && event.mouse.y < (i3.y + 40)) {
                        if ((i3.vida > 0) && !inventarioCheio) {
                            getInimigos(i3);
                            i3.vida = 0;
                        }
                    }
                    if (event.mouse.x > (i4.x - 40) && event.mouse.x < (i4.x + 40) && event.mouse.y >(i4.y - 40) && event.mouse.y < (i4.y + 40)) {
                        if ((i4.vida > 0) && !inventarioCheio) {
                            getInimigos(i4);
                            i4.vida = 0;
                        }
                    }
                    if (event.mouse.x > (i5.x - 40) && event.mouse.x < (i5.x + 40) && event.mouse.y >(i5.y - 40) && event.mouse.y < (i5.y + 40)) {
                        if ((i5.vida > 0) && !inventarioCheio) {
                            getInimigos(i5);
                            i5.vida = 0;
                        }
                    }
                    //printf("Mouse Clicado");
                }
                if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_LEFT) {

                    for (int i = 0; i < MAX_BALAS; i++) {

                        if (bullet[i].estado == 0) {

                            bullet[i].x_bullet = p1.x;
                            bullet[i].y_bullet = p1.y;

                            trajetoria_tiro(
                                event.mouse.y,
                                event.mouse.x,
                                bullet[i].y_bullet,
                                bullet[i].x_bullet,
                                &bullet[i]
                            );

                            break;
                        }
                    }
                };
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_E)
                    if (collide(p1.x - 45, p1.y + 47, p1.x + 40, p1.y - 44, 350, 200, 700, 720)) {
                        if (checkar())
                            condicao = true;
                        else
                            p1.life = p1.life - 40;
                    }

                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            }
            // desenha todas as balas ativas
            for (int i = 0; i < MAX_BALAS; i++) {
                if (bullet[i].estado == 1) {
                    al_draw_filled_circle(
                        bullet[i].x_bullet,
                        bullet[i].y_bullet,
                        6,
                        black
                    );
                }
            }
        //atualiza os elementos graficos da tela
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_bitmap(image);
    printf("retornou ao console");
    return 0;
}