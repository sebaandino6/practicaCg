#include <cmath>
#include <iostream>
#include <cctype>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif
#include "Curva.h"
#include "uglyfont.h"
using namespace std;

Spline curva;
int win_w=600,win_h=400, sel=-1;

// dibuja un caracter en un cuadrado de 1x1 con su vertice inferior iquierdo en 0,0
#define G2R(rad) ((rad)*180/M_PI)

char texto[256];
int texto_len=strlen(texto);
bool ver_curva=true, ver_texto=true, edit_mode=false;

void dibujar_caracter(const char chr);

void reshape_cb (int w, int h) {
	win_w=w; win_h=h;
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void DibujarTexto() {
	if(curva.CantPuntos() >1 ){
		glColor3f(0,.5,0); glLineWidth(2);
		///El tamaño de las letras se debería ajustar de acuerdo a la longitud de la curva y cantidad de letras
		float proporcion = curva.Longitud()/(texto_len*100);
		float t, ang, origenx, origeny;
		punto punto1,derivadaPunto;
		for (int i=0;i<texto_len;i++) {
			/// @@@ utilizar la curva para determinar la posición, tamaño e inclinación
			///     de cada letra, y aplicar con eso la transformacion que corresponda
			// ejemplo de uso de la curva:
			//   - obtener un punto para un t dado (t entre 0 y 1)
			//         punto p = curva.Evaluar(t);
			//   - obtener un punto para un t dado y además la derivada en ese punto
			//         punto p, d; curva.Evaluar(t,p,d);
			//
			glPushMatrix();
			t = (float)i / texto_len;	/// t para obtener el punto
			///obtengo el primer punto
			if(i == texto_len-1){
				punto1 = curva.Evaluar( 1 );
			}
			punto1 = curva.Evaluar(t);	
			///obtengo la derivada
			curva.Evaluar(t,punto1,derivadaPunto);
				std::cout<<"punto "<<i<<std::endl;
				std::cout<<"derivada x: "<<derivadaPunto[0]<<std::endl;
				std::cout<<"derivada y: "<<derivadaPunto[1]<<std::endl;
			///obtengo el angulo para rotar la letra
			if( derivadaPunto[0] < 0.1 and derivadaPunto[1] > 0){
				ang = 90;
			}else if(derivadaPunto[0] < 0.1 and derivadaPunto[1] <= 0){
				ang = -90;
			}else{
				ang = atan(derivadaPunto[1]/derivadaPunto[0]);
			}
			///defino el nuevo origen para la letra
			if(derivadaPunto[1] <= 0){
				origeny = punto1[1] + abs(derivadaPunto[1])*proporcion;
				if( derivadaPunto[0] <= 0){
					origenx = punto1[0] + proporcion*50.f;
				}else{
					origenx = punto1[0];
				}
			}else if(derivadaPunto[1] > 0){
				origeny = punto1[1];
				origenx = punto1[0] - proporcion*50.f;
			}
			else{
				origeny = punto1[1];
				origenx = punto1[0];
			}
			if( i == texto_len-1){
				origeny = punto1[1];
				origenx = punto1[0];
			}
			///proporciona la letra
			float m[] = {
				proporcion, 0, 0, 0.00,
				0, proporcion, 0, 0.00,
				0, 0, 1, 0.00,
				origenx, origeny, 0, 1.00
			};
			///rota la letra
			float m2[] = {
				cos(-ang), -sin(-ang), 0, 0.00,
				sin(-ang), cos(-ang), 0, 0.00,
				0, 0, 1, 0.00,
				1, 1, 0, 1.00
			};
			glMultMatrixf(m);
			glMultMatrixf(m2);
			// dibujar_caracter dibuja una letra de 100x100
			dibujar_caracter(texto[i]);
			glPopMatrix();
		}
	}
}

void display_cb() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (edit_mode) {
		glColor3f(1,0,0);
		glLineWidth(1);
		glPushMatrix();
		glTranslatef(10,10,0);
		glScalef(10,10,10);
		YsDrawUglyFont("edit mode",0,0);
		glPopMatrix();
	}
	
	if (curva.CantPuntos()) {
		if (ver_curva) curva.Dibujar(10);
		if (ver_texto && texto_len) DibujarTexto();
	}
	
	glutSwapBuffers();
}

void motion_cb(int x, int y) {
	y=win_h-y;
	curva.Mover(sel,punto(x,y));
	glutPostRedisplay();
}

void mouse_cb(int button, int state, int x, int y) {
	y=win_h-y;
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			sel=curva.Cerca(punto(x,y),100);
			if (sel==-1) {
				sel=curva.Agregar(punto(x,y));
				if (sel==-1) return;
				glutPostRedisplay();
			} 
			glutMotionFunc(motion_cb);
		} else 
			glutMotionFunc(NULL);
	}
}

void keyboard_cb(unsigned char key, int x, int y) {
	if (edit_mode) {
		if (key=='\b') {
			if (texto_len) texto[--texto_len]='\0';
		} else if (key<32) edit_mode=false;
		else if (texto_len<255) {
			texto[texto_len++]=key;
			texto[texto_len]='\0';
		}
	} else {
		key=tolower(key);
		if (key=='e') edit_mode=true;
		else if (key=='t') ver_texto=!ver_texto;
		else if (key=='c') ver_curva=!ver_curva;
	}
	glutPostRedisplay();
}

void menu_cb(int key) {
	if (key=='e') edit_mode=!edit_mode;
	else if (key=='t') ver_texto=!ver_texto;
	else if (key=='c') ver_curva=!ver_curva;
	glutPostRedisplay();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (win_w,win_h);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glutMouseFunc(mouse_cb);
	glutKeyboardFunc(keyboard_cb);
	glutCreateMenu(menu_cb);
		glutAddMenuEntry("[e] Editar Texto", 'e');
		glutAddMenuEntry("[t] Mostrar/Ocultar Texto", 't');
		glutAddMenuEntry("[c] Mostrar/Ocultar Curva", 'c');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glClearColor(1.f,1.f,1.f,1.f);
}

int main (int argc, char **argv) {
	glutInit (&argc, argv);
	initialize();
	strcpy(texto,"Computacion Grafica");
	texto_len=strlen(texto);
	glutMainLoop();
	return 0;
}
