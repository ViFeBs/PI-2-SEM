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
#include "stage-2.h"
#include "utils.h"


//variavies globais
char inimigosCapturados[2];
char inimigosPrioridade[2];
int contador;
bool inventarioCheio;
bool faseIniciada;

#define KEY_SEEN     1
#define KEY_DOWN     2
#define MAX_BALAS 100




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
    ALLEGRO_FONT* font = al_create_builtin_font();

    //variavel de controle do game loop
    bool running = true;
    int fase = 0;

    //variaveis do puzzle
    bool condicao = false;
    int perguntaAtual = 0;  //qual pergunta da fase 2 o jogador esta
    int tempoErro = 0;      //quanto tempo a mensagem de erro da fase 2 fica na tela

    //contador para reviver os inimigos (o timer roda 60 vezes por segundo)
    int tempoReviver = 0;

    //Variaveis do personagem
    struct Player p1 = { 150,500,100,10 };
    int lifebar = 300;

    // Inicializando a bala e a velocidade

    Bullet bullet[MAX_BALAS] = {0};
    float velocidade_bala = 10.0f;

    //timer não é obrigatorio mas ele da um controle maior ao fps do game
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    //carregar imagem
    ALLEGRO_BITMAP* image = al_load_bitmap("cat.png");
    ALLEGRO_BITMAP* spritesInimigo = al_load_bitmap("enemies_sprites.png");
    ALLEGRO_BITMAP* card = al_load_bitmap("card.png");   //card das perguntas

    //carregar fila de eventos para observar os eventos do teclado
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(display));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());

    //array que define quais teclas podem ser clicadas assim evitando que o pragram cheque teclas desnecessárias
    unsigned char key[ALLEGRO_KEY_MAX];
    //aqui fazemos com que a array definitivamente não tenha nenhum valor
    memset(key, 0, sizeof(key));

    //inimigos da fase 1 (letras)
    struct Inimigo i1 = { 150,120,100,'D' };
    struct Inimigo i2 = { 260,120,100,'O' };
    struct Inimigo i3 = { 370,120,100,'A' };
    struct Inimigo i4 = { 480,120,100,'B' };
    struct Inimigo i5 = { 590,120,100,'S' };

    Inimigo* arr_inimigos[5] = {&i1, &i2, &i3, &i4, &i5};

    //inimigos da fase 2 (graficos de funcoes, veja o stage-2.h)
    struct Inimigo f1 = { 150,120,100,0 };
    struct Inimigo f2 = { 260,120,100,1 };
    struct Inimigo f3 = { 370,120,100,2 };
    struct Inimigo f4 = { 480,120,100,3 };
    struct Inimigo f5 = { 590,120,100,4 };

    Inimigo* arr_funcoes[5] = { &f1, &f2, &f3, &f4, &f5 };

    //velocidade de cada inimigo
    float velocidades[5] = { 4, 3, 5, 2, 3.5 };


    //ativa o timer
    al_start_timer(timer);

    while (running) {

        ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
        ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

        //ativa os eventos
        ALLEGRO_EVENT event;
        al_wait_for_event(fila, &event);

        //desenhar elementos na tela
        al_clear_to_color(al_map_rgb(156, 165, 181));

        //inimigos da fase em que o jogador esta
        Inimigo** inimigosAtuais = arr_inimigos;
        if (fase == 2)
            inimigosAtuais = arr_funcoes;

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
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = false;
                    break;
            }
        }
        //fase 1
        if (fase == 1) {
            fase_1(arr_inimigos, font, &condicao, p1, image, &inventarioCheio, lifebar, spritesInimigo, card);
            exibeInventario(inimigosCapturados, spritesInimigo);
            animaInimigos(arr_inimigos, spritesInimigo);
        }
        //fase 2: puzzle de funcoes
        if (fase == 2) {
            //a porta so abre depois de responder todas as perguntas
            if (perguntaAtual >= TOTAL_PERGUNTAS)
                condicao = true;
            else
                condicao = false;

            fase_2(arr_funcoes, perguntaAtual, tempoErro, font, condicao, p1, image, card);
        }
        //fim do jogo
        if (fase == 3) {
            desenhaTextoGrande(font, black, 640, 320, 3, "Parabens! Voce terminou o jogo.");
            desenhaTextoGrande(font, black, 640, 370, 2, "Aperte ESC para sair");
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                running = false;
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = false;
        }
        if (fase == 1 || fase == 2) {
            switch (event.type)
            {
                case ALLEGRO_EVENT_TIMER:


                    verificaAtaque(inimigosAtuais, &p1);

                    //jogador morreu: volta pro menu e reseta tudo
                    if (p1.life <= 0) {
                        fase = 0;
                        p1.life = 100;
                        p1.x = 150;
                        p1.y = 500;
                        limpaInventario(spritesInimigo, inimigosCapturados, &inventarioCheio, &contador);
                        condicao = false;
                        resetaInimigos(&i1, &i2, &i3, &i4, &i5);
                        resetaInimigosFuncao(arr_funcoes);
                        perguntaAtual = 0;
                        tempoErro = 0;
                        for (int b = 0; b < MAX_BALAS; b++)
                            bullet[b].estado = 0;
                        break;
                    }

                    //move inimigos
                    for (int k = 0; k < 5; k++) {
                        float v = velocidades[k];
                        if (fase == 2)
                            v = v / 2; //na fase 2 eles andam mais devagar para dar tempo de olhar o grafico
                        *inimigosAtuais[k] = moveInimigos(*inimigosAtuais[k], p1, v, inimigosAtuais, 5, k);
                    }

                    //revive os inimigos a cada 10 segundos (600 / 60 = 10)
                    tempoReviver++;
                    if (tempoReviver >= 600) {
                        for (int in = 0; in < 5; in++) {
                            if ((*inimigosAtuais[in]).vida <= 0) {
                                //volta para a posicao do comeco
                                Inimigo novo = *inimigosAtuais[in];
                                novo.x = 150 + in * 110;
                                novo.y = 120;
                                novo.vida = 100;
                                //so revive se nao tiver outro inimigo nesse lugar (senao eles grudam)
                                //se tiver, ele tenta de novo daqui a 10 segundos
                                if (!verificaColisao(inimigosAtuais, novo, 5, in))
                                    *inimigosAtuais[in] = novo;
                            }
                        }
                        tempoReviver = 0;
                    }

                    if (tempoErro > 0)
                        tempoErro--;


                    //aqui observamos a array de teclas para observar qual foi cliclada ao invés do estádo da tecla
                    if (key[ALLEGRO_KEY_W])
                        if (p1.y > 50)
                            p1.y -= p1.speed;
                    if (key[ALLEGRO_KEY_S])
                        if (p1.y < 670)
                            p1.y += p1.speed;
                    if (key[ALLEGRO_KEY_A]) {
                        if (fase != 1) {
                            p1.x -= p1.speed;
                            if (p1.x < 0) {
                                fase--;
                                p1.x = 1280;
                                condicao = true;
                                //as balas nao passam de uma fase para outra
                                for (int b = 0; b < MAX_BALAS; b++)
                                    bullet[b].estado = 0;
                            }
                        }
                        else if (p1.x > 50)
                            p1.x -= p1.speed;
                    }
                    if (key[ALLEGRO_KEY_D]) {
                        if (!condicao) {
                            if ((p1.x < 1280))
                                p1.x += p1.speed;
                        }
                        else {
                            p1.x += p1.speed;
                        }
                        if (p1.x > 1280) {
                            fase++;
                            p1.x = 0;
                            //as balas nao passam de uma fase para outra
                            for (int b = 0; b < MAX_BALAS; b++)
                                bullet[b].estado = 0;
                        }
                    }

                    // movimentação de todas as balas ativas
                    for (int i = 0; i < MAX_BALAS; i++) {

                        //bala desativada nao anda e nao acerta ninguem
                        if (bullet[i].estado != 1)
                            continue;

                        bullet[i].x_bullet +=
                            bullet[i].dx * velocidade_bala;

                        bullet[i].y_bullet +=
                            bullet[i].dy * velocidade_bala;

                        // verifica se a bala saiu da tela: desativa e passa para a proxima
                        if (bullet[i].x_bullet < 0 ||
                            bullet[i].x_bullet > 1280 ||
                            bullet[i].y_bullet < 0 ||
                            bullet[i].y_bullet > 720) {

                            bullet[i].estado = 0;
                            continue;
                        }

                        //verifica se a bala acertou algum inimigo da fase atual
                        for (int in = 0; in < 5; in++) {
                            if (collide(bullet[i].x_bullet - 6, bullet[i].y_bullet - 6,
                                bullet[i].x_bullet + 6, bullet[i].y_bullet + 6,
                                (*inimigosAtuais[in]).x - 40, (*inimigosAtuais[in]).y - 40,
                                (*inimigosAtuais[in]).x + 40, (*inimigosAtuais[in]).y + 40)
                                && (*inimigosAtuais[in]).vida > 0) {

                                    bullet[i].estado = 0;
                                    (*inimigosAtuais[in]).vida -= 10;

                                    break;
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
                    //fase 1: botao direito captura a letra
                    if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_RIGHT && fase == 1) {
                        if (event.mouse.x > (i1.x - 40) && event.mouse.x < (i1.x + 40) && event.mouse.y >(i1.y - 40) && event.mouse.y < (i1.y + 40)) {
                            if ((i1.vida > 0) && !inventarioCheio) {
                                getInimigos(i1, inimigosCapturados, &contador, &inventarioCheio);
                                i1.vida = 0;
                            }
                        }
                        if (event.mouse.x > (i2.x - 40) && event.mouse.x < (i2.x + 40) && event.mouse.y >(i2.y - 40) && event.mouse.y < (i2.y + 40)) {
                            if ((i2.vida > 0) && !inventarioCheio) {
                                getInimigos(i2, inimigosCapturados, &contador, &inventarioCheio);
                                i2.vida = 0;
                            }
                        }
                        if (event.mouse.x > (i3.x - 40) && event.mouse.x < (i3.x + 40) && event.mouse.y >(i3.y - 40) && event.mouse.y < (i3.y + 40)) {
                            if ((i3.vida > 0) && !inventarioCheio) {
                                getInimigos(i3, inimigosCapturados, &contador, &inventarioCheio);
                                i3.vida = 0;
                            }
                        }
                        if (event.mouse.x > (i4.x - 40) && event.mouse.x < (i4.x + 40) && event.mouse.y >(i4.y - 40) && event.mouse.y < (i4.y + 40)) {
                            if ((i4.vida > 0) && !inventarioCheio) {
                                getInimigos(i4, inimigosCapturados, &contador, &inventarioCheio);
                                i4.vida = 0;
                            }
                        }
                        if (event.mouse.x > (i5.x - 40) && event.mouse.x < (i5.x + 40) && event.mouse.y >(i5.y - 40) && event.mouse.y < (i5.y + 40)) {
                            if ((i5.vida > 0) && !inventarioCheio) {
                                getInimigos(i5, inimigosCapturados, &contador, &inventarioCheio);
                                i5.vida = 0;
                            }
                        }
                        //printf("Mouse Clicado");
                    }
                    //fase 2: botao direito captura o grafico (responde a pergunta)
                    if (event.mouse.button == ALLEGRO_MOUSE_BUTTON_RIGHT && fase == 2 && perguntaAtual < TOTAL_PERGUNTAS) {
                        for (int k = 0; k < 5; k++) {
                            Inimigo* f = arr_funcoes[k];
                            if ((*f).vida > 0 &&
                                event.mouse.x > ((*f).x - 40) && event.mouse.x < ((*f).x + 40) &&
                                event.mouse.y > ((*f).y - 40) && event.mouse.y < ((*f).y + 40)) {

                                if ((*f).letra == respostaCerta(perguntaAtual)) {
                                    (*f).vida = 0;      //grafico certo: captura e vai para a proxima pergunta
                                    perguntaAtual++;
                                    tempoErro = 0;
                                }
                                else {
                                    p1.life = p1.life - 20;
                                    tempoErro = 120;    //mostra a mensagem de erro por 2 segundos
                                }
                                break;
                            }
                        }
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
                    //E so funciona na porta da fase 1
                    if (event.keyboard.keycode == ALLEGRO_KEY_E && fase == 1)
                        if (collide(p1.x - 45, p1.y + 47, p1.x + 40, p1.y - 44, 900, 200, 1280, 720)) {
                            if (checkar(arr_inimigos, inimigosCapturados, inimigosPrioridade, &contador, &inventarioCheio)) {
                                condicao = true;
                                limpaInventario(spritesInimigo, inimigosCapturados, &inventarioCheio, &contador);
                            }
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

        }
        //atualiza os elementos graficos da tela (so aqui, uma vez por volta do loop)
        al_flip_display();

    }

    al_destroy_display(display);
    al_destroy_bitmap(image);
    al_destroy_bitmap(spritesInimigo);
    if (card != NULL)
        al_destroy_bitmap(card);
    printf("retornou ao console");
    return 0;
}
