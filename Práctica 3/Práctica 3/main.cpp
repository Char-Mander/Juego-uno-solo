#include "unosolo.h"
#include <ctime>
using namespace std;


int main() {
	tJuego juego;
	string nombre; //El usuario te devuelve el nombre con el .txt
	bool archivo = true; // booleano que comprueba si el archivo se ha abierto sin problemas.
	int eleccion; // eleccion que da el ususario de teclado, para elegir si salir o jugar otra partida.
	bool partidaNueva = true;  // booleano que controla si el usuario quiere seguir jugando, o salir.
	int movimientos;// movimientos (dados por el usuario) que tendrá el tablero generado.
	int guardarPartida;

	srand(time(NULL));

	while (partidaNueva) {
		menuPartida(eleccion);

		switch (eleccion) {
		case 0:  partidaNueva = false; break;
		case 1: cout << '\n' << "Por favor, introduzca el nombre del archivo que desea abrir: " << '\n';
			getchar();
			getline(cin, nombre);
			archivo = cargar(juego, nombre);
			system("cls");
			if (archivo) {
				partida(juego);
				system("pause");
				system("cls");
				if (juego.estadoJuego == ABANDONO) {
					cout << '\n' << "Desea guardar la partida?" << '\n' << "0.-No" << '\n' << "1.-Si." << '\n';
					cin >> guardarPartida;
					while (guardarPartida != 0 && guardarPartida != 1) {
						cout << '\n' << "Por favor, introduzca una opcion valida: " << '\n'; cin >> guardarPartida;
					}
					switch (guardarPartida) {
					case 0: break;
					case 1: guardar(juego, nombre); break;
					}
				}
				cout << '\n' << '\n';
				cout << '\n' << '\n' << "Gracias por jugar" << '\n' << '\n';
				system("cls");
			}
			break;
		case 2: cout << '\n' << "Introduzca el numero de movimientos que desea realizar: " << "   ";
			cin >> movimientos;
			generarTablero(juego, movimientos);
			system("pause");
			system("cls");
			juego.movimientos = 0;
			partida(juego);
			system("pause");
			system("cls");
			if (juego.estadoJuego == ABANDONO) {
				cout << '\n' << "Desea guardar la partida?" << '\n' << "0.-No" << '\n' << "1.-Si." << '\n';
				cin >> guardarPartida;
				while (guardarPartida != 0 && guardarPartida != 1) {
					cout << '\n' << "Por favor, introduzca una opcion valida: " << '\n'; cin >> guardarPartida;
				}
				switch (guardarPartida) {
				case 0: break;
				case 1: cout << '\n' << "Por favor, introduzca el nombre del archivo en el que desea guardar la partida: " << '\n';
					cin >> nombre;
					guardar(juego, nombre); break;
				}
			}
			cout << '\n' << '\n';
			cout << '\n' << '\n' << "Gracias por jugar" << '\n' << '\n';
			system("cls");
			break;
		}
	}

	system("pause");

	return 0;
}

void menuPartida(int &eleccion) {

	cout << '\n' << "//////////////////////";
	cout << '\n' << "//////   MENU   //////";
	cout << '\n' << "//////////////////////" << '\n';
	cout << '\n' << '\n' << "Elija la opcion que desee:               ";
	cout << '\n' << " 0.- Salir.                               ";
	cout << '\n' << " 1.- Jugar partida con archivo existente. ";
	cout << '\n' << " 2.- Jugar partida generando tablero.     " << '\n';
	cin >> eleccion;

	while (eleccion < 0 && eleccion > 2) {
		cout << '\n' << "Por favor, introduzca una opcion valida" << '\n';
		cin >> eleccion;
	}
}