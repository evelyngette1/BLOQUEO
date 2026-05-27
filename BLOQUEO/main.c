#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
int **crear_tablero(int filas, int columnas);
void destruir_tablero(int **tablero, int filas);
void** crearMatriz(size_t filas, size_t columnas, size_t tamElem);
void destruirMatriz(void **matriz, size_t filas);
void dibujarBloque(int x, int y, int tamanio, int color);


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
    int color;
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

typedef struct
{
	char nombre[10];
	int puntuacion;
} Estadisticas;

typedef struct
{
    ConfigJuego configJuego;
    Tablero tablero;
    Pieza actual;
    Pieza colaPiezas[5];
    int piezasCaidas;
    char nombre[10];
    int puntuacion;
    int nivel;
    int pausado;
    int gameOver;
    double velocidadCaida;
    double velocidadFijacion;//TODO: REVISAR ESTO
    Estadisticas estadisticas[5];
} Juego;


typedef struct
{
    char caracter;
    uint8_t ancho;
    uint8_t alto;
    const uint8_t *arr;
} LetraNoTipada;

void dibujarCaracter8x8(const uint8_t letra[8][8], int x, int y, int color);
typedef struct
{
    clock_t inicio;
    double intervalo;
} Temporizador;
void aumentarDificultad(Temporizador *temp);
bool consumir_temporizador(Temporizador *temp);
void rotarPieza (Pieza *pieza, int sentido);
void fijar_pieza(int **tablero, Pieza *p);
void dibujar_pieza(Pieza *p, ConfigPantalla *configPantalla, Tablero * tablero);
void dibujar_pieza_1(Pieza *p, ConfigPantalla *configPantalla, Tablero * tablero);
void dibujar_pieza_2(Pieza *p, ConfigPantalla *configPantalla, Tablero * tablero);
void inicializarTablero(Tablero *tablero);
void reiniciarTablero(Tablero *tablero);
void** crearTablero(size_t filas, size_t columnas, size_t tamElem);
void destruirTablero(void **matriz, size_t filas);
void dibujarCaracter8x16(const uint8_t letra[16][8], int x, int y, int color);
void dibujarFondo(const ConfigPantalla *configPantalla);
void dibujarTablero(Tablero *tablero, ConfigJuego *configJuego);
void fijarPieza(Tablero *tablero, Pieza *pieza);
void imprimirTablero(Tablero *tablero);
int detectarFilasCompletas(Tablero *tablero, int filasCompletas[]);
bool lineaCompleta(int columnas, int* fila);
void borrarFilasAnimado(Juego *juego, int arr[]);
void reordenarFilas(Tablero *tablero, int arr[]);
void reiniciarFila(int colTotales, int* fila);
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
Pieza desencolarPieza(Pieza cola[], ModoJuego modo);
void iniciarCola(Pieza cola[], ModoJuego modo);
int teclaSostenida(eGBT_Tecla tecla, tGBT_Temporizador *tempTeclaSostenida);
const Pieza PIEZAS[] = {
    {    .forma = {
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        .x = POS_X_INICIAL,
        .y = POS_Y_INICIAL,
        .color = 1,
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
        .color = 2,
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
        .color = 3,
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
        .color = 4,
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
        .color = 5,
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
        .color = 6,
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
        .color = 7,
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
        .color = 8,
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
        .color = 9,
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
        .color = 10,
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
        .color = 11,
        .tipo = PIEZA_D
    }
};

void dibujarLetraVar(const LetraNoTipada *letra, int x, int y, uint8_t color);

int main()
{
    Juego juego;
    ConfigPantalla configPantalla;
    ConfigJuego configJuego;
    char res[] = "CGA";
    int modo = MODO_CLASICO; //dato
    int cant_col = 16; //prueba
    int paleta = PALETA_1;
    int vel_inicial = 1000;


    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }


    if(strcmp(/*argv[1]*/ res, RES_CGA) == 0)
    {
        configPantalla.resolucion = RESOLUCION_CGA;
        configPantalla.ancho = ANCHO_CGA;
        configPantalla.alto = ALTO_CGA;
        configPantalla.tamCelda = TAM_CELDA_CGA;
        configPantalla.escala = ESCALA_4_CGA;

    } else {

        configPantalla.resolucion = RESOLUCION_VGA;
        configPantalla.ancho = ANCHO_VGA;
        configPantalla.alto = ALTO_VGA;
        configPantalla.tamCelda = TAM_CELDA_VGA;
        configPantalla.escala = ESCALA_1_VGA;
    }

    if (gbt_crear_ventana(PANTALLA_JUEGO, configPantalla.ancho, configPantalla.alto, configPantalla.escala) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if((juego.configJuego.paleta = paleta) == PALETA_1)
    {
        if (gbt_aplicar_paleta(paletaPastel, sizeof(paletaPastel)/sizeof(paletaPastel[0]), GBT_FORMATO_888) != 0) {
            fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
            return -1;
        }
    } else {
        //aplicarPaletaClasica();
        if (gbt_aplicar_paleta(paletaClasica, sizeof(paletaClasica)/sizeof(paletaClasica[0]) , GBT_FORMATO_888) != 0) {
            fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
            return -1;
        }
    }

    if(modo == MODO_CLASICO)
    {
        configJuego.modoJuego = MODO_CLASICO;
        configJuego.cantPiezas = CANT_PIEZAS_CLASSIC;
        juego.tablero.columnas = CANT_COL_CLASSIC;
    } else if(modo == MODO_DELUXE)
    {
        configJuego.modoJuego = MODO_DELUXE;
        configJuego.cantPiezas = CANT_PIEZAS_DELUXE;
        juego.tablero.columnas = validarCantColDeluxe(cant_col);//8 A 16;
    }
    juego.tablero.filVisibles = CANT_FILAS_VISIBLES;
    juego.tablero.filTotales = CANT_FILAS_TOTALES;
    juego.tablero.matriz = (int**) crearMatriz(juego.tablero.filTotales, juego.tablero.columnas, sizeof(int));

    int anchoTablero = juego.tablero.columnas * configPantalla.tamCelda;
    int altoTablero = juego.tablero.filVisibles * configPantalla.tamCelda;

    configPantalla.tableroX = (configPantalla.ancho - anchoTablero) / 2;
    configPantalla.tableroY = (configPantalla.alto - altoTablero) / 2;

    configJuego.configPantalla = configPantalla;
    configJuego.velocidadInicial = vel_inicial;

    juego.configJuego = configJuego;

    tGBT_Temporizador *temporizador = gbt_temporizador_crear(0.5);
    tGBT_Temporizador *tempTeclaSostenida = gbt_temporizador_crear(0.3);
    tGBT_Temporizador *tempCaida = gbt_temporizador_crear(0.01);
        if (!temporizador) {
            fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
            return -1;
        }

         if (!tempTeclaSostenida) {
            fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
            return -1;
        }
         if (!tempCaida) {
            fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
            return -1;
        }


    reiniciarTablero(&juego.tablero);
    int corriendo = 1;

    srand(time(NULL));
    Pieza cola[PIEZAS_ENCOLADAS];
    iniciarCola(cola, configJuego.modoJuego);
    Pieza pieza = desencolarPieza(cola, configJuego.modoJuego);
    pieza.y = juego.tablero.filTotales - juego.tablero.filVisibles - 4;
     while(corriendo)
    {
        gbt_procesar_entrada();

        if(gbt_tecla_presionada(GBTK_ESCAPE))
        {
            corriendo = 0;
        }
        Pieza piezaCopia = pieza;

        if(teclaSostenida(GBTK_DERECHA, tempTeclaSostenida))
        {
            piezaCopia.x = moverDerecha(juego.tablero.columnas, piezaCopia.x, juego.configJuego.modoJuego);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                pieza = piezaCopia;
        }
        else if(gbt_tecla_presionada(GBTK_IZQUIERDA)|| teclaSostenida(GBTK_IZQUIERDA, tempTeclaSostenida))
        {
            piezaCopia.x = moverIzquierda(juego.tablero.columnas, piezaCopia.x, juego.configJuego.modoJuego);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                pieza = piezaCopia;
        }
        else if(gbt_tecla_presionada(GBTK_ABAJO)|| teclaSostenida(GBTK_ABAJO, tempCaida))
        {
            piezaCopia.y++;
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                pieza = piezaCopia;
        }
        else if (gbt_tecla_presionada(GBTK_z))
        {
            rotarPieza(&piezaCopia, ROTAR_ANTIHORARIO);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                pieza = piezaCopia;
        }
        else if (gbt_tecla_presionada(GBTK_x))
        {
            rotarPieza(&piezaCopia, ROTAR_HORARIO);
            if(validarMovimiento(&juego.tablero, &piezaCopia, juego.configJuego.modoJuego))
                pieza = piezaCopia;
        }


         else if(gbt_tecla_presionada(GBTK_ARRIBA))
        {

        }


        if (gbt_temporizador_consumir(temporizador)) {
            Pieza piezaCopia = pieza;
            piezaCopia = pieza;
            piezaCopia.y++;
            if(validarMovimiento(&juego.tablero, &piezaCopia, configJuego.modoJuego))
            {
                pieza = piezaCopia;
            } else
            {
                fijarPieza(&juego.tablero, &pieza);
                int arr[juego.tablero.filVisibles];
                int filasBorradas = detectarFilasCompletas(&juego.tablero, arr);



                if(filasBorradas > 0)
                {
                    borrarFilasAnimado(&juego, arr);
                    reordenarFilas(&juego.tablero, arr);
                }
                imprimirTablero(&juego.tablero); //TODO QUITAR ESTO
                pieza = desencolarPieza(cola, configJuego.modoJuego);
                pieza.y = juego.tablero.filTotales - juego.tablero.filVisibles - 4;
            }
        }

        gbt_borrar_backbuffer(0);

        dibujarFondo(&configPantalla);

        dibujarTablero(&juego.tablero, &juego.configJuego);
        dibujar_pieza(&pieza, &configPantalla, &juego.tablero);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }


    gbt_temporizador_destruir(temporizador);
    gbt_temporizador_destruir(tempCaida);
    gbt_temporizador_destruir(tempTeclaSostenida);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}





int puntuacion(int filasBorradas, int nivel, int bajadoAMano, int puntuacion)
{
    int bonificacion = 0;
    if(bajadoAMano)
        bonificacion = 5 * bajadoAMano;
    int p = 0;
    if(filasBorradas<5){
        p = puntaje[filasBorradas];
    }
    puntuacion += (p * nivel + bonificacion);
    return puntuacion;
}

int teclaSostenida(eGBT_Tecla tecla, tGBT_Temporizador *tempTeclaSostenida)
{
    if(!gbt_tecla_sostenida(tecla))
    {
        return 0;
    } else {
        while(!gbt_temporizador_consumir(tempTeclaSostenida))
        {

        }
        return 1;
    }
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

            for(int f = 0; f < tamCelda; f++)
            {
                for(int c = 0; c < tamCelda; c++)
                {
                   gbt_dibujar_pixel(xBase + c, yBase + f, tablero->matriz[fila+filaImprimir][col]);
                }
            }
        }
    }
}




