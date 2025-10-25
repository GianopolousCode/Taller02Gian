// BENCH_RUNNER.CPP
// MI ARCHIVO SIMPLEMENTE LLAMA A MAIN_BENCH() DEFINIDO EN BEN
extern int main_bench();  // ESTO DECLARA QUE MAIN BENCH() ESTÁ EN BENCHMARK.CPP

int main() {
    return main_bench();
}

