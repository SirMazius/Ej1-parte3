#include "stdafx.h"

//  N�mero de frames
int frameCount = 0;
//  N�mero de frames por segundo
float fps = 0;
//  currentTime y previousTime - momento actual y momento previo
int currentTime = 0, previousTime = 0;
int selector = 0;
int vbo1, vbo2, vbo3, vbo4, vbo5, vbo6, vbo7;
GLuint vaoHandle1, vaoHandle2, vaoHandle3;
void calculateFPS();
void printFPS();

void draw1Cube();
void drawMultipleCubes();
void drawMultipleCubes2();
void drawMultipleCubes3();
void drawMultipleCubes4();
void drawMultipleCubes5();
void drawMultipleCubes6();
void drawMultipleCubes7();
void drawMultipleCubes8();
void drawMultipleCubes9();
void drawMultipleCubes10();
GLuint initVBO(int, GLfloat *);
GLuint initVBO(int, GLubyte *);

bool init();
void display();
void resize(int, int);
void idle();
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void mouse(int, int, int, int);
void mouseMotion(int, int);


bool fullscreen = false;
bool mouseDown = false;
bool animation = true;

float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;

int g_Width = 500;                          // Ancho inicial de la ventana
int g_Height = 500;                         // Altura incial de la ventana


											// cubo ///////////////////////////////////////////////////////////////////////
											//    v6----- v5
											//   /|      /|
											//  v1------v0|
											//  | |     | |
											//  | |v7---|-|v4
											//  |/      |/
											//  v2------v3

											// Coordenadas del vertex array  =====================================
											// Un cubo tiene 6 lados y cada lado tiene 2 triangles, por tanto, un cubo
											// tiene 36 v�rtices (6 lados * 2 trian * 3 vertices = 36 vertices). Y cada
											// vertice tiene 3 components (x,y,z) de reales, por tanto, el tama�o del vertex
											// array es 108 floats (36 * 3 = 108).
GLfloat vertices1[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
-1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
-1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

-1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
-1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

-1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
-1, 1,-1,   1, 1,-1,   1,-1,-1 };    // v6-v5-v4
									 // color array
GLfloat colores1[] = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4



									// Coordenadas del vertex array eliminando los v�rtices repetidos (m�s compacto)
GLfloat vertices2[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
1,-1,-1,   1, 1,-1,  -1, 1,-1,  -1,-1,-1 };	// v4,v5,v6,v7 (back)

											// color array
GLfloat colores2[] = { 1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1,   // v0,v1,v2,v3 (front)
0, 0, 1,   0, 1, 1,   0, 1, 0,   0, 0, 0 };	// v4,v5,v6,v7 (back)

											// index array del vertex array
GLubyte  indices[] = { 0, 1, 2,   2, 3, 0,      // front
0, 3, 4,   4, 5, 0,      // right
0, 5, 6,   6, 1, 0,      // top
1, 6, 7,   7, 2, 1,      // left
7, 4, 3,   3, 2, 7,      // bottom
4, 7, 6,   6, 5, 4 };    // back

						 // Vertex array entrelazado
						 // Los dos atributos (posici�n y color) se empaquetan juntos. El resultado es 
						 // algo similar a un conjunto de estructuras: ((V,C), (V,C), (V,C),...).
						 // Esta forma de empaquetar los atributos provee mejor localizaci�n en la memoria.
GLfloat vertices3[] = { 1, 1, 1,  1, 1, 1,	// v0
-1, 1, 1,  1, 1, 0,	// v1
-1,-1, 1,  1, 0, 0,	// v2
1,-1, 1,  1, 0, 1,	// v3
1,-1,-1,  0, 0, 1,	// v4
1, 1,-1,  0, 1, 1,	// v5
-1, 1,-1,  0, 1, 0,	// v6
-1,-1,-1,  0, 0, 0 };// v7


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Programa Ejemplo");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		system("pause");
		exit(-1);
	}
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();

	return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// dibuja 1 cubo: modo inmediato
