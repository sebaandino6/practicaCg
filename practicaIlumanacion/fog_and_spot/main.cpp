#include <cmath>
#include <iostream>
#include <algorithm>
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
#include "fas_settings.h"
#include "OSD.hpp"

//------------------------------------------------------------
// variables globales y defaults

GLfloat light_position[] = { 0.0, 3.0, 0.0, 1.0 };
GLfloat light_direction[] = { 0.0, -1.0, 0.0};
GLfloat lig_diffuse [] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat lig_ambient [] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat lig_specular [] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat color[3]={120./255,50./155,30./255};

GLshort modifier;

GLint w=800,h=600;

bool animar = true,
		 help = false;

double t=0;

void update(void) {
	float theta = std::cos(3*t); // angulo de rotación (de la ecuación del pendulo)
	float xr = std::sin(theta), yr = std::cos(theta); // x e y relativos al punto de agarre del péndulo
	light_position[0] = xr;
	light_position[1] = 3.0-yr; // el punto de agarre del péndulo es el [0,4,0].
	light_direction[0] = xr;
	light_direction[1] = -yr;
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
}


void help_text() {
	OSD << "Generales:\n";
	OSD << "  h/H: mostrar/ocultar esta ayuda\n";
	OSD << "  p/P: pausar/reanudar la animacion\n";
	OSD << "  drag con boton izquierdo: mover spot (eje z)\n";
	OSD << "\n";
	OSD << "Teclas para configurar la NIEBLA:\n";
	OSD << "  f/F: activar/desactivar\n";
	OSD << "  m/M: cambiar modo (linear, exp, exp^2)\n";
	OSD << "  d/D: disminuir/aumentar densidad\n";
	OSD << "\n";
	OSD << "Teclas para configurar el SPOT:\n";
	OSD << "  s/S: alternar entre spot y omnidireccional\n";
	OSD << "  a/A: disminuir/aumentar el angulo de cutoff\n";
	OSD << "  e/E: disminuir/aumentar exponente\n";
	OSD << "  c/C: disminuir/aumentar el coef. de atenuacion constante\n";
	OSD << "  l/L: disminuir/aumentar el coef. de atenuacion lineal\n";
	OSD << "  q/Q: disminuir/aumentar el coef. de atenuacion cuadratica\n";
}

void status_text() {
	glColor3f(1,1,1);
	
	if (settings.fog.enable) {
		switch(settings.fog.mode) {
		case FogMode::Linear:
			OSD << "FOG:    ( " << settings.fog.end << " - z )\n";
			OSD << "     ---------------------\n";
			OSD << "      ( " << settings.fog.end << " - " << settings.fog.start << " )";
			break;
		case FogMode::Exp:
			OSD << "FOG:  exp( - " << settings.fog.density<< " * z ) ";
			break;
		case FogMode::Exp2:
			OSD << "FOG:  exp( - (" << settings.fog.density<< " * z)^2 ) ";
			break;
		}
	} else {
		OSD << "FOG:  <desactivado>";
	}
	
	OSD << "\n\n";
	
	
	if (settings.spot.enable) {
		OSD << "SPOT:      ( I*cos(gamma)^"<<settings.spot.exponent<<" )\n";
		OSD << "      ---------------------------------\n";
		OSD << "       ( " << settings.spot.constant_attenuation;
		OSD << " + " << settings.spot.linear_attenuation << "*d";
		OSD << " + " << settings.spot.quadratic_attenuation<< "*d^2 )";
		OSD << "\n\n        cutoff: " << settings.spot.cutoff;
	} else {
		OSD << "SPOT:  <desactivado>";
	}
	
	OSD << "\n\n(presione H para ver la ayuda)";
	
}


