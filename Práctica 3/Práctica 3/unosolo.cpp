#include "unosolo.h"
#include <locale.h>
#include <cstdlib>
#include <stdio.h>
#include <windows.h>
#include <fstream>
using namespace std;


void colorFondo(int color); // Función que sirve para asignar un color para el fondo

void mostrar(const tJuego &juego); // Muestra el tablero  y el número de movimientos realizados por pantalla
bool cargar(tJuego &juego, string nombre); // Carga el archivo (el nonmbre del archivo fue introducido de teclado)
void guardar(const tJuego &juego, string nombre); // Guarda de nuevo en el archivo las dimensiones, dónde está la meta, el tablero, etc.
void partida(tJuego &juego); // Lleva a cabo todo el juego
bool leerMovimiento(const tTablero tablero, tMovimiento &mov); // Lleva a cabo toda la función de pedir, mostrar, etc. movimientos con las
															   //funciones: pedirFicha, posiblesMovimientos, y mostrarPosiblesMovimientos.

bool pedirFicha(const tTablero tablero, tMovimiento &mov); //Función que pide de teclado la fila y columna de la ficha que se desea mover.
														   // Comprueba que haya ficha, y que no se haya salido del tablero.

bool posiblesMovimientos(const tTablero tablero, tMovimiento &mov); //Dada una posición válida, devuelve true si la ficha tiene movimientos.
void mostrarPosiblesMovimientos(const tTablero tablero, tMovimiento &mov); // Muestra por pantalla los movimientos de la ficha 
																		   //elegida (ya comprobada que es válida)
void ejecutarMovimiento(tTablero tablero, const tMovimiento &mov); //Ejecuta el movimiento elegido por el usuario

void nuevoEstado(tJuego &juego); // Recalcula el nuevo estado a partir de la función comprobarEstado

bool comprobarEstado(tJuego &juego); // Función que recorre el tablero comprobando si una ficha tiene algún movimiento posible. Cuando encuentre 
									 // una que sí tiene, saldrá de la función y devolverá true, si no encuentra ninguna, devuelve false

void generarTablero(tJuego &juego, int movimientos); //Función que se encarga de la generación del tablero

void iniciarTablero(tTablero tablero); //Pone en el tablero todas las posiciones NULAS

void fijarMeta(tJuego &juego); //Fija una meta aleatoria

bool movimientoInverso(tJuego &juego);//Función que lleva a cabo el movimiento inverso

void seleccionarFichaAleatoria(tJuego &juego, tMovimiento &movInverso); //selecciona una ficha aleatoria

bool posiblesMovimientosInversos(tTablero &tablero, tMovimiento &movInverso); // comprueba si la ficha tiene movimientos posibles.
																			  //si los tiene, elige uno al azar.

void ejecutarMovimientoInverso(tTablero &tablero, const tMovimiento &movInverso); //ejecuta el movimiento inverso

