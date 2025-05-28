// Angoly Araujo Mayo 2025 9959-24-17623

#ifndef FACTURACION_H
#define FACTURACION_H

#include <string>
#include <vector>
#include "clientes.h"
#include "pedidos.h"
#include "bitacora.h"

using namespace std;

// Estructura que representa una factura
struct Factura {
    int idFactura;
    string idCliente;
    string idPedido;
    double monto;
    bool pagada;
    string cliente;
    string nit;

};

// Clase que gestiona las operaciones relacionadas con la facturaci�n
class Facturacion {
public:
    // Crea una nueva factura y la guarda en el archivo
    static void crearFactura();

    // Muestra todas las facturas almacenadas
    static void mostrarFacturas();

    // Permite modificar una factura existente
    static void modificarFactura();

    // Elimina una factura espec�fica del archivo
    static void eliminarFactura();

    // Muestra el men� principal de opciones de facturaci�n
    static void mostrarMenuFacturacion();

private:
    // Genera un ID �nico para una nueva factura
    static int generarIdFactura();

    // Guarda una factura en el archivo correspondiente
    static void guardarEnArchivo(const Factura& factura);

    // Carga una factura desde archivo binario
    static Factura cargarDesdeArchivo(ifstream& archivo);

    // Registra una acci�n en la bit�cora
    static void registrarBitacora(const Factura& factura,
                                const string& accion,
                                const string& usuario);
};

#endif // FACTURACION_H
