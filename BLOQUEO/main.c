#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
int **crear_tablero(int filas, int columnas);
void destruir_tablero(int **tablero, int filas);
void** crearMatriz(size_t filas, size_t columnas, size_t tamElem);
void destruirMatriz(void **matriz, size_t filas);
void dibujarBloque(int x, int y, int alto, int ancho, int color);
void dibujarTextoVariable(const char *texto, int x, int y);

#define AUMENTO_DIFICULDAD 0.97
#define MAX_DIFICULTAD 0.05
#include "GBT/gbt.h"
#define PANTALLA_JUEGO "PANTALLA_JUEGO"
#define CANT_PIEZAS_CLASSIC 7
#define PIEZAS_ENCOLADAS 5
#define CANT_PIEZAS_DELUXE 11
#define CANT_COLORES 26
#define TAM_CELDA 8
#define TABLERO_X 100
#define TABLERO_Y 30
#include "GBT/gbt.h"
#define POS_Y_INICIAL 5
#define POS_X_INICIAL 3
#define RES_CGA "CGA"
#define RES_VGA "VGA"
#define MIN_COL_DELUXE 8
#define MAX_COL_DELUXE 16

typedef struct
{
    int posX;
    int posY;
} Mouse;

typedef enum
{
    MODO_CLASICO,
    MODO_DELUXE
} ModoJuego;
typedef enum
{
    RESOLUCION_CGA,
    RESOLUCION_VGA
} Resolucion;

typedef enum
{
    NONE,
    ARRIBA,
    DERECHA,
    IZQUIERDA,
    ABAJO,
    ROTAR_HORARIO,
    ROTAR_ANTIHORARIO,
    PAUSA,
    ESCAPE
} Teclado;
Teclado teclado();
typedef enum
{
    PIEZA_I,
    PIEZA_J,
    PIEZA_L,
    PIEZA_O,
    PIEZA_S,
    PIEZA_T,
    PIEZA_Z,
    PIEZA_Q,
    PIEZA_C,
    PIEZA_P,
    PIEZA_D,
} TipoPieza;

typedef struct
{
    int **matriz;
    int filVisibles;
    int filTotales;
    int columnas;
} Tablero;

typedef struct
{
    int forma[4][4];
    int x;
    int y;
    int colorBase;
    TipoPieza tipo;
} Pieza;

int puntaje[]=
{
    0,
    100,
    250,
    500,
    800
};

tGBT_ColorRGB paletaPastel[] = {
    {0x00, 0x00, 0x00}, // 0 negro / fondo

    // CELESTES
    {0xB8, 0xF2, 0xFF}, // 1 celeste claro
    {0x9F, 0xE4, 0xFF}, // 2 celeste medio
    {0x8A, 0xD8, 0xFF}, // 3 celeste oscuro brillante

    // AZULES
    {0x5F, 0x7F, 0xA8}, // 4 azul metalico
    {0xB5, 0xC7, 0xFF}, // 5 azul pastel
    {0x95, 0xB8, 0xFF}, // 6 azul claro

    // NARANJAS
    {0xE0, 0x70, 0x1F}, // 7 naranja metalico
    {0xFF, 0xB8, 0x66}, // 8 naranja pastel
    {0xFF, 0x98, 0x33}, // 9 naranja claro


    // AMARILLOS
    {0xD9, 0xAA, 0x00}, // 10 amarillo metalico
    {0xFF, 0xF0, 0x00}, // 11 amarillo claro intenso
    {0xFF, 0xD8, 0x4D}, // 12 amarillo fuerte

    // VERDES pastel
    {0x7F, 0xBF, 0x95}, // 13 verde metalico pastel
    {0xC8, 0xFF, 0xD4}, // 15 verde suave brillante
    {0xA8, 0xF2, 0xB8}, // 14 verde claro pastel

    {150, 150, 150}, // menta metalico
    {250, 250, 250}, // menta claro brillante
    {180, 180, 180}, // menta suave



    {0x7F, 0xE8, 0xC8}, // menta metalico
    {0xB8, 0xFF, 0xE8}, // menta claro brillante
    {0x9F, 0xF2, 0xD8}, // menta suave


    // LAVANDA
{0x8F, 0x7F, 0xC8}, // lavanda metalico
{0xD8, 0xC8, 0xFF}, // lavanda claro brillante
{0xB8, 0xA8, 0xF2}, // lavanda suave

  {0xF2, 0xF4, 0xFF}, // blanco suave
{0xFF, 0xFF, 0xFF}, // blanco brillante
{0xD8, 0xDE, 0xF0}, // blanco metalico



    {0xA8, 0x5F, 0x68}, // rojo metalico
    {0xFF, 0x8F, 0xA3}, // rojo claro metalico brillante
    {0xD9, 0x74, 0x8F}, // rojo suave



    {150, 50, 50},
    {255, 150, 150},
    {255, 0, 00}, //ROJO


    {205, 20, 155},
    {255, 120, 255}, //VIOLETA
    {255, 0, 255},


    {100, 150, 100},
    {55, 250, 00},
    {70, 200, 00}, //VERDE



    { 10, 10, 170}, //azul oscuro 41
    { 10, 10, 170},//axul mas claro 42
    { 10, 10, 150}, //axul mas claro 43
    { 10, 10, 200}, // azul linea linea 44

{0xFF, 0xFF, 0xFF}, // 16 blanco texto
    {0x80, 0x80, 0x80}, // 17 gris

    {0x30, 0x30, 0x30} // 18 gris oscuro



};

tGBT_ColorRGB paletaClasica[] = {
    {0x00, 0x00, 0x00}, // 0 negro / fondo
    // LAVANDA
{0x8F, 0x7F, 0xC8}, // lavanda metalico
{0xD8, 0xC8, 0xFF}, // lavanda claro brillante
{0xB8, 0xA8, 0xF2}, // lavanda suave
    // CELESTES
    {0xB8, 0xF2, 0xFF}, // 1 celeste claro
    {0x9F, 0xE4, 0xFF}, // 2 celeste medio
    {0x8A, 0xD8, 0xFF}, // 3 celeste oscuro brillante

    // AZULES
    {0x5F, 0x7F, 0xA8}, // 4 azul metalico
    {0xB5, 0xC7, 0xFF}, // 5 azul pastel
    {0x95, 0xB8, 0xFF}, // 6 azul claro

    // NARANJAS
    {0xE0, 0x70, 0x1F}, // 7 naranja metalico
    {0xFF, 0xB8, 0x66}, // 8 naranja pastel
    {0xFF, 0x98, 0x33}, // 9 naranja claro


    // AMARILLOS
    {0xD9, 0xAA, 0x00}, // 10 amarillo metalico
    {0xFF, 0xF0, 0x00}, // 11 amarillo claro intenso
    {0xFF, 0xD8, 0x4D}, // 12 amarillo fuerte

    // VERDES pastel
    {0x7F, 0xBF, 0x95}, // 13 verde metalico pastel
    {0xC8, 0xFF, 0xD4}, // 15 verde suave brillante
    {0xA8, 0xF2, 0xB8}, // 14 verde claro pastel

    {150, 150, 150}, // menta metalico
    {250, 250, 250}, // menta claro brillante
    {180, 180, 180}, // menta suave



    {0x7F, 0xE8, 0xC8}, // menta metalico
{0xB8, 0xFF, 0xE8}, // menta claro brillante
{0x9F, 0xF2, 0xD8}, // menta suave




  {0xF2, 0xF4, 0xFF}, // blanco suave
{0xFF, 0xFF, 0xFF}, // blanco brillante
{0xD8, 0xDE, 0xF0}, // blanco metalico



    {0xA8, 0x5F, 0x68}, // rojo metalico
    {0xFF, 0x8F, 0xA3}, // rojo claro metalico brillante
    {0xD9, 0x74, 0x8F}, // rojo suave



    {150, 50, 50},
    {255, 150, 150},
    {255, 0, 00}, //ROJO


    {205, 20, 155},
    {255, 120, 255}, //VIOLETA
    {255, 0, 255},


    {100, 150, 100},
    {55, 250, 00},
    {70, 200, 00}, //VERDE
};


typedef enum
{
    PALETA_1,
    PALETA_2
} Paleta;


typedef struct
{
    Resolucion resolucion; //para guardar juego
    int ancho;
    int alto;
    int escala;
    int tamCelda;
    int tableroX;
    int tableroY;
    int scoreX;
    int scoreY;
    int tetronimosX;
    int tetronimosY;
    int nombreX;
    int nombreY;
    int nivelX;
    int nivelY;
} ConfigPantalla;

