/**
 * @file ListaGestion.h
 * @brief Define la lista que almacena y gestiona todos los sensores del sistema.
 * @project Sistema IoT de Monitoreo Polimórfico
 */

#ifndef LISTA_GESTION_H
#define LISTA_GESTION_H

#include "SensorBase.h"
#include <iostream>
#include <cstring>

/**
 * @struct NodoGestion
 * @brief Nodo para la ListaGestion. Almacena un puntero polimórfico a SensorBase.
 */
struct NodoGestion {
    /** @brief Puntero polimórfico que puede apuntar a SensorTemperatura o SensorPresion. */
    SensorBase* sensor;
    NodoGestion* sig;
    /** @brief Constructor del nodo de gestión. */
    NodoGestion(SensorBase* s) : sensor(s), sig(nullptr) {}
};

/**
 * @class ListaGestion
 * @brief Colección principal que administra todos los objetos SensorBase.
 * * Implementa las funciones de búsqueda y la ejecución polimórfica de la lógica de procesamiento.
 */
class ListaGestion {
private:
    /** @brief Puntero al primer nodo de la lista. */
    NodoGestion* cabeza;
public:
    /** @brief Constructor. Inicializa la lista vacía. */
    ListaGestion() : cabeza(nullptr) {}

    /**
     * @brief Destructor. Libera toda la memoria dinámica.
     * * Llama a 'delete borr->sensor', activando el destructor virtual para liberar
     * la memoria de cada objeto SensorBase y, a su vez, la de sus ListasSensor internas.
     */
    ~ListaGestion() {
        NodoGestion* tmp = cabeza;
        while (tmp) {
            NodoGestion* borr = tmp;
            tmp = tmp->sig;
            if (borr->sensor) {
                std::cout << "[Destructor General] Liberando Nodo: " << borr->sensor->getNombre() << "\n";
                delete borr->sensor; // Llama al destructor virtual correcto
            }
            delete borr;
        }
        cabeza = nullptr;
    }

    /**
     * @brief Inserta un nuevo sensor al final de la lista.
     * @param s Puntero al objeto SensorBase (ej. SensorTemperatura* o SensorPresion*).
     */
    void insertar(SensorBase* s) {
        NodoGestion* nuevo = new NodoGestion(s);
        if (!cabeza) {
            cabeza = nuevo;
            return;
        }
        NodoGestion* tmp = cabeza;
        while (tmp->sig) tmp = tmp->sig;
        tmp->sig = nuevo;
    }

    /**
     * @brief Busca un sensor por su ID (nombre).
     * @param nom ID (nombre) del sensor a buscar.
     * @return Puntero a SensorBase* si lo encuentra, nullptr si no.
     */
    SensorBase* buscarPorNombre(const char* nom) {
        NodoGestion* tmp = cabeza;
        while (tmp) {
            if (std::strcmp(tmp->sensor->getNombre(), nom) == 0) {
                return tmp->sensor;
            }
            tmp = tmp->sig;
        }
        return nullptr;
    }

    /**
     * @brief Ejecuta la función procesarLectura() en todos los sensores registrados.
     * * Es la función que demuestra el polimorfismo en tiempo de ejecución.
     */
    void procesarTodos() {
        std::cout << "--- Ejecutando Polimorfismo ---\n";
        NodoGestion* tmp = cabeza;
        while (tmp) {
            tmp->sensor->procesarLectura(); // Se llama a la función correcta de cada subclase
            tmp = tmp->sig;
        }
    }

    /**
     * @brief Imprime la información de todos los sensores registrados.
     */
    void imprimir() const {
        std::cout << "[Lista de Gestion]\n";
        NodoGestion* tmp = cabeza;
        if (!tmp) {
            std::cout << "Lista de gestion vacia.\n";
            return;
        }
        while (tmp) {
            tmp->sensor->imprimirInfo();
            tmp = tmp->sig;
        }
    }
};

#endif