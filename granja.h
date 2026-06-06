#ifndef GRANJA_H
#define GRANJA_H

#include <mutex>
#include <chrono>
#include <string>
#include "semaforo.h"

//Entidad Job: Almacena el ID, prioridad estática, estado y el timestamp fundamental para el Aging.
struct Job {
    int id;
    int prioridad; // 1 = Premiun / 0 = Free
    std::string estado;
    std::chrono::steady_clock::time_point tiempo_llegada;
};

extern Job message_queue[2000];
extern int cantidad_en_cola;
extern int tareas_finalizadas;

extern std::mutex mtx_contador;
extern std::mutex mtx_consola;
extern std::mutex mtx_message_queue;
extern std::mutex mtx_log;

// Buffer 2 (Pool de VRAM):Usamos candados para los retardos obligatorios de entrada y salida a la VRAM.
extern std::mutex mtx_vram_entrada;
extern std::mutex mtx_vram_salida;

extern Semaforo jobs_pendientes;
extern Semaforo slots_vram; // Limita a 5 espacios simultáneos

// Firmas del modelo Productor-Consumidor
void nodo_api_gateway(int id_productor, int cantidad_jobs);
void worker_node(int id_worker, int tareas_a_procesar);

// Firmas de sobrecarga para los logs
void log_evento(const std::string& mensaje);
void log_evento(int job_id, const std::string& prioridad, const std::string& evento);

#endif