typedef struct
{
    ModoJuego modoJuego;
    int cantPiezas;
    int velocidadInicial;
    Paleta paleta;//VER SI AGREGO ENUM
    ConfigPantalla configPantalla;
} ConfigJuego;

typedef enum
{
    ESTADO_PRESENTACION,
    ESTADO_MENU_CONFIG,
    ESTADO_JUGANDO,
    ESTADO_NOMBRE
} EstadoJuego;

typedef struct
{
    ConfigJuego configJuego;
    Tablero tablero;
    Pieza actual;
    Pieza colaPiezas[PIEZAS_ENCOLADAS];
    int piezasCaidas;
    int puntuacion;
    int nivel;
    int pausado;
    int gameOver;
    double velocidadCaida;
    double velocidadFijacion;
    EstadoJuego estado;
    int tocandoSuelo;
} Juego;


typedef struct
{
    char caracter;
    uint8_t ancho;
    uint8_t alto;
    const uint8_t *arr;
    uint8_t color;
} LetraNoTipada;

void dibujarCaracter8x8(const uint8_t letra[8][8], int x, int y, int color);
typedef struct
{
    clock_t inicio;
    double intervalo;
} Temporizador;
void dibujarPiezaCola(Pieza *pieza, int xBase, int yBase, int tamCelda);
void aumentarDificultad(Temporizador *temp);
bool consumir_temporizador(Temporizador *temp);
void rotarPieza (Pieza *pieza, int sentido);
void fijar_pieza(int **tablero, Pieza *p);
void inicializarTablero(Tablero *tablero);
void reiniciarTablero(Tablero *tablero);
void** crearTablero(size_t filas, size_t columnas, size_t tamElem);
void destruirTablero(void **matriz, size_t filas);
void dibujarCaracter8x16(const uint8_t letra[16][8], int x, int y, int color);
void dibujarFondo(const ConfigPantalla *configPantalla);
void dibujarTablero(Tablero *tablero, ConfigJuego *configJuego);
void fijarPieza(Juego * juego);
int bloquesZonaInvisible(Tablero *tablero);
void imprimirTablero(Tablero *tablero);
int detectarFilasCompletas(Tablero *tablero, int filasCompletas[]);
bool lineaCompleta(int columnas, int* fila);
void borrarFilasAnimado(Juego *juego, int arr[]);
void reordenarFilas(Tablero *tablero, int arr[]);
void reiniciarFila(int colTotales, int* fila);
void gestionarJuego(Juego *juego, int filasBorradas, tGBT_Temporizador **temporizador);
void puntuacion(Juego *juego, int filasBorradas, int bajadoAMano);
#define RESOLUCION_CGA_ANCHO 320
#define RESOLUCION_CGA_ALTO 200

#define RESOLUCION_VGA_ANCHO 640
#define RESOLUCION_VGA_ALTO 480


#define ANCHO_VGA 640
#define ALTO_VGA 480
#define TAM_CELDA_VGA 20
#define ESCALA_1_VGA 1

#define ANCHO_CGA 320
#define ALTO_CGA 200
#define TAM_CELDA_CGA 8
#define ESCALA_4_CGA 4


#define CANT_FILAS_VISIBLES 20
#define CANT_FILAS_TOTALES 24
#define CANT_COL_CLASSIC 10
int validarMovimiento(Tablero * tablero, Pieza *pieza, ModoJuego modo);
int moverDerecha(int columnas, int x, ModoJuego modo);
int moverIzquierda(int columnas, int x, ModoJuego modo);
int validarCantColDeluxe(int cant_col);
Pieza crearRandom(ModoJuego modo);
void desencolarPieza(Juego *juego);
void iniciarCola(Juego * juego);
int teclaSostenida(eGBT_Tecla tecla, tGBT_Temporizador *tempTeclaSostenida);

#define MAX_RANKING 3
#define MAX_NOMBRE 10

typedef struct
{
    char nombre[MAX_NOMBRE + 1];
    int puntuacion;
    int lineas;
    int nivel;
} Estadistica;

void mostrarRanking(Estadistica ranking[], ConfigPantalla *configPantalla);
void guardarEnRanking(Estadistica ranking[], char nombre[], Juego *juego);


