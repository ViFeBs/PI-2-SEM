#ifndef H_STRUCT
#define H_STRUCT

typedef struct {
    float x_bullet;
    float y_bullet;

    float dx;
    float dy;

    int estado;
} Bullet;

typedef struct Inimigo
{
    int x, y, vida;
    char letra;
} Inimigo;

typedef struct Player
{
    int x, y, life, speed;
} Player;

#endif // !struct


