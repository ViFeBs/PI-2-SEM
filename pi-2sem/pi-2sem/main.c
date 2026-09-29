

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>




int main()
{
    //intalar addons
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    //função para ter certeza que o mouse será iniciado
    //must_init(al_install_mouse(), "mouse");

    //desenhar a tela
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);

    //vairavel de controle do game loop
    bool running = true;


    //Variavel o personagem
    int xPersonagem = 150;
    int yPersonagem = 100;

    //timer não é obrigatorio mas ele da um controle maior ao fps do game
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    //carregar imagem
    ALLEGRO_BITMAP* image = al_load_bitmap("roadhog.png");

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


        //al_draw_filled_circle(300,300,50,blue);



        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                //aqui observamos a array de teclas para observar qual foi cliclada ao invés do estádo da tecla
                if (key[ALLEGRO_KEY_W])
                    yPersonagem--;
                if (key[ALLEGRO_KEY_S])
                    yPersonagem++;
                if (key[ALLEGRO_KEY_A])
                    xPersonagem--;
                if (key[ALLEGRO_KEY_D])
                    xPersonagem++;

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
              
                    printf("Mouse Clicado");
               

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
        al_draw_bitmap(image, xPersonagem, yPersonagem, 0);
       
        //atualiza os elementos graficos da tela
        al_flip_display();
    }

    al_destroy_display(display);
    al_destroy_bitmap(image);
    printf("retornou ao console");
    return 0;
}


