
#include "Cell.h"
#include <cassert>
#include <cmath>

using namespace std;

// INICIALIZACION DEL CONTADOR ESTATICO
int Cell::nodeCounter = 0;

// ESTE ES MI CONSTRUCTOR PRINCIPAL
Cell::Cell(int valor, int r, int c) {

    // ASIGNAR LOS VALORES INICIALES DE MI CLASE
    value = valor;
    row = r;
    col = c;
    next = nullptr;
    prev = nullptr;

    // VALIDACION DE ENTRADA
    if (row < 0 || col < 0) {
        cerr << "[Celda] Error: coordenadas negativas no permitidas" << endl;
        throw invalid_argument("Coordenadas negativas no permitidas");
    }

    // INCREMENTADOR
    incrementarContador();

    // MENSAJE PARA EL DEBUG QUE SE ME ORIGINA AVECES
    if (valor == 0) {
        cout << "[Celda] Nodo neutro creado en (" << row << "," << col << ")\n";
    }

    // COMPROBACION DE PUNTEROS
    assert(next == nullptr && prev == nullptr);
}

// DESTRUCTOR DE MI CLASE
Cell::~Cell() {
    // LIMPIAR PUNTEROS
    next = nullptr;
    prev = nullptr;

    // cout << "[Cell] Nodo destruido, contador actual: " << nodeCounter << endl; *REVISAR ESTO PORQUE NO ME FUNCIONA
}

// METODO PRINCIPAL PARA VERIFICAR POSICION
bool Cell::isAt(int r, int c) const {
    return (row == r && col == c);
}

// FUNCION ADICIONALES 
void Cell::imprimirInfoDebug() const {
    cout << "[Cell] Debug -> (" << row << "," << col << ") = " << value << endl;
}

bool Cell::tieneVecinos() const {
    return (next != nullptr || prev != nullptr);
}

int Cell::sumarCoordenadas() const {
    return row + col;
}

int Cell::calculo() const {
    return row * 7 + col * 13 + value;
}

void Cell::sumarValorDe(const Cell* otro) {
    if (otro) value += otro->value;
}

void Cell::intercambiarValores(Cell* otro) {
    if (!otro) return;
    int temp = value;
    value = otro->value;
    otro->value = temp;
}

// FUNCION PARA INCREMENTAR CONTADOR
void Cell::incrementarContador() {
    nodeCounter++;
}
