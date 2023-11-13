/*
viewing2d01.c
 ra-164905
 Created on: 08 de nov. de 2023
 Author: aline nataly
 */

#include <windows.h>
#include <GL/glut.h>

// Variáveis para o controle do Zoom e Panning
float zoom = 1.0;
float pan_x = 0.0;
float pan_y = 0.0;
float pontoX = 0.0, pontoY = 0.0;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Aplica o Panning e o Zoom
    glTranslatef(pan_x, pan_y, 0.0);
    glScalef(zoom, zoom, 1.0);

    // Desenha algo aqui...

    glClear(GL_COLOR_BUFFER_BIT); //limpa a tela toda
    	//glClearColor(0, 0, 0, 0); //limpa a tela e deixa ela preta
    	glColor3f(0.0f, 0.0f, 1.0f); //define a cor azul com 3f(3 floats)
    	glPointSize(5.0); //define o tamanho do ponto azul
    	glBegin(GL_POINTS); //define que vamos começar apassar os parametros dos vertices; GL_POINTS=vamos passar os vertices de um ponto individual-->
    	glVertex2f(pontoX, pontoY); //fala as coordenadas do ponto com 2f(2 floats)
    	glEnd();
    	glFlush();

    glutSwapBuffers();
}

void keyboard(char key, int x, int y) {
    switch (key) {
        case 'w': // Panning para cima
            pan_y += 0.1;
            break;
        case 's': // Panning para baixo
            pan_y -= 0.1;
            break;
        case 'a': // Panning para esquerda
            pan_x -= 0.1;
            break;
        case 'd': // Panning para direita
            pan_x += 0.1;
            break;
        case '+': // Zoom in
            zoom *= 1.1;
            break;
        case '-': // Zoom out
            zoom /= 1.1;
            break;
    }
    glutPostRedisplay();
}

void desenha(){
	glClear(GL_COLOR_BUFFER_BIT); //limpa a tela toda
	//glClearColor(0, 0, 0, 0); //limpa a tela e deixa ela preta
	glColor3f(0.0f, 0.0f, 1.0f); //define a cor azul com 3f(3 floats)
	glPointSize(5.0); //define o tamanho do ponto azul
	glBegin(GL_POINTS); //define que vamos começar apassar os parametros dos vertices; GL_POINTS=vamos passar os vertices de um ponto individual-->
	/*OUTROS: GL_POINTS: Pontos individuais
	GL_LINES: Segmento de linha definido por 2 v ́ertices
	GL_LINE_STRIP: S ́erie de segmentos conectados
	GL_LINE_LOOP: S ́erie de segmentos conectados com um
	segmento ligando o primeiro e o  ́ultimo v ́ertices
	GL_TRIANGLES: Triangulos*/
	glVertex2f(pontoX, pontoY); //fala as coordenadas do ponto com 2f(2 floats)
	glEnd();
	glFlush(); //força afinalização e execução dos comendos openGL
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("OpenGL Panning e Zoom");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