// 78 llamadas = 36 glVertex*() llamadas + 36 glColor*() llamadas + 6 glNormal*() llamadas
///////////////////////////////////////////////////////////////////////////////
void draw1Cube()
{
	glBegin(GL_TRIANGLES);
	// front faces
	glNormal3f(0, 0, 1);
	// face v0-v1-v2
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);
	glColor3f(1, 1, 0);
	glVertex3f(-1, 1, 1);
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, 1);
	// face v2-v3-v0
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, 1);
	glColor3f(1, 0, 1);
	glVertex3f(1, -1, 1);
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);

	// right faces
	glNormal3f(1, 0, 0);
	// face v0-v3-v4
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);
	glColor3f(1, 0, 1);
	glVertex3f(1, -1, 1);
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, -1);
	// face v4-v5-v0
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, -1);
	glColor3f(0, 1, 1);
	glVertex3f(1, 1, -1);
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);

	// top faces
	glNormal3f(0, 1, 0);
	// face v0-v5-v6
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);
	glColor3f(0, 1, 1);
	glVertex3f(1, 1, -1);
	glColor3f(0, 1, 0);
	glVertex3f(-1, 1, -1);
	// face v6-v1-v0
	glColor3f(0, 1, 0);
	glVertex3f(-1, 1, -1);
	glColor3f(1, 1, 0);
	glVertex3f(-1, 1, 1);
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 1);

	// left faces
	glNormal3f(-1, 0, 0);
	// face  v1-v6-v7
	glColor3f(1, 1, 0);
	glVertex3f(-1, 1, 1);
	glColor3f(0, 1, 0);
	glVertex3f(-1, 1, -1);
	glColor3f(0, 0, 0);
	glVertex3f(-1, -1, -1);
	// face v7-v2-v1
	glColor3f(0, 0, 0);
	glVertex3f(-1, -1, -1);
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, 1);
	glColor3f(1, 1, 0);
	glVertex3f(-1, 1, 1);

	// bottom faces
	glNormal3f(0, -1, 0);
	// face v7-v4-v3
	glColor3f(0, 0, 0);
	glVertex3f(-1, -1, -1);
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, -1);
	glColor3f(1, 0, 1);
	glVertex3f(1, -1, 1);
	// face v3-v2-v7
	glColor3f(1, 0, 1);
	glVertex3f(1, -1, 1);
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, 1);
	glColor3f(0, 0, 0);
	glVertex3f(-1, -1, -1);

	// back faces
	glNormal3f(0, 0, -1);
	// face v4-v7-v6
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, -1);
	glColor3f(0, 0, 0);
	glVertex3f(-1, -1, -1);
	glColor3f(0, 1, 0);
	glVertex3f(-1, 1, -1);
	// face v6-v5-v4
	glColor3f(0, 1, 0);
	glVertex3f(-1, 1, -1);
	glColor3f(0, 1, 1);
	glVertex3f(1, 1, -1);
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, -1);
	glEnd();
}

////////////////////////////////////////////////////////////////////////////////////
// Dibuja m�ltiples cubos
////////////////////////////////////////////////////////////////////////////////////
void drawMultipleCubes()
{
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);

	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				draw1Cube();
				glPopMatrix();
			}
		}
	}
}
void drawMultipleCubes2()
{
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices1);
	glColorPointer(3, GL_FLOAT, 0, colores1);
	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glPopMatrix();
			}
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void drawMultipleCubes3()
{
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices2);
	glColorPointer(3, GL_FLOAT, 0, colores2);
	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

				glPopMatrix();
			}
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void drawMultipleCubes4() {
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 24, vertices3);
	glColorPointer(3, GL_FLOAT, 24, vertices3 + 3);
	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
				glPopMatrix();
			}
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawMultipleCubes5()
{
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glColorPointer(3, GL_FLOAT, 0, 0);

	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glPopMatrix();
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}
void drawMultipleCubes6()
{
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4);
	glColorPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo5);

	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
				glPopMatrix();
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void drawMultipleCubes7() {
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo6);
	glVertexPointer(3, GL_FLOAT, 24, 0);
	glColorPointer(3, GL_FLOAT, 24, (char*)NULL + 12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo7);
	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
				glPopMatrix();
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawMultipleCubes8()
{
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	
	glBindVertexArray(vaoHandle1);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	

	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glPopMatrix();
			}
		}
	}
	glBindVertexArray(0);
}
void drawMultipleCubes9() {
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);
	

	glBindVertexArray(vaoHandle2);

	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);

				glPopMatrix();
			}
		}
	}
	glBindVertexArray(0);
}
void drawMultipleCubes10() {
	const int N = 40; // N�mero de cubos en cada eje
	const float RangoIni = -1.0f;
	const float RangoFin = 1.0f;
	float x, y, z;
	float scale = (RangoFin - RangoIni) / (N * 3);


	glBindVertexArray(vaoHandle3);

	for (int i = 0; i < N; i++)
	{
		x = RangoIni + i * (RangoFin - RangoIni) / (N - 1);
		for (int j = 0; j < N; j++)
		{
			y = RangoIni + j * (RangoFin - RangoIni) / (N - 1);
			for (int k = 0; k < N; k++)
			{
				z = RangoIni + k * (RangoFin - RangoIni) / (N - 1);
				glPushMatrix();
				glTranslatef(x, y, z);
				glScalef(scale, scale, scale);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);

				glPopMatrix();
			}
		}
	}
	glBindVertexArray(0);
}

