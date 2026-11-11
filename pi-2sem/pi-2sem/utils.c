#include "utils.h"

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
    for (i = 0; i < 2; i++)
        printf("%c", arr[i]);

}


void getInimigos(Inimigo inimigo, char* inimigosCapturados, int* contador, bool* inventarioCheio) {
    //printf("%c", inimigo);
    int i;
    for (i = 0; i < 2; i++) {
        if (inimigosCapturados[i] == 0) {
            inimigosCapturados[i] = inimigo.letra;
            (*contador)++;
            break;
        }
    }
    if (*contador >= 2)
        *inventarioCheio = true;
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





bool verificaColisao(Inimigo* arr[], Inimigo i1, int qtd, int pos) {
    for (int i = 0; qtd > i; i++) {
        if (pos == i) continue;
        if ((*arr[i]).vida <= 0) continue;
        if (collide(i1.x - 40, i1.y - 40, i1.x + 40, i1.y + 40, (*arr[i]).x - 40, (*arr[i]).y - 40, (*arr[i]).x + 40, (*arr[i]).y + 40)) {
            return true;
        }
    }

    return false;
}

Inimigo moveInimigos(Inimigo i, Player p1, float v, Inimigo* arr[], int qtd, int pos) {
    if (i.vida > 0) {
        float enemyX = 0;
        float enemyY = 0;
        enemyX = p1.x - i.x;
        enemyY = p1.y - i.y;
        float angulo = atan2(enemyY, enemyX);
        //se o inimigo ja esta grudado em outro, deixa ele andar para conseguir se soltar
        bool jaGrudado = verificaColisao(arr, i, qtd, pos);

        Inimigo teste = i;
        teste.x = i.x + cos(angulo) * v;
        if (jaGrudado || !verificaColisao(arr, teste, qtd, pos))
            i.x = teste.x;

        teste = i;
        teste.y = i.y + sin(angulo) * v;
        if (jaGrudado || !verificaColisao(arr, teste, qtd, pos))
            i.y = teste.y;

        return i;
    }
    return i;
}

void exibeInventario(char* inimigos, ALLEGRO_BITMAP* spritesInimigo) {
    //x 280 posicao inial do alfabeto
    //y 25 começa o alfabeto
    float src_x = 0;
    float src_y = 120;

    float destX = 40;
    float destY = 70;

    for (int i = 0; i < 2; i++) {
        char c = inimigos[i];
        if (c >= 'A' && c <= 'Z') {
            int indice = c - 'A';              // A=0, B=1, ... Z=25

            if (c <= 'I') {
                // primeira linha: A-I
                src_x = (indice * 14);
            }
            else if (c <= 'R') {
                src_x = (c - 'J') * 14;
                src_y = 140;
            }
            else {
                src_x = ((c - 'S') * 14);
                src_y = 160;
            }

            al_draw_scaled_bitmap(
                spritesInimigo,                     // A sprite sheet carregada
                src_x, src_y,                       // Posição X e Y do quadro na imagem original
                14,                                 // Largura do recorte
                20,                                 // Altura do recorte
                destX, destY,                       // Onde desenhar na tela
                14 * 3.0, 20 * 3.0,                 // Escala
                0                                   // Flags (ex: ALLEGRO_FLIP_HORIZONTAL)
            );
        }
        destX += 14 * 3.0;                      // avança para a próxima letra (espaços também avançam)
    }
}

void verificaAtaque(Inimigo* arr[], Player* p1) {
    for (int i = 0; i < 5; i++) {
        if ((*arr[i]).vida <= 0) continue;
        if (collide((*p1).x - 40, (*p1).y - 40, (*p1).x + 40, (*p1).y + 40, (*arr[i]).x - 40, (*arr[i]).y - 40, (*arr[i]).x + 40, (*arr[i]).y + 40)) {
            (*p1).life--;
        }
    }
}

void resetaInimigos(Inimigo* i1, Inimigo* i2, Inimigo* i3, Inimigo* i4, Inimigo* i5) {
    (*i1).x = 150;
    (*i1).y = 120;
    (*i1).vida = 100;

    (*i2).x = 260;
    (*i2).y = 120;
    (*i2).vida = 100;

    (*i3).x = 370;
    (*i3).y = 120;
    (*i3).vida = 100;

    (*i4).x = 480;
    (*i4).y = 120;
    (*i4).vida = 100;


    (*i5).x = 590;
    (*i5).y = 120;
    (*i5).vida = 100;

}

void limpaInventario(ALLEGRO_BITMAP* spritesInimigo, char* inimigosCapturados, bool* inventarioCheio, int* contador) {
    for (int i = 0; i < 2; i++) {
        inimigosCapturados[i] = 0;
    }
    *inventarioCheio = false;
    *contador = 0;

    exibeInventario(inimigosCapturados, spritesInimigo);
}

void animaInimigos(Inimigo* arr[], ALLEGRO_BITMAP* spritesInimigo) {

    int frame = (int)(al_get_time() / 0.5) % 2;
    for (int i = 0; i < 5; i++) {
        if ((*arr[i]).vida <= 0) continue;
        char letra = arr[i]->letra;
        float src_x, src_y;


        if (letra <= 'E') {
            src_y = 0;
            src_x = (letra - 'A') * 28;
        }
        else if (letra <= 'I') {
            src_y = 20;
            src_x = (letra - 'E') * 28;
        }
        else if (letra <= 'N') {
            src_y = 40;
            src_x = (letra - 'J') * 28;
        }
        else if (letra <= 'R') {
            src_y = 60;
            src_x = (letra - 'N') * 14;
        }
        else if (letra <= 'W') {
            src_y = 80;
            src_x = (letra - 'S') * 28;
        }


        al_draw_scaled_bitmap(
            spritesInimigo,                           // A sprite sheet carregada
            src_x + frame * 14, src_y,          // Posição X e Y do quadro na imagem original
            14,                                 // Largura do recorte
            20,                                 // Altura do recorte
            (*arr[i]).x - 40, (*arr[i]).y - 50, // Onde desenhar na tela
            14 * 5.0, 20 * 5.0,                 // escala                   
            0                                   // Flags (ex: ALLEGRO_FLIP_HORIZONTAL)
        );
    }
}

void reviveInimigos(char* inimigosPrioridade, Inimigo* arr[], int qtd) {
    for (int i = 0; i < qtd; i++) {
        for (int j = 0; j < 2; j++) {
            if (arr[i]->letra != inimigosPrioridade[j]) continue;
            if (arr[i]->vida <= 0) {
                arr[i]->vida = 100;
            }
        }
    }
}

//função checkar puzzle
//a resposta certa é "DO": precisa ter um D e um O no inventario
bool checkar(Inimigo* arr[], char* inimigosCapturados, char* inimigosPrioridade, int* contador, bool* inventarioCheio) {
    bool temD = false;
    bool temO = false;
    for (int i = 0; i < 2; i++) {
        if (inimigosCapturados[i] == 'D' && !temD) {
            temD = true;
        }
        else if (inimigosCapturados[i] == 'O' && !temO) {
            temO = true;
        }
        else if (inimigosCapturados[i] != 0) {
            //letra errada (ou repetida): tira do inventario
            inimigosCapturados[i] = 0;
            (*contador)--;
            *inventarioCheio = false;
        }
    }
    if (temD && temO)
        return true;

    inimigosPrioridade[0] = 'D';
    inimigosPrioridade[1] = 'O';
    reviveInimigos(inimigosPrioridade, arr, 5);

    return false;
}


//desenha um texto maior que o normal (a fonte do allegro tem so 8 pixels de altura)
//escreve o texto numa imagem pequena e depois desenha essa imagem esticada
//centroX: o meio do texto | escala: 2 = dobro do tamanho, 3 = triplo...
void desenhaTextoGrande(ALLEGRO_FONT* font, ALLEGRO_COLOR cor, float centroX, float y, float escala, const char* texto) {
    int largura = al_get_text_width(font, texto);
    int altura = al_get_font_line_height(font);
    if (largura <= 0)
        return;

    ALLEGRO_BITMAP* tela = al_get_target_bitmap();
    ALLEGRO_BITMAP* temp = al_create_bitmap(largura, altura);

    //escreve o texto na imagem pequena
    al_set_target_bitmap(temp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_draw_text(font, cor, 0, 0, 0, texto);

    //volta para a tela e desenha a imagem esticada
    al_set_target_bitmap(tela);
    al_draw_scaled_bitmap(temp, 0, 0, largura, altura,
        centroX - (largura * escala) / 2, y, largura * escala, altura * escala, 0);

    al_destroy_bitmap(temp);
}

//desenha o card das perguntas no topo da tela (serve para qualquer puzzle)
//titulo: aparece em vermelho escuro (ex: "Puzzle de Ingles")
//linha1, linha2 e linha3: o texto (use "" se nao precisar de alguma linha)
//cada linha cabe ate uns 36 letras
void desenhaCard(ALLEGRO_BITMAP* card, ALLEGRO_FONT* font, const char* titulo, const char* linha1, const char* linha2, const char* linha3) {
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR darkred = al_map_rgb(150, 2, 2);
    ALLEGRO_COLOR gray = al_map_rgb(90, 70, 50);

    //o card tem 640x160 e fica centralizado no topo da tela
    float x = 320;
    float y = 6;

    if (card != NULL)
        al_draw_bitmap(card, x, y, 0);
    else //se nao achar o card.png, desenha um retangulo no lugar para o jogo nao quebrar
        al_draw_filled_rectangle(x, y, x + 640, y + 160, al_map_rgb(244, 232, 200));

    desenhaTextoGrande(font, darkred, 640, y + 34, 2, titulo);
    desenhaTextoGrande(font, black, 640, y + 60, 2, linha1);
    desenhaTextoGrande(font, black, 640, y + 84, 2, linha2);
    desenhaTextoGrande(font, gray, 640, y + 112, 2, linha3);
}
