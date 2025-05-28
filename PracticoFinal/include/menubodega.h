// Angoly Araujo Mayo 2025 9959-24-17623

#ifndef MENUALMACENES_H
#define MENUALMACENES_H

#include <vector>
#include "bodega.h"
#include "usuarios.h"

// Clase que representa el menú interactivo para gestionar almacenes
class MenuAlmacenes {
public:
    // Muestra el menú de opciones para operar sobre almacenes
    // Permite al usuario agregar, mostrar, modificar o eliminar almacenes
    static void mostrar(std::vector<Almacen>& lista, usuarios& usuario);
};

#endif
