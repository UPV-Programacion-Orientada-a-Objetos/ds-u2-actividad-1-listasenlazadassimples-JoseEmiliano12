cmake_minimum_required(VERSION 3.5)

project(SistemaIoTPolimorfico CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sistema_iot
    src/main.cpp
    src/SensorBase.h
    src/SensorTemperatura.h
    src/SensorPresion.h
    src/ListaSensor.h
    src/ListaGestion.h
)
