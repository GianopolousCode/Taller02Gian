#include "SparseMatrix.h"
#include <stdexcept>
#include <iostream>

using namespace std;

//AQUI VAN MIS AUX

Cell* SparseMatrix::findCell(Cell* start, int r, int c) {
    Cell* cur = start;
    while (cur) {
        if (cur->row == r && cur->col == c) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void SparseMatrix::insertOrdered(Cell*& start, Cell* node) {
    if (!start) { start = node; return; }

    // COLS
    if (node->col < start->col || (node->col == start->col && node->row < start->row)) {
        node->next = start;
        start->prev = node;
        start = node;
        return;
    }

    Cell* cur = start;
    while (cur->next && (cur->next->col < node->col || (cur->next->col == node->col && cur->next->row < node->row))) {
        cur = cur->next;
    }
    node->next = cur->next;
    node->prev = cur;
    if (cur->next) cur->next->prev = node;
    cur->next = node;
}

void SparseMatrix::freeList(Cell*& start) {
    while (start) {
        Cell* nxt = start->next;
        delete start;
        start = nxt;
    }
}

int SparseMatrix::countCells(Cell* start) {
    int cnt = 0;
    for (Cell* p = start; p; p = p->next) ++cnt;
    return cnt;
}

void SparseMatrix::locateMax(Cell* start, int& maxR, int& maxC) {
    maxR = -1; maxC = -1;
    for (Cell* p = start; p; p = p->next) {
        if (p->row > maxR) maxR = p->row;
        if (p->col > maxC) maxC = p->col;
    }
}

//CONSTRUCTOR Y DESTRUCTOR

SparseMatrix::SparseMatrix() : head(nullptr), insertOps(0), eraseOps(0), multiplyOps(0) {}
SparseMatrix::SparseMatrix(const SparseMatrix& other) : head(nullptr), insertOps(0), eraseOps(0), multiplyOps(0) {
    for (Cell* p = other.head; p; p = p->next) insert(p->value, p->row, p->col);
}
SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other) {
    if (this != &other) {
        freeList(head);
        for (Cell* p = other.head; p; p = p->next) insert(p->value, p->row, p->col);
    }
    return *this;
}
SparseMatrix::~SparseMatrix() { freeList(head); }

// CONCATENA .H
void SparseMatrix::dummyLogging() const { cout << "[ELIMINAR] registro activo\n"; }
int SparseMatrix::sumDummy(int a, int b) const { return a + b; }
void SparseMatrix::debugPrintInternal() const {
    cout << "[ELIMINAR] Lista completa de nodos:\n";
    for (Cell* p = head; p; p = p->next)
        cout << "(" << p->row << "," << p->col << ")=" << p->value << "\n";
}
void SparseMatrix::fakeOperation() const { int x = 42; (void)x; }

// OPERACIONES PRINCIPALES DE MI MAIN
void SparseMatrix::insert(int value, int row, int col) {
    if (row < 0 || col < 0) { cout << "No se permiten coordenadas negativas\n"; return; }

    Cell* existing = findCell(head, row, col);
    if (existing) {
        if (value == 0) {
            if (existing->prev) existing->prev->next = existing->next;
            else head = existing->next;
            if (existing->next) existing->next->prev = existing->prev;
            delete existing;
            ++eraseOps;
            cout << "[BORRAR] Elemento en (" << row << "," << col << ") borrado.\n";
        } else {
            cout << "[Actualizar] Valor en (" << row << "," << col << ") de " << existing->value << " a " << value << "\n";
            existing->value = value;
        }
    } else {
        if (value == 0) { cout << "Valor 0 [NULO] no se inserta.\n"; return; }
        Cell* n = new Cell(value, row, col);
        insertOrdered(head, n);
        ++insertOps;
        cout << "[INSERTAR] Valor " << value << " agregado en (" << row << "," << col << ")\n";
    }
}

int SparseMatrix::retrieve(int row, int col) const {
    if (row < 0 || col < 0) return 0;
    Cell* found = findCell(head, row, col);
    return found ? found->value : 0;
}

void SparseMatrix::erase(int row, int col) {
    Cell* toDel = findCell(head, row, col);
    if (!toDel) { cout << "[BORRAR] No existe elemento en (" << row << "," << col << ")\n"; return; }
    if (toDel->prev) toDel->prev->next = toDel->next;
    else head = toDel->next;
    if (toDel->next) toDel->next->prev = toDel->prev;
    delete toDel;
    ++eraseOps;
    cout << "[BORRAR] Elemento eliminado en (" << row << "," << col << ")\n";
}

void SparseMatrix::clearAll() {
    freeList(head);
    insertOps = eraseOps = multiplyOps = 0;
    cout << "[LIMPIAR] Matriz limpiada completamente\n";
}

//AQUI VAN las UTILIDADES PARA QUE SE PUEDA VER LA MATRIZ E INTERFAZ
void SparseMatrix::displayNonZeros() const {
    if (!head) { cout << "[MOSTRAR] Matriz vacia\n"; return; }
    cout << "[MOSTRAR] Elementos no nulos:\n";
    int cnt = 0;
    for (Cell* p = head; p; p = p->next) { cout << "(" << p->row << "," << p->col << ") -> " << p->value << "\n"; ++cnt; }
    cout << "[MOSTRAR] Total: " << cnt << " elementos\n";
}

int SparseMatrix::densityPercent() const {
    int elems = countCells(head);
    if (elems == 0) return 0;
    int maxR, maxC; locateMax(head, maxR, maxC);
    long total = static_cast<long>(maxR + 1) * (maxC + 1);
    if (total == 0) return 0;
    return static_cast<int>((elems * 100) / total);
}

// OPERACIONES PARA LAS FILAS Y COLUMNAS
void SparseMatrix::getBounds(int& maxRow, int& maxCol) const { locateMax(head, maxRow, maxCol); }

SparseMatrix* SparseMatrix::multiplyWith(const SparseMatrix* other) {
    if (!other || !other->head || !head) { cout << "[MULTIPLICAR] Resultado vacio\n"; return new SparseMatrix(); }

    int maxR1, maxC1, maxR2, maxC2; locateMax(head, maxR1, maxC1); locateMax(other->head, maxR2, maxC2);
    if ((maxC1+1) != (maxR2+1)) { cout << "[MULTIPLICAR] Dimensiones incompatibles\n"; return new SparseMatrix(); }

    SparseMatrix* R = new SparseMatrix();
    int ops = 0;
    for (Cell* a = head; a; a = a->next)
        for (Cell* b = other->head; b; b = b->next)
            if (a->col == b->row) {
                int val = R->retrieve(a->row, b->col);
                R->insert(val + a->value * b->value, a->row, b->col);
                ++ops;
            }

    multiplyOps += ops;
    cout << "[MULTIPLICAR] Multiplicacion finalizada. Operaciones: " << ops << "\n";
    return R;
}

// =================== FACHADA DECORATIVA ===================
void SparseMatrix::extraDebugInfo() const { debugPrintInternal(); dummyLogging(); fakeOperation(); }
void SparseMatrix::showCounters() const {
    cout << "[CONTADORES] Insertar: " << insertOps << " | Borra: " << eraseOps << " | Multiplicar operaciones: " << multiplyOps << "\n";
}
