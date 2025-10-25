// BENCHMARK.CPP
// IMPLEMENTACION DE MI BENCHMARK PARA LA CLASE SPARSEMATRIX
#include "SparseMatrix.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace std;

// ESTRCTURA PARA GUARDAR RESULTADOS DE MI INFORMES BENCHMARKS
struct BenchStats {
    int numElements;
    string operation;
    string density;
    double elapsedSec;
    string arch;
    int operationsCount; // CONTADOR DE OPERACIONES
};

// REGISTRO GLOBAL DE RESULTADOS
vector<BenchStats> benchLog;

// FUNCIONES AUXILIARES PARA LOG Y DISPLAY
void logResult(int n, const string& op, const string& dens, double tiempo, const string& arch, int ops=0) {
    benchLog.push_back({n, op, dens, tiempo, arch, ops});
}

void printHeader(const string& arch) {
    cout << "\n=== INICIO DE BENCHMARK PARA " << arch << " ===\n";
    cout << setw(8) << "N" << " | "
         << setw(12) << "Operacion" << " | "
         << setw(6) << "Densidad" << " | "
         << setw(10) << "Tiempo(s)" << " | "
         << setw(8) << "Ops" << "\n";
    cout << string(50, '-') << "\n";
}

void printBenchLine(const BenchStats& b) {
    cout << setw(8) << b.numElements << " | "
         << setw(12) << b.operation << " | "
         << setw(6) << b.density << " | "
         << setw(10) << fixed << setprecision(4) << b.elapsedSec << " | "
         << setw(8) << b.operationsCount << "\n";
}

// LLENAR MATRIZ CON VALORES ALEATORIOS
void fillRandomMatrix(SparseMatrix& M, int n, int range) {
    for (int i = 0; i < n; ++i) {
        int r = rand() % range;
        int c = rand() % range;
        int val = rand() % 100 + 1;
        M.insert(val, r, c);
    }
}

// GENERAR MATRIZ SOCIA PARTNER BASADA EN LA PRINCIPAL
void buildPartnerMatrix(const SparseMatrix& mainMatrix, SparseMatrix& partner) {
    partner.clearAll();
    int maxR, maxC;
    mainMatrix.getBounds(maxR, maxC);
    if(maxR<0 || maxC<0) return;

    for(int r=0; r<=maxR; ++r){
        for(int c=0; c<=maxC; ++c){
            int val = mainMatrix.retrieve(r,c);
            if(val!=0){
                int newVal = rand()%10+1;
                partner.insert(newVal, r, c);
            }
        }
    }
}

// BENCHMARKS ESPECIFICOS

// INSERCION
void insertBenchmark(int n, int range, const string& dens, const string& arch) {
    SparseMatrix M;
    clock_t start = clock();
    int ops = 0;
    for(int i=0; i<n; ++i){
        int r = rand() % range;
        int c = rand() % range;
        int val = rand() % 100 + 1;
        M.insert(val,r,c);
        ops++;
    }
    clock_t end = clock();
    double tiempo = double(end - start)/CLOCKS_PER_SEC;
    logResult(n,"Insercion",dens,tiempo,arch,ops);
    cout << "[Insercion] N="<<n<<" | densidad="<<dens<<" | tiempo="<<tiempo<<" s | densidad real="<<M.densityPercent()<<"%\n";
}

// BUSQUEDA
void searchBenchmark(int n, int range, const string& dens, const string& arch){
    SparseMatrix M;
    fillRandomMatrix(M, n, range);
    clock_t start = clock();
    int ops = 0;
    for(int i=0; i<n; ++i){
        int r=rand()%range;
        int c=rand()%range;
        M.retrieve(r,c);
        ops++;
    }
    clock_t end = clock();
    double tiempo = double(end-start)/CLOCKS_PER_SEC;
    logResult(n,"Busqueda",dens,tiempo,arch,ops);
    cout << "[Busqueda] N="<<n<<" | densidad="<<dens<<" | tiempo="<<tiempo<<" s\n";
}

// MULTIPLICACION
void multiplyBenchmark(int n, const string& arch){
    SparseMatrix A,B;
    int ops=0;
    for(int i=0;i<n;i++){
        A.insert(rand()%10+1,rand()%20,rand()%30);
        B.insert(rand()%10+1,rand()%30,rand()%15);
        ops+=2;
    }
    clock_t start=clock();
    SparseMatrix* R = A.multiplyWith(&B);
    clock_t end=clock();
    double tiempo = double(end-start)/CLOCKS_PER_SEC;
    logResult(n,"Multiplicacion","Media",tiempo,arch,ops);
    cout << "[Multiplicacion] N="<<n<<" | tiempo="<<tiempo<<" s | elementos resultado:\n";
    R->displayNonZeros();
    delete R;
}

// EJECUTAR BENCHMARKS PARA UNA ARQUITECTURA ESPECIFICA DEL TALLER
void executeBenchForArch(const string& arch){
    //VALORES SOLICITADOS
    vector<int> sizes = {50, 250, 500, 1000, 5000};
    printHeader(arch);

    for(int n : sizes){
        insertBenchmark(n,n*12,"Baja",arch);
        searchBenchmark(n,n*12,"Baja",arch);
        insertBenchmark(n,max(5,n/10),"Alta",arch);
        searchBenchmark(n,max(5,n/10),"Alta",arch);
        if(n>=400) multiplyBenchmark(n,arch);
    }
}

// RESUMEN FINAL DE TODOS LOS BENCHMARKS EJECUTADOS
void printFinalSummary(const string& arch){
    cout << "\n=== RESUMEN FINAL ("<<arch<<") ===\n";
    int totalOps = accumulate(benchLog.begin(), benchLog.end(), 0,
        [](int sum,const BenchStats& b){ return sum+b.operationsCount; });

    double avgTime = 0.0;
    int count=0;
    for(auto& b : benchLog){
        if(b.arch==arch){
            printBenchLine(b);
            avgTime+=b.elapsedSec;
            count++;
        }
    }
    if(count>0) avgTime/=count;
    cout << "Total operaciones ejecutadas: "<<totalOps<<"\n";
    cout << "Tiempo promedio por prueba: "<<avgTime<<" s\n";
}

// MAIN BENCHMARK DE MI ENTRY POINT
int main_bench(){
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "=== BENCHMARK ROBUSTO DE MATRICES DISPERSAS ===\n";
    cout << "Seleccione arquitectura (1=Intel,2=AMD,3=Apple): ";
    int opt; cin>>opt;
    string arch="Intel";
    if(opt==2) arch="AMD";
    else if(opt==3) arch="Apple";

    cout << "Ejecutando benchmarks para "<<arch<<"...\n";
    executeBenchForArch(arch);
    printFinalSummary(arch);

    cout << "\n*** FIN DEL BENCHMARK ***\n";
    return 0;
}