void colorFondo(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

void mostrar(const tJuego &juego) {
	cout << '\n';
	for (int numerosColumnas = 0; numerosColumnas < DIM; numerosColumnas++) {
		if (numerosColumnas == 0) {
			cout << "  " << (numerosColumnas + 1);
		}
		else
			cout << "  " << (numerosColumnas + 1);
	}

	for (int f = 0; f < DIM; f++) {
		cout << '\n' << '\n' << (f + 1);
		colorFondo(0);
		cout << "  ";

		for (int c = 0; c < DIM; c++) {
			colorFondo(juego.tablero[f][c]);

			if (f == juego.filaM && c == juego.columnaM) {
				cout << char(254);
			}
			else cout << " ";
			colorFondo(0);
			cout << "  ";
		}
	}
	cout << '\n' << '\n' << "El numero de movimientos realizados es: " << endl;
	cout << juego.movimientos << " movimientos. " << endl;
}

bool cargar(tJuego &juego, string nombre) { //Comprobada
	ifstream archivo;
	bool valido = false;
	int n, celda;

	archivo.open(nombre);

	if (archivo.is_open()) {
		archivo >> n;

		if (n == DIM) {

			valido = true;

			while (!archivo.eof()) {
				archivo >> juego.bolasIniciales;
				archivo >> juego.filaM;
				archivo >> juego.columnaM;

				for (int f = 0; f < DIM; f++) {
					for (int c = 0; c < DIM; c++) {
						archivo >> celda;
						juego.tablero[f][c] = tCelda(celda);
					}
				}
				archivo >> juego.movimientos;
			}
			archivo.close();
		}
		else cout << '\n' << "Las dimensiones no son válidas";

	}
	else cout << '\n' << "El archivo no ha podido abrirse. " << endl;

	return valido;
}

void guardar(const tJuego &juego, string nombre) {
	ofstream archivo;

	archivo.open(nombre);

	archivo << DIM << endl;
	archivo << juego.bolasIniciales << endl;
	archivo << juego.filaM << endl;
	archivo << juego.columnaM;

	for (int f = 0; f < DIM; f++) {
		archivo << '\n';
		for (int c = 0; c < DIM; c++) {
			archivo << juego.tablero[f][c] << " ";
		}
	}

	archivo << '\n' << juego.movimientos;

	archivo.close();

}

void partida(tJuego &juego) {
	tMovimiento mov;
	do {
		mostrar(juego);

		if (leerMovimiento(juego.tablero, mov)) {
			ejecutarMovimiento(juego.tablero, mov);
			juego.movimientos++;
			nuevoEstado(juego);
		}
		else {
			cout << '\n' << "Ha elegido salir. " << '\n';
			juego.estadoJuego = ABANDONO;
		}
	} while (juego.estadoJuego == JUGANDO);
}

bool leerMovimiento(const tTablero tablero, tMovimiento &mov) {
	bool noSalir; //Booleano que controla si en algún momento el usuario decide salir.
				  //Devuelve false si el usuario pulsara '0' (la opción de salir)
	int direccionAux; // Variable auxiliar que sirve para transformar el número introducido de teclado en un tipo tDireccion
	mov.direccionMov = INCORRECTA;

	noSalir = pedirFicha(tablero, mov);

	while (noSalir && mov.direccionMov == INCORRECTA) {

		while (!posiblesMovimientos(tablero, mov) && noSalir) {

			mov.direccionMov = INCORRECTA;

			cout << '\n' << "No hay ningun movimiento valido" << endl;

			noSalir = pedirFicha(tablero, mov);
		}

		if (noSalir) {

			mostrarPosiblesMovimientos(tablero, mov);
			cin >> direccionAux;
			while (direccionAux < 0 || direccionAux>4 || (direccionAux != 0 && !mov.movPosibles[direccionAux - 1])) {
				cout << '\n' << "Por favor, introduzca una eleccion valida: " << '\n';
				cin >> direccionAux;

			}

			if (direccionAux != 0) {
				direccionAux--;

				mov.direccionMov = tDireccion(direccionAux);
			}
			else {
				mov.direccionMov = INCORRECTA;
				while (mov.direccionMov == INCORRECTA&&noSalir) {

					noSalir = pedirFicha(tablero, mov);
					while (!posiblesMovimientos(tablero, mov) && noSalir) {

						mov.direccionMov = INCORRECTA;

						cout << '\n' << "No hay ningun movimiento valido" << endl;

						noSalir = pedirFicha(tablero, mov);
					}

					if (noSalir) {

						mostrarPosiblesMovimientos(tablero, mov);
						cin >> direccionAux;

						while (direccionAux < 0 || direccionAux>4 || (direccionAux != 0 && !mov.movPosibles[direccionAux - 1])) {
							cout << '\n' << "Por favor, introduzca una eleccion valida: " << '\n';
							cin >> direccionAux;

						}
						if (direccionAux != 0) {
							direccionAux--;

							mov.direccionMov = tDireccion(direccionAux);
						}
						else mov.direccionMov = INCORRECTA;

					}


				}

			}
		}
	}


	return noSalir;

}

bool pedirFicha(const tTablero tablero, tMovimiento &mov) {

	bool dimensiones = false; // booleano que hace que se comprueben si se ha salido del tablero el usuario
	bool noSalir = true; // booleano que devolverá la función "pedirFicha" . Si el usuario decide salir, devuelve false
	bool ficha = false; // booleano que comprueba si hay ficha o no en la posición elegida

	cout << '\n' << "Por favor, introduzca la fila y columna de la ficha que desea mover " << endl;
	cout << "Recuerde que puede elegir salir pulsando '0' " << endl;
	cin >> mov.filamov;

	if (mov.filamov != 0) {

		cin >> mov.columnamov;
		mov.filamov--;
		mov.columnamov--;

		while (!dimensiones || !ficha) {

			if (mov.filamov >= DIM || mov.columnamov >= DIM && noSalir) {
				cout << '\n' << " Te has salido fuera del tablero. Introduce la fila y columna de nuevo: " << endl;
				cin >> mov.filamov;
				if (mov.filamov != 0) {
					cin >> mov.columnamov;
					mov.filamov--;
					mov.columnamov--;
				}
				else noSalir = false;
			}
			else dimensiones = true;

			if (tablero[mov.filamov][mov.columnamov] != int(tCelda(FICHA)) && noSalir) {
				cout << '\n' << "Has elegido una posición del tablero que no tiene ficha. Introduzca de nuevo la fila y columna: " << endl;
				cin >> mov.filamov;
				if (mov.filamov != 0) {
					cin >> mov.columnamov;
					mov.filamov--;
					mov.columnamov--;
					dimensiones = false;
				}
				else noSalir = false;
			}
			else ficha = true;
		}
	}
	else {
		noSalir = false;
	}

	return noSalir;   //devolverá false si se ha elegido 0, la opción de salir, 
					  //o true en el caso de que se haya elegido una ficha válida
}

bool posiblesMovimientos(const tTablero tablero, tMovimiento &mov) {

	bool valido = false;

	for (int i = 0; i < NUM_DIRS; i++) {
		switch (i) {
		case ARRIBA: if (tablero[mov.filamov - 1][mov.columnamov] == FICHA && tablero[mov.filamov - 2][mov.columnamov] == VACIA
			&& (mov.filamov - 2)<DIM && (mov.filamov - 1)<DIM) {

			mov.movPosibles[i] = true;
			valido = true;
		}
					 else mov.movPosibles[i] = false; break;

		case ABAJO:	if (tablero[mov.filamov + 1][mov.columnamov] == FICHA && tablero[mov.filamov + 2][mov.columnamov] == VACIA
			&& (mov.filamov + 2)<DIM && (mov.filamov + 1)<DIM) {

			mov.movPosibles[i] = true;
			valido = true;
		}
					else mov.movPosibles[i] = false; break;

		case IZQUIERDA: if (tablero[mov.filamov][mov.columnamov - 1] == FICHA && tablero[mov.filamov][mov.columnamov - 2] == VACIA
			&& (mov.columnamov - 2)<DIM && (mov.columnamov - 1)<DIM) {

			mov.movPosibles[i] = true;
			valido = true;
		}
						else mov.movPosibles[i] = false; break;
		case DERECHA: if (tablero[mov.filamov][mov.columnamov + 1] == FICHA && tablero[mov.filamov][mov.columnamov + 2] == VACIA
			&& (mov.columnamov + 2)<DIM && (mov.columnamov + 1)<DIM) {

			mov.movPosibles[i] = true;
			valido = true;
		}
					  else mov.movPosibles[i] = false; break;
		}

	}


	return valido;

}

void mostrarPosiblesMovimientos(const tTablero tablero, tMovimiento &mov) {
	cout << '\n' << "Elija direccion: ";
	cout << '\n' << "0.- Cambiar de ficha" << '\n';

	for (int i = 0; i < NUM_DIRS; i++) {
		if (mov.movPosibles[i] == true) {
			switch (i) {
			case ARRIBA: cout << i + 1 << ".- Arriba" << '\n'; break;
			case ABAJO: cout << i + 1 << ".- Abajo" << '\n'; break;
			case IZQUIERDA: cout << i + 1 << ".- Izquierda" << '\n'; break;
			case DERECHA: cout << i + 1 << ".- Derecha" << '\n';  break;
			}
		}
	}

}

void ejecutarMovimiento(tTablero tablero, const tMovimiento &mov) {

	tablero[mov.filamov][mov.columnamov] = VACIA;
	switch (mov.direccionMov) {
	case ARRIBA: tablero[mov.filamov - 1][mov.columnamov] = VACIA;
		tablero[mov.filamov - 2][mov.columnamov] = FICHA; break;

	case ABAJO: tablero[mov.filamov + 1][mov.columnamov] = VACIA;
		tablero[mov.filamov + 2][mov.columnamov] = FICHA; break;

	case IZQUIERDA: tablero[mov.filamov][mov.columnamov - 1] = VACIA;
		tablero[mov.filamov][mov.columnamov - 2] = FICHA; break;

	case DERECHA: tablero[mov.filamov][mov.columnamov + 1] = VACIA;
		tablero[mov.filamov][mov.columnamov + 2] = FICHA; break;
	}
}

void nuevoEstado(tJuego &juego) {
	bool sePuedeJugar; // será true si se puede seguir jugando

	if (juego.bolasIniciales - juego.movimientos == 1 && juego.tablero[juego.filaM][juego.columnaM] == FICHA) {
		juego.estadoJuego = GANA;
		cout << '\n' << '\n' << "//////////////////////";
		cout << '\n' << "///// HAS GANADO /////";
		cout << '\n' << "//////////////////////" << '\n' << '\n';
	}
	else {
		if (juego.bolasIniciales - juego.movimientos == 1) {
			juego.estadoJuego = BLOQUEO;
			cout << '\n' << '\n' << "///////////////////////";
			cout << '\n' << "///// HAS PERDIDO /////";
			cout << '\n' << "///////////////////////" << '\n' << '\n';
		}
		else {
			sePuedeJugar = comprobarEstado(juego);
			switch (sePuedeJugar) {
			case true: juego.estadoJuego = JUGANDO; break;
			case false: juego.estadoJuego = BLOQUEO;
				cout << '\n' << '\n' << "///////////////////////";
				cout << '\n' << "///// HAS PERDIDO /////";
				cout << '\n' << "///////////////////////" << '\n' << '\n'; break;
			}
		}
	}
}

bool comprobarEstado(tJuego &juego) {
	int contador = 0; // lleva la cuenta de las filas que va recorriendo el for
					  // se usará para que si dicho for recorriera todas las filas y devuelva false, se salga del while.
	bool valido = false; // variable que indicará si hay algún movimiento posible, o no.
						 // Devuelve true si hay, y false, en caso contrario.
	tMovimiento mov; // variable de tipo Movimiento, para poder manejar el array de tipo tPosibles

	while (!valido && contador < DIM) { //bucle que comprobará cada ficha del tablero 
										//hasta encontrar alguna con movimiento válido
		for (int f = 0; f < DIM; f++) {
			for (int c = 0; c < DIM; c++) {

				if (juego.tablero[f][c] == FICHA) { //solo comprobará la posición si hay una ficha en ella

					for (int i = 0; i < NUM_DIRS; i++) {
						switch (i) {
						case 0: if (juego.tablero[f - 1][c] == FICHA && juego.tablero[f - 2][c] == VACIA) {

							mov.movPosibles[i] = true;
							valido = true;
						}
								else mov.movPosibles[i] = false; break;

						case 1:	if (juego.tablero[f + 1][c] == FICHA && juego.tablero[f + 2][c] == VACIA) {

							mov.movPosibles[i] = true;
							valido = true;
						}
								else mov.movPosibles[i] = false; break;

						case 2: if (juego.tablero[f][c - 1] == FICHA && juego.tablero[f][c - 2] == VACIA) {

							mov.movPosibles[i] = true;
							valido = true;
						}
								else mov.movPosibles[i] = false; break;
						case 3: if (juego.tablero[f][c + 1] == FICHA && juego.tablero[f][c + 2] == VACIA) {

							mov.movPosibles[i] = true;
							valido = true;
						}
								else mov.movPosibles[i] = false; break;
						}
					}
				}
			}
			contador++;
		}
	}

	return valido;
}

void generarTablero(tJuego &juego, int movimientos) {
	bool movOk = true;

	iniciarTablero(juego.tablero);
	fijarMeta(juego);
	mostrar(juego);
	juego.movimientos = 0;
	juego.bolasIniciales = 1;
	while (juego.movimientos < movimientos && movOk) {
		movOk = movimientoInverso(juego);
		mostrar(juego);
		system("pause");
	}
}

void iniciarTablero(tTablero tablero) {
	for (int f = 0; f < DIM; f++) {
		for (int c = 0; c < DIM; c++) {
			tablero[f][c] = NULA;
		}
	}
}

void fijarMeta(tJuego &juego) {
	juego.filaM = rand() % DIM;
	juego.columnaM = rand() % DIM;
	juego.tablero[juego.filaM][juego.columnaM] = FICHA;
	juego.movimientos = 0;
}

bool movimientoInverso(tJuego &juego) {
	tMovimiento movInverso;
	bool valido;
	int contador;

	seleccionarFichaAleatoria(juego, movInverso);
	valido = posiblesMovimientosInversos(juego.tablero, movInverso);

	if (valido) {
		ejecutarMovimientoInverso(juego.tablero, movInverso);
		juego.movimientos++;
		juego.bolasIniciales++;
	}
	else {
		contador = 0;
		while (!valido && contador<DIM*DIM) {
			seleccionarFichaAleatoria(juego, movInverso);
			valido = posiblesMovimientosInversos(juego.tablero, movInverso);
			contador++;
		}
		if (contador != DIM*DIM) {
			ejecutarMovimientoInverso(juego.tablero, movInverso);
			juego.movimientos++;
			juego.bolasIniciales++;
		}
		else cout << '\n' << "El tablero ya no puede generar mas fichas" << '\n';
	}

	return valido;
}

void seleccionarFichaAleatoria(tJuego &juego, tMovimiento &movInverso) {
	int posicionAux;
	int filaFicha, columnaFicha;

	if (juego.movimientos == 0) {//si no ha habido movimientos, se partirá desde la meta
		movInverso.filamov = juego.filaM;
		movInverso.columnamov = juego.columnaM;
	}
	else { //si ha habido al menos un movimiento, se elige ficha aleatoria
		filaFicha = rand() % DIM;
		columnaFicha = rand() % DIM;
		while (juego.tablero[filaFicha][columnaFicha] != FICHA) {
			filaFicha = rand() % DIM;
			columnaFicha = rand() % DIM;
		}
		movInverso.filamov = filaFicha;
		movInverso.columnamov = columnaFicha;
	}
}

bool posiblesMovimientosInversos(tTablero &tablero, tMovimiento &movInverso) {
	bool valido = false;
	int dirFicha;

	for (int i = 0; i < NUM_DIRS; i++) {
		switch (i) {
		case ARRIBA: if (tablero[movInverso.filamov - 1][movInverso.columnamov] != FICHA && tablero[movInverso.filamov - 2][movInverso.columnamov] != FICHA
			&& (movInverso.filamov - 2) >= 0 && (movInverso.filamov - 1)>0) {
			movInverso.movPosibles[i] = true;
			valido = true;
		}
					 else movInverso.movPosibles[i] = false;
					 break;
		case ABAJO:	if (tablero[movInverso.filamov + 1][movInverso.columnamov] != FICHA && tablero[movInverso.filamov + 2][movInverso.columnamov] != FICHA
			&& (movInverso.filamov + 2)<DIM && (movInverso.filamov + 1)<(DIM - 1)) {
			movInverso.movPosibles[i] = true;
			valido = true;
		}
					else movInverso.movPosibles[i] = false;
					break;
		case IZQUIERDA: if (tablero[movInverso.filamov][movInverso.columnamov - 1] != FICHA && tablero[movInverso.filamov][movInverso.columnamov - 2] != FICHA
			&& (movInverso.columnamov - 2) >= 0 && (movInverso.columnamov - 1)>0) {
			movInverso.movPosibles[i] = true;
			valido = true;
		}
						else movInverso.movPosibles[i] = false;
						break;
		case DERECHA: if (tablero[movInverso.filamov][movInverso.columnamov + 1] != FICHA && tablero[movInverso.filamov][movInverso.columnamov + 2] != FICHA
			&& (movInverso.columnamov + 2)<DIM && (movInverso.columnamov + 1)<(DIM - 1)) {
			movInverso.movPosibles[i] = true;
			valido = true;
		}
					  else movInverso.movPosibles[i] = false;
					  break;
		}
	}

	if (valido) { // en el caso de que válido sea true (es decir, que haya un movimiento disponible)
				  // se elegirá una dirección al azar, y si resulta que es de las que se pueden, 

		dirFicha = rand() % NUM_DIRS;

		while (!movInverso.movPosibles[dirFicha]) {
			dirFicha = rand() % NUM_DIRS;
		}
		movInverso.direccionMov = tDireccion(dirFicha);
	}

	return valido;
}

void ejecutarMovimientoInverso(tTablero &tablero, const tMovimiento &movInverso) {

	switch (movInverso.direccionMov) {
		//ARRIBA
	case ARRIBA: tablero[movInverso.filamov - 1][movInverso.columnamov] = FICHA;
		tablero[movInverso.filamov - 2][movInverso.columnamov] = FICHA;
		tablero[movInverso.filamov][movInverso.columnamov] = VACIA;
		break;
		//ABAJO
	case ABAJO: tablero[movInverso.filamov + 1][movInverso.columnamov] = FICHA;
		tablero[movInverso.filamov + 2][movInverso.columnamov] = FICHA;
		tablero[movInverso.filamov][movInverso.columnamov] = VACIA;
		break;
		//IZQUIERDA
	case IZQUIERDA:  tablero[movInverso.filamov][movInverso.columnamov - 1] = FICHA;
		tablero[movInverso.filamov][movInverso.columnamov - 2] = FICHA;
		tablero[movInverso.filamov][movInverso.columnamov] = VACIA;
		break;
		//DERECHA
	case DERECHA: tablero[movInverso.filamov][movInverso.columnamov + 1] = FICHA;
		tablero[movInverso.filamov][movInverso.columnamov + 2] = FICHA;
		tablero[movInverso.filamov][movInverso.columnamov] = VACIA;
		break;
	}

}