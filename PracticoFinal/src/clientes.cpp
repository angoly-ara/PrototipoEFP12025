#include "clientes.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include "bitacora.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Rango de c�digos para clientes
const int CODIGO_INICIAL = 3107;
const int CODIGO_FINAL = 3157;

/**
 * Genera un ID �nico dentro del rango permitido (3107�3157) para un nuevo cliente.
 * @param lista Lista actual de clientes.
 * @return ID disponible como string, o cadena vac�a si el rango est� lleno.
 */
std::string Clientes::generarIdUnico(const std::vector<Clientes>& lista) {
    // Itera dentro del rango permitido para generar un ID �nico
    for (int i = CODIGO_INICIAL; i <= CODIGO_FINAL; ++i) {
        std::string id = std::to_string(i);
        if (idDisponible(lista, id)) {
            return id;
        }
    }
    return "";
}

/**
 * Verifica si un ID est� disponible (no repetido) en la lista de clientes.
 * @param lista Lista de clientes.
 * @param id ID a verificar.
 * @return true si el ID no est� en uso, false si ya est� ocupado.
 */
bool Clientes::idDisponible(const std::vector<Clientes>& lista, const std::string& id) {
    // Verifica si el ID ya existe en la lista de clientes
    return std::none_of(lista.begin(), lista.end(),
        [&id](const Clientes& c) { return c.id == id; });
}

/**
 * Valida si un ID es un n�mero dentro del rango permitido.
 * @param id ID a validar.
 * @return true si es v�lido, false si no.
 */
bool Clientes::esIdValido(const std::string& id) {
    try {
        int num = std::stoi(id);
        return (num >= CODIGO_INICIAL && num <= CODIGO_FINAL);
    } catch (...) {
        return false;
    }
}

/**
 * Agrega un nuevo cliente a la lista, solicitando los datos desde la entrada est�ndar.
 * Asigna autom�ticamente un ID �nico, guarda en archivo y registra en bit�cora.
 * @param lista Lista de clientes (modificable).
 * @param usuarioActual Nombre del usuario activo (para bit�cora).
 */
void Clientes::agregar(std::vector<Clientes>& lista, const std::string& usuarioActual) {
    Clientes nuevo;

    // Genera un ID �nico para el nuevo cliente
    nuevo.id = generarIdUnico(lista);
    if (nuevo.id.empty()) {
        std::cerr << "\n\t\tError: No hay c�digos disponibles para nuevos clientes (rango lleno)\n";
        system("pause");
        return;
    }

    std::cout << "\n\t\t=== AGREGAR CLIENTE (ID Auto-Asignado: " << nuevo.id << ") ===\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\t\tNombre completo: ";
    std::getline(std::cin, nuevo.nombre);

    std::cout << "\t\tDirecci�n: ";
    std::getline(std::cin, nuevo.direccion);

    std::cout << "\t\tTel�fono: ";
    std::getline(std::cin, nuevo.telefono);

    std::cout << "\t\tNIT: ";
    std::getline(std::cin, nuevo.nit);

    lista.push_back(nuevo); // Agrega el cliente a la lista
    guardarEnArchivo(lista); // Guarda la lista actualizada de clientes

    // Registra la acci�n en la bit�cora
    bitacora::registrar(usuarioActual, "CLIENTES", "Cliente agregado - ID: " + nuevo.id);
    std::cout << "\n\t\tCliente registrado exitosamente con ID: " << nuevo.id << "\n";
    system("pause");
}

/**
 * Muestra todos los clientes actualmente almacenados en la lista.
 * Si la lista est� vac�a, muestra un mensaje apropiado.
 * @param lista Lista de clientes a mostrar.
 */
void Clientes::mostrar(const std::vector<Clientes>& lista) {
    if (lista.empty()) {
        cout << "\n\t--- NO HAY CLIENTES REGISTRADOS ---\n";
        cout << "\tEl archivo puede estar vac�o o no se carg� correctamente.\n";
    } else {
        cout << "\n\t" << string(100, '-') << "\n";
        cout << "\t" << left
             << setw(10) << "| ID |"
             << setw(30) << " NOMBRE COMPLETO |"
             << setw(25) << " DIRECCI�N |"
             << setw(15) << " TEL�FONO |"
             << setw(15) << " NIT |" << "\n";
        cout << "\t" << string(100, '-') << "\n";

        for (const auto& cliente : lista) {
            cout << "\t" << left
                 << "| " << setw(6) << cliente.id << "| "
                 << setw(28) << cliente.nombre << "| "
                 << setw(23) << cliente.direccion << "| "
                 << setw(13) << cliente.telefono << "| "
                 << setw(13) << cliente.nit << "|" << "\n";
        }
        cout << "\t" << string(100, '-') << "\n";
    }
    system("pause");
}

