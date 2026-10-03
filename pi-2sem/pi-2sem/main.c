

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>


typedef struct Inimigo
{
    int x, y,vida;
    char letra;
} Inimigo;

char inimigosCapturados[4];

void mostraInimigos(char* arr) {
    int i;
    for (i = 0; i < 4 - 1; i++)
        printf("%c", arr[i]);

}

static void getInimigos(char inimigo) {
    //printf("%c", inimigo);
    int i;
    for (i = 0; sizeof(inimigosCapturados) > i; i++) {
        if (inimigosCapturados[i] == 0) {
            inimigosCapturados[i] = inimigo;
            break;
        }
    }
    mostraInimigos(inimigosCapturados);
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

    //variavel de controle do game loop
    bool running = true;

    //variaveis do puzzle
    bool condicao = false;

    //Variaveis do personagem
    int xPersonagem = 150;
    int yPersonagem = 500;
    int speed = 10;

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

        //inimigos
        al_draw_filled_circle(i1.x,i1.y,50,blue);
        al_draw_filled_circle(i2.x, i2.y, 50, blue);
        al_draw_filled_circle(i3.x, i3.y, 50, blue);
        al_draw_filled_circle(i4.x, i4.y, 50, blue);
        al_draw_filled_circle(i5.x, i5.y, 50, blue);

        //colisão
        al_draw_filled_rectangle(i1.x - 40, i1.y + 40, i1.x + 40, i1.y - 40, red);
        al_draw_filled_rectangle(i2.x - 40, i2.y + 40, i2.x + 40, i2.y - 40, red);
        al_draw_filled_rectangle(i3.x - 40, i3.y + 40, i3.x + 40, i3.y - 40, red);
        al_draw_filled_rectangle(i4.x - 40, i4.y + 40, i4.x + 40, i4.y - 40, red);
        al_draw_filled_rectangle(i5.x - 40, i5.y + 40, i5.x + 40, i5.y - 40, red);

        //colisão interação com a porta
        al_draw_filled_rectangle(350, 200, 700, 720, green);

        //"Porta"
        if(!condicao)
            al_draw_filled_rectangle(700, 10, 780, 720, yellow);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                //aqui observamos a array de teclas para observar qual foi cliclada ao invés do estádo da tecla
                if (key[ALLEGRO_KEY_W]) 
                    if (yPersonagem > 50)
                        yPersonagem -= speed;
                if (key[ALLEGRO_KEY_S])
                    if(yPersonagem < 670)
                        yPersonagem += speed;
                if (key[ALLEGRO_KEY_A])
                    if(xPersonagem > 50)
                        xPersonagem -= speed;
                if (key[ALLEGRO_KEY_D])
                    if(!condicao && (xPersonagem < 670))
                        xPersonagem += speed;

                if (key[ALLEGRO_KEY_ESCAPE])
                    running = false;

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;
                break;


            case ALLEGRO_EVENT_MOUSE_AXES:
                al_draw_line(xPersonagem, yPersonagem, event.mouse.x, event.mouse.y, red, 5);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_LEFT) {
                    if (event.mouse.x > (i1.x - 40) && event.mouse.x < (i1.x + 40) && event.mouse.y > (i1.y - 40) && event.mouse.y < (i1.y + 40)) {
                        getInimigos(i1.letra);
                        printf("Inimigo Capturado");
                    }
                    if (event.mouse.x > (i2.x - 40) && event.mouse.x < (i2.x + 40) && event.mouse.y >(i2.y - 40) && event.mouse.y < (i2.y + 40)) {
                        getInimigos(i2.letra);
                        printf("Inimigo Capturado");
                    }
                    if (event.mouse.x > (i3.x - 40) && event.mouse.x < (i3.x + 40) && event.mouse.y > (i3.y - 40) && event.mouse.y < (i3.y + 40)) {
                        getInimigos(i3.letra);
                        printf("Inimigo Capturado");
                    }
                    if (event.mouse.x > (i4.x - 40) && event.mouse.x < (i4.x + 40) && event.mouse.y >(i4.y - 40) && event.mouse.y < (i4.y + 40)) {
                        getInimigos(i4.letra);
                        printf("Inimigo Capturado");
                    }
                    if (event.mouse.x > (i5.x - 40) && event.mouse.x < (i5.x + 40) && event.mouse.y >(i5.y - 40) && event.mouse.y < (i5.y + 40)) {
                        getInimigos(i5.letra);
                        printf("Inimigo Capturado");
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
        al_draw_bitmap(image, xPersonagem - 50, yPersonagem - 50, 0);
       
        //atualiza os elementos graficos da tela
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_bitmap(image);
    printf("retornou ao console");
    return 0;
}



