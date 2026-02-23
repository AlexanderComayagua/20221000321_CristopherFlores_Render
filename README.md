# Registro de Productos en C++ con Docker y Render

## Estructuras usadas en C++
- `#include <iostream>`, `<fstream>`, `<string>`, `<cstdlib>`
- `using namespace std;`
- Funciones: `guardarProducto` y `obtenerProductos`
- `httplib::Server` para crear servidor HTTP
- Variables de entorno: `getenv("PORT")`

## Compilar y ejecutar localmente
1. Descargar `httplib.h` y ponerlo en `src/`
2. Compilar `g++ -O2 -std=c++17 -pthread src/main.cpp -o server`
3. Ejecutar `server.exe`

## Con Docker
`docker build -t registro-productos .`
`docker run -p 8080:8080 -e PORT=8080 registro-productos`

Prueba:
- http://localhost:8080/
- http://localhost:8080/ver
- http://localhost:8080/agregar?nombre=Teclado&precio=1200

Desplegado en Render → [URL que obtengas]