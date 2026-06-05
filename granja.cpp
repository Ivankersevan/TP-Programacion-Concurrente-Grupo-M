#include "granja.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <iomanip>

std::vector<Job> message_queue;
int tareas_finalizadas = 0;

std::mutex mtx_contador;
std::mutex mtx_consola;
std::mutex mtx_message_queue;
std::mutex mtx_log;
std::mutex mtx_vram_entrada;
std::mutex mtx_vram_salida;

Semaforo jobs_pendientes;
Semaforo slots_vram;


// LOG PARA MENSAJES DEL SISTEMA
void log_evento(const std::string& mensaje) {
    auto ahora = std::chrono::system_clock::now();
    std::time_t tiempo = std::chrono::system_clock::to_time_t(ahora);
    std::tm* tm_info = std::localtime(&tiempo);
    char buffer_hora[30];
    std::strftime(buffer_hora, sizeof(buffer_hora), "%Y-%m-%d %H:%M:%S", tm_info);

    std::string texto = "[" + std::string(buffer_hora) + "] - " + mensaje;

    mtx_log.lock();
    std::ofstream archivo("sistema.log", std::ios::app);
    if (archivo.is_open()) {
        archivo << texto << "\n";
        archivo.close();
    }
    mtx_log.unlock();

    mtx_consola.lock();
    std::cout << texto << std::endl;
    mtx_consola.unlock();
}

// LOG PARA LOS JOBS
void log_evento(int job_id, const std::string& prioridad, const std::string& evento) {
    auto ahora = std::chrono::system_clock::now();
    std::time_t tiempo = std::chrono::system_clock::to_time_t(ahora);
    std::tm* tm_info = std::localtime(&tiempo);
    char buffer_hora[30];
    std::strftime(buffer_hora, sizeof(buffer_hora), "%Y-%m-%d %H:%M:%S", tm_info);

    std::string texto = "[" + std::string(buffer_hora) + "] - Job ID: " +
                        std::to_string(job_id) + " - Prioridad: " +
                        prioridad + " - Evento: " + evento;

    mtx_log.lock();
    std::ofstream archivo("sistema.log", std::ios::app);
    if (archivo.is_open()) {
        archivo << texto << "\n";
        archivo.close();
    }
    mtx_log.unlock();

    mtx_consola.lock();
    std::cout << texto << std::endl;
    mtx_consola.unlock();
}

void nodo_api_gateway(int id_productor, int cantidad_jobs) {
    for (int i = 0; i < cantidad_jobs; ++i) {
        Job nuevo_job;
        nuevo_job.id = (id_productor * 1000) + i;
        nuevo_job.prioridad = (i % 5 == 0) ? 1 : 0;
        nuevo_job.estado = "EN_COLA";
        nuevo_job.tiempo_llegada = std::chrono::steady_clock::now();
        std::string tipo = (nuevo_job.prioridad == 1) ? "Premium" : "Free";

        log_evento(nuevo_job.id, tipo, "CREADO");

        mtx_message_queue.lock();
        message_queue.push_back(nuevo_job);
        mtx_message_queue.unlock();

        log_evento(nuevo_job.id, tipo, "EN_COLA");
        signal(jobs_pendientes);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void worker_node(int id_worker, int tareas_a_procesar) {
    for (int i = 0; i < tareas_a_procesar; ++i) {
        wait(jobs_pendientes);

        mtx_message_queue.lock();
        auto ahora = std::chrono::steady_clock::now();
        auto mejor_job = message_queue.begin();
        double mejor_puntaje = -1.0;

        for (auto it = message_queue.begin(); it != message_queue.end(); ++it) {
            auto espera_ms = std::chrono::duration_cast<std::chrono::milliseconds>(ahora - it->tiempo_llegada).count();
            double puntaje = (it->prioridad * 1000) + (espera_ms * 0.5);

            if (puntaje > mejor_puntaje) {
                mejor_puntaje = puntaje;
                mejor_job = it;
            }
        }

        Job trabajo_actual = *mejor_job;
        message_queue.erase(mejor_job);
        mtx_message_queue.unlock();

        std::string tipo = (trabajo_actual.prioridad == 1) ? "Premium" : "Free";

        wait(slots_vram);

        mtx_vram_entrada.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(450));
        mtx_vram_entrada.unlock();

        log_evento(trabajo_actual.id, tipo, "ASIGNADO_VRAM");

        std::this_thread::sleep_for(std::chrono::milliseconds(600));

        mtx_vram_salida.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        mtx_vram_salida.unlock();

        signal(slots_vram);

        log_evento(trabajo_actual.id, tipo, "FINALIZADO");

        mtx_contador.lock();
        tareas_finalizadas++;
        mtx_contador.unlock();
    }
}
