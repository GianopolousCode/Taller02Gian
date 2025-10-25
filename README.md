# Taller02Gian

# COMANDO EJECUTAR AUTOMATICO BENCHMARK (PARA OBTENER VALORES)
g++ -std=c++17 bench_runner.cpp Benchmark.cpp SparseMatrix.cpp Cell.cpp -o benchmark -static "-Wl,-subsystem,console"
./Benchmark

# COMANDO EJECUTAR MAIN
g++ -std=c++17 main.cpp SparseMatrix.cpp Cell.cpp -o matriz
./matriz