GLuint initVBO(int dataSize, GLfloat *vertexData) {
	GLuint VBOBuffers;
	glGenBuffers(1, &VBOBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);
	glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(GL_FLOAT), vertexData, GL_STATIC_DRAW);
	//delete [] vertexData; 
	return VBOBuffers;
}
GLuint initVBO(int dataSize, GLubyte *vertexData) {
	GLuint VBOBuffers;
	glGenBuffers(1, &VBOBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBuffers);
	glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(GLubyte), vertexData, GL_STATIC_DRAW);
	//delete [] vertexData; 
	return VBOBuffers;
}
void initVAO() {
	///////////////////////////////////// ->> 7
	glGenVertexArrays(1, &vaoHandle1);
	glBindVertexArray(vaoHandle1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindVertexArray(0);

	//////////////////////////////////// ->> 8

	glGenVertexArrays(1, &vaoHandle2);
	glBindVertexArray(vaoHandle2);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4);
	glColorPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo5);

	glBindVertexArray(0);

	//////////////////////////////////// ->> 9

	glGenVertexArrays(1, &vaoHandle3);
	glBindVertexArray(vaoHandle3);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo6);
	glVertexPointer(3, GL_FLOAT, 24, 0);
	glColorPointer(3, GL_FLOAT, 24, (char*)NULL + 12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo7);

	glBindVertexArray(0);
}


bool init()
{
	vbo1 = initVBO(sizeof(vertices1) / sizeof(*vertices1), vertices1);
	vbo2 = initVBO(sizeof(colores1) / sizeof(*colores1), colores1);

	vbo3 = initVBO(sizeof(vertices2) / sizeof(*vertices2), vertices2);
	vbo4 = initVBO(sizeof(colores2) / sizeof(*colores2), colores2);
	vbo5 = initVBO(sizeof(indices) / sizeof(*indices), indices);

	vbo6 = initVBO(sizeof(vertices3) / sizeof(*vertices3), vertices3);
	vbo7 = initVBO(sizeof(indices) / sizeof(*indices), indices);

	initVAO();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);

	return true;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		0.0f, 0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);


	//draw1Cube();
	switch (selector) {
	case 0:
		drawMultipleCubes();
		break;
	case 1:
		drawMultipleCubes2();
		break;
	case 2:
		drawMultipleCubes3();
		break;
	case 3:
		drawMultipleCubes4();
		break;
	case 4:
		drawMultipleCubes5();
		break;
	case 5:
		drawMultipleCubes6();
		break;
	case 6:
		drawMultipleCubes7();
		break;
	case 7:
		drawMultipleCubes8();
		break;
	case 8:
		drawMultipleCubes9();
		break;
	case 9:
		drawMultipleCubes10();
	}

	printFPS();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	g_Width = w;
	g_Height = h;
	glViewport(0, 0, g_Width, g_Height);

	gluPerspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	if (!mouseDown && animation)
	{
		xrot += 0.3f;
		yrot += 0.4f;
	}

	//  Calcula FPS
	calculateFPS();

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: case 'q': case 'Q':
		exit(1);
		break;
	case 'a': case 'A':
		animation = !animation;
		break;
	case '0':
		selector = 0;
		break;
	case '1':
		selector = 1;
		break;
	case '2':
		selector = 2;
		break;
	case '3':
		selector = 3;
		break;
	case '4':
		//2vbo

		selector = 4;
		break;
	case '5':

		//3vbo
		selector = 5;
		break;
	case '6':

		selector = 6;
		//2vbo
		break;
	case '7':
		selector = 7;
		//2vbo
		break;
	case '8':
		selector = 8;
		//2vbo
		break;
	case '9':
		selector = 9;
		//2vbo
		break;
	}

}

void specialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else
		{
			glutReshapeWindow(g_Width, g_Height);
			glutPositionWindow(50, 50);
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

		glutPostRedisplay();
	}
}

//-------------------------------------------------------------------------
//  Print FPS
//-------------------------------------------------------------------------
void printFPS()
{
	std::cout.precision(4);
	std::cout << fps << std::endl;
}

//-------------------------------------------------------------------------
// Calcula el frame rate por segundo
//-------------------------------------------------------------------------
void calculateFPS()
{
	frameCount++;

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);

		previousTime = currentTime;

		frameCount = 0;
	}
}


