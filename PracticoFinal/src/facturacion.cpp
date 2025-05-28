// Angoly Araujo Mayo 2025 9959-24-17623

#include "facturacion.h"
#include "clientes.h"
#include "pedidos.h"
#include "bitacora.h"
#include "usuarios.h"
#include <fstream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <regex>
#include <iostream>
#include <limits>
#include <memory>

using namespace std;

// Variables externas
extern usuarios usuarioRegistrado;
extern bitacora auditoria;

// ============= FUNCIONES AUXILIARES MEJORADAS =============

int Facturacion::generarIdFactura() {
    ifstream archivo("facturas.bin", ios::binary);
    if (!archivo) return 3554;

    int ultimoId = 3554;

    try {
        while (archivo.peek() != EOF) {
            Factura factura = cargarDesdeArchivo(archivo);
            if (factura.idFactura > ultimoId) {
                ultimoId = factura.idFactura;
            }
        }
    } catch (...) {
        archivo.close();
        return -1;
    }

    archivo.close();
    return (ultimoId >= 3606) ? -1 : ultimoId + 1;
}

void Facturacion::guardarEnArchivo(const Factura& factura) {
    ofstream archivo("facturas.bin", ios::binary | ios::app);
    if (!archivo) {
        throw runtime_error("No se pudo abrir facturas.bin para escritura");
    }

    try {
        // Función lambda segura para escribir strings
        auto writeString = [&archivo](const string& str) {
            size_t size = str.size();
            archivo.write(reinterpret_cast<const char*>(&size), sizeof(size));
            archivo.write(str.data(), size);
        };

        // Escribir campos en orden definido
        archivo.write(reinterpret_cast<const char*>(&factura.idFactura), sizeof(factura.idFactura));
        writeString(factura.idCliente);
        writeString(factura.idPedido);
        archivo.write(reinterpret_cast<const char*>(&factura.monto), sizeof(factura.monto));
        archivo.write(reinterpret_cast<const char*>(&factura.pagada), sizeof(factura.pagada));
        writeString(factura.cliente);
        writeString(factura.nit);

        if (!archivo.good()) {
            throw runtime_error("Error durante la escritura");
        }
    } catch (...) {
        archivo.close();
        throw;
    }
    archivo.close();
}

Factura Facturacion::cargarDesdeArchivo(ifstream& archivo) {
    Factura factura;

    try {
        // Leer campos en el mismo orden que se escribieron
        archivo.read(reinterpret_cast<char*>(&factura.idFactura), sizeof(factura.idFactura));

        // Función lambda segura para leer strings
        auto readString = [&archivo]() {
            size_t size;
            archivo.read(reinterpret_cast<char*>(&size), sizeof(size));
            if (size > 1000) { // Prevención contra valores corruptos
                throw runtime_error("Tamaño de string inválido");
            }

            unique_ptr<char[]> buffer(new char[size + 1]);
            archivo.read(buffer.get(), size);
            buffer[size] = '\0';
            return string(buffer.get());
        };

        factura.idCliente = readString();
        factura.idPedido = readString();
        archivo.read(reinterpret_cast<char*>(&factura.monto), sizeof(factura.monto));
        archivo.read(reinterpret_cast<char*>(&factura.pagada), sizeof(factura.pagada));
        factura.cliente = readString();
        factura.nit = readString();

        if (archivo.fail()) {
            throw runtime_error("Error de lectura de archivo");
        }
    } catch (...) {
        throw; // Relanzar la excepción
    }

    return factura;
}

void Facturacion::registrarBitacora(const Factura& factura, const string& accion, const string& usuario) {
    try {
        string descripcion = accion + " Factura ID: " + to_string(factura.idFactura) +
                           " - Cliente: " + factura.cliente +
                           " - Monto: " + to_string(factura.monto);

        bitacora::registrar(usuario, "Facturacion", descripcion);
    } catch (const exception& e) {
        cerr << "Error en bitácora: " << e.what() << endl;
    }
}

