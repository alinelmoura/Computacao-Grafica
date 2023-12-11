/*aline nataly- 164905
 * P2 CG- Robo humanoide
 */
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>


#define STEP 0.1
#define TORSO_HEIGHT 5.0
#define TORSO_RADIUS 1.5
#define UPPER_ARM_HEIGHT 3.0
#define UPPER_ARM_RADIUS  0.5
#define LOWER_ARM_HEIGHT 3.0
#define LOWER_ARM_RADIUS  0.5
#define UPPER_LEG_HEIGHT 4.0
#define UPPER_LEG_RADIUS  0.45
#define LOWER_LEG_HEIGHT 3.5
#define LOWER_LEG_RADIUS  0.45
#define ELBOW_RADIUS 0.6
#define KNEE_RADIUS 0.7
#define HAND_RADIUS 0.6
#define FOOT_RADIUS 0.7
#define SHOULDER_RADIUS 0.7
#define HEAD_HEIGHT 1.5
#define HEAD_RADIUS 1.0
#define NECK_HEIGHT 1.0
#define NECK_RADIUS 0.4

int option = -1;
int animation = -3; //teleytaio animation
int done = 0; //an oloklirwthike to teleytaio animation
double rotate = 0;
double horizontal = 0;
GLfloat robotPositionZ = 0.0f;
GLfloat rodaEsq = 0.0f;
GLfloat rodaDir = 0.0f;
GLfloat gira = 0.0f;

void head();
void torso();
void left_upper_arm();
void right_upper_arm();
void left_upper_leg();
void right_upper_leg();

GLUquadricObj *t, *h, *lua, *lla, *rua, *rla, *lll, *rll, *rul, *lul; //objetos para a construção do robo
GLUquadricObj *relb, *lelb, *rknee, *lknee, *nk, *lhand, *rhand, *lfoot, *rfoot, *rsh, *lsh;

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

    t = gluNewQuadric();
    h = gluNewQuadric();
    lua = gluNewQuadric();
    lla = gluNewQuadric();
    rua = gluNewQuadric();
    rla = gluNewQuadric();
    lul = gluNewQuadric();
    lll = gluNewQuadric();
    rul = gluNewQuadric();
    rll = gluNewQuadric();
    relb = gluNewQuadric();
    lelb = gluNewQuadric();
    rknee = gluNewQuadric();
    lknee = gluNewQuadric();
    nk = gluNewQuadric();
    lhand = gluNewQuadric();
    rhand = gluNewQuadric();
    lfoot = gluNewQuadric();
    rfoot = gluNewQuadric();
    rsh = gluNewQuadric();
    lsh = gluNewQuadric();

    lightning();
}

void torso()
{
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricTexture(t,GL_TRUE);
	gluCylinder(t, TORSO_RADIUS / 1.2, TORSO_RADIUS, TORSO_HEIGHT, 10, 10);
	glPopMatrix();
}

void head()
{
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0.0, 6.5, 0.0);
	glScalef(HEAD_RADIUS, HEAD_HEIGHT, HEAD_RADIUS);
	gluQuadricTexture(h,GL_TRUE);
	gluSphere(h, HEAD_RADIUS, 10, 10);
	glPopMatrix();

	//glasses
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0.0, 6.5, 0.15);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	//glTranslatef(0.0f, -0.4f, -0.15f);
	gluCylinder(h, 0.99*HEAD_RADIUS, 0.99*HEAD_RADIUS, HEAD_HEIGHT / 5, 10, 10);
	glPopMatrix();
}

void rightShoulder()
{
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-2, 4.5, 0.0);
	gluSphere(relb, SHOULDER_RADIUS, 10, 10);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(-2, 1.0, 0.0);
		gluSphere(relb, SHOULDER_RADIUS, 10, 10);
		glPopMatrix();
}

