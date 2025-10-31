/**
 * @file SensorTemperatura.h
 * @brief Define la clase SensorTemperatura, derivada de SensorBase.
 * @project Sistema IoT de Monitoreo Polimórfico
 */

#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <cstdlib> // atof

/**
 * @class SensorTemperatura
 * @brief Implementa un sensor especializado en temperaturas (float).
 * * Su lógica de procesamiento incluye la eliminación del valor menor para 
 * filtrar posibles errores antes de calcular el promedio.
 */
class SensorTemperatura : public SensorBase {
private:
    /** @brief Lista enlazada que almacena el historial de lecturas (float). */
    ListaSensor<float> historial;
public:
    /**
     * @brief Constructor. Llama al constructor de SensorBase.
     * @param nom ID del sensor.
     */
    SensorTemperatura(const char* nom) : SensorBase(nom) {}

    /** @brief Destructor. */
    virtual ~SensorTemperatura() {}

    /**
     * @brief Implementación para agregar la lectura, convierte de char* a float.
     * @param valorTxt Valor de la temperatura en texto.
     */
    void agregarLecturaDesdeTexto(const char* valorTxt) override {
        // Convierte el texto a float (punto flotante)
        float v = static_cast<float>(atof(valorTxt));
        historial.insertarFinal(v);
        std::cout << "[Log] Insertando Nodo<float> en " << nombre << ": " << v << "\n";
    }

    /**
     * @brief Lógica de procesamiento específica para Temperatura (Polimorfismo).
     * Elimina el menor valor y calcula el promedio ajustado.
     */
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << " (Temperatura)\n";
        if (historial.estaVacia()) {
            std::cout << "   No hay lecturas.\n";
            return;
        }
        // Lógica de filtrado: elimina la más baja
        historial.eliminarMenor();
        float prom = historial.promedio();
        std::cout << "   Promedio después de eliminar menor: " << prom << "\n";
    }

    /** @brief Muestra el tipo y el ID del sensor. */
    void imprimirInfo() const override {
        std::cout << "[SensorTemperatura] ID=" << nombre << "\n";
    }
};

#endif