void Facturacion::mostrarMenuFacturacion() {
    int opcion;
    do {
        system("cls");
        cout << "===========================================\n";
        cout << "        SISTEMA DE FACTURACION\n";
        cout << "     Usuario: " << usuarioRegistrado.getNombre() << "\n";
        cout << "===========================================\n";
        cout << " [1] Crear Factura\n";
        cout << " [2] Mostrar Facturas\n";
        cout << " [3] Modificar Factura\n";
        cout << " [4] Eliminar Factura\n";
        cout << " [0] Salir\n";
        cout << "-------------------------------------------\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: crearFactura(); break;
            case 2: mostrarFacturas(); break;
            case 3: modificarFactura(); break;
            case 4: eliminarFactura(); break;
            case 0: cout << "\nSaliendo del modulo de facturacion...\n"; break;
            default: cout << "Opcion invalida. Intente de nuevo.\n"; break;
        }

        if (opcion != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }

    } while (opcion != 0);
}

void Facturacion::crearFactura() {
    system("cls");
    cout << "=====================================\n";
    cout << "        CREAR NUEVA FACTURA         \n";
    cout << "=====================================\n\n";

    // Cargar pedidos
    vector<Pedidos> listaPedidos;
    try {
        Pedidos::cargarDesdeArchivoBin(listaPedidos);
        if (listaPedidos.empty()) {
            cout << "No hay pedidos registrados.\n";
            system("pause");
            return;
        }
    } catch (const exception& e) {
        cerr << "Error crítico al cargar pedidos: " << e.what() << "\n";
        system("pause");
        return;
    }

    // Mostrar lista con límite
    cout << "--- Últimos 20 pedidos ---\n";
    size_t mostrar = min(size_t(20), listaPedidos.size());
    for (size_t i = 0; i < mostrar; ++i) {
        const auto& pedido = listaPedidos[i];
        cout << "ID: " << pedido.getId()
             << " | Cliente: " << pedido.getIdCliente()
             << " | Monto: $" << fixed << setprecision(2) << pedido.getMontoTotal()
             << " | Estado: " << pedido.getEstado() << "\n";
    }

    // Validación de entrada
    string idPedido;
    while (true) {
        cout << "\nIngrese el ID del pedido a facturar (0 para cancelar): ";
        getline(cin, idPedido);

        if (idPedido == "0") return;
        if (!idPedido.empty()) break;

        cout << "Error: ID no puede estar vacío\n";
    }

    // Búsqueda del pedido
    auto it = find_if(listaPedidos.begin(), listaPedidos.end(),
        [&idPedido](const Pedidos& p) { return p.getId() == idPedido; });

    if (it == listaPedidos.end()) {
        cerr << "Error: Pedido no encontrado.\n";
        system("pause");
        return;
    }

    // Crear factura
    Factura nueva;
    try {
        nueva.idFactura = generarIdFactura();
        if (nueva.idFactura == -1) {
            cerr << "Error: Límite de facturas alcanzado.\n";
            system("pause");
            return;
        }

        nueva.idPedido = it->getId();
        nueva.idCliente = it->getIdCliente();
        nueva.monto = it->getMontoTotal();
        nueva.pagada = false;

        // Obtener datos del cliente
        vector<Clientes> clientes;
        try {
            Clientes::cargarDesdeArchivo(clientes);
            auto clienteIt = find_if(clientes.begin(), clientes.end(),
                [&nueva](const Clientes& c) { return c.getId() == nueva.idCliente; });

            if (clienteIt != clientes.end()) {
                nueva.cliente = clienteIt->getNombre();
                nueva.nit = clienteIt->getNit(); // Asumiento que existe getNit() en Clientes
            } else {
                nueva.cliente = "Cliente Desconocido";
                nueva.nit = "CF";
            }
        } catch (const exception& e) {
            nueva.cliente = "Cliente No Registrado";
            nueva.nit = "CF";
            cerr << "Advertencia: " << e.what() << "\n";
        }

        // Guardar factura
        guardarEnArchivo(nueva);
        registrarBitacora(nueva, "Creación", usuarioRegistrado.getNombre());

        // Mostrar factura con diseño profesional
        system("cls");
        cout << "\n\n";
        cout << "=============================================\n";
        cout << "               FACTURA ELECTRONICA           \n";
        cout << "=============================================\n";
        cout << " No. Factura: " << setw(30) << left << nueva.idFactura << "\n";
        cout << "---------------------------------------------\n";
        cout << " Nombre:      " << setw(30) << left << nueva.cliente << "\n";
        cout << " NIT:         " << setw(30) << left << nueva.nit << "\n";
        cout << "---------------------------------------------\n";
        cout << " No. Pedido:  " << setw(30) << left << nueva.idPedido << "\n";
        cout << "---------------------------------------------\n";
        cout << " IMPORTE:     " << setw(30) << right << fixed << setprecision(2) << nueva.monto << "\n";
        cout << "=============================================\n";

    } catch (const exception& e) {
        cerr << " Error crítico: " << e.what() << "\n";
    }

    system("pause");
}