void dibujarLetraVar(const LetraNoTipada *letra, int x, int y, uint8_t color)
{
    for(int fila = 0; fila < letra->alto; fila++)
    {
        for(int col = 0; col < letra->ancho; col++)
        {
            int pos = fila * letra->ancho + col;

            if(letra->arr[pos])
                gbt_dibujar_pixel(x + col, y + fila, color);
        }
    }
}

void dibujar_pieza(Pieza *p, ConfigPantalla *configPantalla, Tablero *tablero)
{
    int brillo = configPantalla->tamCelda / 2 + 1;
    int inicioVisible = tablero->filTotales - tablero->filVisibles;
    for(int fila = 0; fila < 4; fila++)
    {
        for(int col = 0; col < 4; col++)
        {
            if(p->forma[fila][col] != 0)
            {
                int filaReal = p->y + fila;
                int colReal = p->x + col;

                if(filaReal < inicioVisible)
                    continue;

                int yBase = configPantalla->tableroY +
                    (filaReal - inicioVisible) * configPantalla->tamCelda;

                colReal = (colReal + tablero->columnas) % tablero->columnas;

                int xBase = configPantalla->tableroX +
                    colReal * configPantalla->tamCelda;


                for(int f = 0, j = brillo; f < configPantalla->tamCelda; f++, j--)
                {
                    for(int c = 0; c < configPantalla->tamCelda; c++)
                    {
                        if(f == 0 || f == configPantalla->tamCelda - 1 ||
                           c == 0 || c == configPantalla->tamCelda - 1)
                            gbt_dibujar_pixel(xBase + c, yBase + f, 13);
                        else if(f > 0 && c > 0 && c < j)
                            gbt_dibujar_pixel(xBase + c, yBase + f, 14);
                        else
                            gbt_dibujar_pixel(xBase + c, yBase + f, 15);
                    }
                }
            }
        }
    }
}

