#include <iostream>
#include <fstream>
#include <thread>
#include "granja.h"

void productor_puros_premium() {
    for (int i = 0; i < 8; ++i) {
        Job j; j.id = 9000 + i; j.prioridad = 1; j.estado = "EN_COLA";
        j.tiempo_llegada = std::chrono::steady_clock::now();

        // Inserción con arreglo clásico
        mtx_message_queue.lock();
        message_queue[cantidad_en_cola] = j;
        cantidad_en_cola++;
        mtx_message_queue.unlock();

        log_evento(j.id, "Premium", "CREADO_Y_EN_COLA");
        signal(jobs_pendientes);
    }
}

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc); archivo_limpio.close();
    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- PRUEBA 3: SATURACION DE RECURSOS (8 PREMIUM) ---");

    std::thread prod(productor_puros_premium);
    prod.join();

    const int NUM_WORKERS = 8;
    std::thread workers[NUM_WORKERS];

    for (int i = 0; i < NUM_WORKERS; ++i) {
        workers[i] = std::thread(worker_node, i + 1, 1);
    }

    for (int i = 0; i < NUM_WORKERS; ++i) {
        workers[i].join();
    }

    log_evento("--- PRUEBA 3 TERMINADA ---");
    return 0;
}
