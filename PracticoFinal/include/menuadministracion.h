#ifndef MENUADMINISTRACION_H
#define MENUADMINISTRACION_H


#include <vector>
#include "Administracion.h"
#include "usuarios.h"

class MenuAdministracion {
public:
    static void mostrar(std::vector<Administracion>& lista, usuarios& usuarioActual);
};

#endif // MENUADMINISTRACION_H
