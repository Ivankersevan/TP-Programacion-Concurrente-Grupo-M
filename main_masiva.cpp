#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "granja.h"

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc); archivo_limpio.close();
    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- PRUEBA 1: CARGA MASIVA (1500 JOBS) ---");

    const int NUM_HILOS = 3;
    const int JOBS_POR_HILO = 500;

    std::vector<std::thread> productores;
    std::vector<std::thread> consumidores;

    for (int i = 1; i <= NUM_HILOS; ++i) {
        productores.emplace_back(nodo_api_gateway, i, JOBS_POR_HILO);
        consumidores.emplace_back(worker_node, i, JOBS_POR_HILO);
    }

    for (auto& p : productores) p.join();
    for (auto& c : consumidores) c.join();

    log_evento("--- PRUEBA 1 TERMINADA. Total Finalizadas: " + std::to_string(tareas_finalizadas) + " ---");
    return 0;
}
