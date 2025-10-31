/**
 * @file ListaSensor.h
 * @brief Implementa una lista enlazada genérica para el historial de lecturas.
 * @project Sistema IoT de Monitoreo Polimórfico
 */

#ifndef LISTA_SENSOR_H
#define LISTA_SENSOR_H

#include <iostream>
#include <limits> // Para numeric_limits

/**
 * @struct NodoLS
 * @brief Nodo para la ListaSensor.
 * @tparam T Tipo de dato a almacenar (int o float).
 */
template <typename T>
struct NodoLS {
    T dato;
    NodoLS<T>* sig;
    /** @brief Constructor del nodo. */
    NodoLS(const T& d) : dato(d), sig(nullptr) {}
};

/**
 * @class ListaSensor
 * @brief Lista enlazada simple y genérica (template) que almacena el historial de lecturas.
 * @tparam T Tipo de dato a almacenar.
 */
template <typename T>
class ListaSensor {
private:
    /** @brief Puntero al primer nodo de la lista. */
    NodoLS<T>* cabeza;
public:
    /** @brief Constructor. Inicializa la lista vacía. */
    ListaSensor() : cabeza(nullptr) {}

    /** @brief Destructor. Llama a limpiar() para liberar todos los nodos. */
    ~ListaSensor() {
        limpiar();
    }
    
    // Simplificando por ser un ejemplo, se deben implementar Regla de 3/5:
    ListaSensor(const ListaSensor& other) = delete;
    ListaSensor& operator=(const ListaSensor& other) = delete;

    /**
     * @brief Inserta un nuevo valor al final de la lista.
     * @param valor El dato de tipo T a insertar.
     */
    void insertarFinal(const T& valor) {
        NodoLS<T>* nuevo = new NodoLS<T>(valor);
        if (!cabeza) {
            cabeza = nuevo;
            return;
        }
        NodoLS<T>* tmp = cabeza;
        while (tmp->sig) tmp = tmp->sig;
        tmp->sig = nuevo;
    }

    /**
     * @brief Verifica si la lista está vacía.
     * @return true si está vacía, false en caso contrario.
     */
    bool estaVacia() const {
        return cabeza == nullptr;
    }

    /**
     * @brief Calcula el promedio de todos los elementos en la lista.
     * @return El promedio de los datos. Devuelve 0 si la lista está vacía.
     */
    T promedio() {
        if (!cabeza) return static_cast<T>(0);
        
        T suma = static_cast<T>(0);
        int c = 0;
        NodoLS<T>* tmp = cabeza;
        while (tmp) {
            suma += tmp->dato;
            c++;
            tmp = tmp->sig;
        }
        if (c == 0) return static_cast<T>(0);
        return suma / c;
    }

    /**
     * @brief Elimina el nodo que contiene el valor más pequeño.
     * * Esta lógica es usada por SensorTemperatura para filtrar datos anómalos.
     */
    void eliminarMenor() {
        if (!cabeza || !cabeza->sig) return;

        NodoLS<T>* menor = cabeza;
        NodoLS<T>* antMenor = nullptr;

        NodoLS<T>* ant = nullptr;
        NodoLS<T>* cur = cabeza;
        
        while (cur) {
            if (cur->dato < menor->dato) {
                menor = cur;
                antMenor = ant;
            }
            ant = cur;
            cur = cur->sig;
        }

        if (antMenor == nullptr) {
            // el menor es la cabeza
            cabeza = cabeza->sig;
            delete menor;
        } else {
            antMenor->sig = menor->sig;
            delete menor;
        }
    }

    /**
     * @brief Libera la memoria de todos los nodos de la lista.
     */
    void limpiar() {
        NodoLS<T>* tmp = cabeza;
        while (tmp) {
            NodoLS<T>* borr = tmp;
            tmp = tmp->sig;
            delete borr;
        }
        cabeza = nullptr;
    }
};

#endif