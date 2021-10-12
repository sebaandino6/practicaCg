#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include "drawObjects.h"

using namespace std;

//------------------------------------------------------------
// variables globales y defaults

int
	w=800,h=600, // tamaño de la ventana
	xm0, ym0,
	modifierKey;
float
	tiempo=0,
	eye[4], // camara
	lpos[4], // posicion luz
	shadowMatrix[4][4]; // matriz que proyecta los objetos sobre el piso (para la sombra)
double
	distancia = 5, distancia0,
	latitud = M_PI * .5, latitud0,
	longitud = M_PI * .25, longitud0,
	fovy=50; // field of view
bool showRGBStencil=false;

void updateShadowMatrix(){
	shadowMatrix[0][0] = lpos[1];
	shadowMatrix[1][0] = -lpos[0];
	shadowMatrix[2][0] = 0.f;
	shadowMatrix[3][0] = 0.f;
	
	shadowMatrix[0][1] = 0.f;
	shadowMatrix[1][1] = 0.f;
	shadowMatrix[2][1] = 0.f;
	shadowMatrix[3][1] = 0.f;
	
	shadowMatrix[0][2] = 0.f;
	shadowMatrix[1][2] = -lpos[2];
	shadowMatrix[2][2] = lpos[1];
	shadowMatrix[3][2] = 0.f;
	
	shadowMatrix[0][3] = 0.f;
	shadowMatrix[1][3] = -lpos[3];
	shadowMatrix[2][3] = 0.f;
	shadowMatrix[3][3] = lpos[1];
}

void buildStencil(){
	// actualizamos la matriz de proyección de la sombra (la luz se mueve)
	updateShadowMatrix(); 
	
	glPushAttrib(GL_ALL_ATTRIB_BITS); // guardamos los atributos actuales
	glColorMask(false,false,false,false); // no dibuja en el color buffer
	glDisable(GL_DEPTH_TEST); // no actualiza el z buffer
	glDisable(GL_LIGHTING); // inhabilitamos el modelo de iluminacion
	
//  @@@ Dibujar en el stencil los objetos necesarios.
//	Para proyectar la tetera en el piso usar la matriz shadowMatrix
	
	/** i think here draw in the stencil buffer **/
	// what the values of glStencilOp??glStencilOp(GL_KEEP,GL_KEEP, GL_REPLACE)??
	// what the values of glStencilFunc??glStencilFunc(GL_EQUAL, 1, 0xFF)?
	//how to draw the projected object in the stencil buffer with no change color buffer????
	// i think we have to multiply shadowMatrix and then draw the teapot.
	glEnable(GL_STENCIL_TEST);    
	glStencilMask(0xFF); 
	glStencilOp(GL_KEEP,GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	
	glPushMatrix();
		glMultMatrixf((GLfloat *)shadowMatrix);
		drawTeapots(tiempo);
	glPopMatrix();
	/** i think here draw in the stencil buffer **/
	
	glPopAttrib();
}


void showStencil(unsigned char val, float r, float g, float b) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glStencilFunc(GL_EQUAL, val, ~0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor4f(r, g, b, 0.3);
	glBegin(GL_QUADS);
	glVertex3f(-1,-1, -1);
	glVertex3f( 1,-1, -1);
	glVertex3f( 1, 1, -1);
	glVertex3f(-1, 1, -1);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
//------------------------------------------------------------
// redibuja los objetos
// Cada vez que hace un redisplay
void Display_cb() {
	lpos[0] = 2*sin(tiempo/1000);
	lpos[1] = -2;
	lpos[2] = 2*cos(tiempo/1000);
	glLightfv(GL_LIGHT1,GL_POSITION,lpos);  // ubica la luz reflejada
	lpos[1] = -lpos[1];
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);  // ubica la luz real
	
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	
	buildStencil(); 
	
	//	@@@: Utilizar el stencil para que los reflejos no se vean fuera del piso
	// to do that i use that guide 
	// -> http://nehe.gamedev.net/tutorial/clipping__reflections_using_the_stencil_buffer/17004/
	double clip_plane[]= {
		0.f,-1.f,0.f,0.f
	};
	glColorMask(0,0,0,0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST); // La sombra se genera dibujando el piso con al iluminación deshabilitada
	drawFloor();
	glPopAttrib();
	
	glEnable(GL_DEPTH_TEST);
	glColorMask(1,1,1,1);
	glStencilFunc(GL_EQUAL,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, clip_plane);
	
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glScalef(1.f,-1.f,1.f);
	
	drawTeapots(tiempo);
	drawLight(lpos);
	glPopMatrix();
	glPopAttrib();
	
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_STENCIL_TEST);
	
//  @@@ Utilizar el stencil para que el piso iluminado se dibuje solo donde
//  no hay sombra (de otra forma se superpone piso iluminado+sombra)
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//here is where draw the scene in normal form
	drawFloor();
	glPopAttrib();
	drawTeapots(tiempo);
	drawLight(lpos);
	
//  @@@ Utilizar el stencil para dibujar la sombra
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING); // La sombra se genera dibujando el piso con al iluminación deshabilitada
	drawFloor();
	glPopAttrib();
	
	if(showRGBStencil) {
		showStencil(0, 1,0,0);
		showStencil(1, 0,1,0);
		showStencil(2, 0,0,1);
	}
	
	glutSwapBuffers();
	
	// chequea errores
	int errornum=glGetError();
	while(errornum!=GL_NO_ERROR){
		if(errornum==GL_INVALID_ENUM)
			cout << "GL_INVALID_ENUM" << endl;
		else if(errornum==GL_INVALID_VALUE)
			cout << "GL_INVALID_VALUE" << endl;
		else if (errornum==GL_INVALID_OPERATION)
			cout << "GL_INVALID_OPERATION" << endl;
		else if (errornum==GL_STACK_OVERFLOW)
			cout << "GL_STACK_OVERFLOW" << endl;
		else if (errornum==GL_STACK_UNDERFLOW)
			cout << "GL_STACK_UNDERFLOW" << endl;
		else if (errornum==GL_OUT_OF_MEMORY)
			cout << "GL_OUT_OF_MEMORY" << endl;
		errornum=glGetError();
	}
}

