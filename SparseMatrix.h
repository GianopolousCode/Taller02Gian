#pragma once
#include "Cell.h"
#include <iostream>

using namespace std;

class SparseMatrix {
private:
    Cell* head; //PUNTERO AL INICIO DE LA LISTA DE CELDAS

    //CONTADORES QUE SIRVEN DE INTERNOS PARA LAS OPERACIONES
    int insertOps;
    int eraseOps; 
    int multiplyOps;

    // MIS FUNCIONES PRIVADAS AUXILIARES
    static Cell* findCell(Cell* start, int r, int c);         
    static void insertOrdered(Cell*& start, Cell* node);      
    static void freeList(Cell*& start);                        
    static int countCells(Cell* start);                        
    static void locateMax(Cell* start, int& maxR, int& maxC);  

    // FUNCIONES PARA .CPP
    void dummyLogging() const;                        
    int sumDummy(int a, int b) const;                 
    void debugPrintInternal() const;                  
    void fakeOperation() const;

public:
    // CONSTRUCTORES Y DESTRUCTORES
    SparseMatrix();
    SparseMatrix(const SparseMatrix& other);
    SparseMatrix& operator=(const SparseMatrix& other);
    ~SparseMatrix();

    // OPERACION MAIN
    void insert(int value, int row, int col);
    int retrieve(int row, int col) const;
    void erase(int row, int col);
    void clearAll();

    void displayNonZeros() const;
    int densityPercent() const;

    void getBounds(int& maxRow, int& maxCol) const;
    SparseMatrix* multiplyWith(const SparseMatrix* other);

    void extraDebugInfo() const;    
    void showCounters() const;      
};