void leftShoulder()
{
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(2, 4.5, 0.0);
	gluSphere(lelb, SHOULDER_RADIUS, 10, 10);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(2, 1.0, 0.0);
		gluSphere(lelb, SHOULDER_RADIUS, 10, 10);
		glPopMatrix();
}

void leftFoot()
{
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0.5, -4.0, 0.0);
	gluSphere(lknee, FOOT_RADIUS, 10, 10);
	glPopMatrix();
}

void rightFoot()
{
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-0.5, -4.0, 0.0);
	gluSphere(lknee, FOOT_RADIUS, 10, 10);
	glPopMatrix();
}

void left_upper_arm()
{
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(2, 1.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricTexture(lua,GL_TRUE);
	gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS - 0.1, UPPER_ARM_HEIGHT, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(2, -2.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricTexture(lua,GL_TRUE);
	gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS - 0.1, UPPER_ARM_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_arm()
{
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(-2, 1.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricTexture(rua,GL_TRUE);
	gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS - 0.1, UPPER_ARM_HEIGHT, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(-2, -2.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricTexture(rua,GL_TRUE);
	gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS - 0.1, UPPER_ARM_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_upper_leg()
{
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0.5, -3.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(rodaDir, 1.0, 0.0, 0.0);
	gluQuadricTexture(lul,GL_TRUE);
	gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS - 0.1, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0.5, -7.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(rodaDir, 1.0, 0.0, 0.0);
	gluQuadricTexture(lul,GL_TRUE);
	gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS - 0.1, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_leg()
{
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(-0.5, -3.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluQuadricTexture(lul,GL_TRUE);
	gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS - 0.1, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(-0.5, -7.5, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(rodaDir, 1.0, 0.0, 0.0);
	gluQuadricTexture(lul,GL_TRUE);
	gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS - 0.1, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void perna_right(){
	glPushMatrix();
	glRotatef(rodaEsq, 1.0, 0.0, 0.0);
	right_upper_leg();
	rightFoot();
	glPopMatrix();
}

void perna_left(){
	glPushMatrix();
	glRotatef(rodaDir, 1.0, 0.0, 0.0);
	left_upper_leg();
	leftFoot();
	glPopMatrix();
}
void robo() {
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glRotatef(gira, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, 0.0f, robotPositionZ);  // Translação ao longo do eixo Z
    torso();
    head();
    leftShoulder();
    rightShoulder();
    left_upper_arm();
    right_upper_arm();
    perna_left();
    perna_right();
    glPopMatrix();
}

void andar(int key, int x, int y){
     switch (key){
        case GLUT_KEY_UP :
        	robotPositionZ += 0.3f;
        	if(rodaEsq<30){
        			rodaEsq+=1;
        			rodaDir-=1;
        	}
        	else{
        		rodaEsq=0;
        		rodaDir=0;
        	}
           break ;
        case GLUT_KEY_DOWN :
        	robotPositionZ -= 0.3f;
        	if(rodaEsq<30){
        	        			rodaEsq+=1;
        	        			rodaDir-=1;
        	        	}
        	        	else{
        	        		rodaEsq=0;
        	        		rodaDir=0;
        	        	}
           break ;
        case GLUT_KEY_LEFT :
                   gira-=0.5;
                   break ;
                case GLUT_KEY_RIGHT :
                   gira+=0.5;
                   break ;
     }
     glutPostRedisplay() ;
}

void displayFunc() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa o buffer

        glMatrixMode(GL_MODELVIEW); //define que a matrix é a de modelo
        //glBindTexture(GL_TEXTURE_2D, texture_id[0]);
        glPushMatrix();
        glScalef(0.1, 0.1, 0.1);
        robo();
        glPopMatrix();

        glFlush();                  //força o desenho das primitivas
}

int main(int argc, char *argv[]){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(500,500);
    glutCreateWindow("Robo humanoide");
    glutDisplayFunc(displayFunc);
    glutSpecialFunc(andar);
    init();
    initTexture();
    glutMainLoop();
    return 0;
}
