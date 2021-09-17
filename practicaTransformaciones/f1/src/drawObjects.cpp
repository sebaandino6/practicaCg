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


void drawAuto(int lod) {
  
  /// @TODO: aplicar las transformaciones necesarias para ubicar las partes del auto como van
   glPushMatrix();
    float mt[] = { 
      0.95, 0   , 0   , 0.00,  
      0   , 0.25, 0   , 0.00,  
      0   , 0   , 0.1 , 0.00,  
      0   , 0   , 0   , 1.00 
    };
    glMultMatrixf(mt);
    drawChasis();
  glPopMatrix();
  
  glPushMatrix();
    float mtRueda0[] = { 
      0   , 0.15, 0   , 0.00,  
      0.11, 0   , 0   , 0.00,  
      0   , 0   , 0.18, 0.00,  
      0.48, 0.28, 0   , 1.00 
    };
    glMultMatrixf(mtRueda0);
    drawRueda();
  glPopMatrix();
  
  glPushMatrix();
    float mtRueda1[] = { 
      0   , 0.15, 0   , 0.00,  
      0.11, 0   , 0   , 0.00,  
      0   , 0   , 0.18, 0.00,  
      0.48,-0.28, 0   , 1.00 
    };
    glMultMatrixf(mtRueda1);
    drawRueda();
  glPopMatrix();
    
  glPushMatrix();
    float mtRueda2[] = { 
      0   , 0.15, 0   , 0.00,  
      0.11, 0   , 0   , 0.00,  
      0   , 0   , 0.18, 0.00,  
      -0.6,-0.35, 0   , 1.00 
    };
    glMultMatrixf(mtRueda2);
    drawRueda();
  glPopMatrix();
    
  glPushMatrix();
    float mtRueda3[] = { 
      0   , 0.15, 0   , 0.00,  
      0.11, 0   , 0   , 0.00,  
      0   , 0   , 0.18, 0.00,  
      -0.6,0.35, 0   , 1.00 
    };
    glMultMatrixf(mtRueda3);
    drawRueda();
  glPopMatrix();
    
  glPushMatrix();
    float mtToma0[] = { 
      0.35, 0   , 0   , 0.00,  
      0   , 0.2 , 0   , 0.00,  
      0   , 0   , 0.05, 0.00,  
      0   , -0.25, 0.01, 1.00 
    };
    glMultMatrixf(mtToma0);
    drawToma();
  glPopMatrix();
  
  glPushMatrix();
    float mtToma1[] = {  
      0.35, 0   , 0   , 0.00,  
      0   , -0.2 , 0   , 0.00,  
      0   , 0   , 0.05, 0.00,  
      0   , 0.25, 0.01, 1.00 
    };
    glMultMatrixf(mtToma1);
    drawToma();
  glPopMatrix();
      
  glPushMatrix();
    float mtChasis[] = { 
      0.6*cos(180/(M_PI*10)), 0   , 0   , 0.00,  
      0  , 0.15 , 0   , 0.00,  
      0  , 0   , 0.1*cos(180/(M_PI*10)), 0.00,  
      0.2, 0, 0.05, 1.00 
    };
    glMultMatrixf(mtChasis);
    drawChasis();
  glPopMatrix();
  
  glPushMatrix();
  float mtAleronDelantero[] = { 
    0.1, 0   , 0   , 0.00,  
    0  , 0.3 , 0   , 0.00,  
    0  , 0   , 0.03, 0.00,  
    0.8 , 0   , 0   , 1.00 
  };
  glMultMatrixf(mtAleronDelantero);
  drawAleron();
  glPopMatrix();
  
  glPushMatrix();
  float mtAleronTrasero[] = { 
      0.2 , 0   , 0   , 0.00,  
      0   , 0.4 , 0   , 0.00,  
      0   , 0   , 0.05, 0.00,  
     -0.75, 0   , 0.3 , 1.00 
  };
  glMultMatrixf(mtAleronTrasero);
  drawAleron();
  glPopMatrix();
  
  glPushMatrix();
    float mtCasco[] = { 
        0.1 , 0   , 0   , 0.00,  
        0   , 0.1 , 0   , 0.00,  
        0   , 0   , 0.1 , 0.00,  
        0   , 0   , 0.2 , 1.00 
    };
    glMultMatrixf(mtCasco);
    drawCasco(lod);
  glPopMatrix();
}

void drawObjects(bool animado, int lod) {
  
  if (animado) {
    drawPista(la_pista.ancho,la_pista.alto);
    /// @TODO: ubicar el auto en la pista
  }
  
  drawAuto(lod);
  
  if (!animado) {
    drawCube();
    drawEjes();
  }
  
}
