#include <iostream> // cin, cout
#include <cstdlib> // exit
#include <cmath> // fabs sqrt
#include <cstring> // memcpy
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif
#include "p4f.hpp"

#include <cmath>
using namespace std;


// variables
int
  w,h, // alto y ancho de la pantalla
  tol=10, // tolerancia para seleccionar un punto
  yclick, // y picado
  lod=32; // nivel de detalle (subdivisiones de lineas)

float // colores
  fondo[]={0.95f,0.98f,1.0f}, // color de fondo
  lc_c[]={.4f,.4f,.6f,1.f},  // poligono de control
  pc_c[]={.2f,.2f,.4f,1.f},  // puntos de control
  c_c[]={.8f,.4f,.4f,1.f},   // curva
  la_c[]={.6f,.8f,.6f,1.f},  // lineas accesorias
  n_c[]={.6f,.8f,.6f,1.f},   // normales
  p_c[]={1.f,0.f,0.f,1.f};   // punto de la curva

bool
  draw_auxiliares=false, // dibuja los puntos, lineas y normales
  antialias=false;  // antialiasing

// Interpolacion con Bezier
// Hay npc puntos de control, los interpolados y los invisibles
// El primero y el ultimo interpolan ==> :
//  segundo grado entre el primero, el primer invisible y el segundo
//  tercer grado en el medio
//  segundo grado entre el penultimo, el ultimo invisible y el ultimo
// El primero no tiene invisible asociado (usa el del segundo) y lo mismo para el ultimo
// Se complica la aritmetica de cuales (i??) hay que recalcular por cambios
int npc=0; const int MAXPC=252; p4f pc[255];  // puntos de control
int pcsel=-3; // punto seleccionado
static const int metodos=4; 
int metodo=0; // 0=Catmull-Rohm 1=Overhausser .....
char txt_metodo[256];

