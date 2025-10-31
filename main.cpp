/**
 * @file main.cpp
 * @brief Contiene la lógica principal, la interfaz de usuario y las funciones de comunicación serial.
 * @project Sistema IoT de Monitoreo Polimórfico
 * @author [Tu Nombre]
 * @date [Fecha Actual]
 */

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <cstdlib> // atoi, atof

#include "ListaGestion.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"

using namespace std;

// ===================== FUNCIONES DE SERIAL/COM =======================

/**
 * @brief Configura y abre el puerto serial para la comunicación (Linux/macOS).
 * @param puerto Nombre del puerto (ej. "/dev/ttyUSB0").
 * @return Descriptor de archivo (int) del puerto abierto, o -1 si falla.
 */
int configurarSerial(const char* puerto) {
    int serial = open(puerto, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial < 0) {
        perror("No se pudo abrir el puerto serial");
        return -1;
    }

    fcntl(serial, F_SETFL, 0); // Modo bloqueante

    struct termios opciones;
    tcgetattr(serial, &opciones);

    cfsetispeed(&opciones, B115200); // Velocidad de entrada
    cfsetospeed(&opciones, B115200); // Velocidad de salida

    opciones.c_cflag |= (CLOCAL | CREAD);
    opciones.c_cflag &= ~PARENB;
    opciones.c_cflag &= ~CSTOPB;
    opciones.c_cflag &= ~CSIZE;
    opciones.c_cflag |= CS8;

    opciones.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Modo RAW (lectura sin formato)
    opciones.c_iflag &= ~(IXON | IXOFF | IXANY);
    opciones.c_oflag &= ~OPOST;

    tcsetattr(serial, TCSANOW, &opciones);

    return serial;
}

/**
 * @brief Lee una línea completa (hasta \n) del puerto serial.
 * @param fd Descriptor de archivo del puerto.
 * @param buffer Buffer donde se guardará la línea leída.
 * @param buf_size Tamaño del buffer.
 * @return true si se leyó una línea completa, false en caso contrario.
 */
