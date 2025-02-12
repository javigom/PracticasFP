#include <iostream>
#include <fstream>

#include "conjunto.h"
#include "casilla.h"
#include "tablero.h"
#include "checkML.h"

using namespace std;

//Inicializa el tablero tablero de forma que todas las casillas sean vac�as y tengan como valores posibles todos los del intervalo [1, 9].
void iniciaTablero(tTablero tablero)
{
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			iniciaCasilla(tablero[i][j]);
		}
	}
}

//Recibe el tablero y un string con el nombre del fichero. Devuelve true si la carga del tablero ha tenido �xito.
bool cargarTablero(string nombreFichero, tTablero tablero)
{
	bool cargado = false;
	char num;
	ifstream fichero;
	fichero.open(nombreFichero);
	if (fichero.is_open())
	{
		cargado = true;
		for (int i = 0; i < MAX; i++)
		{
			for (int j = 0; j < MAX; j++)
			{
				fichero.get(num);
				rellenaCasilla(tablero[i][j], num, true);
			}
			fichero.ignore();
		}
		fichero.close();

		actualizarPosiblesTablero(tablero);
	}
	return cargado;
}

//Muestra por pantalla el tablero dado.
void dibujarTablero(const tTablero tablero)
{
	cout << endl;
	for (int i = 0; i < MAX; i++)
	{
		if (i == 0 || i == 3 || i == 6)
		{
			cout << "----------------------------------" << endl;
		}
		for (int j = 0; j < MAX; j++)
		{
			if (j == 0 || j == 3 || j == 6)
			{
				cout << "| ";
			}
			dibujaCasilla(tablero[i][j]);
			cout << "  ";
			if (j == 8)
			{
				cout << "| ";
			}
		}
		cout << endl;

		if (i == 8)
		{
			cout << "----------------------------------" << endl;

		}
	}
	cout << endl;
}

//Devuelve true si se ha podido introducir el valor c en fila y col. Actualiza los valores posibles de las casillas afectadas.
bool ponerNum(tTablero tablero, int fila, int col, int c)
{
	bool introducido = false;
	int filAux, colAux;
	if (casillaPertenece(tablero[fila][col], c) && casillaVacia(tablero[fila][col]))
	{
		rellenaCasilla(tablero[fila][col], c, false);

		for (int i = 0; i < MAX; i++)
		{
			actualizaPosiblesCasilla(tablero[fila][i], tablero[fila][col]);
			actualizaPosiblesCasilla(tablero[i][col], tablero[fila][col]);
			
		}

		filAux = fila / 3;
		filAux = filAux * 3;
		colAux = col / 3;
		colAux = colAux * 3;

		for (int i = filAux; i < (filAux + 3); i++)
		{
			for (int j = colAux; j < (colAux + 3); j++)
			{
				actualizaPosiblesCasilla(tablero[i][j], tablero[fila][col]);
			}
		}

		introducido = true;
	}

	return introducido;
}

//Devuelve true si se ha podido borrar el valor del tablero de la fila y columna.
bool borrarNum(tTablero tablero, int fila, int col)
{
	bool borrado = false;
	int filAux, colAux;
	if (!casillaVacia(tablero[fila][col]) && !casillaFija(tablero[fila][col]))
	{
		for (int i = 0; i < MAX; i++)
		{
			annadeElementoCasilla(tablero[i][col], tablero[fila][col]);
			annadeElementoCasilla(tablero[fila][i], tablero[fila][col]);
		}

		filAux = fila / 3;
		filAux = filAux * 3;
		colAux = col / 3;
		colAux = colAux * 3;

		for (int i = filAux; i < (filAux + 3); i++)
		{
			for (int j = colAux; j < (colAux + 3); j++)
			{
				annadeElementoCasilla(tablero[i][j], tablero[fila][col]);
			}
		}

		iniciaCasilla(tablero[fila][col]);
		actualizarPosiblesTablero(tablero);
		borrado = true;
	}
	return borrado;
}

//Devuelve true si el tablero esta completo, es decir, que no exista ninguna casilla vacia.
bool tableroLleno(const tTablero tablero)
{
	bool lleno = true;
	int i = 0, j;

	while ((i < MAX) && (lleno))
	{
		j = 0;
		while ((j < MAX) && (lleno))
		{
			if (casillaVacia(tablero[i][j]))
			{
				lleno = false;
			}
			j++;
		}
		i++;
	}
	return lleno;
}

//Muestra los valores posibles de la casilla fila y col del tablero.
void mostrarPosibles(const tTablero tablero, int fila, int col)
{
	if (casillaVacia(tablero[fila][col]))
	{
		cout << "Posibles valores de la casilla [" << fila + 1 << "][" << col + 1 << "]: ";
		mostrarPosiblesCasilla(tablero[fila][col]);
	}

	else
	{
		cout << "Casilla no vacia. No se puede consultar." << endl;
	}
	
}

//Rellena aquellas casillas que solo tengan un �nico valor posible.
void rellenarSimples(tTablero tablero)
{
	int num;
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (esSimple(tablero[i][j], num))
			{
				ponerNum(tablero, i, j, num);
				actualizarPosiblesTablero(tablero);
			}
		}
	}
}

//FUNCIONES PROPIAS

//Actualiza los valores posibles de todas las casillas del tablero
void actualizarPosiblesTablero(tTablero tablero)
{
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			actualizarPosiblesTableroFila(tablero, i, j);
			actualizarPosiblesTableroCol(tablero, i, j);
			actualizarPosiblesTableroCuadrado(tablero, i, j);
		}
	}
}

//Actualiza los valores posibles de una fila
void actualizarPosiblesTableroFila(tTablero tablero, int fila, int col)
{
	for (int i = 0; i < MAX; i++)
	{
		if (!casillaVacia(tablero[fila][i]))
		{
			actualizaPosiblesCasilla(tablero[fila][col], tablero[fila][i]);
		}
	}
}

//Actualiza los valores posibles de una columna
void actualizarPosiblesTableroCol(tTablero tablero, int fila, int col)
{
	for (int i = 0; i < MAX; i++)
	{
		if (!casillaVacia(tablero[i][col]))
		{
			actualizaPosiblesCasilla(tablero[fila][col], tablero[i][col]);
		}
	}
}

//Actualiza los valores posibles de una cuadrado
void actualizarPosiblesTableroCuadrado(tTablero tablero, int fila, int col)
{
	int filAux = 0, colAux = 0;

	//filAux = fila / 3; filAux = filAux*3;

	for (int i = 0; i < MAX; i = i + 3)
	{
		if (i <= fila)
		{
			filAux = i;
		}

		if (i <= col)
		{
			colAux = i;
		}
	}

	for (int i = filAux; i < (filAux + 3); i++)
	{
		for (int j = colAux; j < (colAux + 3); j++)
		{
			if (!casillaVacia(tablero[i][j]))
			{
				actualizaPosiblesCasilla(tablero[fila][col], tablero[i][j]);
			}
		}
	}
}