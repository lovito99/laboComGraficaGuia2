/*
 * Practica 2 - Algoritmos para Trazo de Lineas
 * Implementacion y prueba de 4 algoritmos:
 *   1. Pendiente-Interseccion (ecuacion basica y = mx + b)
 *   2. DDA (Digital Differential Analyzer)
 *   3. Bresenham (todos los octantes)
 *   4. Punto Medio (primer octante: 0 < pendiente <= 1)
 *
 * Compilar: g++ -o algoritmos_lineas algoritmos_lineas.cpp -lGL -lGLU -lglut
 */

#include <GL/glut.h>
#include <cmath>
#include <algorithm>

/* ------------------------------------------------------------------
 * Funcion utilitaria: dibuja un pixel en (ix, iy)
 * ------------------------------------------------------------------ */
void plot(int ix, int iy) {
    glBegin(GL_POINTS);
        glVertex2i(ix, iy);
    glEnd();
}

/* ------------------------------------------------------------------
 * Algoritmo 1: Pendiente-Interseccion  (y = m*x + b)
 * Calcula la interseccion b y evalua la ecuacion en cada x.
 * Requiere x0 < x1; se normalizan los extremos si es necesario.
 * ------------------------------------------------------------------ */
void PendienteInterseccion(int x0, int y0, int x1, int y1) {
    if (x0 > x1) { std::swap(x0, x1); std::swap(y0, y1); }
    float m = (float)(y1 - y0) / (x1 - x0);
    float b = (float)y0 - m * (float)x0;
    for (int x = x0; x <= x1; x++) {
        plot(x, (int)roundf(m * (float)x + b));
    }
}

/* ------------------------------------------------------------------
 * Algoritmo 2: DDA (Digital Differential Analyzer)
 * Incrementa y en 'm' en cada paso de x.
 * Maneja pendientes positivas y negativas; normaliza la direccion.
 * ------------------------------------------------------------------ */
void DDA(int x0, int y0, int x1, int y1) {
    if (x0 > x1) { std::swap(x0, x1); std::swap(y0, y1); }
    float m = (float)(y1 - y0) / (x1 - x0);
    float y = (float)y0;
    for (int x = x0; x <= x1; x++) {
        plot(x, (int)roundf(y));
        y += m;
    }
}

/* ------------------------------------------------------------------
 * Algoritmo 3: Bresenham  (generalizado para todos los octantes)
 * Usa solo arimetica entera. Maneja:
 *   - Lineas "steep" (|dy| > |dx|) mediante transposicion.
 *   - Pendientes negativas mediante dy_step = -1.
 * ------------------------------------------------------------------ */
void Bresenham(int x0, int y0, int x1, int y1) {
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) { std::swap(x0, y0); std::swap(x1, y1); }
    if (x0 > x1) { std::swap(x0, x1); std::swap(y0, y1); }

    int dy_step  = (y0 < y1) ? 1 : -1;
    int delta_x  = x1 - x0;
    int delta_y  = std::abs(y1 - y0);
    int y        = y0;
    int error    = 0;

    for (int x = x0; x <= x1; x++) {
        if (steep) plot(y, x); else plot(x, y);
        error += delta_y;
        if (2 * error >= delta_x) {
            y     += dy_step;
            error -= delta_x;
        }
    }
}

/* ------------------------------------------------------------------
 * Algoritmo 4: Punto Medio
 * Valido para el primer octante: x0 < x1, y0 < y1, dy <= dx.
 * Usa solo arimetica entera con decision incremental.
 * ------------------------------------------------------------------ */
void PtoMedio(int x0, int y0, int x1, int y1) {
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;
    int dm      = 2 * delta_y - delta_x;
    int dm_p1   = 2 * delta_y;
    int dm_p2   = 2 * delta_y - 2 * delta_x;
    int y       = y0;

    for (int x = x0; x <= x1; x++) {
        plot(x, y);
        if (dm <= 0) {
            dm += dm_p1;
        } else {
            dm += dm_p2;
            y++;
        }
    }
}

/* ------------------------------------------------------------------
 * Helper: imprime texto en pantalla usando coordenadas del mundo
 * ------------------------------------------------------------------ */
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

/* ------------------------------------------------------------------
 * Funcion de renderizado
 * Cada algoritmo dibuja una linea diferente en su propio color y
 * region de la ventana para poder comparar los resultados.
 * ------------------------------------------------------------------ */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    /* ---- Algoritmo 1: Pendiente-Interseccion  (ROJO) ----
     * Linea muy suave positiva en la parte inferior             */
    glColor3f(0.9f, 0.1f, 0.1f);
    PendienteInterseccion(30, 55, 460, 115);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(30, 40, "1. Pendiente-Interseccion (rojo)");

    /* ---- Algoritmo 2: DDA  (AZUL) ----
     * Linea con pendiente moderada positiva                     */
    glColor3f(0.1f, 0.1f, 0.9f);
    DDA(30, 175, 460, 265);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(30, 160, "2. DDA (azul)");

    /* ---- Algoritmo 4: Punto Medio  (MAGENTA) ----
     * Primer octante obligatorio: dy(=80) < dx(=430)            */
    glColor3f(0.7f, 0.0f, 0.7f);
    PtoMedio(30, 305, 460, 385);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(30, 290, "4. Punto Medio (magenta) - primer octante");

    /* ---- Algoritmo 3: Bresenham  (VERDE) ----
     * Pendiente negativa para demostrar manejo multi-octante    */
    glColor3f(0.0f, 0.65f, 0.0f);
    Bresenham(30, 460, 460, 420);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(30, 472, "3. Bresenham (verde) - pendiente negativa");

    glFlush();
}

/* ------------------------------------------------------------------
 * Inicializacion de OpenGL
 * ------------------------------------------------------------------ */
void myinit() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

/* ------------------------------------------------------------------
 * Funcion principal
 * ------------------------------------------------------------------ */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Practica 2 - Algoritmos de Trazo de Lineas");
    myinit();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
