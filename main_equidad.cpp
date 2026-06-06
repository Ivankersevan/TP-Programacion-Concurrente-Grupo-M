#include <iostream>
#include <fstream>
#include <thread>
#include "granja.h"

void productor_tramposo() {
    Job free_job; free_job.id = 1111; free_job.prioridad = 0;
    free_job.estado = "EN_COLA"; free_job.tiempo_llegada = std::chrono::steady_clock::now();

    mtx_message_queue.lock();
    message_queue[cantidad_en_cola] = free_job;
    cantidad_en_cola++;
    mtx_message_queue.unlock();

    log_evento(free_job.id, "Free", "CREADO");
    signal(jobs_pendientes);

    for (int i = 0; i < 6; ++i) {
        Job prem; prem.id = 9000 + i; prem.prioridad = 1;
        prem.estado = "EN_COLA"; prem.tiempo_llegada = std::chrono::steady_clock::now();

        mtx_message_queue.lock();
        message_queue[cantidad_en_cola] = prem;
        cantidad_en_cola++;
        mtx_message_queue.unlock();

        log_evento(prem.id, "Premium", "CREADO");
        signal(jobs_pendientes);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // Retardo para demostrar Prueba
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc); archivo_limpio.close();
    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- PRUEBA 4: EQUIDAD Y AGING (ANTI-STARVATION) ---");

    std::thread prod(productor_tramposo);
    std::thread worker(worker_node, 1, 7);

    prod.join();
    worker.join();

    log_evento("--- PRUEBA 4 TERMINADA ---");
    return 0;
}
