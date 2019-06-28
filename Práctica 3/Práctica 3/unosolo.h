#include <string>
#include <iostream>
using namespace std;	



const int DIM = 6; // Constante que servir� para representar las dimensiones del tablero

const int NUM_DIRS = 4; //Constante que indica n�mero de posiciones que tiene el array tPosibles

typedef enum tEstado { BLOQUEO, GANA, JUGANDO, ABANDONO }; //enumerado que representa el estado del juego

typedef enum tCelda { NULA = 0, VACIA = 2, FICHA = 10 }; // valores de cada celda del tablero

typedef enum tDireccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA, INCORRECTA }; // enumerado que representa las direcciones

typedef tCelda tTablero[DIM][DIM]; // matriz que representa el tablero, de dimensiones DIMxDIM

typedef bool tPosibles[NUM_DIRS]; // array donde se van a guardar los posibles movimientos

typedef struct {

	tTablero tablero;  //Tablero del juego
	int filaM, columnaM; // La fila y columna en la que se encuentra la meta (tendr� el color char(254)
	int bolasIniciales; // bolas iniciales que ten�a el tablero
	int movimientos; // n�mero de movimientos realizados en la partida
	tEstado estadoJuego; //estado del juego

}tJuego;

typedef struct {
	int filamov, columnamov;  // fila y columna de la ficha que se quieren mover
	tPosibles movPosibles; // movimientos posibles que tiene dicha ficha
	tDireccion direccionMov; // direcci�n elegida para mover la ficha.
}tMovimiento;


void colorFondo(int color); // Funci�n que sirve para asignar un color para el fondo

void mostrar(const tJuego &juego); // Muestra el tablero  y el n�mero de movimientos realizados por pantalla
bool cargar(tJuego &juego, string nombre); // Carga el archivo (el nonmbre del archivo fue introducido de teclado)
void guardar(const tJuego &juego, string nombre); // Guarda de nuevo en el archivo las dimensiones, d�nde est� la meta, el tablero, etc.
void partida(tJuego &juego); // Lleva a cabo todo el juego
bool leerMovimiento(const tTablero tablero, tMovimiento &mov); // Lleva a cabo toda la funci�n de pedir, mostrar, etc. movimientos con las
															   //funciones: pedirFicha, posiblesMovimientos, y mostrarPosiblesMovimientos.

bool pedirFicha(const tTablero tablero, tMovimiento &mov); //Funci�n que pide de teclado la fila y columna de la ficha que se desea mover.
														   // Comprueba que haya ficha, y que no se haya salido del tablero.

bool posiblesMovimientos(const tTablero tablero, tMovimiento &mov); //Dada una posici�n v�lida, devuelve true si la ficha tiene movimientos.
void mostrarPosiblesMovimientos(const tTablero tablero, tMovimiento &mov); // Muestra por pantalla los movimientos de la ficha 
																		   //elegida (ya comprobada que es v�lida)
void ejecutarMovimiento(tTablero tablero, const tMovimiento &mov); //Ejecuta el movimiento elegido por el usuario

void nuevoEstado(tJuego &juego); // Recalcula el nuevo estado a partir de la funci�n comprobarEstado
bool comprobarEstado(tJuego &juego); // Funci�n que recorre el tablero comprobando si una ficha tiene alg�n movimiento posible. Cuando encuentre 
									 // una que s� tiene, saldr� de la funci�n y devolver� true, si no encuentra ninguna, devuelve false

void menuPartida(int &eleccion);  // funci�n que muestra por pantalla el men� de opciones

void generarTablero(tJuego &juego, int movimientos); //Funci�n que se encarga de la generaci�n del tablero

void iniciarTablero(tTablero tablero); //Pone en el tablero todas las posiciones NULAS

void fijarMeta(tJuego &juego); //Fija una meta aleatoria

bool movimientoInverso(tJuego &juego);//Funci�n que lleva a cabo el movimiento inverso

void seleccionarFichaAleatoria(tJuego &juego, tMovimiento &movInverso); //selecciona una ficha aleatoria

bool posiblesMovimientosInversos(tTablero &tablero, tMovimiento &movInverso); // comprueba si la ficha tiene movimientos posibles.
																			  //si los tiene, elige uno al azar.

void ejecutarMovimientoInverso(tTablero &tablero, const tMovimiento &movInverso); //ejecuta el movimiento inverso