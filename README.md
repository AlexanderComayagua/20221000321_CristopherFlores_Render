# Registro de Productos en C++ con Docker y Render

## Estructuras usadas en C++

- `#include <iostream>`, `<fstream>`, `<string>`, `<cstdlib>`
- `using namespace std;`
- Funciones propias: `guardarProducto()` y `obtenerProductos()`
- Clase `httplib::Server` para crear el servidor HTTP
- Uso de variable de entorno: `getenv("PORT")` (obligatorio para Render)

## Funcionalidades actuales

- Página principal (`/`) con enlaces
- Formulario para agregar producto (`/form`)
- Agregar producto vía URL (`/agregar?nombre=...&precio=...`)
- Ver lista de productos en tabla HTML (`/ver`)

**Nota**: Los datos se guardan en `productos.txt` (se crea automáticamente)

## Compilar y ejecutar localmente (Windows con MinGW/MSYS2)

1. Descargar `httplib.h` desde https://github.com/yhirose/cpp-httplib/blob/master/httplib.h y ponerlo en `src/`
2. Compilar (necesario `-lws2_32` por Winsock en Windows): g++ -O2 -std=c++17 -pthread src/main.cpp -o server -lws2_32
3. Ejecutar: server.exe

**Mantén la terminal abierta** (el servidor se queda escuchando en el puerto).  
Si ves "ERROR CRITICO: listen falló" → el puerto está ocupado; mata el proceso con: 
netstat -ano | findstr :8080

taskkill /PID <numero_PID> /F

##Con Docker (prueba local)

```bash
# Construir la imagen
docker build -t registro-productos .

# Ejecutar (mapea puerto 8080)
docker run -p 8080:8080 -e PORT=8080 registro-productos
Pruebas locales (solo funcionan con el servidor corriendo)

http://localhost:8080/                                   ---> página principal con enlaces
http://localhost:8080/form                               ---> formulario interactivo para agregar producto
http://localhost:8080/ver                                ---> ver lista de productos en tabla
http://localhost:8080/agregar?nombre=Teclado&precio=1200 ---> agregar directamente por URL

Si ves 404 o "cannot connect":
Asegúrate de que server.exe o el contenedor Docker esté ejecutándose

##Desplegado en Render: https://two0221000321-cristopherflores-render.onrender.com

