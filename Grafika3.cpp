#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
using namespace std;

//zmienne ogolne
typedef float point3[3];
int testedObject = 1; //rysowany obiekt 
int task = 1;  //wybrane zadanie

//zmienne jajka
int n = 40;//ilosc punktow
float scale = 3.0;//wielkosc obiektu
point3** points;//siatka punktow
point3** color; //kolory

//zmienne zadanie 1 i 2
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLfloat pix2angle;     // przelicznik pikseli na stopnie 
static GLint statusLeft = 0;       // stan klawisza lewego
static GLint statusRight = 0;// stan klawisza prawego
static int x_pos_old = 0;       // pozycje kursora myszy
static int delta_x = 0;
static int y_pos_old = 0;       
static int delta_y = 0;

//zmienne zadanie 2
float rViewer = 10;//R wokol punktu obserwowanego 
float rObject = 0;//R punktu obserwowanego (uzyte w celu mozliwosci przesuwania tegoz punktu)
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };//pozycja punktu widzenia
static GLfloat object[] = { 0.0, 0.0, 0.0 };//pozycja punktu obserwowanego
static GLfloat azymuth = 0;    //katy obserwacji punktu obserwowanego
static GLfloat elevation = 0;


void Axes(void)
{
	point3  x_min = { -50.0, 0.0, 0.0 };//wspolrzedne koncow osi
	point3  x_max = { 50.0, 0.0, 0.0 };
	point3  y_min = { 0.0, -50.0, 0.0 };
	point3  y_max = { 0.0,  50.0, 0.0 };
	point3  z_min = { 0.0, 0.0, -50.0 };
	point3  z_max = { 0.0, 0.0,  50.0 };

	glColor3f(1.0f, 0.0f, 0.0f);//os x
	glBegin(GL_LINES);
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);//os y
	glBegin(GL_LINES);
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);//os z
	glBegin(GL_LINES);
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

void egg() {
	float u = 0, v = 0;
	float udiff = 1.0 / n, vdiff = 1.0 / n; //n - liczba punktow na powierzchni jajka
	glTranslated(0, (-(160 * pow(0.5, 4) - 320 * pow(0.5, 3) + 160 * pow(0.5, 2)) / 2) * (scale + 7) / 10, 0);//obniżenie środka figury do centrum ukladu wspolrzednych

	for (int i = 0; i <= n; i++) {
		v = 0;//obliczenie poteg w celu ulatwienia kodu
		float u2 = pow(u, 2);
		float u3 = pow(u, 3);
		float u4 = pow(u, 4);
		float u5 = pow(u, 5);

		for (int ii = 0; ii <= n; ii++) {//obliczenie wspolrzednych punktow
			points[i][ii][0] = ((-90 * u5 + 225 * u4 - 270 * u3 + 180 * u2 - 45 * u) * cos(M_PI * v)) * (scale + 7) / 10;
			points[i][ii][1] = (160 * u4 - 320 * u3 + 160 * u2) * (scale + 7) / 10;
			points[i][ii][2] = ((-90 * u5 + 225 * u4 - 270 * u3 + 180 * u2 - 45 * u) * sin(M_PI * v)) * (scale + 7) / 10;
			v = v + vdiff;
		}
		u = u + udiff;
	}
	for (int i = 0; i < n; i++) {
		for (int ii = 0; ii < n; ii++) {
			glBegin(GL_TRIANGLES);//rysowanie pierwszego trojkata
			glColor3f(color[i][ii][0], color[i][ii][1], color[i][ii][2]);
			glVertex3f(points[i][ii][0], points[i][ii][1], points[i][ii][2]);
			glColor3f(color[i + 1][ii][0], color[i + 1][ii][1], color[i + 1][ii][2]);
			glVertex3f(points[i + 1][ii][0], points[i + 1][ii][1], points[i + 1][ii][2]);
			glColor3f(color[i + 1][ii + 1][0], color[i + 1][ii + 1][1], color[i + 1][ii + 1][2]);
			glVertex3f(points[i + 1][ii + 1][0], points[i + 1][ii + 1][1], points[i + 1][ii + 1][2]);
			glEnd();


			glBegin(GL_TRIANGLES);//rysowanie drugiego trojkata
			glColor3f(color[i][ii][0], color[i][ii][1], color[i][ii][2]);
			glVertex3f(points[i][ii][0], points[i][ii][1], points[i][ii][2]);
			glColor3f(color[i][ii + 1][0], color[i][ii + 1][1], color[i][ii + 1][2]);
			glVertex3f(points[i][ii + 1][0], points[i][ii + 1][1], points[i][ii + 1][2]);
			glColor3f(color[i + 1][ii + 1][0], color[i + 1][ii + 1][1], color[i + 1][ii + 1][2]);
			glVertex3f(points[i + 1][ii + 1][0], points[i + 1][ii + 1][1], points[i + 1][ii + 1][2]);
			glEnd();
		}
	}
}