Estadistica ranking[MAX_RANKING] = {
    {"---", 0, 0, 0},
    {"---", 0, 0, 0},
    {"---", 0, 0, 0}
};
const Pieza PIEZAS[] = {
    {    .forma = {
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 1,
        .tipo = PIEZA_I
    },
     {

    .forma = {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 4,
        .tipo = PIEZA_J
    },
    {

    .forma = {
            {1,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 7,
        .tipo = PIEZA_L
    },
    {
           .forma = {
            {1,1,0,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 10,
        .tipo = PIEZA_O

    },
    {        .forma = {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 13,
        .tipo = PIEZA_S
    },
    {

        .forma = {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 17,
        .tipo = PIEZA_T
    },
    {

    .forma = {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 20,
        .tipo = PIEZA_Z
    },
    {

    .forma = {
            {1,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 23,
        .tipo = PIEZA_Q
    },
    {

    .forma = {
            {1,1,1,0},
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 26,
        .tipo = PIEZA_C
    },
    {

    .forma = {
            {1,1,0,0},
            {1,1,0,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 29,
        .tipo = PIEZA_P
    },
    {

    .forma = {
            {1,0,0,0},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .colorBase = 32,
        .tipo = PIEZA_D
    }
};

const uint8_t LETRA_T_VAR[] = {
    0,0,0,0,0,0,0,1,1,1,
    0,0,0,0,0,1,1,1,1,1,
    0,0,1,1,1,1,1,1,0,0,
    1,1,1,1,1,1,1,0,0,0,
    1,1,1,0,1,1,1,1,0,0,
    0,0,0,0,0,1,1,1,0,0,
    0,0,0,0,0,0,1,1,1,0,
    0,0,0,0,0,0,1,1,1,1,
    0,0,0,0,0,0,0,1,1,1,

};



const uint8_t LETRA_E_VAR[] = {
    1,1,1,1,1,
    1,0,0,0,0,
    1,1,1,1,1,
    1,1,1,1,1,
    1,0,0,0,0,
    1,1,1,1,1,
};

const uint8_t LETRA_R_VAR[] = {
    0,1,1,1,1,1,1,1,0,0,
	1,1,1,1,1,1,1,1,1,0,
    1,1,0,0,0,0,0,1,1,1,
    1,1,0,0,0,0,0,1,1,1,
    1,1,1,1,1,1,1,1,1,0,
    1,1,1,1,1,0,0,0,0,0,
    1,1,0,0,1,1,0,0,0,0,
    1,1,0,0,0,1,1,0,0,0,
    1,1,0,0,0,0,1,1,0,0,
    1,1,0,0,0,0,0,1,1,0,
    1,1,0,0,0,0,0,0,1,1,
    0,1,0,0,0,0,0,0,0,1,
};


const uint8_t LETRA_I_VAR[] = {
    1,1,1,
    1,1,1,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    1,1,1,
    1,1,1
};

const uint8_t LETRA_O_VAR[] = {
    1,1,1,1,
    1,1,1,1,
    1,0,0,1,
    1,0,0,1,
    1,0,0,1,
    1,1,1,1,
    1,1,1,1
};


const uint8_t LETRA_Q_VAR[] = {
    0,0,0,1,1,1,1,0,0,0,
    0,1,1,1,1,1,1,1,1,0,
    1,1,1,1,0,0,1,1,1,1,
    1,1,1,0,0,0,0,1,1,1,
    1,1,1,0,1,1,0,1,1,1,
    0,1,1,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,0,0,
    0,0,1,1,1,0,0,0,0,0,
    0,1,1,1,0,0,0,0,0,0,
    1,1,1,0,0,0,0,0,0,0,
    1,1,1,0,0,0,0,0,0,0
};

const uint8_t LETRA_S_VAR[] = {
    0,0,0,1,1,1,1,1,0,0,
    0,0,1,1,1,1,1,1,1,1,
    0,1,1,1,0,0,0,0,1,1,
    0,0,0,1,1,0,0,0,1,0,
    0,0,0,1,1,1,0,0,0,0,
    0,0,0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,1,1,0,
    0,0,0,1,1,1,1,1,1,0,
    1,1,1,1,1,1,1,0,0,0,
    1,1,1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0
};


const uint8_t LETRA_L_VAR[] = {
    0,0,0,1,1,1,0,0,
    0,0,1,1,1,1,0,0,
    0,0,1,1,1,0,0,0,
    0,0,1,1,1,0,0,0,
    0,1,1,1,0,0,0,0,
    0,1,1,1,0,0,0,0,
    1,1,1,1,0,0,0,0,
    1,1,1,1,1,1,0,0,
    0,0,1,1,1,1,1,1,
    0,0,0,0,0,1,1,1
};


const uint8_t LETRA_B_VAR[] = {
    0,1,1,1,1,1,1,1,0,0,
    1,1,1,1,1,1,1,1,1,0,
    1,1,0,0,0,0,0,1,1,1,
    1,1,0,0,0,0,0,1,1,1,
    1,1,1,1,1,1,1,1,0,0,
    1,1,1,1,1,1,1,1,1,0,
    1,1,0,0,0,0,0,1,1,1,
    1,1,0,0,0,0,0,1,1,1,
    1,1,0,0,0,0,0,0,1,1,
    1,1,0,0,0,0,0,0,1,1,
    1,1,0,0,0,0,0,0,1,1,
    1,1,0,0,0,0,0,1,1,0,
    1,1,1,1,1,1,1,1,0,0,
    1,1,1,1,1,1,1,1,0,0
};


const uint8_t LETRA_U_VAR[] = {
    1,1,0,1,1,
    1,1,0,1,1,
    1,1,0,1,1,
    1,1,0,1,1,
    1,1,1,1,1
};

const LetraNoTipada fuenteVar[] = {
    {'T', 10, 9, LETRA_T_VAR, 2},
    {'E', 5, 6, LETRA_E_VAR, 3},
    {'R', 10, 12, LETRA_R_VAR, 4},
    {'I', 3, 9, LETRA_I_VAR, 5},
    {'S', 10, 12, LETRA_S_VAR, 6},
    {'B', 10, 14, LETRA_B_VAR, 7},
    {'L', 8, 10, LETRA_L_VAR, 8},
    {'O', 4, 7, LETRA_O_VAR, 9},
    {'U', 5, 5, LETRA_U_VAR, 10},
    {'Q', 10, 12, LETRA_Q_VAR, 11}
};


const uint8_t LETRA_D_8x16[16][8] = {
    {1,1,1,1,1,1,0,0},
    {1,1,0,0,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,1,1,1,0},
    {1,1,1,1,1,1,0,0}
};

const uint8_t LETRA_X_8x16[16][8] = {
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {0,1,1,0,0,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1}
};


const uint8_t LETRA_A_16x8[16][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    };
    const uint8_t LETRA_B_8x16[16][8] = {
    {1,1,1,1,1,1,0,0},
    {1,0,0,1,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {1,1,1,1,1,0,0,0}
};


    const uint8_t LETRA_C_8x16[16][8] = {
    {0,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1}
};

const uint8_t LETRA_E_8x16[16][8] = {
    {0,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1}
};
const uint8_t LETRA_F_8x16[16][8] = {
    {0,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0}
};

const uint8_t LETRA_G_8x16[16][8] = {
     {0,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,0},
    {1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,1,1,1,0,0},
    {1,1,1,1,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,1,0},
    {0,0,1,1,1,1,1,0},
    {0,1,1,1,1,1,0,0}
};

const uint8_t LETRA_H_8x16[16][8] = {
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1}
};

const uint8_t LETRA_I_8x16[16][8] = {
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0}
};


const uint8_t LETRA_J_8x16[16][8] = {
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {1,0,0,1,1,0,0,0},
    {1,1,0,1,1,0,0,0},
    {0,1,1,1,0,0,0,0},
    {0,0,1,0,0,0,0,0}
};

const uint8_t LETRA_K_8x16[16][8] = {
    {1,0,0,0,0,0,1,1},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,1,1,0,0},
    {1,1,0,1,1,0,0,0},
    {1,1,1,1,0,0,0,0},
    {1,1,1,0,0,0,0,0},
    {1,1,1,1,0,0,0,0},
    {1,1,0,1,1,0,0,0},
    {1,1,0,0,1,1,0,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1}
};


const uint8_t LETRA_L_8x16[16][8] = {
    {1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {1,1,1,1,1,1,1,1}
};
const uint8_t LETRA_M_8x16[16][8] = {
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1}
};

const uint8_t LETRA_N_8x16[16][8] = {
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,0,1,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1}
};

const uint8_t LETRA_O_8x16[16][8] = {
    {1,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,0,0},
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {0,0,1,1,1,1,0,0},
    {1,1,1,1,1,1,1,1}
};

const uint8_t LETRA_P_8x16[16][8] = {
    {1,0,0,1,1,1,0,0},
    {1,1,0,0,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,1,1,1,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0}
};

const uint8_t LETRA_Q_8x16[16][8] = {
    {0,1,1,1,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {0,1,1,1,1,1,0,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,0,1,1}
};

const uint8_t LETRA_R_8x16[16][8] = {
    {1,0,0,1,1,1,0,0},
    {1,1,0,0,1,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,1,1,1,0},
    {1,1,0,1,1,1,0,0},
    {1,1,0,0,1,1,0,0},
    {1,1,0,0,0,1,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1}
};

const uint8_t LETRA_S_8x16[16][8] = {
    {0,1,1,1,1,1,1,0},
    {1,1,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0}
};

const uint8_t LETRA_T_8x16[16][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0}
};

const uint8_t LETRA_U_8x16[16][8] = {
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1}
};

const uint8_t LETRA_V_8x16[16][8] = {
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0},
    {0,1,1,0,0,0,1,1},
    {0,0,1,1,1,1,1,0},
    {0,0,0,1,1,1,0,0},
    {0,0,0,1,1,1,0,0}
};

const uint8_t LETRA_W_8x16[16][8] = {
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,0,1,1,0,1,1},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,0,0,1,0,0}
};
const uint8_t LETRA_Y_8x16[16][8] = {
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0}
};

const uint8_t LETRA_Z_8x16[16][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,0,1,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {1,1,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1}
};

const uint8_t LETRA_0_8x8[8][8] = {
    {0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,0,0},
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0}
};

const uint8_t LETRA_1_8x8[8][8] = {
    {0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,1}
};

const uint8_t LETRA_2_8x8[8][8] = {
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,0,0},
    {1,1,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1}
};

const uint8_t LETRA_3_8x8[8][8] = {
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,0,0,0,0,1,1},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0}
};

const uint8_t LETRA_4_8x8[8][8] = {
    {0,0,0,0,0,1,1,1},
    {0,0,0,0,1,1,1,1},
    {0,0,0,1,1,0,1,1},
    {0,0,1,1,0,0,1,1},
    {0,1,1,1,1,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,1}
};

const uint8_t LETRA_5_8x8[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,0},
    {0,0,0,1,1,1,1,1},
    {0,0,0,0,0,0,1,1},
    {0,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0}
};

const uint8_t LETRA_6_8x8[8][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,1},
    {0,0,1,1,1,1,1,1},
    {0,1,0,0,0,0,1,1},
    {1,1,0,0,0,0,0,1},
    {0,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0}
};

const uint8_t LETRA_7_8x8[8][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,1}
};

const uint8_t LETRA_8_8x8[8][8] = {
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
};


const uint8_t LETRA_9_8x8[8][8] = {
    {0,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,0,0,0,0,1,1},
    {0,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,1},
};

