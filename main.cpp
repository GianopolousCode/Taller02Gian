#include "SparseMatrix.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

//FUNCIONES AUX

// DEBO LEER MI INTEGER DE UNA FORMA SEGURA PARA QUE NO HAYAN ERRORES DE EJECUCION AL MOMENTO DE INGRESAR VALORES
int safeReadInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente nuevamente.\n";
    }
}

// MI MENU DISPLAY PRINCIPAL PARA EJECUTAR MIS OPCIONES 
void displayMenu() {
    cout << "\n===== GESTOR MATRIZ DISPERSA =====\n";
    cout << "[1] Inserte/Suba su valor\n";
    cout << "[2] Calcular valor\n";
    cout << "[3] Eliminar valor\n";
    cout << "[4] Lista de valores guardados\n";
    cout << "[5] Ver densidad calculada (%)\n";
    cout << "[6] Multiplicar con matrices diferentes\n";
    cout << "[7] Estadisticas rapidas\n";
    cout << "[8] Saliendo, programa terminado\n";
}

// AQUI AYUDA A MOSTRAR MI MATRIZ POR FILAS 
void showMatrixByRows(const SparseMatrix& mat) {
    int maxRow, maxCol;
    mat.getBounds(maxRow, maxCol);
    if (maxRow < 0 || maxCol < 0) {
        cout << "[Matriz vacia]\n";
        return;
    }
    cout << "Vista de la matriz por filas:\n";
    for (int r = 0; r <= maxRow; ++r) {
        for (int c = 0; c <= maxCol; ++c) {
            cout << mat.retrieve(r, c) << " ";
        }
        cout << "\n";
    }
}

// EN ESTA FUNCION DEBO GENERAR MI MATRIZ COMPAÑERA PARA MULTIPLICAR CON LA ORIGINAL
void generatePartnerMatrix(const SparseMatrix& mainMatrix, SparseMatrix& partnerMatrix) {
    partnerMatrix.clearAll();
    int maxRow, maxCol;
    mainMatrix.getBounds(maxRow, maxCol);

    if (maxRow < 0 || maxCol < 0) {
        cout << "[Aviso] La matriz principal esta vacia. Nada que generar.\n";
        return;
    }

    cout << "Generando partner matrix de tamano " << (maxRow+1) << "x" << (maxCol+1) << "...\n";

    // GUARDAR POSICIONES NO NULAS DE LA MATRIZ PRINCIPAL
    vector<std::pair<int,int>> nonZeroPositions;
    for (int c = 0; c <= maxCol; ++c) {
        for (int r = 0; r <= maxRow; ++r) {
            if (mainMatrix.retrieve(r, c) != 0) {
                nonZeroPositions.emplace_back(r, c);
            }
        }
    }

    cout << "Encontradas " << nonZeroPositions.size() << " posiciones no nulas. Insertando valores aleatorios...\n";

    // GENERAR VALORES ALEATORIOS PARA ESAS POSICIONES
    int insertedCount = 0;
    for (const auto& pos : nonZeroPositions) {
        int val = std::rand() % 10 + 1;
        partnerMatrix.insert(val, pos.first, pos.second);
        ++insertedCount;
    }

    cout << "[Listo] Partner matrix generada con " << insertedCount << " elementos.\n";
}

// MOSTRAR ESTADISTICAS RAPIDAS DE MI MATRIZ
void showQuickStats(const SparseMatrix& mat) {
    int maxR, maxC;
    mat.getBounds(maxR, maxC);
    int dens = mat.densityPercent();
    cout << "--- Estadisticas rapidas ---\n";
    cout << "Dimensiones: " << (maxR+1) << "x" << (maxC+1) << "\n";
    cout << "Densidad aproximada: " << dens << "%\n";
}

// PROGRAMA PRINCIPAL EJECUTABLE 
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SparseMatrix mainMatrix;
    SparseMatrix partnerMatrix;
    vector<string> operationLog;

    cout << "Bienvenido al Gestor de Matriz Dispersa\n";

    bool running = true;
    while (running) {
        displayMenu();
        int option = safeReadInt("Seleccione opcion: ");

        switch(option) {
            case 1: { // INSERTAR
                int val = safeReadInt("Ingrese valor: ");
                int row = safeReadInt("Fila: ");
                int col = safeReadInt("Columna: ");
                mainMatrix.insert(val, row, col);
                operationLog.push_back("Ingresado " + std::to_string(val) + " a (" + std::to_string(row) + "," + std::to_string(col) + ")");
                break;
            }
            case 2: { // REVISAR ESTO**
                int row = safeReadInt("Fila: ");
                int col = safeReadInt("Columna: ");
                int val = mainMatrix.retrieve(row, col);
                cout << "Valor en (" << row << "," << col << ") = " << val << "\n";
                operationLog.push_back("Revisando valor en (" + std::to_string(row) + "," + std::to_string(col) + ")");
                break;
            }
            case 3: { // ELIMINAR LOS VALORES
                int row = safeReadInt("Fila: ");
                int col = safeReadInt("Columna: ");
                mainMatrix.erase(row, col);
                operationLog.push_back("Eliminando elemento a (" + std::to_string(row) + "," + std::to_string(col) + ")");
                break;
            }
            case 4: { // LISTA DE VALORES NO NULOS
                mainMatrix.displayNonZeros();
                break;
            }
            case 5: { // DENSIDAD
                int dens = mainMatrix.densityPercent();
                cout << "Densidad actual: " << dens << "%\n";
                break;
            }
            case 6: { //mULTIPLICAR CON OTRA MATRIZ
                generatePartnerMatrix(mainMatrix, partnerMatrix);
                cout << "Matriz companera generada:\n";
                showMatrixByRows(partnerMatrix);
                cout << "Realizando multiplicacion A x B...\n";
                SparseMatrix* result = mainMatrix.multiplyWith(&partnerMatrix);
                cout << "Resultado de la multiplicacion:\n";
                showMatrixByRows(*result);
                delete result;
                operationLog.push_back("Multiplicacion realizada con matriz companera");
                break;
            }
            case 7: { // ESTADISTICAS RAPIDAS COMO LA ANTERIOR
                showQuickStats(mainMatrix);
                break;
            }
            case 8: { // SALIENDO Y TERMINANDO PROGRAMA
                cout << "Cerrando programa. Hasta luego\n";
                running = false;
                break;
            }
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }
    }

    return 0;
}