// funciones
// recalcula (lo que sea necesario) al modificar un punto
// 0 (1) 2 (3) (4) 5 (6) (7) 8 ...
// (i-4) i-3 (i-2) (i-1) i (i+1) (i+2) i+3 (i+4)
// ... (n-4) n-3 (n-2) n-1
// primario = llamado desde fuera, debe recalcular vecinos
// no primario = llamado por recursi�n desde aqui mismo
void recalc(int i,bool primario=true){
  if (npc<=2) return;
  if (i==0){ // primer interpolado
    if (primario) recalc(2,false); // recalcula los asociados al 2do interpolado
    return;
  }
  if (i==npc-1){
    if (primario) recalc(npc-3,false); // recalcula los asociados al penultimo interpolado
    return;
  }

  int iant=i-3; if (i==2) iant=0;
  int ipos=i+3; if (i==npc-3) ipos=npc-1;
  p4f l;
  p4f vant=pc[i]-pc[iant], vpos=pc[ipos]-pc[i];
  float dant=(vant).mod(),dpos=(vpos).mod();
  if (dant<1e-6) dant=1e-6; if (dpos<1e-6) dpos=1e-6;
  if (metodo==0){ /// Catmull-Rohm
    /// direccion que une anterior y posterior
    /// derivada = distancia anterior a posterior /2/grado
    /// derivadas iguales a ambos lados={
    strcpy(txt_metodo,"Catmull-Rohm: dir: ant->pos --- vel:dist(ant-pos)/2/grado");
    l=vant+vpos;
    if (i!=2)     pc[i-1]=pc[i]-l/6; else pc[i-1]=pc[i]-l/4;
    if (i!=npc-3) pc[i+1]=pc[i]+l/6; else pc[i+1]=pc[i]+l/4;
  }
///@@@@ Implementar el m�todo de Overhausser
  else if (metodo==1){
    /// direccion proporcional a la cercania
    /// derivada media = distancia anterior a posterior /2/grado
    /// derivadas proporcionales a las distancias
    strcpy(txt_metodo,"Overhausser: dir: inv prop dist --- vel media=dist(ant-pos)/2/grado; indiv=prop dist");
    ///  1) calcular vectores diferencia y sus modulos (distancias)
    ///  2) direcci�n en proporcion inversa a las distancias (apunta mas al mas cercano)
    ///  3) puntos invisibles en proporcion directa con las distancias (mas corto el mas cercano)

	///tomamos el vi-1 
	p4f vimenos = vant / dant; 
	///tomamos el vi+1
	p4f vimas = vpos / dpos;
	///calculamos el vi en base a las direcciones y sus modulos
	p4f vi = ( (dpos*vimenos) + (dant*vimas) ) / (dpos + dant); 
	
	if (i!=2){		pc[i-1] = pc[i]-(dant*vi)/ 3; 
	}else{ 			pc[i-1] = pc[i]-(dant*vi)/ 2;}
	
	if(i!=npc-3){	pc[i+1] = pc[i]+(dpos*vi)/ 3;
	}else{			pc[i+1] = pc[i]+(dpos*vi)/ 2;}
	
  }
  
///@@@@ Implementar algun otro modo de evitar el overshooting o hacerlo mas bonito
  else if (metodo==2){
		/// Una idea:
		/// direccion que une anterior y posterior (proporcional a la distancia)
		/// derivada media = distancia anterior a posterior /2/3
		/// derivadas proporcionales a las distancias
		strcpy(txt_metodo,"dir: ant->pos --- vel media=dist(ant-pos)/2/grado; indiv=prop dist");
		p4f dir = vant+vpos;
		p4f v = dir / dir.mod()/2;		
		if (i!=2)     pc[i-1] = pc[i] - dant*v/3; else pc[i-1] = pc[i] - dant*v/2;
		if (i!=npc-3) pc[i+1] = pc[i] + dpos*v/3; else pc[i+1] = pc[i] + dpos*v/2;
		
  }
///@@@@ Implementar otro m�todo que ademas de evitar el overshooting sea C1 (derivadas iguales a cada lado)
  else if (metodo==3){
	  
    strcpy(txt_metodo,"M�todo 3: con continuidad C1");
	///pongo las derivadas de los dos lados iguales
	///tengo que tener cuidado con los extremos porque no tienen dant o dpos
	p4f c, v, dir;
	dir = vant+vpos;
	v = dir / dir.mod()/2;	
	float d = std::min(dant,dpos);	///con la minima derivada se evita el overshooting
	if (i!=2)     pc[i-1] = pc[i] - d*v/3; else pc[i-1] = pc[i] - d*v/3;///divido por 3 en todos los casos y las 
	if (i!=npc-3) pc[i+1] = pc[i] + d*v/3; else pc[i+1] = pc[i] + d*v/3;///derivadas me quedan iguales
  }

  if (primario){
    recalc(iant,false);
    recalc(ipos,false);
  }
}

void recalc_all(){
  for (int i=2;i<=npc-3;i+=3) recalc(i,false);
}

void check_gl_error(){
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

void salir(){
  exit(EXIT_SUCCESS);
}

//--------
void dibuja(){
  if (!npc) return;

  int i;
  glPointSize(3); glLineWidth(1);
  // puntos dados
  glColor4fv(p_c);
  glBegin(GL_POINTS);
    for (i=2;i<=npc-3;i+=3) glVertex4fv(pc[i]);
    glVertex4fv(pc[0]); glVertex4fv(pc[npc-1]);
  glEnd();
  if (npc==1) return; // hay un solo punto

  if (draw_auxiliares){
    // puntos
    glColor4fv(pc_c);
    glBegin(GL_POINTS);
    for (i=2;i<=npc-3;i+=3) {
      glVertex4fv(pc[i-1]); glVertex4fv(pc[i+1]);
    }
    glEnd();

    // lineas
    glBegin(GL_LINES);
    for (i=2;i<=npc-3;i+=3) {
        glVertex4fv(pc[i-1]); glVertex4fv(pc[i+1]);
    }
    glEnd();

    // normales
    int l=30; // l pixeles de largo
    glColor4fv(n_c);
    glBegin(GL_LINES);
    glVertex4fv(pc[0]-(pc[1]-pc[0]).dir().giro_z_90()*l/2);
    glVertex4fv(pc[0]+(pc[1]-pc[0]).dir().giro_z_90()*l/2);
    glVertex4fv(pc[npc-1]-(pc[npc-1]-pc[npc-2]).dir().giro_z_90()*l/2);
    glVertex4fv(pc[npc-1]+(pc[npc-1]-pc[npc-2]).dir().giro_z_90()*l/2);
    for (i=2; i<=npc-3; i+=3){
      glVertex4fv(pc[i]-(pc[i+1]-pc[i]).dir().giro_z_90()*l/2);
      glVertex4fv(pc[i]+(pc[i+1]-pc[i]).dir().giro_z_90()*l/2);
    }
    glEnd();
  }

  // dibuja la curva
  glColor4fv(c_c);
  if (npc==2){ // hay dos puntos => una linea
    glBegin(GL_LINES);
      glVertex4fv(pc[0]);
      glVertex4fv(pc[1]);
    glEnd();
    return;
  }
  // curvas de Bezier de grado 2 al principio y final
  glEnable(GL_MAP1_VERTEX_4);
  glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 3, (const float*)(&pc[0]));
  glMapGrid1f(lod,0,1); glEvalMesh1(GL_LINE,0,lod);
  check_gl_error();
  glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 3, (const float*)(&pc[npc-3]));
  glMapGrid1f(lod,0,1); glEvalMesh1(GL_LINE,0,lod);
  check_gl_error();
  // curvas de Bezier de grado 3
  for (i=2;i<npc-3;i+=3){
    glMap1f(GL_MAP1_VERTEX_4, 0.0, 1.0, 4, 4, (const float*)(&pc[i]));
    glMapGrid1f(lod,0,1); glEvalMesh1(GL_LINE,0,lod);
    check_gl_error();
  }
}