const uint8_t LETRA_GUION_8x16[16][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

void dibujarLetraVar(const LetraNoTipada *letra, int x, int y);
void dibujarPiezaMovimiento(Juego *juego);
void dibujarTexto8x8(const char *texto, int x, int y, int color);
void dibujarPieza(int tamCelda, int x, int y, int colorBase);

void inicializarPartida(Juego *juego, ConfigPantalla *configPantalla, int vel_inicial);
int main(int argc, char *argv[])


{

char *res = "CGA";
if(argc > 1)
{
    res = argv[1];
} else {
    return -1;
}
      ModoJuego modoElegido = MODO_CLASICO;
        Paleta paletaElegida = PALETA_1;
        Resolucion resolucionElegida = RESOLUCION_CGA;
        int velocidadElegida = 1000;
        int columnasDeluxe = CANT_COL_CLASSIC;


    int opcionMenu = 0;
    char nombreJugador[11] = "";
    int posNombre = 0;

    char textoPuntaje[11];


    Juego juego = {0};
    juego.configJuego.modoJuego = MODO_CLASICO;
    juego.configJuego.paleta = PALETA_1;
    juego.configJuego.velocidadInicial = 1000;
    juego.configJuego.configPantalla.resolucion = RESOLUCION_CGA;
    juego.tablero.columnas = CANT_COL_CLASSIC;
    juego.estado = ESTADO_PRESENTACION;


    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }


    if(strcmp(res, RES_CGA) == 0)
    {
        juego.configJuego.configPantalla.resolucion = RESOLUCION_CGA;
        juego.configJuego.configPantalla.ancho = ANCHO_CGA;
        juego.configJuego.configPantalla.alto = ALTO_CGA;
        juego.configJuego.configPantalla.tamCelda = TAM_CELDA_CGA;
        juego.configJuego.configPantalla.escala = ESCALA_4_CGA;

    } else {

        juego.configJuego.configPantalla.resolucion = RESOLUCION_VGA;
        juego.configJuego.configPantalla.ancho = ANCHO_VGA;
        juego.configJuego.configPantalla.alto = ALTO_VGA;
        juego.configJuego.configPantalla.tamCelda = TAM_CELDA_VGA;
        juego.configJuego.configPantalla.escala = ESCALA_1_VGA;
    }

    ConfigPantalla configPantalla = juego.configJuego.configPantalla;

    if (gbt_crear_ventana(PANTALLA_JUEGO, configPantalla.ancho, configPantalla.alto, configPantalla.escala) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if((juego.configJuego.paleta) == PALETA_1)
    {
        if (gbt_aplicar_paleta(paletaPastel, sizeof(paletaPastel)/sizeof(paletaPastel[0]), GBT_FORMATO_888) != 0) {
            fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
            return -1;
        }
    } else {
        if (gbt_aplicar_paleta(paletaClasica, sizeof(paletaClasica)/sizeof(paletaClasica[0]) , GBT_FORMATO_888) != 0) {
            fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
            return -1;
        }
    }

    int corriendo = 1;

    srand(time(NULL));

    tGBT_Temporizador *temporizador = NULL;
    tGBT_Temporizador *tempTeclaSostenida = NULL;
    tGBT_Temporizador *tempCaida = NULL;
    tGBT_Temporizador *tempFijacion = NULL;
     while(corriendo)
    {
        gbt_procesar_entrada();

        if(gbt_tecla_presionada(GBTK_ESCAPE))
        {
            corriendo = 0;
        }

        if(juego.estado == ESTADO_PRESENTACION)
        {
            dibujarFondo(&configPantalla);

            dibujarTextoVariable("BLOQUEO", configPantalla.ancho / 2 - configPantalla.tamCelda * 4, configPantalla.alto /2 - configPantalla.tamCelda * 9);
            dibujarTextoVariable("TETRIS", configPantalla.ancho / 2 - configPantalla.tamCelda * 4, configPantalla.alto /2 - configPantalla.tamCelda * 6);

            dibujarTexto8x8("TOP 3", configPantalla.ancho /2 - configPantalla.tamCelda * 4, configPantalla.alto /2 - configPantalla.tamCelda * 3, 30);
            mostrarRanking(ranking, &configPantalla);
            dibujarTexto8x8("ENTER", configPantalla.ancho /2 - configPantalla.tamCelda * 4, configPantalla.alto - configPantalla.tamCelda * 3, 16);
            if(gbt_tecla_presionada(GBTK_ENTER))
            {
                juego.estado = ESTADO_MENU_CONFIG;

            }
            gbt_volcar_backbuffer();
            gbt_esperar(16);
            continue;
        }
            if(juego.estado == ESTADO_MENU_CONFIG)
{
    if(gbt_tecla_presionada(GBTK_ARRIBA) && opcionMenu > 0)
        opcionMenu--;

    if(gbt_tecla_presionada(GBTK_ABAJO))
    {
        if(modoElegido == MODO_DELUXE)
        {
            if(opcionMenu < 5)
                opcionMenu++;
        }
        else
        {
            if(opcionMenu < 4)
                opcionMenu++;
        }
    }

    if(gbt_tecla_presionada(GBTK_DERECHA))
    {
        if(opcionMenu == 0)
            modoElegido = MODO_DELUXE;

        if(opcionMenu == 1)
            paletaElegida = PALETA_2;

        if(opcionMenu == 2)
            resolucionElegida = RESOLUCION_VGA;

        if(opcionMenu == 3)
            velocidadElegida += 100;

        if(opcionMenu == 4 && columnasDeluxe < MAX_COL_DELUXE)
            columnasDeluxe++;
    }

    if(gbt_tecla_presionada(GBTK_IZQUIERDA))
    {
        if(opcionMenu == 0)
            modoElegido = MODO_CLASICO;

        if(opcionMenu == 1)
            paletaElegida = PALETA_1;

        if(opcionMenu == 2)
            resolucionElegida = RESOLUCION_CGA;

        if(opcionMenu == 3 && velocidadElegida > 100)
            velocidadElegida -= 100;

        if(opcionMenu == 4 && columnasDeluxe > MIN_COL_DELUXE)
            columnasDeluxe--;
    }

    if(gbt_tecla_presionada(GBTK_ENTER))
    {
        juego.configJuego.modoJuego = modoElegido;
        juego.configJuego.paleta = paletaElegida;
        juego.configJuego.velocidadInicial = velocidadElegida;
        juego.configJuego.configPantalla.resolucion = resolucionElegida;

        if(modoElegido == MODO_CLASICO)
            juego.tablero.columnas = CANT_COL_CLASSIC;
        else
            juego.tablero.columnas = columnasDeluxe;


        if(juego.tablero.matriz)
        {
            destruirMatriz((void**)juego.tablero.matriz, juego.tablero.filTotales);
            juego.tablero.matriz = NULL;
        }

        inicializarPartida(&juego, &configPantalla, juego.configJuego.velocidadInicial);

        if(temporizador)
            gbt_temporizador_destruir(temporizador);

        if(tempTeclaSostenida)
            gbt_temporizador_destruir(tempTeclaSostenida);

        if(tempCaida)
            gbt_temporizador_destruir(tempCaida);

        if(tempFijacion)
            gbt_temporizador_destruir(tempFijacion);

        temporizador = gbt_temporizador_crear(juego.velocidadCaida);
        tempTeclaSostenida = gbt_temporizador_crear(0.3);
        tempCaida = gbt_temporizador_crear(0.01);
        tempFijacion = gbt_temporizador_crear(0.1);


        if(!temporizador || !tempTeclaSostenida || !tempCaida || !tempFijacion)
        {
            fprintf(stderr, "Error al crear temporizadores: %s\n", gbt_obtener_log());
            return -1;
        }
        juego.tocandoSuelo = 0;
        juego.estado = ESTADO_JUGANDO;
    }

    gbt_borrar_backbuffer(0);

    dibujarFondo(&configPantalla);

    int inicioY = configPantalla.alto / 4;
    int separacion = configPantalla.tamCelda * 3;

    int xLabel = configPantalla.ancho / 6;
    int xValor = configPantalla.ancho / 2 - configPantalla.tamCelda;

    dibujarTexto8x8("MENU", configPantalla.ancho / 2 - configPantalla.tamCelda * 4, inicioY - separacion, 30);
    dibujarTexto8x8("MODO", xLabel, inicioY, opcionMenu == 0 ? 30 : 16);
    dibujarTexto8x8(modoElegido == MODO_CLASICO ? "CLASSIC" : "DELUXE", xValor, inicioY, 30);
    dibujarTexto8x8("PALETA", xLabel, inicioY + separacion, opcionMenu == 1 ? 30 : 16);
    dibujarTexto8x8(paletaElegida == PALETA_1 ? "1" : "2", xValor, inicioY + separacion, 30);
    dibujarTexto8x8("RES", xLabel, inicioY + separacion * 2, opcionMenu == 2 ? 30 : 16);
    dibujarTexto8x8(resolucionElegida == RESOLUCION_CGA ? "CGA" : "VGA", xValor, inicioY + separacion * 2, 30);
    dibujarTexto8x8("VEL", xLabel, inicioY + separacion * 3, opcionMenu == 3 ? 30 : 16);

    char textoVel[20];
    sprintf(textoVel, "%d", velocidadElegida);

    dibujarTexto8x8(textoVel, xValor, inicioY + separacion * 3, 30);

    if(modoElegido == MODO_DELUXE)
    {
        dibujarTexto8x8("COLUMNAS", xLabel, inicioY + separacion * 4, opcionMenu == 4 ? 30 : 16);

        char textoCol[10];
        sprintf(textoCol, "%d", columnasDeluxe);

        dibujarTexto8x8(textoCol, xValor, inicioY + separacion * 4, 30);

        dibujarTexto8x8("JUGAR", configPantalla.ancho / 2 - configPantalla.tamCelda * 4, inicioY + separacion * 5, opcionMenu == 5 ? 30 : 16);
    }
    else
    {
        dibujarTexto8x8("JUGAR", configPantalla.ancho / 2 - configPantalla.tamCelda * 4, inicioY + separacion * 4, opcionMenu == 4 ? 30 : 16);
    }
        gbt_volcar_backbuffer();
    gbt_esperar(16);

    continue;
    }



    int bajadoAMano = 0;

    if(juego.estado == ESTADO_JUGANDO)
    {
        if(juego.gameOver)
        {
            gbt_temporizador_pausar(temporizador);
            gbt_temporizador_pausar(tempCaida);
            gbt_temporizador_pausar(tempTeclaSostenida);
            gbt_temporizador_pausar(tempFijacion);

            gbt_borrar_backbuffer(0);

            dibujarFondo(&configPantalla);

            dibujarTexto8x8("GAME OVER", configPantalla.ancho / 2 - configPantalla.tamCelda * 6,
                configPantalla.alto / 2 - configPantalla.tamCelda * 7, 30);


            sprintf(textoPuntaje, "%09d", juego.puntuacion);

            dibujarTexto8x8(textoPuntaje,configPantalla.ancho / 2 - configPantalla.tamCelda * 6,
                configPantalla.alto / 2 - configPantalla.tamCelda, 30);

            dibujarTexto8x8("GUARGAR PARTIDA",
                configPantalla.ancho / 2 - configPantalla.tamCelda * 10,
                configPantalla.alto / 2 + configPantalla.tamCelda * 3, 16);

            dibujarTexto8x8("ESC SALIR", configPantalla.ancho / 2 - configPantalla.tamCelda * 6,
                configPantalla.alto / 2 + configPantalla.tamCelda * 6,
                16);

              if(gbt_tecla_presionada(GBTK_ENTER))
                {
                    nombreJugador[0] = '\0';
                    posNombre = 0;
                    juego.estado = ESTADO_NOMBRE;
                }

            if(gbt_tecla_presionada(GBTK_ESCAPE))
            {
                corriendo = 0;
            }

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }



    if(gbt_tecla_presionada(GBTK_p) || gbt_tecla_presionada(GBTK_ESPACIO))
        {
            juego.pausado = !juego.pausado;

            if(juego.pausado)
            {
                gbt_temporizador_pausar(temporizador);
                gbt_temporizador_pausar(tempCaida);
                gbt_temporizador_pausar(tempTeclaSostenida);
                gbt_temporizador_pausar(tempFijacion);
            }
            else
            {
                gbt_temporizador_reanudar(temporizador);
                gbt_temporizador_reanudar(tempCaida);
                gbt_temporizador_reanudar(tempTeclaSostenida);
                gbt_temporizador_reanudar(tempFijacion);
            }
        }

        if(juego.pausado)
        {
            gbt_borrar_backbuffer(0);

            dibujarFondo(&configPantalla);

            dibujarTablero(&juego.tablero, &juego.configJuego);
            dibujarPiezaMovimiento(&juego);


            dibujarTexto8x8("PAUSA", configPantalla.ancho / 2 - configPantalla.tamCelda * 3 ,
                            configPantalla.alto / 2 - configPantalla.tamCelda, 30);
            gbt_volcar_backbuffer();
            gbt_esperar(16);

            continue;
        }

        Pieza piezaCopia = juego.actual;
        if(teclaSostenida(GBTK_DERECHA, tempTeclaSostenida) || teclaSostenida(GBTK_DERECHA, tempTeclaSostenida))
        {
            piezaCopia.x = moverDerecha(juego.tablero.columnas, piezaCopia.x, juego.configJuego.modoJuego);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                juego.actual = piezaCopia;
        }
        else if(gbt_tecla_presionada(GBTK_IZQUIERDA)|| teclaSostenida(GBTK_IZQUIERDA, tempTeclaSostenida))
        {
            piezaCopia.x = moverIzquierda(juego.tablero.columnas, piezaCopia.x, juego.configJuego.modoJuego);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                juego.actual = piezaCopia;
        }
        else if(gbt_tecla_presionada(GBTK_ABAJO)|| teclaSostenida(GBTK_ABAJO, tempCaida))
        {
            piezaCopia.y++;
            bajadoAMano++;
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                juego.actual = piezaCopia;
        }
        else if (gbt_tecla_presionada(GBTK_z))
        {
            rotarPieza(&piezaCopia, ROTAR_ANTIHORARIO);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                juego.actual = piezaCopia;
        }
        else if (gbt_tecla_presionada(GBTK_x))
        {
            rotarPieza(&piezaCopia, ROTAR_HORARIO);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                juego.actual = piezaCopia;
        }

         if(gbt_temporizador_consumir(temporizador))
        {
            static int contadorLento = 0;
            int ejecutarCaida = 1;

            if(gbt_tecla_sostenida(GBTK_ARRIBA))
            {
                contadorLento++;

                if(contadorLento < 3)
                    ejecutarCaida = 0;
                else
                    contadorLento = 0;
            }
            else
            {
                contadorLento = 0;
            }

            if(ejecutarCaida)
            {
                piezaCopia = juego.actual;
                piezaCopia.y++;
                if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                {
                    juego.actual = piezaCopia;
                    juego.tocandoSuelo = 0;
                } else
                {
                    if(!juego.tocandoSuelo)
                    {
                        juego.tocandoSuelo = 1;

                        gbt_temporizador_destruir(tempFijacion);

                        tempFijacion = gbt_temporizador_crear(0.1);
                        if(!tempFijacion)
                        {
                            fprintf(stderr, "Error al crear temporizador de fijacion: %s\n", gbt_obtener_log());
                            return -1;
                        }
                    }

                    if(gbt_temporizador_consumir(tempFijacion))
                    {
                        fijarPieza(&juego);
                        juego.tocandoSuelo = 0;
                        if(bloquesZonaInvisible(&juego.tablero)){
                             juego.gameOver = 1;
                            continue;
                        }
                        int arr[juego.tablero.filVisibles];
                        int filasBorradas = detectarFilasCompletas(&juego.tablero, arr);

                        if(filasBorradas > 0)
                        {
                            borrarFilasAnimado(&juego, arr);
                            reordenarFilas(&juego.tablero, arr);
                            puntuacion(&juego, filasBorradas, bajadoAMano);
                            bajadoAMano = 0;
                        }

                        gestionarJuego(&juego, filasBorradas, &temporizador);

                        desencolarPieza(&juego);
                        tempFijacion = gbt_temporizador_crear(0.1);

                        if(!tempFijacion)
                        {
                            fprintf(stderr, "Error al crear temporizador de fijacion: %s\n", gbt_obtener_log());
                            return -1;
                        }

                        if(!validarMovimiento(&juego.tablero, &juego.actual, juego.configJuego.modoJuego))
                            {
                                juego.gameOver = 1;
                                continue;
                            }
                    }
                }
            }
        }
        gbt_borrar_backbuffer(0);

        dibujarFondo(&configPantalla);



        int xCola = configPantalla.tamCelda * 2;
        int yCola = configPantalla.tableroY;
        int xPuntaje = configPantalla.tableroX + juego.tablero.columnas * configPantalla.tamCelda;
        int yPuntaje = configPantalla.tableroY- configPantalla.tamCelda *2;


        int altoCola = PIEZAS_ENCOLADAS * 4 * configPantalla.tamCelda + configPantalla.tamCelda;
        int anchoCola = configPantalla.tamCelda * 6;

        int altoPuntaje = configPantalla.tamCelda * 2;
        int anchoPuntaje = configPantalla.tamCelda * 14;

        dibujarBloque(xCola, yCola, altoCola, anchoCola, 0);
        dibujarBloque(xPuntaje, yPuntaje , altoPuntaje, anchoPuntaje, 0);


            for(int i = 0; i < PIEZAS_ENCOLADAS; i++)
                {
                    dibujarPiezaCola(&juego.colaPiezas[i], xCola + configPantalla.tamCelda,
                        yCola + configPantalla.tamCelda + (i * 4 * configPantalla.tamCelda),
                        configPantalla.tamCelda);
                }

        sprintf(textoPuntaje, "%09d", juego.puntuacion %1000000000);
        dibujarTexto8x8(textoPuntaje, xPuntaje + configPantalla.tamCelda/2 , yPuntaje+configPantalla.tamCelda /2 ,30);
        dibujarTextoVariable("BLOQUEO", xPuntaje + configPantalla.tamCelda * 2, yPuntaje + configPantalla.tamCelda * 5);
        dibujarTextoVariable("TETRIS", xPuntaje + configPantalla.tamCelda * 2, yPuntaje + configPantalla.tamCelda * 8);
        dibujarTablero(&juego.tablero, &juego.configJuego);
        dibujarPiezaMovimiento(&juego);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }


    if(juego.estado == ESTADO_NOMBRE)
        {
            eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

            if(tecla >= GBTK_a && tecla <= GBTK_z && posNombre < 10)
            {
                nombreJugador[posNombre] = tecla - 32;
                posNombre++;
                nombreJugador[posNombre] = '\0';
            }

            if(tecla >= GBTK_0 && tecla <= GBTK_9 && posNombre < 10)
            {
                nombreJugador[posNombre] = tecla;
                posNombre++;
                nombreJugador[posNombre] = '\0';
            }

            if(gbt_tecla_presionada(GBTK_RETROCESO) && posNombre > 0)
            {
                posNombre--;
                nombreJugador[posNombre] = '\0';
            }


            if(gbt_tecla_presionada(GBTK_ENTER) && posNombre > 0)
            {
                guardarEnRanking(ranking, nombreJugador, &juego);

                destruirMatriz((void**)juego.tablero.matriz, juego.tablero.filTotales);

                juego.tablero.matriz = NULL;

                juego.gameOver = 0;
                juego.pausado = 0;

                opcionMenu = 0;

                juego.estado = ESTADO_MENU_CONFIG;

                continue;
            }


            gbt_borrar_backbuffer(0);

            dibujarFondo(&configPantalla);

            dibujarTexto8x8("INGRESE NOMBRE", configPantalla.ancho / 2 - 90,
                configPantalla.alto / 2 - 50, 30);

            dibujarTexto8x8(nombreJugador, configPantalla.ancho / 2 - 50,
                configPantalla.alto / 2, 30);


            dibujarTexto8x8("ENTER GUARDAR", configPantalla.ancho / 2 - 90,
                configPantalla.alto / 2 + 50, 16);

            gbt_volcar_backbuffer();
            gbt_esperar(16);
            continue;
        }
    }


    if(!juego.tablero.matriz)
        destruirMatriz((void**)juego.tablero.matriz, juego.tablero.filTotales);
    gbt_temporizador_destruir(temporizador);
    gbt_temporizador_destruir(tempCaida);
    gbt_temporizador_destruir(tempTeclaSostenida);
    gbt_temporizador_destruir(tempFijacion);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}


void mostrarRanking(Estadistica ranking[], ConfigPantalla *configPantalla)
{
    int inicioY = configPantalla->alto / 2;
    int separacion = configPantalla->tamCelda * 3;

    for(int i = 0; i < MAX_RANKING; i++)
    {
        char textoPuntaje[11];

        snprintf(textoPuntaje,sizeof(textoPuntaje),"%09d", ranking[i].puntuacion % 1000000000);

        int y = inicioY + i * separacion;

        dibujarTexto8x8(ranking[i].nombre,configPantalla->ancho / 3, y, 30);

        dibujarTexto8x8( textoPuntaje, configPantalla->ancho / 2, y, 30);
    }
}

void guardarEnRanking(Estadistica ranking[], char nombre[], Juego *juego)
{
    Estadistica nueva;

    strcpy(nueva.nombre, nombre);
    nueva.puntuacion = juego->puntuacion;
    nueva.nivel = juego->nivel;
    nueva.lineas = 0;

    ranking[MAX_RANKING - 1] = nueva;

    for(int i = 0; i < MAX_RANKING - 1; i++)
    {
        for(int j = i + 1; j < MAX_RANKING; j++)
        {
            if(ranking[j].puntuacion > ranking[i].puntuacion)
            {
                Estadistica aux = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = aux;
            }
        }
    }
}

void dibujarTexto8x8(const char *texto, int x, int y, int color)
{
    while(*texto)
    {
        switch(*texto)
        {
            case '-':
                dibujarCaracter8x16(LETRA_GUION_8x16, x, y, color);
                break;

            case 'A':
                dibujarCaracter8x16(LETRA_A_16x8, x, y, color);
                break;
            case 'B':
                dibujarCaracter8x16(LETRA_B_8x16, x, y, color);
                break;
            case 'C':
                dibujarCaracter8x16(LETRA_C_8x16, x, y, color);
                break;
            case 'D':
                dibujarCaracter8x16(LETRA_D_8x16, x, y, color);
                break;
            case 'E':
                dibujarCaracter8x16(LETRA_E_8x16, x, y, color);
                break;
            case 'F':
                dibujarCaracter8x16(LETRA_F_8x16, x, y, color);
                break;
            case 'G':
                dibujarCaracter8x16(LETRA_G_8x16, x, y, color);
                break;
            case 'H':
                dibujarCaracter8x16(LETRA_H_8x16, x, y, color);
                break;
            case 'I':
                dibujarCaracter8x16(LETRA_I_8x16, x, y, color);
                break;
            case 'J':
                dibujarCaracter8x16(LETRA_J_8x16, x, y, color);
                break;
            case 'K':
                dibujarCaracter8x16(LETRA_K_8x16, x, y, color);
                break;
            case 'L':
                dibujarCaracter8x16(LETRA_L_8x16, x, y, color);
                break;
            case 'M':
                dibujarCaracter8x16(LETRA_M_8x16, x, y, color);
                break;
            case 'N':
                dibujarCaracter8x16(LETRA_N_8x16, x, y, color);
                break;
            case 'O':
                dibujarCaracter8x16(LETRA_O_8x16, x, y, color);
                break;
            case 'P':
                dibujarCaracter8x16(LETRA_P_8x16, x, y, color);
                break;
            case 'Q':
                dibujarCaracter8x16(LETRA_Q_8x16, x, y, color);
                break;
            case 'R':
                dibujarCaracter8x16(LETRA_R_8x16, x, y, color);
                break;
            case 'S':
                dibujarCaracter8x16(LETRA_S_8x16, x, y, color);
                break;
            case 'T':
                dibujarCaracter8x16(LETRA_T_8x16, x, y, color);
                break;
            case 'U':
                dibujarCaracter8x16(LETRA_U_8x16, x, y, color);
                break;
            case 'V':
                dibujarCaracter8x16(LETRA_V_8x16, x, y, color);
                break;
            case 'W':
                dibujarCaracter8x16(LETRA_W_8x16, x, y, color);
                break;
            case 'X':
                dibujarCaracter8x16(LETRA_X_8x16, x, y, color);
                break;
            case 'Y':
                dibujarCaracter8x16(LETRA_Y_8x16, x, y, color);
                break;
            case 'Z':
                dibujarCaracter8x16(LETRA_Z_8x16, x, y, color);
                break;
            case '0':
                dibujarCaracter8x8(LETRA_0_8x8, x, y, color);
                break;
            case '1':
                dibujarCaracter8x8(LETRA_1_8x8, x, y, color);
                break;
            case '2':
                dibujarCaracter8x8(LETRA_2_8x8, x, y, color);
                break;
            case '3':
                dibujarCaracter8x8(LETRA_3_8x8, x, y, color);
                break;
            case '4':
                dibujarCaracter8x8(LETRA_4_8x8, x, y, color);
                break;
            case '5':
                dibujarCaracter8x8(LETRA_5_8x8, x, y, color);
                break;
            case '6':
                dibujarCaracter8x8(LETRA_6_8x8, x, y, color);
                break;
            case '7':
                dibujarCaracter8x8(LETRA_7_8x8, x, y, color);
                break;
            case '8':
                dibujarCaracter8x8(LETRA_8_8x8, x, y, color);
                break;
            case '9':
                dibujarCaracter8x8(LETRA_9_8x8, x, y, color);
                break;
            case ' ':
                break;

        }

        x += 12;

        texto++;
    }
}

void dibujarTextoVariable(const char *texto, int x, int y)
{
    while(*texto)
    {
        for(int i = 0;
            i < sizeof(fuenteVar)/sizeof(fuenteVar[0]);
            i++)
        {
            if(fuenteVar[i].caracter == *texto)
            {
                dibujarLetraVar(&fuenteVar[i], x, y);

                x += fuenteVar[i].ancho + 2;

                break;
            }
        }

        texto++;
    }
}

void gestionarJuego(Juego *juego, int filasBorradas, tGBT_Temporizador **temporizador)
{
   juego->piezasCaidas++;

    if(juego->piezasCaidas % 5 == 0)
    {
        juego->nivel++;
        juego->velocidadCaida *= AUMENTO_DIFICULDAD;

        if(juego->velocidadCaida < MAX_DIFICULTAD)
            juego->velocidadCaida = MAX_DIFICULTAD;

        juego->velocidadFijacion = juego->velocidadCaida / 2.0;
        gbt_temporizador_destruir(*temporizador);
        *temporizador = gbt_temporizador_crear(juego->velocidadCaida);

        if(!*temporizador)
        {
            fprintf(stderr, "Error recreando temporizador: %s\n", gbt_obtener_log());
            exit(EXIT_FAILURE);
        }
    }
}

void puntuacion(Juego *juego, int filasBorradas, int bajadoAMano)
{
    int p = 0;
    if(filasBorradas<5){
        p = puntaje[filasBorradas];
    }
    double velocidad = 1.0 / juego->velocidadCaida;
    juego->puntuacion += ((p * juego->nivel + bajadoAMano) * velocidad);
}

int teclaSostenida(eGBT_Tecla tecla, tGBT_Temporizador *tempTeclaSostenida)
{
    if(!gbt_tecla_sostenida(tecla))
        return 0;

    return gbt_temporizador_consumir(tempTeclaSostenida);
}
void inicializarPartida(Juego *juego, ConfigPantalla *configPantalla, int vel_inicial)
{
    juego->tablero.filVisibles = CANT_FILAS_VISIBLES;
    juego->tablero.filTotales = CANT_FILAS_TOTALES;

    juego->tablero.matriz = (int**) crearMatriz(juego->tablero.filTotales,
        juego->tablero.columnas, sizeof(int));

    if(!juego->tablero.matriz)
    {
        fprintf(stderr, "Error al crear la matriz del tablero\n");
        exit(EXIT_FAILURE);
    }

    int anchoTablero =
        juego->tablero.columnas * configPantalla->tamCelda;

    int altoTablero =
        juego->tablero.filVisibles * configPantalla->tamCelda;

    configPantalla->tableroX =
        (configPantalla->ancho - anchoTablero) / 2;

    configPantalla->tableroY =
        (configPantalla->alto - altoTablero) / 2;

    juego->configJuego.configPantalla = *configPantalla;
    reiniciarTablero(&juego->tablero);
    iniciarCola(juego);
    desencolarPieza(juego);
    juego->configJuego.velocidadInicial = vel_inicial;
    juego->velocidadCaida = vel_inicial / 1000.0;
    juego->velocidadFijacion = juego->velocidadCaida / 2.0;
    juego->nivel = 1;
    juego->puntuacion = 0;
    juego->piezasCaidas = 0;
    juego->pausado = 0;
    juego->gameOver = 0;
}

void dibujarTablero(Tablero *tablero, ConfigJuego *configJuego)
{
    int filaImprimir = tablero->filTotales - tablero->filVisibles;
    int tamCelda = configJuego->configPantalla.tamCelda;
    for(int fila = 0; fila < tablero->filVisibles; fila++)
    {
        for(int col = 0; col < tablero->columnas; col++)
        {
            int xBase =
                configJuego->configPantalla.tableroX +
                col * tamCelda;

            int yBase =
                configJuego->configPantalla.tableroY +
                fila * tamCelda;

            dibujarPieza(tamCelda, xBase, yBase, tablero->matriz[fila+filaImprimir][col]);
        }
    }
}



void dibujarLetraVar(const LetraNoTipada *letra, int x, int y)
{
    for(int fila = 0; fila < letra->alto; fila++)
    {
        for(int col = 0; col < letra->ancho; col++)
        {
            int pos = fila * letra->ancho + col;

            if(letra->arr[pos])
                gbt_dibujar_pixel(x + col, y + fila, letra->color);
        }
    }
}

void dibujarPiezaMovimiento(Juego *juego)
{
    int inicioVisible = juego->tablero.filTotales - juego->tablero.filVisibles;
    for(int fila = 0; fila < 4; fila++)
    {
        for(int col = 0; col < 4; col++)
        {
            if(juego->actual.forma[fila][col] != 0)
            {
                int filaReal = juego->actual.y + fila;
                int colReal = juego->actual.x + col;

                if(filaReal < inicioVisible)
                    continue;

                colReal = (colReal + juego->tablero.columnas) % juego->tablero.columnas;

                int x = juego->configJuego.configPantalla.tableroX +
                    colReal * juego->configJuego.configPantalla.tamCelda;
                int y = juego->configJuego.configPantalla.tableroY +
                    (filaReal - inicioVisible) * juego->configJuego.configPantalla.tamCelda;
                dibujarPieza(juego->configJuego.configPantalla.tamCelda, x, y, juego->actual.colorBase);

            }
        }
    }
}

void dibujarPiezaCola(Pieza *pieza, int xBase, int yBase, int tamCelda)
{
    for(int fila = 0; fila < 4; fila++)
    {
        for(int col = 0; col < 4; col++)
        {
            if(pieza->forma[fila][col] != 0)
            {
                int x = xBase + col * tamCelda;
                int y = yBase + fila * tamCelda;

                dibujarPieza(tamCelda, x, y, pieza->colorBase);
            }
        }
    }
}

void dibujarPieza(int tamCelda, int x, int y, int colorBase)
{
    int brillo = tamCelda / 2 + 1;
    for(int fila = 0, j = brillo; fila < tamCelda; fila++, j--)
    {
        for(int col = 0; col < tamCelda; col++)
        {
            if(colorBase == 0){
                gbt_dibujar_pixel(x + col, y + fila, colorBase);
            } else {
                if(fila == 0 || fila == tamCelda - 1 ||
                    col == 0 || col == tamCelda - 1)
                    {
                        gbt_dibujar_pixel(x + col, y + fila, colorBase);
                    } else if(fila > 0 && col > 0 && col < j) {

                        gbt_dibujar_pixel(x + col, y + fila, colorBase + 1);
                    } else {
                        gbt_dibujar_pixel(x + col, y + fila, colorBase + 2);
                    }
            }
        }
    }

}

void dibujarBloque(int x, int y, int alto, int ancho, int color)
{

    for(int fila = 0; fila < alto; fila++)
    {
        for(int col = 0; col < ancho; col++)
        {
            gbt_dibujar_pixel(x + col, y + fila, color);
        }
    }
}





int validarMovimiento(Tablero * tablero, Pieza *pieza, ModoJuego modo)
{
    for(int f = 0; f < 4; f++)
    {
        for(int c = 0; c < 4; c++)
        {
            if(pieza->forma[f][c] != 0)
            {
                int col = pieza->x + c;
                int fila = pieza->y + f;

                if(fila < 0 || fila >= tablero->filTotales)
                    return 0;

                if(modo == MODO_CLASICO)
                {
                    if(col < 0 || col >= tablero->columnas)
                        return 0;
                } else {
                    if(col < 0 || col >= tablero->columnas){
                        col = (col + tablero->columnas) % tablero->columnas;
                    }
                }

                if(tablero->matriz[fila][col] != 0)
                    return 0;
            }
        }
    }
    return 1;
}



void fijarPieza(Juego * juego)
{
    for(int fil = 0; fil < 4; fil ++)
    {
        for(int col = 0; col < 4; col++)
        {
            int columna = juego->actual.x + col;
            if(columna < 0 || columna > juego->tablero.columnas-1){
                 columna = (columna + juego->tablero.columnas) % juego->tablero.columnas;
            }
            if(juego->actual.forma[fil][col] != 0)
               juego->tablero.matriz[juego->actual.y + fil][columna] = juego->actual.colorBase;
        }
    }
}

int bloquesZonaInvisible(Tablero *tablero)
{
    int inicioVisible = tablero->filTotales - tablero->filVisibles;

    for(int fila = inicioVisible - 1; fila >= 0; fila--)
    {
        for(int col = 0; col < tablero->columnas; col++)
        {
            if(tablero->matriz[fila][col] != 0)
                return 1;
        }
    }

    return 0;
}









////////////////////////////////////////////
void rotarPieza(Pieza *pieza, int sentido)
{
    int nuevaForma[4][4];

   for(int i = 0; i < 4; i++){
     for(int j = 0 ;j < 4; j++){
        if(ROTAR_ANTIHORARIO == sentido){
            nuevaForma[3-j][i] = pieza->forma[i][j];
        }else{
            nuevaForma[j][3-i] = pieza->forma[i][j];
        }
     }
   }

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            pieza->forma[i][j] = nuevaForma[i][j];
        }
    }
    int minCol = 4;
    int minFila = 4;

    for(int f = 0; f < 4; f++)
    {
        for(int c = 0; c < 4; c++)
        {
            if(pieza->forma[f][c])
            {
                if(c < minCol)
                    minCol = c;

                if(f < minFila)
                    minFila = f;
            }
        }
    }

    while(minCol > 0)
    {
        for(int f = 0; f < 4; f++)
        {
            for(int c = 0; c < 3; c++)
            {
                pieza->forma[f][c] = pieza->forma[f][c + 1];
            }

            pieza->forma[f][3] = 0;
        }

        minCol--;
    }

    while(minFila > 0)
    {
        for(int f = 0; f < 3; f++)
        {
            for(int c = 0; c < 4; c++)
            {
                pieza->forma[f][c] = pieza->forma[f + 1][c];
            }
        }

        for(int c = 0; c < 4; c++)
        {
            pieza->forma[3][c] = 0;
        }

        minFila--;
    }
}

