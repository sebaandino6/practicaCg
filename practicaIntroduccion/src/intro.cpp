// Programa introductorio
// triángulo con puntos editables

#include <iostream> // cout
#include <cstdlib> // exit (si va, va necesariamente antes del include de glut)
#include <cmath> // fabs
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif

using namespace std;

//------------------------------------------------------------
// variables globales
static const int max_puntos=3;
int
  w=640,h=480, // tamaño inicial de la ventana
  npuntos=0, // cantidad de puntos
  pt[2*max_puntos], // los puntos (hasta 3: x0,y0,x1,y1,x2,y2)
  ep=-1; // índice del punto a editar

//============================================================
// callbacks

//------------------------------------------------------------

// arma un un nuevo buffer (back) y reemplaza el fraontbuffer
void Display_cb() {
  //static int counter=0; cout << "display: " << counter++ << endl;

  // arma el back-buffer, rellena con color de fondo
  glClear(GL_COLOR_BUFFER_BIT);

  // dibuja en background (en el buffer de atrás o backbuffer)
  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); glLineWidth(3);
  if (npuntos==3) { // triángulo (sólo si ya están los tres puntos)
    {
      glColor3f(.6f,.5f,.1f); // color del triangulo
      glBegin(GL_TRIANGLES);
      for(int i=0;i<npuntos;i++) 
        glVertex2i(pt[2*i],pt[2*i+1]);
      glEnd();
    }
  }
  /*else if (npuntos==4) { // cuadrilátero si están los cuatro puntos
    glBegin(GL_QUADS);
    .........
    glEnd();
  }*/
  // puntos (después de la figura, para que se vean encima)
  glColor3f(.1f,.2f,.3f); // color de puntos
  glPointSize(5.0); // punto grueso
  glBegin(GL_POINTS);
  for(int i=0;i<npuntos;i++) 
    glVertex2i(pt[2*i],pt[2*i+1]);
  glEnd();

  glutSwapBuffers(); // lo manda al monitor (al frontbuffer)
}

//------------------------------------------------------------
// Maneja cambios de ancho y alto de la ventana
void Reshape_cb(int width, int height){
  w=width; h=height;
  glViewport(0,0,w,h); // región donde se dibuja (toda la ventana)
  // rehace la matriz de proyección (la porcion de espacio visible)
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  glOrtho(0,w,0,h,-1,1); // unidades = pixeles
  glutPostRedisplay(); // avisa que se debe redibujar
}

//------------------------------------------------------------
// Mouse

// Seguimiento pasivo del cursor
void PassiveMotion_cb(int x, int y){
  y=h-y; // el 0 está arriba
  cout << "x,y: " << x << "," << y << "                    \r" << flush;
}

// Drag (movimiento con algun boton apretado)
void Motion_cb(int x, int y){
  y=h-y; // el 0 está arriba
  cout << "x,y: " << x << "," << y << "                    \r" << flush;
  pt[2*ep]=x;pt[2*ep+1]=y; // fija el punto editado en x,y
  glutPostRedisplay(); // avisa que se debe redibujar
}

// Botones picados o soltados
void Mouse_cb(int button, int state, int x, int y){
  y=h-y; // el 0 está arriba
  cout << "\nx,y: " << x << "," << y << " boton: " << button << " estado: " << state << endl;
  if (button==GLUT_LEFT_BUTTON){ // boton izquierdo
    if (state==GLUT_DOWN) { // clickeado
      ep=-1;
      // verifica si picó a menos de 5 pixeles de algún punto previo
      for (int i=0;i<npuntos;i++){
        int d=abs(x-pt[2*i])+abs(y-pt[2*i+1]); // distancia simple (métrica del rombo)
        if (d>10) continue; // lejos
        ep=i; // edita el punto i
        glutMotionFunc(Motion_cb); // define el callback para los drags
        return;
      }
      // no pico cerca de otro
      if (npuntos>=max_puntos) return; // ya estan todos ==> no hace nada
      // agrega un punto
      pt[2*npuntos]=x; pt[2*npuntos+1]=y; npuntos++;
      glutPostRedisplay(); // Redibuja
      // y queda listo para editarlo hasta que suelte el botón izquierdo
      ep=npuntos-1; // edita el ultimo punto
      glutMotionFunc(Motion_cb); // define el callback para los drags
    } // fin clickeado
    else if (state==GLUT_UP) // soltado
      glutMotionFunc(0); // anula el callback para los drags
  } // fin botón izquierdo
}

//------------------------------------------------------------
// Teclado

// Maneja pulsaciones del teclado (ASCII keys)
// x,y posicion del mouse cuando se teclea (aqui no importan)
void Keyboard_cb(unsigned char key,int x,int y) {
  if (key==127||key==8){ // DEL o Backspace ==> borra el punto ep (el editable)
    if (ep==-1||ep==npuntos) return;
    // corre los siguientes hacia atras
    for(int i=ep;i<npuntos-1;i++) {pt[2*i]=pt[2*i+2];pt[2*i+1]=pt[2*i+3];}
    npuntos--;
    glutPostRedisplay(); // avisa que se debe redibujar
    // Backspace borra para atras, DEL borra para adelante
    if (key==8) ep--; // ep pasa a ser el anterior
  }
}

// Special keys (non-ASCII)
// teclas de funcion, flechas, page up/dn, home/end, insert
void Special_cb(int key,int xm=0,int ym=0) {
  if (key==GLUT_KEY_F4 && glutGetModifiers()==GLUT_ACTIVE_ALT) // alt+f4 => exit
    exit(EXIT_SUCCESS);
}

//------------------------------------------------------------
// Inicialización

void inicializa() {
  // GLUT
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);// pide color RGB y double buffering
  glutInitWindowSize(w,h); glutInitWindowPosition(100,50);
  glutCreateWindow("introducción"); // crea el main window

  //declara los callbacks, los que (aun) no se usan (aun) no se declaran
  glutDisplayFunc(Display_cb);
  glutReshapeFunc(Reshape_cb);
  glutKeyboardFunc(Keyboard_cb);
  glutSpecialFunc(Special_cb);
  glutMouseFunc(Mouse_cb);
  glutPassiveMotionFunc(PassiveMotion_cb);

  // OpenGL
  glClearColor(0.85f,0.9f,0.95f,1.f); // color de fondo
  glMatrixMode(GL_MODELVIEW); glLoadIdentity(); // constante
}

//------------------------------------------------------------
// main
int main(int argc,char** argv) {
  glutInit(&argc,argv); // inicialización interna de GLUT
  inicializa(); // define el estado inicial de GLUT y OpenGL
  glutMainLoop(); // entra en loop de reconocimiento de eventos
  return 0; // nunca se llega acá, es sólo para evitar un warning
}
