#include <iostream>
#include <fstream>
#include <thread>
#include "granja.h"

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc); archivo_limpio.close();
    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- PRUEBA 2: VACUIDAD (0 JOBS) ---");
    log_evento("El sistema quedara en espera pasiva (No consume CPU). Cierre la consola manualmente.");

    std::thread worker(worker_node, 1, 1);

    worker.join();

    return 0;
}
