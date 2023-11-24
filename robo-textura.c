//aline nataly- 164905
#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct BMPImagem
{
    int   width;
    int   height;
    char *data;
}BMPImage;

/* Qtd máxima de texturas a serem usadas no programa */
#define MAX_NO_TEXTURES 5

/* vetor com os números das texturas */
GLuint texture_id[MAX_NO_TEXTURES];

char* filenameArray[MAX_NO_TEXTURES] = {
        "surface1.bmp",
        "surface2.bmp",
        "surface3.bmp",
        "surface4.bmp",
        "surface5.bmp"
};

GLUquadricObj *obj;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;

void getBitmapImageData( char *pFileName, BMPImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
    int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
        printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ", pFileName);

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ", pFileName);

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);

    if( nNumPlanes != 1 )
        printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
        printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);

    if( nNumBPP != 24 )
        printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

    // Calculate the image's total size in bytes. Note how we multiply the
    // result of (width * height) by 3. This is becuase a 24 bit color BMP
    // file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);

    //
    // Finally, rearrange BGR to RGB
    //

    char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
    {
        charTemp = pImage->data[i];
        pImage->data[i] = pImage->data[i+2];
        pImage->data[i+2] = charTemp;
    }
}


/*Função para Carregar uma imagem .BMP */
void CarregaTexturas()
{
    BMPImage textura;

    /* Define quantas texturas serão usadas no programa  */
    glGenTextures(MAX_NO_TEXTURES, texture_id); /* 1 = uma textura; */
                                /* texture_id = vetor que guardas os números das texturas */

    int i;
    for ( i=0; i<MAX_NO_TEXTURES; i++ ) {
        getBitmapImageData( filenameArray[i], &textura);
        glBindTexture(GL_TEXTURE_2D, texture_id[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
}

void initTexture (void)
{
    /* Habilita o uso de textura bidimensional  */
    glEnable ( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    CarregaTexturas();
}

void lightning(){
    GLfloat light0_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0,GL_POSITION,light0_pos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,black);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white);
    glLightfv(GL_LIGHT0,GL_SPECULAR,white);

    //Fonte de Luz Direcional - Efeito de Holofote
    GLfloat light1_pos[] = {-2.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT1,GL_POSITION,light1_pos);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1,GL_SPECULAR, white);
    GLfloat direction[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,direction);       //vetor direção
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,45.0f);               //espalhamento angular
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.1f);              //atenuação angular

    //Parâmetros definidos globalmente
    //GLfloat global_ambient[] = {0.9f, 0.9f, 0.9f, 1.0f};
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    ////Reflexão especular definida como a posição de visão corrente
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    ////Habilitar cálculos de iluminação para ambas as faces dos polígonos
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void init(void)
{
    glEnable ( GL_COLOR_MATERIAL );
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //define a cor de fundo
    glEnable(GL_DEPTH_TEST); //habilita o teste de profundidade

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW); //define que a matrix é a model view
    glLoadIdentity(); //carrega a matrix de identidade
    gluLookAt(1.0, 2.0, 4.0,   //posição da câmera
              0.0, 0.0, 0.0,   //para onde a câmera aponta
              0.0, 1.0, 0.0);  //vetor view-up

    glMatrixMode(GL_PROJECTION); //define que a matrix é a de projeção
    glLoadIdentity(); //carrega a matrix de identidade
    //glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção ortogonal
    gluPerspective(45.0, 1.0, 2.0, 8.0); //define uma projeção perspectiva
    //glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva simétrica
    //glFrustum(-2.0, 1.0, -1.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva obliqua
    glViewport(0, 0, 500, 500);

    lightning();
}

