#include <cmath> // exp
#include <cstdlib> // exit
#include <cstring> // memmove
#include <iostream> // cout
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
/*
// comentar este bloque si se quiere una ventana de comando
#ifdef _WIN32
 #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
*/

using namespace std;

//------------------------------------------------------------
// variables globales y defaults
static const int max_cantidad=100;
int posicion_cono[max_cantidad][2],cantidad=0;
float color_cono[max_cantidad][3];
int cono_editado=-1; // si -1 => no se esta desplazando ningun cono
float radio=1000, radio0=1000; // radio de los conos
int xclick,yclick; // x e y cuando clickeo un boton
float lpos[]={100,100,100,1}; // posicion luz, l[4]: 0 => direccional -- 1 => posicional

bool zbuffer=true; // habilita el z-buffer
bool luz=true;     // habilita la iluminacion

short modifiers=0;  // ctrl, alt, shift al picar
//------------------------------------------------------------
// redibuja los objetos
void Display_cb() { // Este tiene que estar
  // arma el backbuffer
  // inicializa los buffers de color y z con color de fondo y zfar
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT); 
  
  // dibuja los conos
  
  static GLUquadricObj *q=gluNewQuadric();
  gluQuadricNormals(q,GLU_SMOOTH);//GLU_FLAT
  gluQuadricOrientation(q,GLU_OUTSIDE);
  //GLU FILL, GLU LINE, GLU POINT or GLU SILHOUETTE
  gluQuadricDrawStyle(q,GLU_FILL);

  for (int j=0;j<cantidad;j++){
    int x=posicion_cono[j][0], y=posicion_cono[j][1];
    float r=color_cono[j][0], g=color_cono[j][1], b=color_cono[j][2];
    // vértice en color opuesto
    glColor3f(1-r,1-g,1-b); glPointSize(10);
    glBegin(GL_POINTS); glVertex3i(x,y,1); glEnd();
    //cono
    static const float altura=1000;
    glPushMatrix(); glTranslated(x,y,-altura);
    glColor3f(r,g,b);
    gluCylinder(q,radio,0,altura,128,128);
    glPopMatrix();
  }
  
  glPopAttrib();
  
  glutSwapBuffers(); // pasa al front y al monitor
}

//------------------------------------------------------------
// Maneja cambios de ancho y alto del programa
void Reshape_cb(int w, int h){
  glViewport(0,0,w,h); // region donde se dibuja
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glOrtho(0,w,h,0,1,1003); // rango visible
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  gluLookAt(0,0,2,0,0,0,0,1,0); // espacio del modelo
  glLightfv(GL_LIGHT0,GL_POSITION,lpos);
  glutPostRedisplay();
}

//------------------------------------------------------------
// Teclado y Mouse
/*
modificadores que devuelve glutGetModifiers()
GLUT ACTIVE SHIFT //Set if the Shift modifier or Caps Lock is active.
GLUT ACTIVE CTRL //Set if the Ctrl modifier is active.
GLUT ACTIVE ALT //Set if the Alt modifier is active.
*/

// mouse
// GLUT LEFT BUTTON, GLUT MIDDLE BUTTON, or GLUT RIGHT BUTTON
// The state parameter is either GLUT UP or GLUT DOWN
// glutGetModifiers may be called to determine the state of modifier keys

void Motion_cb(int xm, int ym){ // drag
  if (!modifiers){ // traslada un cono
    posicion_cono[cono_editado][0]=xm;
    posicion_cono[cono_editado][1]=ym;
  }
  else if (modifiers==GLUT_ACTIVE_SHIFT){ // cambio de radio
    radio=radio0*exp((yclick-ym)/100.0);
  }
  else if (modifiers==GLUT_ACTIVE_CTRL){ // cambio de posicion o dirección de la luz
    if (!luz) return;
    lpos[0]=xm; lpos[1]=ym; // z queda en 100
    glLightfv(GL_LIGHT0,GL_POSITION,lpos);
  }
  else return;
  glutPostRedisplay(); // redibuja mientras trabaja
}

void Mouse_cb(int button, int state, int x, int y){ // click
  modifiers=0; 
  static int epsilon=10; // define "cerca" en pixeles
  if (button==GLUT_LEFT_BUTTON){
    if (state==GLUT_DOWN){
      xclick=x; yclick=y;
      modifiers=(short)glutGetModifiers();
      if (!modifiers){ // mueve o agrega un cono
        cono_editado=-1; // ninguno
        // busca un cono a menos de epsilon del punto picado
        int j,d=epsilon;
        for (j=0;(j<cantidad)&&(d>=epsilon);j++){
          d=abs(x-posicion_cono[j][0])+abs(y-posicion_cono[j][1]);
        }
        if (d>=epsilon){// ninguno cerca => agrega un punto
          if (cantidad==max_cantidad) return; // no caben mas
          posicion_cono[cantidad][0]=x; posicion_cono[cantidad][1]=y;
          // un color aleatorio para cada cono
          color_cono[cantidad][0]=float(rand())/RAND_MAX;
          color_cono[cantidad][1]=float(rand())/RAND_MAX;
          color_cono[cantidad][2]=float(rand())/RAND_MAX;
          cantidad++; // agrega
          cono_editado=cantidad-1; // este es el que hay que mover
        }else{ // el cono j-1 estaba cerca
          cono_editado=j-1;
        }
      }
      // si picó el izquierdo puede hacer drag
      glutMotionFunc(Motion_cb); // callback para los drags
      glutPostRedisplay();
      return;
    }//end GLUT_DOWN
    else if (state==GLUT_UP){ // soltó el botón izquierdo
      glutMotionFunc(0); // anula el callback para los drags
      radio0=radio; // ultimo radio = radio actual
    }
  }//end if GLUT_LEFT_BUTTON
}

