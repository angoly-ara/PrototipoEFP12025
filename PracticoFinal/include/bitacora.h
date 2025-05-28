#ifndef BITACORA_H
#define BITACORA_H
// Angoly Araujo Mayo 2025 9959-24-17623

#include <string>
#include <unordered_map>

struct RegistroBitacora {
    int id_accion;              ///< ID �nico de la acci�n basada en el m�dulo
    int codigo;                 ///< C�digo del m�dulo al que pertenece la acci�n
    char usuario[50];           ///< Nombre del usuario que ejecut� la acci�n
    char modulo[30];            ///< Nombre del m�dulo donde se ejecut� la acci�n
    char descripcion[100];      ///< Descripci�n de la acci�n realizada
    char fecha_hora[20];        ///< Fecha y hora en que ocurri� la acci�n
};

/**
 * @class CodigosBitacora
 * @brief Administra los c�digos �nicos asignados a cada m�dulo para identificaci�n en la bit�cora.
 *
 * Esta clase permite obtener un ID espec�fico por m�dulo, �til para clasificar
 * y rastrear acciones dentro del sistema.
 */
class CodigosBitacora {
private:
    static std::unordered_map<std::string, int> rangos;///< Mapa de modulos

public:
    /**
     * @brief Devuelve el c�digo �nico asociado a un m�dulo.
     * @param modulo Nombre del m�dulo (ej. "Clientes", "Pedidos", etc.).
     * @return C�digo entero asociado al m�dulo.
     */
    static int getCodigo(const std::string& modulo);
};

/**
 * @class bitacora
 * @brief Clase encargada de registrar y administrar las acciones del sistema en una bit�cora binaria.
 *
 * Proporciona funciones para registrar eventos, generar respaldos,
 * realizar b�squedas y mostrar el contenido de la bit�cora.
 */
class bitacora {
public:

    /**
     * @brief Registra una acci�n en la bit�cora.
     * @param usuario Nombre del usuario que realiz� la acci�n.
     * @param modulo Nombre del m�dulo donde ocurri� la acci�n.
     * @param descripcion Breve descripci�n de la acci�n realizada.
     */
    static void registrar(const std::string& usuario, const std::string& modulo, const std::string& descripcion);

    /**
     * @brief Inserta un registro en la bit�cora (alias de registrar).
     * @param usuario Nombre del usuario que realiz� la acci�n.
     * @param modulo Nombre del m�dulo donde ocurri� la acci�n.
     * @param descripcion Breve descripci�n de la acci�n realizada.
     */
    static void insertar(const std::string& usuario, const std::string& modulo, const std::string& descripcion);

    /**
     * @brief Muestra en consola todos los registros almacenados en la bit�cora.
     */
    static void mostrarBitacora();

    /**
     * @brief Genera una copia de seguridad del archivo de la bit�cora.
     */
    static void generarBackup();

    /**
     * @brief Elimina todos los registros existentes en la bit�cora.
     */
    static void reiniciarBitacora();

    /**
     * @brief Permite buscar registros por nombre de usuario.
     */
    static void buscarPorNombreUsuario();

    /**
     * @brief Permite buscar registros por fecha.
     */
    static void buscarPorFecha();

    /**
     * @brief Muestra el men� interactivo de gesti�n de la bit�cora.
     */
    static void menuBitacora();

private:

    /**
     * @brief Obtiene la fecha y hora actual en formato de cadena.
     * @return Fecha y hora actual como string.
     */
    static std::string obtenerFechaActual();

};

#endif // BITACORA_H