///////////////////////////////////////////////////////////////////////////////
void display_cb(void) {
	
	glEnable(GL_LIGHTING);
	settings.Apply(GL_LIGHT0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// array de teteras
	glColor3fv(color);
	for(int i=-5;i<=5;i++){
		for(int j=-2;j<50;j++){
			glPushMatrix();
			glTranslatef(i*3.0,j*0.0,-j*2.0);
			glutSolidTeapot(1);
			glPopMatrix();
		}
	}
	// punto luz
	glDisable(GL_LIGHTING);
	glPointSize(10); glColor3f(1,1,1);
	glBegin(GL_POINTS);
	glVertex3fv(light_position);
	glEnd();
	glBegin(GL_LINES);
	glVertex3fv(light_position);
	glVertex3f(light_position[0] - light_direction[0],
			   light_position[1] - light_direction[1],
			   light_position[2] - light_direction[2]);
	glEnd();
	
	if (help) help_text();
	else status_text();
	OSD.Render(w,h,true);
	
	glutSwapBuffers();
}


//------------------------------------------------------------
// Animacion
void idle_cb() {
	// Cuenta el lapso de tiempo
	static int anterior=glutGet(GLUT_ELAPSED_TIME);
	int tiempo = glutGet(GLUT_ELAPSED_TIME), lapso=tiempo-anterior;
	if (lapso < 16) return;
	anterior=tiempo;
	if(animar) t += float(lapso) / 1000.0;
	update();
	glutPostRedisplay(); // redibuja
}

// Maneja cambios de ancho y alto de la ventana
void reshape_cb(int w0, int h0) {
	w=w0,h=h0;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)w / (GLfloat)h,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 4, 8, 0,0,0, 0,1,0);
	glutPostRedisplay();
}

//------------------------------------------------------------
// mouse
void Motion_cb(int x, int y) {
	light_position[2] = (float)(y - h/2.0)/50;
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutPostRedisplay();
}

void mouse_cb(int button, int state, int x, int y) {
	if (button==GLUT_LEFT_BUTTON){
		if (state==GLUT_DOWN) {
			modifier = (short) glutGetModifiers();
			glutMotionFunc(Motion_cb); // callback para los drags
		}
		else glutMotionFunc(0); // anula el callback para los drags    
	}
}

//------------------------------------------------------------
void keyboard_cb(unsigned char key,int x=0,int y=0) {
	switch (key){
	case 'm': case 'M':
		settings.fog.enable = true;
		switch(settings.fog.mode) {
		case FogMode::Linear: settings.fog.mode = FogMode::Exp;    break;
		case FogMode::Exp:    settings.fog.mode = FogMode::Exp2;   break;
		case FogMode::Exp2:   settings.fog.mode = FogMode::Linear; break;
		}
		break;
	case 'e':
		settings.spot.exponent += 0.2f;
		break;
	case 'E':
		settings.spot.exponent = std::max(0.f, settings.spot.exponent-0.2f);
		break;
	case 'a':
		settings.spot.cutoff = std::min(settings.spot.cutoff+1.f,90.f);
		break;
	case 'A':
		settings.spot.cutoff = std::max(settings.spot.cutoff-1.f,0.f);
		break;
	case 'c':
		settings.spot.constant_attenuation /= 0.81f;
		break;
	case 'C':
		settings.spot.constant_attenuation *= 0.81f;
		break;
	case 'q':
		settings.spot.quadratic_attenuation += 0.02f;
		break;
	case 'Q':
		settings.spot.quadratic_attenuation = std::max(0.f, settings.spot.quadratic_attenuation-0.02f);
		break;
	case 'l':
		settings.spot.linear_attenuation += 0.02f;
		break;
	case 'L':
		settings.spot.linear_attenuation = std::max(0.f, settings.spot.linear_attenuation-0.02f);
		break;
	case 'f': case 'F':
		settings.fog.enable = !settings.fog.enable;
		break;
	case 'd':
		settings.fog.density+=0.01f; settings.fog.end*=0.9f;
		break;
	case 'D':
		if(settings.fog.density>=0.01f) {
			settings.fog.density-=0.01f; settings.fog.end/=0.9f;
		}
		break;
	case 's': case 'S':
		settings.spot.enable = !settings.spot.enable;
		break;
	case 'p': case 'P':
		animar = !animar;
		break;
	case 'h': case 'H':
		help = !help;
		break;
	}
	glutPostRedisplay();
}

	
//------------------------------------------------------------
// Inicializa GLUT y OpenGL
void inicializa(void) {
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowSize (w,h);
	glutInitWindowPosition (100,50);
	glutCreateWindow ("NIEBLA");
	
	glutReshapeFunc(reshape_cb);
	glutDisplayFunc(display_cb);
	glutMouseFunc(mouse_cb);
	glutKeyboardFunc(keyboard_cb);
	glutIdleFunc(idle_cb);
	
	glEnable(GL_LIGHTING);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	glMaterialf(GL_FRONT, GL_SHININESS,127);
	glColorMaterial(GL_FRONT_FACE,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lig_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lig_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lig_specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	glEnable(GL_LIGHT0);
	
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	glHint (GL_FOG_HINT, GL_NICEST);
	
	settings.Apply(GL_LIGHT0);
}

//------------------------------------------------------------
// main
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	inicializa();
	glutMainLoop();
	return 0;
}