void cubo(){
  //glColor3f(VERMELHO);
  glBegin(GL_POLYGON);
  	glNormal3f(-0.1,0.1,0.1);
  	glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.1,0.1,0.1);//1
    glNormal3f(0.1,0.1,0.1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.1,0.1,0.1);//2
    glNormal3f(0.1,-0.1,0.1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(0.1,-0.1,0.1);//3
    glNormal3f(-0.1,-0.1,0.1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-0.1,-0.1,0.1);//4
  glEnd();
  //glColor3f(ROSA);
  glBegin(GL_POLYGON);
    glNormal3f(-0.1,0.1,-0.1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-0.1,0.1,-0.1);//1

    glNormal3f(0.1,0.1,-0.1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(0.1,0.1,-0.1);

    glNormal3f(0.1,0.1,-0.1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.1,-0.1,-0.1);

    glNormal3f(-0.1,-0.1,-0.1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.1,-0.1,-0.1);

  glEnd();
  //glColor3f(LARANJA);
  glBegin(GL_POLYGON);
    glNormal3f(0.1,-0.1,0.1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(0.1,-0.1,0.1);

    glNormal3f(0.1,0.1,0.1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.1,0.1,0.1);

    glNormal3f(0.1,0.1,-0.1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(0.1,0.1,-0.1);

    glNormal3f(0.1,0.1,-0.1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.1,-0.1,-0.1);
  glEnd();

  //glColor3f(AMARELO);
  glBegin(GL_POLYGON);
    glNormal3f(-0.1,-0.1,0.1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.1,-0.1,0.1);

    glNormal3f(-0.1,0.1,0.1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-0.1,0.1,0.1);

    glNormal3f(-0.1,0.1,-0.1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.1,0.1,-0.1);

    glNormal3f(-0.1,-0.1,-0.1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-0.1,-0.1,-0.1);
  glEnd();
  //glColor3f(AZUL);
  glBegin(GL_POLYGON);
    glNormal3f(-0.1,0.1,0.1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.1,0.1,0.1);

    glNormal3f(0.1,0.1,0.1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.1,0.1,0.1);

    glNormal3f(-0.1,0.1,0.1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(0.1,0.1,-0.1);

    glNormal3f(-0.1,0.1,-0.1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-0.1,0.1,-0.1);
  glEnd();
  //glColor3f(BRANCO);
  glBegin(GL_POLYGON);
    glNormal3f(-0.1,-0.1,0.1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.1,-0.1,0.1);

    glNormal3f(0.1,-0.1,0.1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.1,-0.1,0.1);

    glNormal3f(0.1,-0.1,-0.1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(0.1,-0.1,-0.1);

    glNormal3f(-0.1,-0.1,-0.1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-0.1,-0.1,-0.1);
  glEnd();
}

void cuboFino() {
  //glColor3f(VERMELHO);
  glBegin(GL_POLYGON);
    glNormal3f(-0.04,0.04,0.04);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.04,0.04,0.04);

    glNormal3f(0.04,0.04,0.04);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.04,0.04,0.04);

    glNormal3f(0.04,-0.04,0.04);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.04,-0.04,0.04);

    glNormal3f(-0.04,-0.04,0.04);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.04,-0.04,0.04);
  glEnd();

  //glColor3f(ROSA);
  glBegin(GL_POLYGON);
    glNormal3f(-0.04,0.04,-0.04);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.04,0.04,-0.04);

    glNormal3f(0.04,0.04,-0.04);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.04,0.04,-0.04);

    glNormal3f(0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.04,-0.04,-0.04);

    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.04,-0.04,-0.04);
  glEnd();
  //glColor3f(LARANJA);
  glBegin(GL_POLYGON);
  glNormal3f(-0.04,0.04,-0.04);
  glTexCoord2f(1.0f,0.0f);
    glVertex3f(0.04,-0.04,0.04);

    glNormal3f(-0.04,0.04,-0.04);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.04,0.04,0.04);

    glNormal3f(-0.04,0.04,-0.04);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.04,0.04,-0.04);

    glNormal3f(-0.04,0.04,-0.04);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(0.04,-0.04,-0.04);
  glEnd();
  //glColor3f(AMARELO);
  glBegin(GL_POLYGON);
    glNormal3f(-0.04,-0.04,0.04);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.04,-0.04,0.04);

    glNormal3f(-0.04,0.04,0.04);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-0.04,0.04,0.04);

    glNormal3f(-0.04,0.04,-0.04);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-0.04,0.04,-0.04);

    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.04,-0.04,-0.04);
  glEnd();
  //glColor3f(AZUL);
  glBegin(GL_POLYGON);
    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.04,0.04,0.04);

    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.04,0.04,0.04);

    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.04,0.04,-0.04);

    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.04,0.04,-0.04);
  glEnd();
  //glColor3f(BRANCO);
  glBegin(GL_POLYGON);
    glNormal3f(-0.04,-0.04,0.04);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-0.04,-0.04,0.04);

    glNormal3f(0.04,-0.04,0.04);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(0.04,-0.04,0.04);

    glNormal3f(0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(0.04,-0.04,-0.04);

    glNormal3f(-0.04,-0.04,-0.04);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-0.04,-0.04,-0.04);
  glEnd();
}

