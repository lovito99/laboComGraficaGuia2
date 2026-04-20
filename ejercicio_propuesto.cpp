/*
 * Practica 2 - Ejercicio Propuesto
 * Dibuja 5 lineas al azar usando el algoritmo de Bresenham y
 * calcula + señala todos los puntos de interseccion entre ellas.
 *
 * La deteccion de intersecciones usa la forma parametrica:
 *   P(t) = A + t*(B-A),  Q(s) = C + s*(D-C)
 *   Hay interseccion si t,s in [0,1] (dentro de los segmentos).
 *
 * Compilar: g++ -o ejercicio_propuesto ejercicio_propuesto.cpp -lGL -lGLU -lglut
 */

#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <vector>

/* ------------------------------------------------------------------
 * Estructuras de datos
 * ------------------------------------------------------------------ */
struct Line {
    int x0, y0, x1, y1;
};

struct Point2D {
    float x, y;
};

/* ------------------------------------------------------------------
 * 5 lineas con coordenadas definidas para producir intersecciones
 * visibles dentro de la ventana 500x500
 * ------------------------------------------------------------------ */
static const int NUM_LINES = 5;
Line lines[NUM_LINES] = {
    { 20, 380, 490, 250 },   // Linea 0: pendiente negativa suave
    { 100, 490, 400,  10 },  // Linea 1: pendiente negativa pronunciada
    {  10, 140, 490, 360 },  // Linea 2: pendiente positiva
    { 240, 490, 310,  10 },  // Linea 3: casi vertical
    { 480, 450,  10, 170 }   // Linea 4: pendiente negativa, derecha a izquierda
};

/* Lista de intersecciones encontradas */
std::vector<Point2D> intersections;

/* ------------------------------------------------------------------
 * Funcion utilitaria: dibuja un pixel en (ix, iy)
 * ------------------------------------------------------------------ */
void plot(int ix, int iy) {
    glBegin(GL_POINTS);
        glVertex2i(ix, iy);
    glEnd();
}

/* ------------------------------------------------------------------
 * Algoritmo de Bresenham (todos los octantes)
 * Reutilizado del codigo de la practica 2.
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
 * Calcula la interseccion de dos segmentos usando forma parametrica.
 *
 * Dados:  A=(a.x0,a.y0)->B=(a.x1,a.y1)  y  C=(b.x0,b.y0)->D=(b.x1,b.y1)
 *
 * P(t) = A + t*(B-A)
 * Q(s) = C + s*(D-C)
 *
 * Resolviendo el sistema:
 *   D = dx2*dy1 - dx1*dy2
 *   t = ( dx2*(Cy-Ay) - dy2*(Cx-Ax) ) / D
 *   s = ( dx1*(Cy-Ay) - dy1*(Cx-Ax) ) / D
 *
 * Hay interseccion de segmento si t in [0,1] y s in [0,1].
 * Retorna true y escribe el punto en 'pt' si hay interseccion.
 * ------------------------------------------------------------------ */
bool segmentIntersect(const Line& a, const Line& b, Point2D& pt) {
    float dx1 = (float)(a.x1 - a.x0);
    float dy1 = (float)(a.y1 - a.y0);
    float dx2 = (float)(b.x1 - b.x0);
    float dy2 = (float)(b.y1 - b.y0);

    float D = dx2 * dy1 - dx1 * dy2;
    if (fabsf(D) < 1e-6f) return false;  /* segmentos paralelos o colineales */

    float cx = (float)(b.x0 - a.x0);
    float cy = (float)(b.y0 - a.y0);

    float t = (dx2 * cy - dy2 * cx) / D;
    float s = (dx1 * cy - dy1 * cx) / D;

    if (t >= 0.0f && t <= 1.0f && s >= 0.0f && s <= 1.0f) {
        pt.x = (float)a.x0 + t * dx1;
        pt.y = (float)a.y0 + t * dy1;
        return true;
    }
    return false;
}

/* ------------------------------------------------------------------
 * Calcula todas las intersecciones entre los C(5,2)=10 pares de lineas
 * ------------------------------------------------------------------ */
void computeIntersections() {
    intersections.clear();
    for (int i = 0; i < NUM_LINES; i++) {
        for (int j = i + 1; j < NUM_LINES; j++) {
            Point2D pt;
            if (segmentIntersect(lines[i], lines[j], pt)) {
                intersections.push_back(pt);
            }
        }
    }
}

/* ------------------------------------------------------------------
 * Dibuja un marcador cuadrado centrado en (x,y) con lado 'size'
 * ------------------------------------------------------------------ */
void drawMarker(float x, float y, float size) {
    float h = size * 0.5f;
    glBegin(GL_QUADS);
        glVertex2f(x - h, y - h);
        glVertex2f(x + h, y - h);
        glVertex2f(x + h, y + h);
        glVertex2f(x - h, y + h);
    glEnd();
}

/* ------------------------------------------------------------------
 * Funcion de renderizado
 * ------------------------------------------------------------------ */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    /* Dibujar las 5 lineas en azul usando Bresenham */
    glColor3f(0.2f, 0.4f, 0.85f);
    glPointSize(1.5f);
    for (int i = 0; i < NUM_LINES; i++) {
        Bresenham(lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1);
    }

    /* Señalar los puntos de interseccion en rojo */
    glColor3f(0.9f, 0.1f, 0.1f);
    for (const Point2D& pt : intersections) {
        drawMarker(pt.x, pt.y, 9.0f);
    }

    glFlush();
}

/* ------------------------------------------------------------------
 * Inicializacion de OpenGL
 * ------------------------------------------------------------------ */
void myinit() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPointSize(1.5f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    computeIntersections();
}

/* ------------------------------------------------------------------
 * Funcion principal
 * ------------------------------------------------------------------ */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(620, 100);
    glutCreateWindow("Practica 2 - Ejercicio: 5 Lineas e Intersecciones");
    myinit();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