int moverDerecha(int columnas, int x, ModoJuego modo)
{
    if(modo == MODO_CLASICO)
        return x + 1;
    return (x + 1) % columnas;
}

int moverIzquierda(int columnas, int x, ModoJuego modo)
{
    if(modo == MODO_CLASICO)
        return x - 1;
    return (x - 1 + columnas) % columnas;
}


////////////////////////////////////////////////////

void iniciarCola(Juego * juego)
{
    for(int p = 0; p < PIEZAS_ENCOLADAS; p++)
    {
        juego->colaPiezas[p] = crearRandom(juego->configJuego.modoJuego);
    }
}

Pieza crearRandom(ModoJuego modo)
{
    if(modo==MODO_CLASICO)
    {
        return PIEZAS[rand() % CANT_PIEZAS_CLASSIC];
    }
    return PIEZAS[rand() % CANT_PIEZAS_DELUXE];
}

void desencolarPieza(Juego *juego)
{
    juego->actual = juego->colaPiezas[0];
    juego->actual.y = juego->tablero.filTotales - juego->tablero.filVisibles - 4;
    int p;
    for(p = 0; p < PIEZAS_ENCOLADAS - 1; p++)
    {
        juego->colaPiezas[p] = juego->colaPiezas[p+1];
    }
    juego->colaPiezas[p] = crearRandom(juego->configJuego.modoJuego);
}

