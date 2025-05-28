//Angoly Araujo Mayo 2025 9959-24-17623
#ifndef MENUINFORMES_H
#define MENUINFORMES_H

#include "usuarios.h"

class MenuInformes {
public:
    // Muestra el men� de informes y recibe el usuario actual para registrar acciones
    void mostrar(const usuarios& usuarioActual);

private:
    // Registra una acci�n en el archivo informes.bin
    void registrarAccion(const char* nombreUsuario, const char* accion);
};

#endif






