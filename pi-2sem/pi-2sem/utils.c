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
    for (i = 0; i < sizeof(arr); i++)
        printf("%c", arr[i]);

}


void getInimigos(Inimigo inimigo, char* inimigosCapturados[], int* contador, bool* inventarioCheio) {
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

Inimigo moveInimigos(Inimigo i, Player p1, float v, Inimigo* arr, int qtd, int pos) {
    if (i.vida > 0) {
        float enemyX = 0;
        float enemyY = 0;
        enemyX = p1.x - i.x;
        enemyY = p1.y - i.y;
        float angulo = atan2(enemyY, enemyX);
        Inimigo teste = i;
        teste.x = i.x + cos(angulo) * v;
        if (!verificaColisao(arr, teste, qtd, pos))
            i.x = teste.x;

        teste = i;
        teste.y = i.y + sin(angulo) * v;
        if (!verificaColisao(arr, teste, qtd, pos))
            i.y = teste.y;

        return i;
    }
    return i;
}

void exibeInventario(char* inimigos[], ALLEGRO_BITMAP* spritesInimigo) {
    //x 280 posicao inial do alfabeto
    //y 25 começa o alfabeto
    float src_x = 0;
    float src_y = 120;

    float destX = 40;
    float destY = 70;

    for (int i = 0; sizeof(inimigos) > i; i++) {
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

void limpaInventario(ALLEGRO_BITMAP* spritesInimigo, char* inimigosCapturados[], bool* inventarioCheio) {
    for (int i = 0; sizeof(inimigosCapturados) > i; i++) {
        inimigosCapturados[i] = 0;
    }
    inventarioCheio = false;

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
        for (int j = 0; j < sizeof(inimigosPrioridade); j++) {
            if (arr[i]->letra != inimigosPrioridade[j]) continue;
            if (arr[i]->vida <= 0) {
                arr[i]->vida = 100;
            }
        }
    }
}

//função checkar puzzle
bool checkar(Inimigo* arr[],char* inimigosCapturados[], char* inimigosPrioridade[], int* contador, bool* inventarioCheio) {
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

    inimigosPrioridade[0] = 'D';
    inimigosPrioridade[1] = 'O';
    reviveInimigos(inimigosPrioridade, arr, 5);

    return false;
}