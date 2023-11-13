/*
  aline nataly
164905
*/
#include <windows.h>
#include <GL/glut.h>

#define MAX_POINTS 100

typedef struct Points {
    int x, y;
}Point;

int nPoints = 0;
Point polygon[MAX_POINTS];

// Função para adicionar um ponto ao polígono
void addPoint(Point p) {
    if (nPoints < MAX_POINTS) {
        polygon[nPoints] = p;
        nPoints++;
    }
}

// Função para limpar todos os pontos do polígono
void clearPoints() {
    nPoints = 0;
}

// Função para encontrar o ponto de interseção
Point getIntersect(Point a, Point b, Point p, Point q) {
    float A1 = b.y - a.y;
    float B1 = a.x - b.x;
    float C1 = A1 * (a.x) + B1 * (a.y);

    float A2 = q.y - p.y;
    float B2 = p.x - q.x;
    float C2 = A2 * (p.x) + B2 * (p.y);

    float determinant = A1 * B2 - A2 * B1;

    Point intersect;
    intersect.x = (B2 * C1 - B1 * C2) / determinant;
    intersect.y = (A1 * C2 - A2 * C1) / determinant;

    return intersect;
}

// Função para realizar o recorte
void sutherlandHodgman(Point p1, Point p2) {
    Point newPolygon[MAX_POINTS];
    int newPoints = 0;

    for (int i = 0; i < nPoints; i++) {
        int k = (i + 1) % nPoints;
        int ix = polygon[i].x, iy = polygon[i].y;
        int kx = polygon[k].x, ky = polygon[k].y;

        // Calcula a posição do ponto em relação à linha de recorte
        int iPos = (p2.x - p1.x) * (iy - p1.y) - (p2.y - p1.y) * (ix - p1.x);
        int kPos = (p2.x - p1.x) * (ky - p1.y) - (p2.y - p1.y) * (kx - p1.x);

        // Caso 1: Ambos os pontos estão dentro da região de recorte
        if (iPos < 0 && kPos < 0) {
            newPolygon[newPoints] = {kx, ky};
            newPoints++;
        }
        // Caso 2: Primeiro ponto está fora e o segundo ponto está dentro da região de recorte
        else if (iPos >= 0 && kPos < 0) {
            newPolygon[newPoints] = getIntersect({ix, iy}, {kx, ky}, p1, p2);
            newPoints++;
            newPolygon[newPoints] = {kx, ky};
            newPoints++;
        }
        // Caso 3: Primeiro ponto está dentro e o segundo ponto está fora da região de recorte
        else if (iPos < 0 && kPos >= 0) {
            newPolygon[newPoints] = getIntersect({ix, iy}, {kx, ky}, p1, p2);
            newPoints++;
        }
    }

    // Atualiza o polígono com o novo polígono recortado
    for (int i = 0; i < newPoints; i++) {
        polygon[i] = newPolygon[i];
    }
    nPoints = newPoints;
}

// Função para desenhar o polígono
void drawPolygon() {
    glBegin(GL_POLYGON);
    for (int i = 0; i < nPoints; i++) {
        glVertex2i(polygon[i].x, polygon[i].y);
    }
    glEnd();
}

// Função para desenhar a cena
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Define o polígono e a região de recorte
    // Define o polígono e a região de recorte
    clearPoints();
    addPoint({100, 150});
    addPoint({200, 250});
    addPoint({300, 200});
    Point p1 = {150, 100}, p2 = {250, 300};

    // Desenha o polígono original
    glColor3f(1.0, 0.0, 0.0);
    drawPolygon();

    // Aplica o algoritmo de Sutherland-Hodgman
    sutherlandHodgman(p1, p2);

    // Desenha o polígono recortado
    glColor3f(0.0, 1.0, 0.0);
    drawPolygon();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Recorte de Polígonos com Sutherland-Hodgman");
    gluOrtho2D(0, 500, 0, 500);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
