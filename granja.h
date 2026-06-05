#ifndef GRANJA_H
#define GRANJA_H

#include <vector>
#include <mutex>
#include <chrono>
#include <string>
#include "semaforo.h"

struct Job {
    int id;
    int prioridad;
    std::string estado;
    std::chrono::steady_clock::time_point tiempo_llegada;
};

extern std::vector<Job> message_queue;
extern int tareas_finalizadas;

extern std::mutex mtx_contador;
extern std::mutex mtx_consola;
extern std::mutex mtx_message_queue;
extern std::mutex mtx_log;
extern std::mutex mtx_vram_entrada;
extern std::mutex mtx_vram_salida;

extern Semaforo jobs_pendientes;
extern Semaforo slots_vram;

void nodo_api_gateway(int id_productor, int cantidad_jobs);
void worker_node(int id_worker, int tareas_a_procesar);

void log_evento(const std::string& mensaje);
void log_evento(int job_id, const std::string& prioridad, const std::string& evento);

#endif