//============================================================
// callbacks

//------------------------------------------------------------
// redibuja cada vez que hace un redisplay
void Display_cb() { // Este tiene que estar

  glClear(GL_COLOR_BUFFER_BIT);  // borra el buffer de pantalla
  dibuja();
  glutSwapBuffers();

  check_gl_error();
}

//------------------------------------------------------------
// Maneja cambios de ancho y alto del screen
void Reshape_cb(int wi, int hi){
  w=wi; h=hi;
  glViewport(0,0,w,h); // region donde se dibuja

  // matriz de proyeccion
  glMatrixMode(GL_PROJECTION);  glLoadIdentity();
  glOrtho(-w/2,w-w/2,h-h/2,-h/2,-1,1);

  glutPostRedisplay();
}

//------------------------------------------------------------
// Teclado y Mouse
// GLUT ACTIVE SHIFT Set if the Shift modifier or Caps Lock is active.
// GLUT ACTIVE CTRL  Set if the Ctrl modifier is active.
// GLUT ACTIVE ALT   Set if the Alt modifier is active.

short modifiers=0;  // ctrl, alt, shift (de GLUT)
inline short get_modifiers() {return modifiers=(short)glutGetModifiers();}

// Maneja pulsaciones del teclado (ASCII keys)
// x,y posicion del mouse cuando se teclea
void Keyboard_cb(unsigned char key,int x=0,int y=0) {
  if (key<'0'+metodos&&key>='0'){ // numero que define el metodo
    if (metodo==key-'0') return;
    metodo=key-'0';
    recalc_all();
    cout << "Metodo " << metodo << " -- " << txt_metodo << endl;
  }
  else if (key==27){ // escape => exit
    get_modifiers();
    if (!modifiers) salir();
  }
  else if (key==8||key==127){ // backspace o del => borra pt
    // @@@@ esta es la rutina mas complicada! analizar
    if (npc==0) return;
    if (pcsel==0) { // borra el primero
      if (npc==1) npc=0;
      else if (npc==2) {pc[0]=pc[1]; npc=1;}
      else { // desaparecen tambien el 1 y el 3
        npc-=3; pc[0]=pc[2];
        for (int i=1;i<npc;i++) pc[i]=pc[i+3];
      }
    }
    else if (pcsel==npc-1) { // borra el ultimo
      if (npc==2) npc=1;
      else { // desaparecen tambien el npc-2 y el npc-4
        pc[npc-4]=pc[npc-3]; npc-=3;
      }
      pcsel=npc-1;
    }
    else { // desaparecen tres intermedios: el anterior, este y el posterior
      npc-=3;
      for (int i=pcsel-1;i<npc;i++) pc[i]=pc[i+3];
      if (npc==2) pcsel=1; else if (pcsel!=2) pcsel-=3;
    }
    recalc(pcsel);
  }
  else if (key=='a'||key=='A'){ // Antialiasing
    antialias=!antialias;
    if (antialias){
      glEnable(GL_BLEND);
      glEnable(GL_POINT_SMOOTH); glEnable(GL_LINE_SMOOTH);
      cout << "Antialiasing" << endl;
    }
    else {
      glDisable(GL_BLEND);
      glDisable(GL_POINT_SMOOTH); glDisable(GL_LINE_SMOOTH);
      cout << "Sin Antialiasing" << endl;
    }
  }
  else if (key=='b'||key=='B'){ // Balance
    if (key=='B') metodo+=metodos-1; else  metodo++;
    metodo%=metodos;
    recalc_all();
    cout << "Metodo " << metodo << ": " << txt_metodo << endl;
  }
  else if (key=='x'||key=='X'){ // auxiliares
    draw_auxiliares=!draw_auxiliares;
    if (!draw_auxiliares) cout << "no ";
    cout << "dibuja lineas auxiliares" << endl;
  }
  else if (key=='+'){ // lod
    lod++;
    cout << "Nivel de Detalle: " << lod  << "               \r" << flush;
  }
  else if (key=='-'){ // lod
    if (lod==2) return;
    lod--;
    cout << "Nivel de Detalle: " << lod  << "               \r" << flush;
  }
  glutPostRedisplay();
}