void zadanie1() {//obiekt jest obracany a zoom jest wykonany jako poweikszanie/pomnijeszanie obiektu
	Axes();

	if (statusLeft == 1) {//obrot 
		theta[0] += delta_x * pix2angle;
		theta[1] += delta_y * pix2angle;
	}
	glRotatef(theta[0], 0.0, 1.0, 0.0);
	glRotatef(theta[1], 1.0, 0.0, 0.0);

	if (statusRight == 1) {//zoom
		if (delta_y < 0) {
			scale += 0.1;
		}
		else {
			scale -= 0.1;
		}
		if (scale <= 0.1) {
			scale = 0.1;
		}
	}

	switch (testedObject) {//wybranie obiektu
	case 1:
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireTeapot(scale);
		break;
	case 2:
		egg();
		break;
	}
}

void zadanie2() {//przesuwany jest punkt widzenia, a zoom jest wykonany jako przesuwanie go w strone punktu obserwowanego
	Axes();
	if (statusLeft == 1) {//obrot
		azymuth += delta_x * pix2angle / 100;
		elevation += delta_y * pix2angle / 100;
	}

	if (statusRight == 1) {//zoom 
		if (delta_y > 0) {
			rViewer += 0.2;
		}
		else {
			rViewer -= 0.2;
		}
	}


	rObject = rViewer - 10;//obliczenie pozycji punktu obserwowanego 
	object[0] = rObject * cos(azymuth) * cos(elevation);
	object[1] = rObject * sin(elevation);
	object[2] = rObject * sin(azymuth) * cos(elevation);


	viewer[0] = rViewer * cos(azymuth) * cos(elevation);//obliczenie pozycji punktu widzenia
	viewer[1] = rViewer * sin(elevation);
	viewer[2] = rViewer * sin(azymuth) * cos(elevation);


	switch (testedObject) {//wybranie obiektu
	case 1:
		glColor3f(1.0f, 1.0f, 1.0f);
		glutWireTeapot(scale);
		break;
	case 2:
		scale = 3.0;
		egg();
		break;
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//deklaracja bufferow
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], object[0], object[1], object[2], 0.0, 1.0, 0.0);//ustawienie pozycji punktu widzenia i pozycji punktu obserwowanego

	switch (task) {//wywolanie odpowiedniego zadania
	case 1:
		zadanie1();
		break;
	case 2:
		zadanie2();
		break;
	}
	glutSwapBuffers();//zmiana bufferow i wyswietlanie
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){//sprawdzenie czy przycisniety zostal prawy klawisz
		y_pos_old = y;        			 
		statusRight = 1;        //ustawienie flagi przycisku   
	}
	else {
		statusRight = 0;		//ustawienie flagi przycisku  
	}

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){//sprawdzenie czy przycisniety zostal lewy klawisz
		x_pos_old = x;        
		y_pos_old = y;  					
		statusLeft = 1;    //ustawienie flagi przycisku        
	}
	else {
		statusLeft = 0;		//ustawienie flagi przycisku  
	}
	RenderScene();
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;//zmiana pozycji x
	x_pos_old = x;

	delta_y = y - y_pos_old;//zmiana pozycji y
	y_pos_old = y;

	glutPostRedisplay();
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'j') {//wyswietlanie jajka
		testedObject = 2;
	}
	if (key == 'c') {//wyswietlenie czajnika
		testedObject = 1;
	}
	if (key == '1') {//uruchomienie 1 zadania 
		if (task == 1) {
			return;
		}
		rViewer = 10;//reset zmiennych z 2 zadania
		viewer[0] = 0.0;
		viewer[1] = 0.0;
		viewer[2] = 10.0;
		object[0] = 0.0;
		object[1] = 0.0;
		object[2] = 0.0;
		task = 1;
	}
	if (key == '2') {//uruchomienie 2 zadania 
		if (task == 2) {
			return;
		}
		rViewer = 10;//reset zmiennych z 1 zadania
		azymuth = M_PI / 4;
		elevation = M_PI / 4;
		scale = 3.0;
		task = 2;
	}
	if (key == 'r') {//reset ustawienia zooma w 2 zadaniu 
		rViewer = 10;
	}

	RenderScene();
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;//usatwienie wielkosci do obracania obiektu
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 100000.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void)
{
	srand(time(NULL));
	color = new point3 * [n + 1];//tablica kolorow
	for (int i = 0; i <= n; i++) {
		color[i] = new point3[n + 1];
	}
	for (int i = 0; i <= n; i++) {//losowanie kolorow
		for (int ii = 0; ii <= n; ii++) {
			color[i][ii][0] = (rand() % 101) * 0.01;
			color[i][ii][1] = (rand() % 101) * 0.01;
			color[i][ii][2] = (rand() % 101) * 0.01;
		}
	}

	points = new  point3 * [n + 1];//tablica punktow
	for (int i = 0; i <= n; i++) {
		points[i] = new point3[n + 1]; 
	}

	cout << "zadanie 1: kliknij 1\nzadanie 2: kliknij 2\ntestuj czajnik: kliknij c\ntestuj jajko: kliknij j\njesli podczas zoomowania w 2 zadaniu utknales: kliknij r, aby scentrowac na obiekcie" << endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Rzutowanie perspektywiczne");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMouseFunc(Mouse);//"lapanie" akcji na przyciskach myszy
 	glutMotionFunc(Motion);//"lapanie" ruchu myszki
	glutKeyboardFunc(keys);//"lapanie" akcji na klawiaturze
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