void tronco(){

	  //cabeça
		glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	  glPushMatrix();
	  glTranslatef(0.0,0.5,0.0);
	  cubo();
	  glPopMatrix();

	  //corpo 1
	  glBindTexture(GL_TEXTURE_2D, texture_id[2]);
	  glPushMatrix();
	  glTranslatef(0.0,0.25,0.0);
	  cubo();
	  glPopMatrix();

	  //corpo 2
	  glBindTexture(GL_TEXTURE_2D, texture_id[2]);
	  glPushMatrix();
	  glTranslatef(0.0,0.05,0.0);
	  cubo();
	  glPopMatrix();
}

void pernas(){
	//perna direita
	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  glPushMatrix();
		  glTranslatef(0.056,-0.13,0.0);
		  cuboFino();
		  glPopMatrix();

		  glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  glPushMatrix();
		  glTranslatef(0.056,-0.20,0.0);
		  cuboFino();
		  glPopMatrix();

		  glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  glPushMatrix();
		  glTranslatef(0.056,-0.31,0.0);
		  cuboFino();
		  glPopMatrix();

		  glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  glPushMatrix();
		  glTranslatef(0.056,-0.39,0.0);
		  cuboFino();
		  glPopMatrix();

		  //perna esquerda
		  glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  	  glPushMatrix();
		  	  glTranslatef(-0.056,-0.13,0.0);
		  	  cuboFino();
		  	  glPopMatrix();

		  	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  	  glPushMatrix();
		  	  glTranslatef(-0.056,-0.20,0.0);
		  	  cuboFino();
		  	  glPopMatrix();

		  	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  	  glPushMatrix();
		  	  glTranslatef(-0.056,-0.31,0.0);
		  	  cuboFino();
		  	  glPopMatrix();

		  	glBindTexture(GL_TEXTURE_2D, texture_id[3]);
		  	  glPushMatrix();
		  	  glTranslatef(-0.056,-0.39,0.0);
		  	  cuboFino();
		  	  glPopMatrix();
}

void bracoEsq(){
	//braço esquerdo
	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  glPushMatrix();
		  glTranslatef(-0.17,0.31,0.0);
		  cuboFino();
		  glPopMatrix();

		  glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  glPushMatrix();
		  glTranslatef(-0.25,0.31,0.0);
		  cuboFino();
		  glPopMatrix();

		  glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  glPushMatrix();
		  glTranslatef(-0.36,0.31,0.0);
		  cuboFino();
		  glPopMatrix();

		  glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  glPushMatrix();
		  glTranslatef(-0.44,0.31,0.0);
		  cuboFino();
		  glPopMatrix();

}

void bracoDir(){
	 //braço direito
	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  	  glPushMatrix();
		  	  glTranslatef(0.17,0.31,0.0);
		  	  cuboFino();
		  	  glPopMatrix();

		  	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  	  glPushMatrix();
		  	  glTranslatef(0.25,0.31,0.0);
		  	  cuboFino();
		  	  glPopMatrix();

		  	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  	  glPushMatrix();
		  	  glTranslatef(0.36,0.31,0.0);
		  	  cuboFino();
		  	  glPopMatrix();

		  	glBindTexture(GL_TEXTURE_2D, texture_id[4]);
		  	  glPushMatrix();
		  	  glTranslatef(0.44,0.31,0.0);
		  	  cuboFino();
		  	  glPopMatrix();


}

void robo(){
  glMatrixMode(GL_MODELVIEW);

  tronco();

  pernas();

  bracoEsq();

  bracoDir();
}

void displayFunc() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa o buffer

        glMatrixMode(GL_MODELVIEW); //define que a matrix é a de modelo
        glBindTexture(GL_TEXTURE_2D, texture_id[0]);
        glPushMatrix();
        glRotatef(angleX,1.0,0.0,0.0);
        glRotatef(angleY,0.0,1.0,0.0);
        robo();
        glPopMatrix();

        glFlush();                  //força o desenho das primitivas
}

void rotacoes(int key, int x, int y){
     switch (key){
        case GLUT_KEY_UP :
           angleX+=15;
           break ;
        case GLUT_KEY_DOWN :
           angleX-=15;
           break ;
        case GLUT_KEY_LEFT :
           angleY-=15;
           break ;
        case GLUT_KEY_RIGHT :
           angleY+=15;
           break ;
        default:
           break;
     }
     glutPostRedisplay() ;
}

int main(int argc, char *argv[]){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(500,500);
    glutCreateWindow("Textura");
    glutDisplayFunc(displayFunc);
    glutSpecialFunc(rotacoes);
    init();
    initTexture();
    glutMainLoop();
    return 0;
}
