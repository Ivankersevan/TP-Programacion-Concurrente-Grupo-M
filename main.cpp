#include <iostream>
#include <fstream>
#include <thread>
#include "granja.h"

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc);
    archivo_limpio.close();

    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- Iniciando Granja de Renderizado ---");
    log_evento("Ejecutando Escenario C: 3 Productores / 3 Consumidores");

    const int NUM_PRODUCTORES = 3;
    const int NUM_CONSUMIDORES = 3;
    const int JOBS_POR_PRODUCTOR = 10;
    const int JOBS_POR_CONSUMIDOR = 10;

    std::thread hilos_productores[NUM_PRODUCTORES];
    std::thread hilos_consumidores[NUM_CONSUMIDORES];

    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        hilos_productores[i] = std::thread(nodo_api_gateway, i + 1, JOBS_POR_PRODUCTOR);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        hilos_consumidores[i] = std::thread(worker_node, i + 1, JOBS_POR_CONSUMIDOR);
    }

    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        hilos_productores[i].join();
    }
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        hilos_consumidores[i].join();
    }

    log_evento("--- Proceso Terminado. Tareas Finalizadas: " + std::to_string(tareas_finalizadas) + " ---");

    return 0;
}
