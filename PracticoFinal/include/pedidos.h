#ifndef PEDIDOS_H
#define PEDIDOS_H
#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>
#include "usuarios.h"
#include "bitacora.h"
#include "clientes.h"
#include "producto.h"
#include "almacen.h"
#include "envios.h"
#include "transportistas.h"

//Declaraciones anticipadas de clases para evitar dependencias cr�ticas
class Clientes;
class Producto;
class Almacen;

//variables globales de usuarios y bitacora
extern usuarios usuarioRegistrado;
extern bitacora auditoria;

class Pedidos {
public:

    double getMontoTotal() const {
    double total = 0.0;
    for (const auto& detalle : detalles) {
        total += detalle.cantidad * detalle.precioUnitario;
    }
    return total;
}

    struct DetallePedido {
        std::string codigoProducto;
        int cantidad;
        double precioUnitario;
    };

    // Declaraci�n del constructor
    Pedidos();

    static std::vector<Pedidos> listaPedidos;  // Lista est�tica

    void gestionPedidos(const std::vector<Clientes>& clientes,
                      std::vector<Producto>& productos,
                      const std::vector<Almacen>& almacenes);

    void crearPedido(const std::vector<Clientes>& clientes,
                   const std::vector<Producto>& productos,
                   const std::vector<Almacen>& almacenes);

    void consultarPedidos();
    void modificarPedido(const std::vector<Clientes>& clientes,
                        const std::vector<Producto>& productos,
                        const std::vector<Almacen>& almacenes);
    void cancelarPedido();
    void completarPedido(std::vector<Producto>& productos);
    void verHistorial();

    void setEstado(const std::string& nuevoEstado) { estado = nuevoEstado; }

    static void guardarEnArchivoBin(const std::vector<Pedidos>& lista);
    static void cargarDesdeArchivoBin(std::vector<Pedidos>& lista);

    std::string getId() const { return id; }
    std::string getDetalles() const;
    std::string getEstado() const { return estado; }
    std::string getIdCliente() const { return idCliente; }

private:
    // Orden correcto de miembros para coincidir con la inicializaci�n
    std::string id;
    std::string idCliente;
    std::string idAlmacen;
    std::time_t fechaPedido;
    std::string estado;
    std::vector<DetallePedido> detalles;

    static std::string generarIdUnico(const std::vector<Pedidos>& lista);
    static bool idDisponible(const std::vector<Pedidos>& lista, const std::string& id);
    static bool validarCliente(const std::string& idCliente, const std::vector<Clientes>& clientes);
    static bool validarProducto(const std::string& codigoProducto, const std::vector<Producto>& productos);
    static bool validarAlmacen(const std::string& idAlmacen, const std::vector<Almacen>& almacenes);
};

#endif // PEDIDOS_H
