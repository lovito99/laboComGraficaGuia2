# Universidad Nacional de San Antonio Abad del Cusco
## Departamento Académico de Ingeniería Informática y de Sistemas

---

**Asignatura:** Computación Gráfica I  
**Práctica Nº 2:** Algoritmos para Trazo de Líneas  
**Profesor:** Dr. Hans Harley Ccacyahuillca Bejar  
**Estudiante:** Efrain Vitorino Marin  
**Código:** 160337  

---

## Descripción

Implementación en C++ con OpenGL/GLUT de los cuatro algoritmos clásicos para el trazado de líneas en pantalla, más el ejercicio propuesto de detección de intersecciones.

---

## Estructura del Proyecto

```
laboComGraficaGuia2/
├── algoritmos_lineas.cpp    # Prueba de los 4 algoritmos de trazado
├── ejercicio_propuesto.cpp  # 5 líneas al azar con detección de intersecciones
├── Makefile                 # Compilación automática
└── README.md
```

---

## Algoritmos Implementados

### 1. Pendiente-Intersección (y = mx + b)
Evalúa la ecuación de la recta `y = m*x + b` en cada valor entero de `x`.  
- Calcula la pendiente `m = (y2-y1)/(x2-x1)` y la intersección `b = y1 - m*x1`.  
- Sencillo pero usa aritmética flotante en cada paso.

### 2. DDA (Digital Differential Analyzer)
Incrementa `y` en `m` en cada paso de `x`, usando acumulación flotante.  
- Más eficiente que pendiente-intersección al evitar la multiplicación en el bucle.
- Maneja pendientes positivas y negativas normalizando la dirección.

### 3. Bresenham (todos los octantes)
Algoritmo de solo aritmética entera con variable de error acumulado.  
- Soporta los 8 octantes mediante transposición (`steep`) y `dy_step`.
- El más eficiente de los tres para hardware con enteros rápidos.

### 4. Punto Medio
Versión incremental de solo enteros válida para el primer octante (0 < m ≤ 1).  
- Usa una variable de decisión `dm` que indica qué píxel queda más cerca de la línea ideal.

---

## Ejercicio Propuesto

**Archivo:** `ejercicio_propuesto.cpp`

- Dibuja **5 líneas** con coordenadas predefinidas usando el algoritmo de Bresenham.
- Calcula todos los **C(5,2) = 10 pares** de segmentos y determina cuáles se intersectan.
- La intersección se calcula con la **forma paramétrica de segmentos**:
  - `P(t) = A + t*(B-A)` y `Q(s) = C + s*(D-C)`
  - Hay intersección si `t ∈ [0,1]` y `s ∈ [0,1]`
- Los puntos de intersección se señalan con un **marcador cuadrado rojo**.

---

## Requisitos

- Sistema operativo Linux
- `g++` (GCC C++11 o superior)
- `freeglut3-dev` (OpenGL/GLUT)

Instalación de dependencias:
```bash
sudo apt install g++ make freeglut3-dev
```

---

## Compilación y Ejecución

### Con Makefile
```bash
make                    # Compila ambos programas
make run_algoritmos     # Ejecuta prueba de los 4 algoritmos
make run_ejercicio      # Ejecuta el ejercicio propuesto
make clean              # Elimina los ejecutables
```

### Manual
```bash
g++ -o algoritmos_lineas  algoritmos_lineas.cpp  -lGL -lGLU -lglut
g++ -o ejercicio_propuesto ejercicio_propuesto.cpp -lGL -lGLU -lglut

./algoritmos_lineas
./ejercicio_propuesto
```

---

## Vista de los Programas

### `algoritmos_lineas`
Ventana 500×500 px con las 4 líneas de prueba, cada una en un color diferente:

| Color   | Algoritmo              |
|---------|------------------------|
| Rojo    | Pendiente-Intersección |
| Azul    | DDA                    |
| Verde   | Bresenham              |
| Magenta | Punto Medio            |

### `ejercicio_propuesto`
Ventana 500×500 px con 5 líneas en azul y los puntos de intersección marcados en rojo.

---

## Bibliografía

- Donald Hearn, M. Pauline Baker; *Gráficas por Computadora con OpenGL*; Pearson Prentice Hall; 3° edición; Madrid 2006.
- Rimon Elias; *Digital Media: A problem-solving Approach for Computer Graphics*; eBook. New York.
- Sumanta Guha; *Computer Graphics Through OpenGL*; Third Edition; Taylor & Francis Group. 2019.