bool leerLinea(int fd, char* buffer, size_t buf_size) {
    ssize_t n = read(fd, buffer, buf_size - 1);
    if (n > 0) {
        buffer[n] = '\0';
        // Eliminar caracteres de retorno de carro o nueva linea
        for (int i = 0; i < n; i++) {
            if (buffer[i] == '\r' || buffer[i] == '\n') {
                buffer[i] = '\0';
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Parsea una línea de texto recibida por serial en sus componentes.
 * @param linea La trama de entrada (ej. "T;T-001;25.6").
 * @param tipo Puntero donde se guardará el Tipo (T o P).
 * @param id Buffer donde se guardará el ID del sensor.
 * @param valor Buffer donde se guardará el valor de la lectura.
 */
void parsearLinea(char* linea, char* tipo, char* id, char* valor) {
    *tipo = '\0';
    id[0] = '\0';
    valor[0] = '\0';

    char* token = std::strtok(linea, ";");
    if (token) {
        *tipo = token[0];
        token = std::strtok(NULL, ";");
        if (token) {
            std::strncpy(id, token, 49);
            id[49] = '\0';
            token = std::strtok(NULL, ";");
            if (token) {
                std::strncpy(valor, token, 49);
                valor[49] = '\0';
            }
        }
    } else {
         // Si la trama está vacía o incompleta
        *tipo = 'X'; 
    }
}

// ===================== FUNCIONES DE GESTIÓN =======================

/**
 * @brief Muestra el menú de opciones en la consola.
 */
void mostrarMenu() {
    cout << "\n===== Sistema IoT de Monitoreo Polimórfico =====\n";
    cout << "1. Agregar Instrumento (Manual)\n";
    cout << "2. Ingresar Dato de Lectura (Manual)\n";
    cout << "3. Ejecutar Logica de Procesamiento (Polimorfico)\n";
    cout << "4. Listar Instrumentos Registrados\n";
    cout << "5. Leer 1 Trama de la UART/COM\n";
    cout << "6. Monitoreo Continuo (Ciclo de recepcion)\n";
    cout << "7. Salir del Sistema\n";
    cout << "Elige opcion: ";
}

/**
 * @brief Crea una instancia de SensorTemperatura o SensorPresion.
 * @param tipo Tipo de sensor ('T' o 'P').
 * @param id ID del sensor.
 * @param lista Referencia a la lista de gestión donde se insertará el sensor.
 * @return Puntero al nuevo objeto SensorBase*, o nullptr si el tipo no es válido.
 */
SensorBase* crearSensorPorTipo(char tipo, const char* id, ListaGestion& lista) {
    SensorBase* nuevo_sensor = nullptr;
    if (tipo == 'T') {
        nuevo_sensor = new SensorTemperatura(id);
        lista.insertar(nuevo_sensor);
        cout << "[Log] Sensor de Temperatura '" << id << "' creado.\n";
    } else if (tipo == 'P') {
        nuevo_sensor = new SensorPresion(id);
        lista.insertar(nuevo_sensor);
        cout << "[Log] Sensor de Presion '" << id << "' creado.\n";
    } else {
        cout << "[Error] Tipo de sensor no valido.\n";
    }
    return nuevo_sensor;
}

// ===================== PROGRAMA PRINCIPAL =======================

/**
 * @brief Función principal que contiene el bucle de la interfaz de usuario.
 */
int main() {
    ListaGestion lista;
    int fdSerial = -1;
    bool salir = false;
    
    // Intentar abrir el puerto una vez al inicio
    fdSerial = configurarSerial("/dev/ttyUSB0");

    while (!salir) {
        mostrarMenu();
        int op;
        if (!(cin >> op)) {
            // Manejar error de lectura o EOF
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (op == 1) { // Agregar Sensor (Manual)
            char tipo;
            char id[50];
            cout << "Tipo de instrumento (T=Temperatura, P=Presion): ";
            cin >> tipo;
            cout << "ID del instrumento (ej. T-001): ";
            cin >> id;
            crearSensorPorTipo(tipo, id, lista);
        }
        else if (op == 2) { // Ingresar Dato (Manual)
            char id[50];
            char valor[50];
            cout << "ID del instrumento: ";
            cin >> id;
            cout << "Valor de la lectura: ";
            cin >> valor;

            SensorBase* s = lista.buscarPorNombre(id);
            if (s) {
                s->agregarLecturaDesdeTexto(valor);
            } else {
                cout << "[Error] Sensor '" << id << "' no encontrado.\n";
            }
        }
        else if (op == 3) { // Ejecutar Lógica de Procesamiento (Polimorfico)
            lista.procesarTodos();
        }
        else if (op == 4) { // Listar Sensores
            lista.imprimir();
        }
        else if (op == 5 || op == 6) { // Leer Trama Serial
            if (fdSerial < 0) {
                cout << "[Error] No se pudo abrir el puerto serial. Verifique la conexión.\n";
                continue;
            }

            if (op == 5) { // 5. Leer 1 Trama
                cout << "Esperando a que el dispositivo se inicialice...\n";
                usleep(2000000); // 2 segundos de espera
                cout << "Esperando 1 trama...\n";
                char linea[128];
                if (leerLinea(fdSerial, linea, sizeof(linea))) {
                    if (std::strlen(linea) == 0) continue;
                    cout << "[RX] Trama recibida: " << linea << "\n";

                    char tipo;
                    char id[50];
                    char valor[50];
                    parsearLinea(linea, &tipo, id, valor);

                    SensorBase* s = lista.buscarPorNombre(id);
                    if (!s) {
                        cout << "Sensor " << id << " no existe, creando...\n";
                        s = crearSensorPorTipo(tipo, id, lista);
                    }
                    if (s) {
                        s->agregarLecturaDesdeTexto(valor);
                    }
                } else {
                    cout << "[Error] Tiempo de espera de lectura agotado o trama vacía.\n";
                }
            } else { // 6. Monitoreo Continuo
                cout << "Esperando a que Arduino reinicie...\n";
                usleep(2000000); // 2 segundos de espera
                
                cout << "Leyendo continuamente (Ctrl+C para matar el programa)...\n";
                int contador = 0;
                while (true) {
                    char linea[128];
                    if (leerLinea(fdSerial, linea, sizeof(linea))) {
                        if (std::strlen(linea) == 0) continue;
                        cout << "[RX] Trama recibida: " << linea << "\n";

                        char tipo;
                        char id[50];
                        char valor[50];
                        parsearLinea(linea, &tipo, id, valor);

                        SensorBase* s = lista.buscarPorNombre(id);
                        if (!s) {
                            cout << "Sensor " << id << " no existe, creando...\n";
                            s = crearSensorPorTipo(tipo, id, lista);
                        }
                        if (s) {
                            s->agregarLecturaDesdeTexto(valor);
                        }

                        contador++;
                        if (contador % 5 == 0) {
                            lista.procesarTodos();
                        }
                    }
                }
            }
        }
        else if (op == 7) {
            salir = true;
        }
        else {
            cout << "Opcion no valida.\n";
        }
    }

    if (fdSerial >= 0) {
        close(fdSerial);
    }

    cout << "Saliendo... (la lista se libera sola)\n";
    return 0;
}