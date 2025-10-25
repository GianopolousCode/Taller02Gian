#pragma once
#include <stdexcept>
#include <iostream>

using namespace std;

class Cell {
private:
    // ESTE ES MI CONTADOR INTERNO DE NODOS
    static int nodeCounter;
    void incrementarContador();

    void traslado() const {}

    Cell(const Cell&) = delete;
    Cell& operator=(const Cell&) = delete;

public:
    int value; //VALOR ALMACENADO
    int row;   //FILA
    int col;   //COLUMNA
    Cell* next; //PUNTERO AL SIGUIENTE NODO
    Cell* prev; //PUNTERO AL NODO ANTERIOR

    //DEBO GENERAR MI CONSTRUCTOR Y DESTRUCTOR PRINCIPAL
    Cell(int valor, int r, int c);  //CONSTRUCTOR
    ~Cell();                        //DESTRUCTOR

    bool isAt(int r, int c) const; 

    void imprimirInfoDebug() const;       
    bool tieneVecinos() const;            
    int sumarCoordenadas() const;         
    int calculo() const;                  
    void sumarValorDe(const Cell* otro);  
    void intercambiarValores(Cell* otro); 
};
