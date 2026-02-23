#include "httplib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;

// Función 1: Guardar producto
void guardarProducto(const string& nombre, double precio) {
    ofstream archivo("productos.txt", ios::app);
    if (archivo.is_open()) {
        archivo << nombre << "," << precio << endl;
        archivo.close();
    }
}

// Función 2: Obtener productos como HTML (mejor presentación)
string obtenerProductosHTML() {
    ifstream archivo("productos.txt");
    string linea;
    stringstream ss;

    ss << "<h2>Productos registrados</h2>";
    ss << "<table border='1' cellpadding='8' style='border-collapse: collapse; width: 60%;'>";
    ss << "<tr><th>Nombre</th><th>Precio (L)</th></tr>";

    bool hayDatos = false;
    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            size_t coma = linea.find(',');
            if (coma != string::npos) {
                string nombre = linea.substr(0, coma);
                string precio = linea.substr(coma + 1);
                ss << "<tr><td>" << nombre << "</td><td>" << precio << "</td></tr>";
                hayDatos = true;
            }
        }
        archivo.close();
    }

    if (!hayDatos) {
        ss << "<tr><td colspan='2' align='center'>Aún no hay productos registrados</td></tr>";
    }
    ss << "</table>";
    ss << "<br><a href='/'>Volver al inicio</a>";
    return ss.str();
}

// Función 3: Formulario HTML para agregar producto
string formularioAgregar() {
    string html = "<h2>Agregar nuevo producto</h2>";
    html += "<form action='/agregar' method='GET'>";
    html += "  <label>Nombre del producto:</label><br>";
    html += "  <input type='text' name='nombre' required style='width:300px; padding:6px;'><br><br>";
    html += "  <label>Precio (L):</label><br>";
    html += "  <input type='number' step='0.01' name='precio' required style='width:300px; padding:6px;'><br><br>";
    html += "  <input type='submit' value='Agregar producto' style='padding:8px 16px; background:#4CAF50; color:white; border:none; cursor:pointer;'>";
    html += "</form>";
    html += "<br><a href='/ver'>Ver productos</a> | <a href='/'>Volver al inicio</a>";
    return html;
}

int main() {
    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;

    cout << "Iniciando servidor en puerto " << port << endl;

    httplib::Server svr;

    // Ruta raíz: menú principal
    svr.Get("/", [](const httplib::Request&, httplib::Response &res) {
        string html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Registro de Productos</title></head><body>";
        html += "<h1>Registro de Productos - C++ en Render</h1>";
        html += "<p>Elige una opción:</p>";
        html += "<ul style='font-size:18px;'>";
        html += "<li><a href='/form'>Agregar producto (formulario)</a></li>";
        html += "<li><a href='/ver'>Ver todos los productos</a></li>";
        html += "</ul>";
        html += "</body></html>";
        res.set_content(html, "text/html");
    });

    // Formulario para agregar
    svr.Get("/form", [](const httplib::Request&, httplib::Response &res) {
        res.set_content(formularioAgregar(), "text/html");
    });

    // Ver productos 
    svr.Get("/ver", [](const httplib::Request&, httplib::Response &res) {
        string contenido = obtenerProductosHTML();
        string html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Productos</title></head><body>";
        html += contenido;
        html += "</body></html>";
        res.set_content(html, "text/html");
    });

    // Agregar producto
    svr.Get("/agregar", [](const httplib::Request &req, httplib::Response &res) {
        string nombre = req.has_param("nombre") ? req.get_param_value("nombre") : "Sin nombre";
        string precio_str = req.has_param("precio") ? req.get_param_value("precio") : "0.0";

        double precio = 0.0;
        try {
            precio = stod(precio_str);
        } catch (...) {
            precio = 0.0;
        }

        guardarProducto(nombre, precio);

        string html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Producto agregado</title></head><body>";
        html += "<h2>Producto agregado exitosamente</h2>";
        html += "<p><strong>Nombre:</strong> " + nombre + "</p>";
        html += "<p><strong>Precio:</strong> L " + precio_str + "</p>";
        html += "<br><a href='/ver'>Ver todos los productos</a> | ";
        html += "<a href='/form'>Agregar otro</a> | ";
        html += "<a href='/'>Volver al inicio</a>";
        html += "</body></html>";

        res.set_content(html, "text/html");
    });

    // Health check (para Render)
    svr.Get("/health", [](const httplib::Request&, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    cout << "Intentando bind y listen en puerto " << port << "..." << endl;

    if (!svr.listen("0.0.0.0", port)) {
        cerr << "ERROR CRITICO: listen falló (puerto ocupado, permisos, o fallo interno)" << endl;
        return 1;
    }

    return 0;
}