#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "granja.h"

void productor_tramposo() {
    Job free_job; free_job.id = 1111; free_job.prioridad = 0;
    free_job.estado = "EN_COLA"; free_job.tiempo_llegada = std::chrono::steady_clock::now();
    mtx_message_queue.lock(); message_queue.push_back(free_job); mtx_message_queue.unlock();
    log_evento(free_job.id, "Free", "CREADO");
    signal(jobs_pendientes);

    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        Job prem; prem.id = 9000 + i; prem.prioridad = 1;
        prem.estado = "EN_COLA"; prem.tiempo_llegada = std::chrono::steady_clock::now();
        mtx_message_queue.lock(); message_queue.push_back(prem); mtx_message_queue.unlock();
        log_evento(prem.id, "Premium", "CREADO");
        signal(jobs_pendientes);
    }
}

int main() {
    std::ofstream archivo_limpio("sistema.log", std::ios::trunc); archivo_limpio.close();
    init(jobs_pendientes, 0);
    init(slots_vram, 5);

    log_evento("--- PRUEBA 4: EQUIDAD Y AGING (ANTI-STARVATION) ---");

    std::thread prod(productor_tramposo);

    std::thread worker(worker_node, 1, 11);

    prod.join();
    worker.join();

    log_evento("--- PRUEBA 4 TERMINADA ---");
    return 0;
}
