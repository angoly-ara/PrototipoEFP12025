// Angoly Araujo Mayo 2025 9959-24-17623
#include "menuinformes.h"
#include "usuarios.h"
#include "bitacora.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <ctime>
using namespace std;

void registrarInforme(const string& accion, const string& usuario) {
    // Registro para bitacora.bin
    ofstream bitacora("bitacora.bin", ios::binary | ios::app);
    if (bitacora) {
        struct {
            char usuario[30];
            char accion[100];
            char fechaHora[25];
        } reg;

        time_t now = time(0);
        tm* ltm = localtime(&now);
        strftime(reg.fechaHora, sizeof(reg.fechaHora), "%Y-%m-%d %H:%M:%S", ltm);

        strncpy(reg.usuario, usuario.c_str(), sizeof(reg.usuario));
        strncpy(reg.accion, accion.c_str(), sizeof(reg.accion));

        bitacora.write(reinterpret_cast<char*>(&reg), sizeof(reg));
        bitacora.close();
    }

    // Registro para informes.txt
    ofstream informes("informes.txt", ios::app);
    if (informes) {
        struct {
            char usuario[30];
            char accion[100];
            char fechaHora[25];
        } info;

        time_t now = time(0);
        tm* ltm = localtime(&now);
        strftime(info.fechaHora, sizeof(info.fechaHora), "%Y-%m-%d %H:%M:%S", ltm);

        strncpy(info.usuario, usuario.c_str(), sizeof(info.usuario));
        strncpy(info.accion, accion.c_str(), sizeof(info.accion));

        informes.write(reinterpret_cast<char*>(&info), sizeof(info));
        informes.close();
    }
}

void MenuInformes::mostrar(const usuarios& usuarioActual) {
    int opcion;
    do {
        system("cls");
        cout << "\t\t========================================\n"
             << "\t\t|          SUBMENU DE INFORMES         |\n"
             << "\t\t========================================\n"
             << "\t\t| Usuario: " << usuarioActual.getNombre() << "\n"
             << "\t\t 1. Informe diario de operaciones\n"
             << "\t\t 2. Informe mensual de ventas\n"
             << "\t\t 3. Resumen de inventario\n"
             << "\t\t 4. Historico de envios\n"
             << "\t\t 5. Volver al menu principal\n"
             << "\t\t========================================\n"
             << "\t\tIngresa tu opcion: ";

        cin >> opcion;

        switch(opcion) {
            case 1: {
                registrarInforme("Accedio al informe diario de operaciones", usuarioActual.getNombre());

                ifstream bitacora("bitacora.bin", ios::binary);
                if (!bitacora) {
                    cout << "\n\t\tNo se pudo abrir el archivo de bitacora.\n";
                    break;
                }

                cout << "\n\t\t====== INFORME DIARIO DE OPERACIONES ======\n\n";

                struct {
                    char usuario[30];
                    char accion[100];
                    char fechaHora[25];
                } reg;

                while (bitacora.read(reinterpret_cast<char*>(&reg), sizeof(reg))) {
                    cout << left << setw(30) << reg.usuario
                         << setw(25) << reg.fechaHora
                         << reg.accion << endl;
                }

                bitacora.close();
                break;
            }

            case 2: {
                registrarInforme("Accedio al informe mensual de ventas", usuarioActual.getNombre());
                ifstream facturas("facturas.bin", ios::binary);
                if (!facturas) {
                        cout << "\n\t\tNo se pudo abrir el archivo de facturas.\n";
                        break;
                        }

    cout << "\n\t\t======= INFORME DE VENTAS (RESUMEN GENERAL) =======\n\n";
    cout << left << setw(12) << "ID Factura"
         << setw(12) << "ID Pedido"
         << setw(12) << "ID Cliente"
         << setw(12) << "Monto"
         << setw(12) << "Pagada" << endl;

    struct Factura {
        int idFactura;
        int idPedido;
        int idCliente;
        float monto;
        bool pagada;
        char cliente[50]; // Esto lo ignoraremos aqu�
    } factura;

    float total = 0.0;
    float totalPagado = 0.0;
    int cantidadFacturas = 0;

    while (facturas.read(reinterpret_cast<char*>(&factura), sizeof(factura))) {
        cout << left << setw(12) << factura.idFactura
             << setw(12) << factura.idPedido
             << setw(12) << factura.idCliente
             << setw(12) << fixed << setprecision(2) << factura.monto
             << (factura.pagada ? "Si" : "No") << endl;

        total += factura.monto;
        if (factura.pagada) {
            totalPagado += factura.monto;
        }
        cantidadFacturas++;
    }

    cout << "\n\t\tFacturas totales: " << cantidadFacturas << endl;
    cout << "\t\tMonto total facturado: Q" << fixed << setprecision(2) << total << endl;
    cout << "\t\tMonto total pagado:    Q" << fixed << setprecision(2) << totalPagado << endl;
    cout << "\t\tMonto pendiente:        Q" << fixed << setprecision(2) << (total - totalPagado) << endl;

    facturas.close();
    break;
}




            case 3: {
                registrarInforme("Accedio al resumen de inventario", usuarioActual.getNombre());
                ifstream inv("inventario.bin", ios::binary);
                if (!inv) {
                    cout << "\n\t\tNo se pudo abrir el archivo de inventario.\n";
                    break;
                }

                cout << "\n\t\t========= RESUMEN DE INVENTARIO =========\n\n";
                cout << left << setw(10) << "ID"
                     << setw(30) << "Nombre"
                     << setw(10) << "Cant"
                     << setw(10) << "Precio" << endl;

                struct {
                    int id;
                    char nombre[50];
                    int cantidad;
                    float precio;
                } prod;

                while (inv.read(reinterpret_cast<char*>(&prod), sizeof(prod))) {
                    cout << left << setw(10) << prod.id
                         << setw(30) << prod.nombre
                         << setw(10) << prod.cantidad
                         << fixed << setprecision(2) << prod.precio << endl;
                }

                inv.close();
                break;
            }

            case 4: {
                registrarInforme("Accedio al historico de envios", usuarioActual.getNombre());
                ifstream env("envios.bin", ios::binary);
                if (!env) {
                    cout << "\n\t\tNo se pudo abrir el archivo de envios.\n";
                    break;
                }

                cout << "\n\t\t========= HISTORICO DE ENVIOS =========\n\n";
                cout << left << setw(10) << "ID"
                     << setw(30) << "Destino"
                     << setw(20) << "Fecha"
                     << "Entregado" << endl;

                struct {
                    int idEnvio;
                    char destino[50];
                    char fecha[20];
                    bool entregado;
                } envio;

                while (env.read(reinterpret_cast<char*>(&envio), sizeof(envio))) {
                    cout << left << setw(10) << envio.idEnvio
                         << setw(30) << envio.destino
                         << setw(20) << envio.fecha
                         << (envio.entregado ? "Si" : "No") << endl;
                }

                env.close();
                break;
            }

            case 5:
                cout << "\n\t\tVolviendo al menu principal...\n";
                break;

            default:
                cout << "\n\t\tOpcion invalida... Intenta de nuevo.\n";
        }

        if (opcion != 5) {
            system("pause");
        }

    } while(opcion != 5);
}