///////////////////////////////////////////////////
int detectarFilasCompletas(Tablero *tablero, int arr[])
{
    int filasBorradas = 0;
    int filaReal = tablero->filTotales - tablero->filVisibles;
    for(int fila = filaReal; fila < tablero->filTotales; fila++)
    {
        if(lineaCompleta(tablero->columnas, tablero->matriz[fila]))
        {
            arr[fila-filaReal] = 1;
            filasBorradas++;
        } else {
            arr[fila-filaReal] = 0;
        }
    }
    return filasBorradas;
}

bool lineaCompleta(int columnas, int* fila) {

   for(int col = 0; col < columnas; col++)
    {
            if(fila[col]==0)
                return false;
    }
    return true;
}

void borrarFilasAnimado(Juego *juego, int arr[])
{
    int filaReal = juego->tablero.filTotales - juego->tablero.filVisibles;
    for(int col = 0; col < juego->tablero.columnas; col++)
    {
        for(int i = 0; i < juego->tablero.filVisibles; i++)
        {
            if(arr[i])
            {
                int fila = filaReal + i;
                juego->tablero.matriz[fila][col] = 0;
            }
        }

        gbt_borrar_backbuffer(0);
        dibujarFondo(&juego->configJuego.configPantalla);
        dibujarTablero(&juego->tablero, &juego->configJuego);
        gbt_volcar_backbuffer();
        gbt_esperar(50);
    }
}