//------------------------------------------------------------
// Regenera la matriz de proyeccion
// cuando cambia algun parametro de la vista
void regen() {
	// matriz de proyeccion
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	
//  @@@ Resolver problemas de Z-fighting (ver variable "distancia")
	double znear= 0.0001,
		   zfar = 3000;
	
	gluPerspective(fovy, float(w) / float(h), znear, zfar);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); // matriz del modelo->view
	
	eye[0] = distancia * cos(latitud) * sin(longitud); 
	eye[1] = distancia * cos(longitud);
	eye[2] = distancia * sin(latitud) * sin(longitud); 
	gluLookAt(eye[0],   eye[1],   eye[2],
			  0, 0, 0,
			  0, 1, 0);// ubica la camara
	
	glutPostRedisplay();
}


void Idle_cb() {
	static float anterior=glutGet(GLUT_ELAPSED_TIME); // milisegundos desde que arranco
	tiempo = glutGet(GLUT_ELAPSED_TIME);
	if (tiempo-anterior > 16) {
		anterior=tiempo;
		glutPostRedisplay(); // redibuja
	}
}

//------------------------------------------------------------
// Maneja cambios de ancho y alto de la ventana
void Reshape_cb(int width, int height){
	h=height; w=width;
	glViewport(0,0,w,h); // region donde se dibuja
	regen(); //regenera la matriz de proyeccion
}