void dibujarBloque(int x, int y, int tamanio, int color)
{
    for(int fila = 0; fila < tamanio; fila++)
    {
        for(int col = 0; col < tamanio; col++)
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

                if(modo == MODO_DELUXE)
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

void aumentarDificultad(Temporizador *temp)
{
    temp->intervalo *= 0.97;

    if(temp->intervalo < 0.05)
    {
        temp->intervalo = 0.05;
    }
}



void fijarPieza(Tablero *tablero, Pieza *pieza)
{
    for(int fil = 0; fil < 4; fil ++)
    {
        for(int col = 0; col < 4; col++)
        {
            int columna = pieza->x + col;
            if(columna < 0 || columna > tablero->columnas-1){
                 columna = (columna + tablero->columnas) % tablero->columnas;
            }
            if(pieza->forma[fil][col] != 0)
               tablero->matriz[pieza->y + fil][columna] = pieza->color;
        }
    }
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
    modo = MODO_DELUXE; //TODO: borrar esto
    if(modo == MODO_CLASICO)
        return x + 1;
    return (x + 1) % columnas;
}

int moverIzquierda(int columnas, int x, ModoJuego modo)
{
    modo= MODO_DELUXE;//TODO: borrar esto
    if(modo == MODO_CLASICO)
        return x - 1;
    return (x - 1 + columnas) % columnas;
}


////////////////////////////////////////////////////

void iniciarCola(Pieza cola[], ModoJuego modo)
{
    for(int p = 0; p < PIEZAS_ENCOLADAS; p++)
    {
        cola[p] = crearRandom(MODO_CLASICO);
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

Pieza desencolarPieza(Pieza cola[], ModoJuego modo)
{
    Pieza piezaActual = cola[0];
    int p;
    for(p = 0; p < PIEZAS_ENCOLADAS - 1; p++)
    {
        cola[p] = cola[p+1];
    }
    cola[p] = crearRandom(modo);
    return piezaActual;
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
