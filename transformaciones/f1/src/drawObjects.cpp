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


/// ejemplo de sintaxis (nota: la matriz se escribe "transpuesta" respecto a como la escribimos normalmente)
///  glPushMatrix();
///    float mt[] = { ex_x, ex_y, ex_z, 0.00,  
///                   ey_x, ey_y, ey_z, 0.00,  
///                   ez_x, ez_y, ez_z, 0.00,  
///                    dx ,  dy ,  dz , 1.00 };
///    glMultMatrixf(m);
///    drawSomething();
///  glPopMatrix();


void drawAuto(int lod) {

  /// @TODO: aplicar las transformaciones necesarias para ubicar las partes del auto como van
  
  float ex_x,ex_y,ex_z,ey_x,ey_y,ey_z,ez_x,ez_y,ez_z,dx,dy,dz;
  
  /// 1.9 unidades de largo (x), 0.5 unidades de ancho (y) y 0.2 unidades de 
  /// profundidad (z) y permanecer centrada en (0;0;0).
  ex_x=.95,ex_y=0,ex_z=0;
  ey_x=0,ey_y=0.25,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.1;
  dx=0,dy=0,dz=0;
  
  glPushMatrix();
  float mt[16] = { ex_x, ex_y, ex_z, 0.00,  
                 ey_x, ey_y, ey_z, 0.00,  
                 ez_x, ez_y, ez_z, 0.00,  
                 dx ,  dy ,  dz , 1.00 };
  glMultMatrixf(mt);
  
  drawChasis(); // cuerpo principal
  glPopMatrix();
  
  
  ///(+0.48;+0.28;0), (+0.48;-0.28;0),(-0.6;+0.35,0) y (-0.6;-0.35;0). 
  ///Las ruedas traseras deben medir 0.22 x 0.36 x 0.36 las delanteras tienen el 90% del tamaÃ±o de las trasera
  
  ///Dimension rueda 1
  glPushMatrix();
//  ex_x=0, ex_y=-0.099, ex_z=0;
//  ey_x=0.162, ey_y= 0, ey_z=0;
//  ez_x=0, ez_y=0, ez_z=0.162;

  ///ROTACION CUANDO ESTA GIRANDO EL AUTO
//  ex_x=0.162*sin(el_auto.rang1), ex_y=-0.099*cos(el_auto.rang1), ex_z=0;
//  ey_x=0.162*cos(el_auto.rang1), ey_y=0.099*sin(el_auto.rang1), ey_z=0;
//  ez_x=0, ez_y=0, ez_z=0.162;
  
//  ex_x=0,ex_y=0.099,ex_z=0;
//  ey_x=0.162*cos(el_auto.rang2),ey_y=0,ey_z=0.162*sin(el_auto.rang2);
//  ez_x=-0.162*sin(el_auto.rang2),ez_y=0,ez_z=0.162*cos(el_auto.rang2); 
//  dx=0.48,dy=0.28,dz=0;
//    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
//    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz; 
//    
    
    
  ///rota en movimiento
  float mt2[16] = {
      0.162*sin(el_auto.rang1), -0.1*cos(el_auto.rang1), 0, 0.00,
      0.162*cos(el_auto.rang2)*cos(el_auto.rang1), 0.1*sin(el_auto.rang1)*cos(el_auto.rang2), 0.162*sin(el_auto.rang2), 0.00,
      -0.162*cos(el_auto.rang1)*sin(el_auto.rang2), -0.1*sin(el_auto.rang1)*cos(el_auto.rang2), 0.162*cos(el_auto.rang2), 0.00,
      0.48, 0.28, 0, 1.00  };

//  glMultMatrixf(mt);
  glMultMatrixf(mt2);
  
  drawRueda(lod); // x 4
  glPopMatrix();
  
  ///rueda 2
  float mt3[16] = {
    0.162*sin(el_auto.rang1), -0.1*cos(el_auto.rang1), 0, 0.00,
      0.162*cos(el_auto.rang2)*cos(el_auto.rang1), 0.1*sin(el_auto.rang1)*cos(el_auto.rang2), 0.162*sin(el_auto.rang2), 0.00,
      -0.162*cos(el_auto.rang1)*sin(el_auto.rang2), -0.1*sin(el_auto.rang1)*cos(el_auto.rang2), 0.162*cos(el_auto.rang2), 0.00,
      0.48, -0.28, 0, 1.00  };
//  dy=-0.28;
  mt[13]=dy;
  glPushMatrix();
  glMultMatrixf(mt3);
  drawRueda(lod); // x 4
  glPopMatrix();

  ///rueda 3 (-0.6;+0.35,0)
  ex_x=0,ex_y=0.11,ex_z=0;
  ey_x=0.18*cos(el_auto.rang2),ey_y=0,ey_z=0.18*sin(el_auto.rang2);
  ez_x=-0.18*sin(el_auto.rang2),ez_y=0,ez_z=0.18*cos(el_auto.rang2); 

  dx=-0.6,dy=0.35,dz=0;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz; 
  glPushMatrix();
  glMultMatrixf(mt);
  drawRueda(lod); // x 4
  
  glPopMatrix();
  
  ///rueda 4 (-0.6;-0.35,0)
  dy=-0.35;
  mt[13]=dy;
  glPushMatrix();
  glMultMatrixf(mt);
  drawRueda(lod); // x 4
  glPopMatrix();
  
  ///Las tomas deben escalarse para medir 0.7 x 0.4 x 0.1. Para ubicarlas correctamente, 
  ///debe desplazarlas 0.25 hacia un lateral del chasis (abajo en el dibujo, eje y) y 0.01 hacia arriba (eje z).
    ///lado izquierdo
  ex_x=.35,ex_y=0,ex_z=0;
  ey_x=0,ey_y=.2,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.05;
  dx=0,dy=-0.25,dz=0.01;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz; 
  glPushMatrix();
  glMultMatrixf(mt);
  drawToma(); // x2 (izquierda y derecha, "espejadas")
  glPopMatrix();
  
    ///lado derecho
  ex_x=0,ex_y=0.2,ex_z=0;
  ey_x=-.35,ey_y=0,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.05;
  dy=0.25;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz; 
  glPushMatrix();
  glMultMatrixf(mt);
  drawToma(); // x2 (izquierda y derecha, "espejadas")
  glPopMatrix();
  
  ///El tamaño final de la pieza debe ser 1.2 x .3 x .2,debe desplazarse 0.2 hacia atras y 
  ///0.05 hacia arriba, y rotarse 10 grados para quedar como en la figura.
  ex_x=.6*cos(180/(M_PI*10)),ex_y=0,ex_z=0;
  ey_x=0,ey_y=.15,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.1*cos(180/(M_PI*10));
  dx=-0.2,dy=0,dz=0.05;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz;
  glPushMatrix();
  glMultMatrixf(mt);
  drawChasis(); // parte superior
  glPopMatrix();  
  
  ///El delantero mide 0.2;0.6;0.06 y va centrado en el punto 0.8;0;0, mientras que el 
  ///trasero mide 0.4;0.8;0.1, y va centrado en -0.75;0;0.3
  ///delantero
  ex_x=.1,ex_y=0,ex_z=0;
  ey_x=0,ey_y=.3,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.03;
  dx=0.8,dy=0,dz=0;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz;
  glPushMatrix();
  glMultMatrixf(mt);
  drawAleron(); // delantero
  glPopMatrix();
  
  ///trasero
  ex_x=.2,ex_y=0,ex_z=0;
  ey_x=0,ey_y=.4,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.05;
  dx=-0.75,dy=0,dz=0.3;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz;
  glPushMatrix();
  glMultMatrixf(mt);
  drawAleron(); // delantero
  glPopMatrix();
  
///Intente escalar y posicionar la esfera que simula el casco del piloto determinando usted un tamaño 
///  y una posicion adecuados.
  ex_x=0.15,ex_y=0,ex_z=0;
  ey_x=0,ey_y=0.15,ey_z=0;
  ez_x=0,ez_y=0,ez_z=0.15;
  dx=0,dy=0,dz=0;
    mt[0]=ex_x; mt[1]=ex_y; mt[2]=ex_z; mt[4]=ey_x; mt[5]=ey_y; mt[6]=ey_z;
    mt[8]=ez_x; mt[9]=ez_y; mt[10]=ez_z; mt[12]=dx; mt[13]=dy; mt[14]=dz;
  glPushMatrix();
  glMultMatrixf(mt);
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
