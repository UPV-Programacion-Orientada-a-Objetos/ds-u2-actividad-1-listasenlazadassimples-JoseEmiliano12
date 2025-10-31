/**
 * @file SensorBase.h
 * @brief Define la clase base abstracta para el Sistema IoT de Monitoreo Polimórfico.
 * @project Sistema IoT de Monitoreo Polimórfico
 * @author [Tu Nombre]
 * @date [Fecha Actual]
 */

#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <iostream>
#include <cstring>

/**
 * @class SensorBase
 * @brief Clase abstracta (contrato) para todos los sensores.
 * * Implementa el polimorfismo. Cualquier clase que herede de SensorBase DEBE
 * implementar las funciones virtuales puras.
 */
class SensorBase {
protected:
    /** @brief Identificador único del sensor (ej. "T-001"). */
    char nombre[50]; 
public:
    /**
     * @brief Constructor de la clase SensorBase.
     * @param nom Nombre o ID del sensor.
     */
    SensorBase(const char* nom = "SIN-NOMBRE") {
        std::strncpy(nombre, nom, sizeof(nombre));
        nombre[sizeof(nombre)-1] = '\0';
    }

    /**
     * @brief Destructor virtual.
     * Crucial para garantizar la liberación correcta de memoria de las clases derivadas.
     */
    virtual ~SensorBase() {} 

    /**
     * @brief Obtiene el ID del sensor.
     * @return Const char* con el nombre (ID) del sensor.
     */
    const char* getNombre() const { 
        return nombre;
    }

    /**
     * @brief Método virtual puro para agregar una lectura.
     * @param valorTxt Valor de la lectura en formato de texto (char*).
     */
    virtual void agregarLecturaDesdeTexto(const char* valorTxt) = 0; 

    /**
     * @brief Método virtual puro que ejecuta la lógica de análisis del sensor.
     * Implementa el polimorfismo.
     */
    virtual void procesarLectura() = 0; 

    /**
     * @brief Método virtual puro para mostrar la información básica del sensor.
     */
    virtual void imprimirInfo() const = 0; 
};

#endif