// Special keys (non-ASCII)
//  GLUT KEY F[1,12] F[1,12] function key.
//  GLUT KEY LEFT Left directional key.
//  GLUT KEY UP Up directional key.
//  GLUT KEY RIGHT Right directional key.
//  GLUT KEY DOWN Down directional key.
//  GLUT KEY PAGE UP Page up directional key.
//  GLUT KEY PAGE DOWN Page down directional key.
//  GLUT KEY HOME Home directional key.
//  GLUT KEY END End directional key.
//  GLUT KEY INSERT Inset directional key.

// aca es int key
void Special_cb(int key,int xm=0,int ym=0) {
  if (key==GLUT_KEY_F4){ // alt+f4 => exit
    get_modifiers();
    if (modifiers==GLUT_ACTIVE_ALT) salir();
  }
}

// Menu
void Menu_cb(int value){
  if(value<256) Keyboard_cb(value);
  else Special_cb(value-256);
}
//------------------------------------------------------------

// Movimientos del mouse
void Motion_cb(int x, int y){ // drag
  x-=w/2;y-=h/2;

  // mueve el punto
  // permite drag solo dentro del viewport
  // en caso contrario, sin zoom, no se pueden editar los que esten fuera
  if (x<-w/2) x=-w/2; else if (x>w-w/2-1) x=w-w/2-1;
  if (y<-h/2) y=-h/2; else if (y>h-h/2-1) y=h-h/2-1;
  p4f pm(x,y);

  // evita que se pegue con los vecinos inmediatos
  // posterior
  if (pcsel==0&&pm.cerca2D(pc[2],1)) return;
  else if (pcsel==npc-3&&pm.cerca2D(pc[npc-1],1)) return;
  else if (pcsel<npc-3&&pm.cerca2D(pc[pcsel+3],1)) return;
  // anterior
  if (pcsel==npc-1&&pm.cerca2D(pc[npc-3],1)) return;
  else if (pcsel==2&&pm.cerca2D(pc[0],1)) return;
  else if (pcsel>2&&pm.cerca2D(pc[pcsel-3],1)) return;

  pc[pcsel]=pm; recalc(pcsel);  // mueve y recalcula

  glutPostRedisplay();
}