// Maneja pulsaciones del teclado (ASCII keys)
// x,y posicion del mouse cuando se teclea
void Keyboard_cb(unsigned char key,int x=0,int y=0) {
  if (key==27){ // escape => exit
    if (!glutGetModifiers())
      exit(EXIT_SUCCESS);
  }
  else if (key==127||key==8){ // DEL o Backspace ==> borra
    if (!cantidad) return;
    int ultimo=cantidad-1;
    if(cono_editado==-1) cono_editado=ultimo;    
    if (cono_editado!=ultimo){ // cambia el borrado por el ultimo (desordena)
      //posicion
      static const size_t szp=2*sizeof(int);
      memmove(
              &posicion_cono[cono_editado][0], // destino
              &posicion_cono[ultimo][0],//fuente
              szp);//cantidad
      
      //color
      static const size_t szc=3*sizeof(float);
      memmove(
              &color_cono[cono_editado][0], // destino
              &color_cono[ultimo][0],//fuente
              szc);//cantidad
    }
    cantidad--;
    cono_editado--;
  }
  else if (key=='l'||key=='L'){ // luces/color
    luz=!luz;
    if (luz) {glEnable(GL_LIGHTING);glEnable(GL_LIGHT0);glEnable(GL_COLOR_MATERIAL);}
    else {glDisable(GL_LIGHTING);glDisable(GL_LIGHT0);glDisable(GL_COLOR_MATERIAL);}
    if (luz)
      cout << "Iluminacion" << endl;
    else
      cout << "Color" << endl;
  }
  else if (key=='p'||key=='P'){ // Posicional/Direccional
    lpos[3]=1-lpos[3]; // cuarta componente (w) 0 o 1
    glLightfv(GL_LIGHT0,GL_POSITION,lpos); // posiciona la luz
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,(lpos[3])?GL_TRUE:GL_FALSE);//observador: Local or Infinite Viewpoint
    if (lpos[3])
      cout << "Luz y Observador Posicional" << endl;
    else
      cout << "Luz y Observador Direccional" << endl;
  }
  else if (key=='d'||key=='D'){ // borrar todos los conos
    cantidad=0;
  }
  else return;
  glutPostRedisplay(); // redibuja
}
//------------------------------------------------------------
// Menu
void Menu_cb(int value) {
  Keyboard_cb(value);
}

//------------------------------------------------------------
// Inicializa GLUT y OpenGL
void initialize() {
  // pide z-buffer, color RGBA y double buffering
  glutInitDisplayMode(GLUT_DEPTH|GLUT_RGB|GLUT_DOUBLE);

  glutInitWindowSize(640,480); glutInitWindowPosition(100,50);

  glutCreateWindow("CONOS"); // crea el main window

  //declara los callbacks
  //los que no se usan no se declaran
  glutDisplayFunc(Display_cb); // redisplays
  glutReshapeFunc(Reshape_cb);
  glutKeyboardFunc(Keyboard_cb);
  glutMouseFunc(Mouse_cb);

  glutCreateMenu(Menu_cb);
  glutAddMenuEntry("Luz ON/OFF [L]            ", 'l');
  glutAddMenuEntry("Luz Posicional ON/OFF [P]", 'p');
  glutAddMenuEntry("Borrar conos [D]          ", 'd');
  glutAddMenuEntry("Exit [Esc]                 ",  27);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // ========================
  // estado normal del OpenGL
  // ========================

  glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); // habilita el z-buffer

  // interpola normales por nodos o una normal por plano
  glShadeModel(GL_SMOOTH); // o GL_FLAT

  // color de fondo
  glClearColor(.9f,.9f,.9f,1.f);// luces y colores en float;

  // direccion de los poligonos
  glFrontFace(GL_CW); // la cara delantera esta definida en sentido horario
  glPolygonMode(GL_FRONT,GL_FILL); // las caras de adelante se dibujan rellenas
  glCullFace(GL_BACK);glEnable(GL_CULL_FACE);// se descartan las caras de atras

  // define la iluminacion
  float
    lambient[]={.4f,.4f,.4f}, // luz ambiente
    lspecular[]={1,1,1}, // luz especular
    ldiffuse[]={1,1,1}; // luz difusa
  glLightfv(GL_LIGHT0,GL_AMBIENT,lambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,ldiffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,lspecular);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);// las caras traseras de los polígonos no se iluminarán  
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,(lpos[3])?GL_TRUE:GL_FALSE);//Local or Infinite Viewpoint

  // define material estandar para los conos
  //(sin especular queda mas bonito)
  //glMaterialfv(GL_FRONT,GL_SPECULAR,lspecular);// el color reflejado es cte para todos
  //glMaterialf(GL_FRONT,GL_SHININESS,60);// el brillo es cte para todos
  // el color ambiente y difuso se cambian con glColor
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  if (luz) {glEnable(GL_LIGHTING);glEnable(GL_LIGHT0);glEnable(GL_COLOR_MATERIAL);}
  else {glDisable(GL_LIGHTING);glDisable(GL_LIGHT0);glDisable(GL_COLOR_MATERIAL);}

  glutPostRedisplay(); // avisa que hay que redibujar
}

//------------------------------------------------------------
// main
int main(int argc,char** argv) {
  glutInit(&argc,argv);// inicializa glut
  initialize(); // condiciones iniciales de la ventana y OpenGL
  glutMainLoop(); // entra en loop de reconocimiento de eventos
  return 0;
}