/**
 * Modifica los datos de un cliente existente seg�n su ID.
 * Guarda los cambios en el archivo y registra en la bit�cora.
 * @param lista Lista de clientes (modificable).
 * @param usuarioActual Usuario que realiza la modificaci�n.
 * @param id ID del cliente a modificar.
 */
void Clientes::modificar(std::vector<Clientes>& lista, const std::string& usuarioActual, const std::string& id) {
    auto it = find_if(lista.begin(), lista.end(),
        [&id](const Clientes& c) { return c.id == id; });

    if (it != lista.end()) {
        cout << "\n--- MODIFICAR CLIENTE (ID: " << id << ") ---\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Nuevo nombre (" << it->nombre << "): ";
        getline(cin, it->nombre);

        cout << "Nueva direcci�n (" << it->direccion << "): ";
        getline(cin, it->direccion);

        cout << "Nuevo tel�fono (" << it->telefono << "): ";
        getline(cin, it->telefono);

        cout << "Nuevo NIT (" << it->nit << "): ";
        getline(cin, it->nit);

        guardarEnArchivo(lista); // Guarda los cambios en el archivo
        bitacora::registrar(usuarioActual, "CLIENTES", "Cliente modificado - ID: " + id);
        cout << "Cliente modificado!\n";
    } else {
        cout << "Cliente no encontrado.\n";
    }
    system("pause");
}

/**
 * Elimina un cliente de la lista dado su ID.
 * Guarda los cambios y registra la acci�n en bit�cora.
 * @param lista Lista de clientes (modificable).
 * @param usuarioActual Usuario que realiza la eliminaci�n.
 * @param id ID del cliente a eliminar.
 */
void Clientes::eliminar(std::vector<Clientes>& lista, const std::string& usuarioActual, const std::string& id) {
    auto it = find_if(lista.begin(), lista.end(),
        [&id](const Clientes& c) { return c.id == id; });

    if (it != lista.end()) {
        lista.erase(it); // Elimina el cliente de la lista
        guardarEnArchivo(lista); // Guarda la lista actualizada
        bitacora::registrar(usuarioActual, "CLIENTES", "Cliente eliminado - ID: " + id);
        cout << "Cliente eliminado!\n";
    } else {
        cout << "Cliente no encontrado.\n";
    }
    system("pause");
}

/**
 * Guarda la lista completa de clientes en un archivo binario.
 * @param lista Lista actual de clientes.
 */
void Clientes::guardarEnArchivo(const std::vector<Clientes>& lista) {
    std::ofstream archivo("clientes.bin", std::ios::binary | std::ios::trunc);
    if (!archivo) {
        std::cerr << "Error al abrir clientes.dat para escritura.\n";
        return;
    }

    for (const auto& cliente : lista) {
        // Guardar ID
        size_t size = cliente.id.size();
        archivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivo.write(cliente.id.c_str(), size);

        // Guardar Nombre
        size = cliente.nombre.size();
        archivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivo.write(cliente.nombre.c_str(), size);

        // Guardar Direcci�n
        size = cliente.direccion.size();
        archivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivo.write(cliente.direccion.c_str(), size);

        // Guardar Tel�fono
        size = cliente.telefono.size();
        archivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivo.write(cliente.telefono.c_str(), size);

        // Guardar NIT
        size = cliente.nit.size();
        archivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
        archivo.write(cliente.nit.c_str(), size);
    }
    archivo.close();
    std::cout << "\tDatos guardados correctamente.\n";
}

/**
 * Carga la lista de clientes desde el archivo binario "clientes.dat".
 * Si el archivo no existe o est� vac�o, la lista queda vac�a.
 * @param lista Lista donde se cargar�n los clientes.
 */
void Clientes::cargarDesdeArchivo(std::vector<Clientes>& lista) {
    lista.clear();

    std::ifstream archivo("clientes.bin", std::ios::binary);
    if (!archivo) {
        std::cerr << "Error al abrir clientes.dat para lectura.\n";
        return;
    }

    while (archivo.peek() != EOF) {
        Clientes cliente;
        size_t size;

        // Leer ID
        if (!archivo.read(reinterpret_cast<char*>(&size), sizeof(size))) break;
        cliente.id.resize(size);
        archivo.read(&cliente.id[0], size);

        // Leer Nombre
        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        cliente.nombre.resize(size);
        archivo.read(&cliente.nombre[0], size);

        // Leer Direcci�n
        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        cliente.direccion.resize(size);
        archivo.read(&cliente.direccion[0], size);

        // Leer Tel�fono
        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        cliente.telefono.resize(size);
        archivo.read(&cliente.telefono[0], size);

        // Leer NIT
        archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
        cliente.nit.resize(size);
        archivo.read(&cliente.nit[0], size);

        lista.push_back(cliente);
    }
    archivo.close();
    std::cout << "\tDatos cargados correctamente.\n";
}
