#include "httplib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>   

using namespace std;

// Función 1: Guardar producto
void guardarProducto(const string& nombre, double precio) {
    ofstream archivo("productos.txt", ios::app);
    if (archivo.is_open()) {
        archivo << nombre << "," << precio << endl;
        archivo.close();
    }
}

// Función 2: Leer todos los productos como string
string obtenerProductos() {
    ifstream archivo("productos.txt");
    string linea, resultado = "--- Productos registrados ---\n";
    bool hayDatos = false;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            resultado += linea + "\n";
            hayDatos = true;
        }
        archivo.close();
        if (!hayDatos) {
            resultado += "(Aún no hay productos registrados)\n";
        }
    } else {
        resultado += "(No se encontró el archivo todavía)\n";
    }
    resultado += "-----------------------------\n";
    return resultado;
}

int main() {
    // Puerto
    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;

    cout << "Iniciando servidor en puerto " << port << endl;

    httplib::Server svr;

    // Ruta raíz
    svr.Get("/", [](const httplib::Request&, httplib::Response &res) {
        string html = "<h1>Registro de Productos - C++ en Render</h1>";
        html += "<p>Usa las rutas:</p>";
        html += "<ul>";
        html += "<li><a href=\"/ver\">Ver productos</a></li>";
        html += "<li><a href=\"/agregar?nombre=Mouse&precio=850.5\">Ej: Agregar producto</a></li>";
        html += "</ul>";
        res.set_content(html, "text/html");
    });

    // Ruta para VER productos 
    svr.Get("/ver", [](const httplib::Request&, httplib::Response &res) {
        string contenido = obtenerProductos();
        res.set_content(contenido, "text/plain");
    });

    // Ruta para AGREGAR producto
    svr.Get("/agregar", [](const httplib::Request &req, httplib::Response &res) {
        string nombre = req.has_param("nombre") ? req.get_param_value("nombre") : "Sin nombre";
        string precio_str = req.has_param("precio") ? req.get_param_value("precio") : "0.0";

        double precio = stod(precio_str); 

        guardarProducto(nombre, precio);

        string mensaje = "Producto agregado:\n";
        mensaje += "Nombre: " + nombre + "\n";
        mensaje += "Precio: L " + precio_str + "\n";
        mensaje += "\n<a href=\"/ver\">Ver todos los productos</a>";
        res.set_content(mensaje, "text/plain");
    });

    // Health check
    svr.Get("/health", [](const httplib::Request&, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    cout << "Servidor escuchando en http://0.0.0.0:" << port << endl;

    svr.listen("0.0.0.0", port);

    return 0;
}