// Clicks del mouse
// GLUT LEFT BUTTON, GLUT MIDDLE BUTTON, or GLUT RIGHT BUTTON
// The state parameter is either GLUT UP or GLUT DOWN
// glutGetModifiers may be called to determine the state of modifier keys
void Mouse_cb(int button, int state, int x, int y){
  x-=w/2;y-=h/2;
  if (button==GLUT_LEFT_BUTTON){
    if (state==GLUT_DOWN) {
      p4f pm(x,y);
      int i,cercano=-1;
      // verifica tolerancia
      if (pm.cerca2D(pc[0],tol)) cercano=0;
      else if (pm.cerca2D(pc[npc-1],tol)) cercano=npc-1;
      else for(i=2;i<npc;i+=3) if (pm.cerca2D(pc[i],tol)) {cercano=i; break;}
      if (cercano!=-1) {pcsel=cercano; glutMotionFunc(Motion_cb); return;}
      // no pico cerca de ninguno => agrega
      if (npc>=MAXPC-3) return;
      if (npc==0) {npc=1; pcsel=0;} // primero
      else if (npc==1) {npc=2; pcsel=1;} // segundo
      else { // cualquier otro
        npc+=3; // hace lugar para tres mas
        if (pcsel==npc-4) pc[pcsel+1]=pc[pcsel];// despues del ultimo
        else for (i=npc-1;i>pcsel+1;i--) pc[i]=pc[i-3];
        if (pcsel==0) pcsel=2; else pcsel+=3;
      }
      pc[pcsel]=pm; if (pcsel==npc-1) recalc(pcsel-2); else recalc(pcsel);
      glutPostRedisplay();
      glutMotionFunc(Motion_cb);
    } // down
    else if (state==GLUT_UP){// fin del drag
      glutMotionFunc(0); // anula el callback para los drags
      modifiers=0;
    } // up
  } // left
}

//------------------------------------------------------------
int integerv(GLenum pname){
  int value;
  glGetIntegerv(pname,&value);
  return value;
}
#define _PRINT_INT_VALUE(pname) #pname << ": " << integerv(pname) <<endl

//------------------------------------------------------------
// Inicializa GLUT y OpenGL
void initialize() {
  // pide color RGB y double buffering
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

  glutInitWindowSize(640,480); glutInitWindowPosition(250,100);

  glutCreateWindow("Interpolaci�n con splines"); // crea el main window

  //declara los callbacks
  //los que no se usan no se declaran
  glutDisplayFunc(Display_cb); // redisplays
  glutReshapeFunc(Reshape_cb); // resize
  glutKeyboardFunc(Keyboard_cb); // teclado
  glutSpecialFunc(Special_cb); // ctrl alt y shift
  glutMouseFunc(Mouse_cb); // clicks

  glutCreateMenu(Menu_cb);
    glutAddMenuEntry("[Del] Borra Punto de Control", 127);
    glutAddMenuEntry("[+]   +Nivel de Detalle       ", '+');
    glutAddMenuEntry("[-]   -Nivel de Detalle       ", '-');
    glutAddMenuEntry("[b/B] Cambia el Metodo        ", 'b');
    glutAddMenuEntry("[x]   Auxiliares   ON/OFF     ", 'x');
    glutAddMenuEntry("[a]   Antialiasing ON/OFF     ", 'a');
    glutAddMenuEntry("----------------------------", 255);
    glutAddMenuEntry("[Esc] Exit                  ", 27);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // ========================
  // estado normal del OpenGL
  // ========================
  glClearColor(fondo[0],fondo[1],fondo[2],1);  // color de fondo
  glDrawBuffer(GL_BACK);
  glMatrixMode(GL_MODELVIEW); glLoadIdentity(); // constante
  glShadeModel(GL_FLAT); // no interpola color

  if (antialias){
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH); glEnable(GL_LINE_SMOOTH);
  }
  else {
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH); glDisable(GL_LINE_SMOOTH);
  }
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  // ========================
  // info
  cout << "\n\n\nPuntos de control (Maximo " << MAXPC << "):\n"
    << "     Click izq -> Selecciona Punto (tol. 10) o\n"
       "                    Agrega despues del seleccionado\n"
    << "      Drag izq -> Mueve el seleccionado\n"
    << "         <DEL> -> Borra el seleccionado\n"    << "         0-"<< metodos-1 <<" -> Selecciona el Metodo\n"
    << endl;
}

//------------------------------------------------------------
// main
int main(int argc,char** argv) {
  glutInit(&argc,argv);// inicializa glut
  initialize(); // condiciones iniciales de la ventana y OpenGL
  glutMainLoop(); // entra en loop de reconocimiento de eventos
  return 0; //solo para que el builder se quede tranquilo (aqui nunca llega)
}
