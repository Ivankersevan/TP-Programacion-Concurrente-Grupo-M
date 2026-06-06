#include <iostream>
#include <fstream>
#include <thread>
#include "granja.h"

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc); archivo_limpio.close();
    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- PRUEBA 1: CARGA MASIVA (1500 JOBS) ---");

    const int NUM_HILOS = 3;
    const int JOBS_POR_HILO = 500;

    std::thread productores[NUM_HILOS];
    std::thread consumidores[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; ++i) {
        productores[i] = std::thread(nodo_api_gateway, i + 1, JOBS_POR_HILO);
        consumidores[i] = std::thread(worker_node, i + 1, JOBS_POR_HILO);
    }

    for (int i = 0; i < NUM_HILOS; ++i) {
        productores[i].join();
        consumidores[i].join();
    }

    log_evento("--- PRUEBA 1 TERMINADA. Total Finalizadas: " + std::to_string(tareas_finalizadas) + " ---");
    return 0;
}
