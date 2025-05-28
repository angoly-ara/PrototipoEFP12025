// Angoly Araujo Mayo 2025 9959-24-17623

#include "MenuProveedores.h"  // Incluye el encabezado donde se declara la clase MenuProveedores
#include <iostream>           // Librería estándar para entrada y salida
#include <limits>             // Para limpiar buffer con numeric_limits
using namespace std;          // Permite evitar el uso del prefijo std::

/*
 * Método estático que muestra el menú de gestión de proveedores.
 * Permite al usuario actual realizar acciones como agregar, mostrar,
 * modificar y eliminar proveedores.
 */
void MenuProveedores::mostrar(vector<Proveedor>& lista, usuarios& usuarioActual) {
    // Se agregan los datos guardados previamente
    Proveedor::cargarDesdeArchivoBinario(lista);

    int opcion; // Variable para almacenar la opción del menú elegida por el usuario

    do {
        system("cls"); // Limpia la pantalla (Windows)
        // Diseño mejorado del menú
        cout << "===========================================\n";
        cout << "           SISTEMA DE PROVEEDORES\n";
        cout << "     Usuario: " << usuarioActual.getNombre() << "\n";
        cout << "===========================================\n";
        cout << " [1] Agregar proveedor\n";
        cout << " [2] Mostrar proveedores\n";
        cout << " [3] Modificar proveedor\n";
        cout << " [4] Eliminar proveedor\n";
        cout << " [5] Volver\n";
        cout << "-------------------------------------------\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer de entrada

        string id; // Variable para capturar el ID del proveedor cuando se requiere
        switch (opcion) {
            case 1:
                Proveedor::agregar(lista, usuarioActual.getNombre());
                break;
            case 2:
                Proveedor::mostrar(lista);
                break;
            case 3:
                Proveedor::modificar(lista, usuarioActual.getNombre(), "");
                break;
            case 4:
                Proveedor::eliminar(lista, usuarioActual.getNombre(), "");
                break;
            case 5:
                cout << "\nSaliendo del modulo de proveedores...\n";
                return;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
                break;
        }

        system("pause"); // Espera que el usuario presione una tecla
    } while (true);
}

