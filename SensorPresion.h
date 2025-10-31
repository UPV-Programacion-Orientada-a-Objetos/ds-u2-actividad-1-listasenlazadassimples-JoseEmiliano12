/**
 * @file SensorPresion.h
 * @brief Define la clase SensorPresion, derivada de SensorBase.
 * @project Sistema IoT de Monitoreo Polimórfico
 */

#ifndef SENSOR_PRESION_H
#define SENSOR_PRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <cstdlib> // atoi

/**
 * @class SensorPresion
 * @brief Implementa un sensor especializado en presión (int).
 * * Su lógica de procesamiento (procesarLectura) es el cálculo de un promedio simple.
 */
class SensorPresion : public SensorBase {
private:
    /** @brief Lista enlazada que almacena el historial de lecturas (int). */
    ListaSensor<int> historial;
public:
    /**
     * @brief Constructor. Llama al constructor de SensorBase.
     * @param nom ID del sensor.
     */
    SensorPresion(const char* nom) : SensorBase(nom) {}

    /** @brief Destructor. */
    virtual ~SensorPresion() {}

    /**
     * @brief Implementación para agregar la lectura, convierte de char* a int.
     * @param valorTxt Valor de la presión en texto.
     */
    void agregarLecturaDesdeTexto(const char* valorTxt) override {
        int v = atoi(valorTxt);
        historial.insertarFinal(v);
        std::cout << "[Log] Insertando Nodo<int> en " << nombre << ": " << v << "\n";
    }

    /**
     * @brief Lógica de procesamiento específica para Presión (Polimorfismo).
     * Calcula el promedio simple de los valores.
     */
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << " (Presion)\n";
        if (historial.estaVacia()) {
            std::cout << "   No hay lecturas.\n";
            return;
        }
        int prom = historial.promedio();
        std::cout << "   Promedio de lecturas: " << prom << "\n";
    }

    /** @brief Muestra el tipo y el ID del sensor. */
    void imprimirInfo() const override {
        std::cout << "[SensorPresion] ID=" << nombre << "\n";
    }
};

#endif