//------------------------------------------------------------
// Teclado
// Maneja pulsaciones del teclado (ASCII keys)
// x,y posicion del mouse cuando se teclea
void Keyboard_cb(unsigned char key,int x=0,int y=0) {
	switch (key){
	case 27: // escape => exit
		if (!glutGetModifiers())
			exit(EXIT_SUCCESS);
		break;
	case 's':
		showRGBStencil = !showRGBStencil;
	}
	glutPostRedisplay();
}

//------------------------------------------------------------
// Mouse
// Movimientos del mouse
void Motion_cb(int xm, int ym){ // drag
		if (modifierKey == GLUT_ACTIVE_CTRL) {
			distancia = distancia0 * exp(double(ym0-ym)/100.0);
			distancia = max(distancia, 0.1);
		}
		else {
			longitud= longitud0 + 2 * M_PI * double(ym0-ym)/h;
			longitud = max(0.01, longitud);
			longitud = min(M_PI * .49, longitud);
			latitud = latitud0  - M_PI * double(xm0-xm)/w;
		}
		regen();
}

// Movimiento pasivo del mouse
void PMotion_cb(int xm, int ym){
	unsigned char s;
	glReadPixels(xm,h-ym,1,1,GL_STENCIL_INDEX,GL_UNSIGNED_BYTE,&s);
	cout <<"\rStencil buffer: "<<int(s)<<"        "; cout.flush();
}

// Clicks del mouse
void Mouse_cb(int button, int state, int x, int y){
	if (button==GLUT_LEFT_BUTTON){
		if (state==GLUT_DOWN) {
			modifierKey = glutGetModifiers();
			xm0=x; ym0=y;
			distancia0 = distancia;
			latitud0 = latitud;
			longitud0 = longitud;
			glutMotionFunc(Motion_cb); // callback para los drags
		}
		else if (state==GLUT_UP) {
			glutMotionFunc(0); // anula el callback para los drags
		}
	}
}
//------------------------------------------------------------
// pregunta a OpenGL por el valor de una variable de estado
int integerv(GLenum pname){
	int value;
	glGetIntegerv(pname,&value);
	return value;
}
#define _PRINT_INT_VALUE(pname) #pname << ": " << integerv(pname) <<endl

//------------------------------------------------------------
// Inicializa GLUT y OpenGL
void initialize() {
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE|GLUT_STENCIL);
	glutInitWindowSize(w,h); glutInitWindowPosition(50,50);
	glutCreateWindow("Sombras planas y reflejos"); // crea el main window
	
	glutDisplayFunc(Display_cb);
	glutReshapeFunc(Reshape_cb);
	glutKeyboardFunc(Keyboard_cb);
	glutPassiveMotionFunc(PMotion_cb);
	glutMouseFunc(Mouse_cb);
	glutIdleFunc(Idle_cb);
	
	// ========================
	// estado normal del OpenGL
	// ========================
	
	glDisable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); // habilita el z-buffer
	glShadeModel(GL_SMOOTH); // interpola normales por nodos o una normal por plano
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.,1.);
	
	glEnable(GL_LINE_SMOOTH);
	
	glCullFace(GL_BACK);
	
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lDiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lSpecular);
	glLightfv(GL_LIGHT1,GL_AMBIENT,lAmbient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,lDiffuse);
	glLightfv(GL_LIGHT1,GL_SPECULAR,lSpecular);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);
	lpos[3] = 1;
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	// color de fondo
	glClearColor(.1, .1, .1, 1);
	
	// para el stencil
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0); // al borrar el stencil lo llena de ceros
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP); // normalmente no escribe en el stencil
}

//------------------------------------------------------------
// main
int main(int argc,char** argv) {
	cout<<"drag\t\t -> mueve la camara alrededor de las teteras"<<endl;
	cout<<"ctrl+drag\t -> acerca o aleja la camara"<<endl;
	cout<<"s\t -> muestra el contenido del stencil buffer"<<endl;
	glutInit(&argc,argv);// inicializa glut
	initialize(); // condiciones iniciales de la ventana y OpenGL
	glutMainLoop(); // entra en loop de reconocimiento de eventos
	return 0;
}