// Modificar factura
void Facturacion::modificarFactura() {
    int volver;
    cout << "Desea regresar al menu principal? (1: Si / 0: No): ";
    cin >> volver;
    if (volver == 1) return;

    mostrarFacturas();

    int id;
    cout << "Ingrese el ID de la factura a modificar: ";
    cin >> id;

    vector<Factura> facturas;
    ifstream archivoEntrada("facturas.bin", ios::binary);
    if (!archivoEntrada.is_open()) {
        cout << "Error al abrir el archivo de facturas.\n";
        system("pause");
        return;
    }

    try {
        while (archivoEntrada.peek() != EOF) {
            Factura fact = cargarDesdeArchivo(archivoEntrada);
            facturas.push_back(fact);
        }
    } catch (...) {
        cout << "Error al leer las facturas.\n";
        archivoEntrada.close();
        system("pause");
        return;
    }
    archivoEntrada.close();

    bool encontrado = false;
    for (auto& fact : facturas) {
        if (fact.idFactura == id) {
            encontrado = true;
            cout << "Modificar estado de pago (1: Pagada / 0: No Pagada): ";
            cin >> fact.pagada;
            break;
        }
    }

    if (!encontrado) {
        cout << "Factura no encontrada.\n";
        system("pause");
        return;
    }

    ofstream archivoSalida("facturas.bin", ios::binary | ios::trunc);
    for (const auto& fact : facturas) {
        guardarEnArchivo(fact);
    }
    archivoSalida.close();

    cout << "Factura modificada correctamente.\n";
    system("pause");
}

// Eliminar factura
void Facturacion::eliminarFactura() {
    int volver;
    cout << "Desea regresar al menu principal? (1: Si / 0: No): ";
    cin >> volver;
    if (volver == 1) return;

    mostrarFacturas();

    int id;
    cout << "Ingrese el ID de la factura a eliminar: ";
    cin >> id;

    vector<Factura> facturas;
    ifstream archivo("facturas.bin", ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir facturas\n";
        system("pause");
        return;
    }

    try {
        while (archivo.peek() != EOF) {
            Factura fact = cargarDesdeArchivo(archivo);
            if (fact.idFactura != id) {
                facturas.push_back(fact);
            }
        }
    } catch (...) {
        cout << "Error al leer facturas\n";
        archivo.close();
        system("pause");
        return;
    }
    archivo.close();

    ofstream out("facturas.bin", ios::binary | ios::trunc);
    for (const auto& fact : facturas) {
        guardarEnArchivo(fact);
    }
    out.close();

    cout << "Factura eliminada correctamente\n";
    system("pause");
}

// Mostrar facturas
void Facturacion::mostrarFacturas() {
    system("cls");
    cout << "===========================================\n";
    cout << "          LISTADO DE FACTURAS           \n";
    cout << "===========================================\n";

    vector<Factura> facturas;
    ifstream archivo("facturas.bin", ios::binary);
    if (!archivo.is_open()) {
        cout << "\nNo hay facturas registradas.\n";
        system("pause");
        return;
    }

    try {
        while (archivo.peek() != EOF) {
            Factura fact = cargarDesdeArchivo(archivo);
            facturas.push_back(fact);
        }
    } catch (...) {
        cout << "\nError al leer el archivo de facturas.\n";
        archivo.close();
        system("pause");
        return;
    }
    archivo.close();

    vector<Clientes> clientes;
    Clientes::cargarDesdeArchivo(clientes);

    for (const auto& fact : facturas) {
        string nombreCliente = "No encontrado";
        for (const auto& cli : clientes) {
            if (cli.getId() == fact.idCliente) {
                nombreCliente = cli.getNombre();
                break;
            }
        }

        cout << "\nID Factura: " << fact.idFactura;
        cout << "\nID Pedido: " << fact.idPedido;
        cout << "\nCliente: " << nombreCliente;
        cout << "\nMonto: Q" << fixed << setprecision(2) << fact.monto;
        cout << "\nEstado: " << (fact.pagada ? "PAGADA" : "PENDIENTE");
        cout << "\n-------------------------------------------\n";
    }

    cout << "\nTotal de facturas: " << facturas.size() << "\n";
    system("pause");
}

