

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_font.h>


typedef struct Inimigo
{
    int x, y,vida;
    char letra;
} Inimigo;

typedef struct Player
{
    int x, y, life, speed;
} Player;

char inimigosCapturados[2];
int contador;
bool inventarioCheio;

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

    //variaveis do puzzle
    bool condicao = false;

    //Variaveis do personagem
    struct Player p1 = { 150,500,100,10 };
   

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

    #define KEY_SEEN     1
    #define KEY_DOWN     2

    //array que define quais teclas podem ser clicadas assim evitando que o pragram cheque teclas desnecessárias
    unsigned char key[ALLEGRO_KEY_MAX];
    //aqui fazemos com que a array definitivamente não tenha nenhum valor
    memset(key, 0, sizeof(key));

    struct Inimigo i1 = {150,120,100,'D'};
    struct Inimigo i2 = {260,120,100,'O'};
    struct Inimigo i3 = {370,120,100,'A'};
    struct Inimigo i4 = {480,120,100,'B'};
    struct Inimigo i5 = {590,120,100,'S'};
    

    while (running) {
        //game loop
            
        //ativa o timer
        al_start_timer(timer);
        //ativa os eventos
        ALLEGRO_EVENT event;
        al_wait_for_event(fila, &event);

        //desenhar elementos na tela
        al_clear_to_color(al_map_rgb(255, 255, 255));

        ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
        ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
        ALLEGRO_COLOR yellow = al_map_rgb(227, 176, 36);
        ALLEGRO_COLOR green = al_map_rgba(22, 255, 20, 100);
        ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

        //inimigos
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

        //colisão do personagem
        //al_draw_filled_rectangle(xPersonagem - 45, yPersonagem + 47, xPersonagem + 40, yPersonagem - 44, red);

        //colisão interação com a porta
        al_draw_filled_rectangle(350, 200, 700, 720, green);

        //"Porta"
        if(!condicao)
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

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                //aqui observamos a array de teclas para observar qual foi cliclada ao invés do estádo da tecla
                if (key[ALLEGRO_KEY_W]) 
                    if (p1.y > 50)
                        p1.y -= p1.speed;
                if (key[ALLEGRO_KEY_S])
                    if(p1.y < 670)
                        p1.y += p1.speed;
                if (key[ALLEGRO_KEY_A])
                    if(p1.x > 50)
                        p1.x -= p1.speed;
                if (key[ALLEGRO_KEY_D])
                    if (!condicao) {
                        if ((p1.x < 670))
                            p1.x += p1.speed;
                    }else
                        p1.x += p1.speed;

                if (key[ALLEGRO_KEY_E])
                    if (collide(p1.x - 45, p1.y + 47, p1.x + 40, p1.y - 44, 350, 200, 700, 720) && checkar())
                        condicao = true;
                    else
                        p1.life = p1.life - 40;

                if (key[ALLEGRO_KEY_ESCAPE])
                    running = false;

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;
                break;


            case ALLEGRO_EVENT_MOUSE_AXES:
                al_draw_line(p1.x, p1.y, event.mouse.x, event.mouse.y, red, 5);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_LEFT) {
                    if (event.mouse.x > (i1.x - 40) && event.mouse.x < (i1.x + 40) && event.mouse.y > (i1.y - 40) && event.mouse.y < (i1.y + 40)) {
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
                    if (event.mouse.x > (i3.x - 40) && event.mouse.x < (i3.x + 40) && event.mouse.y > (i3.y - 40) && event.mouse.y < (i3.y + 40)) {
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
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
        }

        //esconde o cursor da tela
        //al_hide_mouse_cursor(display);


        //desenha a imagem na tela
        al_draw_bitmap(image, p1.x - 50, p1.y - 50, 0);
       
        //atualiza os elementos graficos da tela
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_bitmap(image);
    printf("retornou ao console");
    return 0;
}