void reordenarFilas(Tablero *tablero, int arr[])
{
    int inicioVisible = tablero->filTotales - tablero->filVisibles;
    int destino = tablero->filTotales - 1;
    int *filasBorradas[tablero->filVisibles];
    int cantBorradas = 0;

    for(int fila = tablero->filTotales - 1; fila >= 0; fila--)
    {
        int borrar = fila >= inicioVisible && arr[fila - inicioVisible];

        if(borrar)
        {
            filasBorradas[cantBorradas++] = tablero->matriz[fila];
        }
        else
        {
            tablero->matriz[destino] = tablero->matriz[fila];
            destino--;
        }
    }

    for(int i = 0; i < cantBorradas; i++)
    {
        tablero->matriz[destino] = filasBorradas[i];
        reiniciarFila(tablero->columnas, tablero->matriz[destino]);
        destino--;
    }
}

////////////////////////////////////////////
void dibujarFondo(const ConfigPantalla *configPantalla)
{
    for(int fila = 0; fila < configPantalla->alto; fila+=configPantalla->tamCelda)
        {
            for(int col = 0; col < configPantalla->ancho ; col +=configPantalla->tamCelda)
            {
                for(int f = 0 ; f < configPantalla->tamCelda; f++)
                {
                    for(int c = 0; c < configPantalla->tamCelda; c++)
                    {
                        if(col + c < configPantalla->ancho && fila + f < configPantalla->alto)
                        {
                            int color = (f == 0 || c == 0) ? 42 : 43;
                            gbt_dibujar_pixel(col+c, fila+f, color);
                        }

                    }
                }
            }
        }
}

