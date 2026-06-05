#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
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

    std::vector<std::thread> hilos_productores;
    std::vector<std::thread> hilos_consumidores;

    for (int i = 1; i <= NUM_PRODUCTORES; ++i) {
        hilos_productores.emplace_back(nodo_api_gateway, i, JOBS_POR_PRODUCTOR);
    }

    for (int i = 1; i <= NUM_CONSUMIDORES; ++i) {
        hilos_consumidores.emplace_back(worker_node, i, JOBS_POR_CONSUMIDOR);
    }

    for (auto& p : hilos_productores) p.join();
    for (auto& c : hilos_consumidores) c.join();

    log_evento("--- Proceso Terminado. Tareas Finalizadas: " + std::to_string(tareas_finalizadas) + " ---");

    return 0;
}
