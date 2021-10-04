#include <cmath> // sin cos
#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif
#include "Auto.hpp"
#include "drawObjects.hpp"
#include "drawPartes.hpp"

#define mat_error DEBEN_ARMAR_LAS_MATRICES_MANUALMENTE
#define glRotated(a,x,y,z) mat_error
#define glRotatef(a,x,y,z) mat_error
#define glTranslatef(x,y,z) mat_error
#define glTranslated(x,y,z) mat_error
#define glScaled(x,y,z) mat_error
#define glScalef(x,y,z) mat_error
#include <cmath>
#include <iostream>
using namespace std;

void drawAuto(int lod) {

  /// @TODO: aplicar las transformaciones necesarias para ubicar las partes del auto como van
  
  float ex_x,ex_y,ex_z,ey_x,ey_y,ey_z,ez_x,ez_y,ez_z,dx,dy,dz;
  
  /// 1.9 unidades de largo (x), 0.5 unidades de ancho (y) y 0.2 unidades de 
  /// profundidad (z) y permanecer centrada en (0;0;0).

  
  glPushMatrix();
  float mt[16] = {  0.95, 0   , 0  , 0.00,
                    0   , 0.25, 0  , 0.00,
                    0   , 0   , 0.1, 0.00,
                    0   , 0   , 0  , 1.00
  };
  glMultMatrixf(mt);
  drawChasis(); // cuerpo principal
  glPopMatrix();
  
  ///(+0.48;+0.28;0), (+0.48;-0.28;0),(-0.6;+0.35,0) y (-0.6;-0.35;0). 
  ///Las ruedas traseras deben medir 0.22 x 0.36 x 0.36 las delanteras tienen el 90% del tamaÃ±o de las trasera
  
  glPushMatrix();
  ///rota en movimiento
  float mtr1[] = {  0                        , 0.099, 0                       , 0.00,
                    0.162*cos(el_auto.rang2) , 0    , 0.162*sin(el_auto.rang2), 0.00,
                    -0.162*sin(el_auto.rang2), 0    , 0.162*cos(el_auto.rang2), 0.00,
                    0                        , 0    , 0                       , 1.00
  };
  ///ROTACION CUANDO ESTA GIRANDO EL AUTO Y POSICIONA LA RUEDA
  float mtr12[] ={  cos(-el_auto.rang1), -sin(-el_auto.rang1), 0, 0.00,
                    sin(-el_auto.rang1), cos(-el_auto.rang1) , 0, 0.00,
                    0                 , 0                  , 1, 0.00,
                    0.48              , 0.28               , 0, 1.00
  };
  glMultMatrixf(mtr12);
  glMultMatrixf(mtr1);
  drawRueda(lod); // x 4
  glPopMatrix();
  
  ///rueda 2
  glPushMatrix();
  float mtr2[] = {  0                        , 0.099, 0                       , 0.00,
                    0.162*cos(el_auto.rang2) , 0    , 0.162*sin(el_auto.rang2), 0.00,
                    -0.162*sin(el_auto.rang2), 0    , 0.162*cos(el_auto.rang2), 0.00,
                    0                        , 0    , 0                       , 1.00
  };
  float mtr22[] ={  cos(-el_auto.rang1), -sin(-el_auto.rang1), 0, 0.00,
                    sin(-el_auto.rang1), cos(-el_auto.rang1) , 0, 0.00,
                    0                 , 0                  , 1, 0.00,
                    0.48              , -0.28              , 0, 1.00
  };
  glMultMatrixf(mtr22);
  glMultMatrixf(mtr2);
  drawRueda(lod); // x 4
  glPopMatrix();

  ///rueda 3 (-0.6;+0.35,0)
  glPushMatrix();
  float mtr3[] ={ 0                       , 0.11, 0                      , 0.00,
                  0.18*cos(el_auto.rang2) , 0   , 0.18*sin(el_auto.rang2), 0.00,
                  -0.18*sin(el_auto.rang2), 0   , 0.18*cos(el_auto.rang2), 0.00,
                  -0.6                    , 0.35, 0                      , 1.00
  };
  glMultMatrixf(mtr3);
  drawRueda(lod); // x 4
  glPopMatrix();
  
  ///rueda 4 (-0.6;-0.35,0)
  glPushMatrix();
  float mtr4[] ={ 0                       , 0.11 , 0                      , 0.00,
                  0.18*cos(el_auto.rang2) , 0    , 0.18*sin(el_auto.rang2), 0.00,
                  -0.18*sin(el_auto.rang2), 0    , 0.18*cos(el_auto.rang2), 0.00,
                  -0.6                    , -0.35, 0                      , 1.00
  };
  glMultMatrixf(mtr4);
  drawRueda(lod); // x 4
  glPopMatrix();
  
  ///Las tomas deben escalarse para medir 0.7 x 0.4 x 0.1. Para ubicarlas correctamente, 
  ///debe desplazarlas 0.25 hacia un lateral del chasis (abajo en el dibujo, eje y) y 0.01 hacia arriba (eje z).
  ///lado izquierdo
  glPushMatrix();
  float mtti[] = {  0.35, 0    , 0   , 0.00,
                    0   , 0.2  , 0   , 0.00,
                    0   , 0    , 0.05, 0.00,
                    0   , -0.25, 0.01, 1.00
  };
  glMultMatrixf(mtti);
  drawToma(); // x2 (izquierda y derecha, "espejadas")
  glPopMatrix();
  
    ///lado derecho
  glPushMatrix();
  float mttd[] = {  0    , 0.2 , 0   , 0.00,
                    -0.35, 0   , 0   , 0.00,
                    0    , 0   , 0.05, 0.00,
                    0    , 0.25, 0.01, 1.00
  };
  glMultMatrixf(mttd);
  drawToma(); // x2 (izquierda y derecha, "espejadas")
  glPopMatrix();
  
  ///El tamaño final de la pieza debe ser 1.2 x .3 x .2,debe desplazarse 0.2 hacia atras y 
  ///0.05 hacia arriba, y rotarse 10 grados para quedar como en la figura.
  glPushMatrix();
  float mtchasis[] ={ 0.6*cos(170), 0, 0, 0.00,
                      0           , 0.15, 0 , 0.00,
                      0, 0, 0.1*cos(170), 0.00,
                      -0.2, 0, 0.05, 1.00
  };
  glMultMatrixf(mtchasis);
  drawChasis(); // parte superior
  glPopMatrix();  
  
  ///El delantero mide 0.2;0.6;0.06 y va centrado en el punto 0.8;0;0, mientras que el 
  ///trasero mide 0.4;0.8;0.1, y va centrado en -0.75;0;0.3
  ///delantero
  glPushMatrix();
  float mtaleron1[] = { 0.1, 0, 0, 0.00,
                        0, 0.3, 0, 0.00,
                        0, 0, 0.03, 0.00,
                        0.8, 0, 0, 1.00
  };
  glMultMatrixf(mtaleron1);
  drawAleron(); // delantero
  glPopMatrix();
  
  ///trasero
  glPushMatrix();
  float mtaleron2[] = { 0.2, 0, 0, 0.00,
                        0, 0.4, 0, 0.00,
                        0, 0, 0.05, 0.00,
                        -0.75, 0, 0.3, 1.00 
  };
  glMultMatrixf(mtaleron2);
  drawAleron(); // delantero
  glPopMatrix();
  
///Intente escalar y posicionar la esfera que simula el casco del piloto determinando usted un tamaño 
///  y una posicion adecuados.
  glPushMatrix();
  float mtEsfera[] ={ 0.15, 0, 0, 0.00,
                      0, 0.15, 0, 0.00,
                      0, 0, 0.15, 0.00,
                      0, 0, 0, 1.00
  };
  glMultMatrixf(mtEsfera);
  drawCasco(lod);
  glPopMatrix();  
  

  
}

void drawObjects(bool animado, int lod) {
  
  if (animado) {
   
    drawPista(la_pista.ancho,la_pista.alto);
    /// @TODO: ubicar el auto en la pista
    glPushMatrix();
    float mt2[16] = { cos(el_auto.ang), sin(el_auto.ang), 0, 0.00,  
                      -sin(el_auto.ang),cos(el_auto.ang) , 0, 0.00,  
                      0 , 0 , 1, 0.00,  
                      el_auto.x ,  el_auto.y ,  0.17 , 1.00 };
    glMultMatrixf(mt2);
      drawAuto(lod);
    glPopMatrix();

  }
  
  if (!animado) {
    drawCube();
    drawEjes();  ///colocar el auto sobre la pista
    glPushMatrix();
    float mt2[16] = { 1, 0, 0, 0.00,  
      0, 1, 0, 0.00,  
      0, 0, 1, 0.00,  
      0, 0, 0.17, 1.00 };
    glMultMatrixf(mt2);
    drawAuto(lod);
    glPopMatrix();
    
    
  }
}