////////////////////////////////////////////

int validarCantColDeluxe(int cantCol)
{
    if(cantCol >= MIN_COL_DELUXE && cantCol <= MAX_COL_DELUXE)
        return cantCol;
    return CANT_COL_CLASSIC;
}


void** crearMatriz(size_t filas, size_t columnas, size_t tamElem)
{
    if(!filas || !columnas || !tamElem)
        return NULL;

    void **matriz = malloc(filas * sizeof(*matriz));

    if(!matriz)
        return NULL;

    void **ultimaFila = matriz + filas - 1;
    for(void **fila = matriz; fila <= ultimaFila; fila++)
    {
        *fila = malloc(columnas * tamElem); //usar calloc
        if(!*fila)
        {
            destruirMatriz(matriz, fila - matriz);
            return NULL;
        }
    }
    return matriz;
}

void destruirMatriz(void **matriz, size_t filas)
{
    if(!matriz)
        return;
    void **ultimaFila = matriz + filas - 1;
    for(void **fila = matriz; fila <= ultimaFila; fila++)
    {
        free(*fila);
        *fila = NULL;
    }
    free(matriz);
    matriz = NULL;
}

void destruir_tablero(int **tablero, int filas)//
{
    for (int i = 0; i < filas; i++)
    {
        free(tablero[i]);
    }
    free(tablero);
}

void reiniciarTablero(Tablero *tablero)
{
    for(int fila = 0; fila < tablero->filTotales; fila++)
    {
        reiniciarFila(tablero->columnas, tablero->matriz[fila]);
    }
}

void reiniciarFila(int colTotales, int* fila)
{
    for(int col = 0; col < colTotales; col++)
        {
            fila[col] = 0;
        }
}
//////////////////////////////

Temporizador *crear_temporizador(double intervalo)
{
    Temporizador *temp = malloc(sizeof(Temporizador));
    if(temp == NULL)
        return NULL;
    temp->inicio = clock();
    temp->intervalo = intervalo;
    return temp;
}

bool consumir_temporizador(Temporizador *temp)
{
    clock_t actual = clock();
    double seg = (double)(actual - temp-> inicio) / CLOCKS_PER_SEC;
    if(seg >= temp-> intervalo)
    {
        temp->inicio = clock();
        return true;
    }
    return false;
}

void destruir_temporizador(Temporizador *temp)
{
    if(temp != NULL)
        free(temp);
}


Teclado teclado()
{
    if(!kbhit())
        return NONE;

    int tecla = getch();

    if(tecla == 224 || tecla == 0)
    {
        tecla = getch();
        switch(tecla)
        {
             case 75: return IZQUIERDA;
             case 77: return DERECHA;
             case 72: return ARRIBA;
             case 80: return ABAJO;
        }
    }
    switch(tecla)
    {
        case 'd': return DERECHA;
        case 'a': return IZQUIERDA;
        case 'w': return ARRIBA;
        case 's': return ABAJO;
        case 'z': return ROTAR_ANTIHORARIO;
        case 'x': return ROTAR_HORARIO;
        case ' ':
        case 'p': return PAUSA;
        case 27: return ESCAPE;
    }

    return NONE;
}

void dibujarCaracter8x8(const uint8_t letra[8][8], int x, int y, int color)
{
    for(int fila = 0; fila < 8; fila++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(letra[fila][col])
                gbt_dibujar_pixel(x + col, y + fila, color);
        }
    }
}

void dibujarCaracter8x16(const uint8_t letra[16][8], int x, int y, int color)
{
    for(int fila = 0; fila < 16; fila++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(letra[fila][col])
                gbt_dibujar_pixel(x + col, y + fila, color);
        }
    }
}


void imprimirTablero(Tablero *tablero)
{
    printf("\n");

    for(int fila = 0; fila < tablero->filTotales; fila++)
    {
        printf("%02d | ", fila);

        for(int col = 0; col < tablero->columnas; col++)
        {
            printf("%2d ", tablero->matriz[fila][col]);
        }

        if(fila < tablero->filTotales - tablero->filVisibles)
            printf(" <- invisible");

        printf("\n");
    }

    printf("\n");
}

