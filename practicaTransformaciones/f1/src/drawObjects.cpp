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
  
//  
//  //drawChasis(); // cuerpo principal
//  glPushMatrix();
//    float mt[] = { 0.95, 0   , 0   , 0.00,  
//                   0   , 0.25, 0   , 0.00,  
//                   0   , 0   , 0.1 , 0.00,  
//                   0   , 0   , 0   , 1.00 };
//      glMultMatrixf(mt);
//      drawChasis();
//    glPopMatrix();
//  
//  
//  //drawRueda(lod); // x 4
//    glPushMatrix();
//      float mtRueda0[] = { 0   , 0.15, 0   , 0.00,  
//                     0.11, 0   , 0   , 0.00,  
//                     0   , 0   , 0.18, 0.00,  
//                     0.48, 0.28, 0   , 1.00 };
//        glMultMatrixf(mtRueda0);
//        drawRueda();
//    glPopMatrix();
//  
//    glPushMatrix();
//    float mtRueda1[] = { 0   , 0.15, 0   , 0.00,  
//      0.11, 0   , 0   , 0.00,  
//      0   , 0   , 0.18, 0.00,  
//      0.48,-0.28, 0   , 1.00 };
//    glMultMatrixf(mtRueda1);
//    drawRueda();
//    glPopMatrix();
//    
//    glPushMatrix();
//    float mtRueda2[] = { 0   , 0.15, 0   , 0.00,  
//      0.11, 0   , 0   , 0.00,  
//      0   , 0   , 0.18, 0.00,  
//      -0.6,-0.35, 0   , 1.00 };
//    glMultMatrixf(mtRueda2);
//    drawRueda();
//    glPopMatrix();
//    
//    
//    glPushMatrix();
//    float mtRueda3[] = { 0   , 0.15, 0   , 0.00,  
//      0.11, 0   , 0   , 0.00,  
//      0   , 0   , 0.18, 0.00,  
//      -0.6,0.35, 0   , 1.00 };
//    glMultMatrixf(mtRueda3);
//    drawRueda();
//    glPopMatrix();
//    
//  //drawToma(); // x2 (izquierda y derecha, "espejadas")
//      glPushMatrix();
//      float mtToma0[] = { 0.35, 0   , 0   , 0.00,  
//                         0   , 0.2 , 0   , 0.00,  
//                         0   , 0   , 0.05, 0.00,  
//                         0   , -0.25, 0.01, 1.00 };
//      glMultMatrixf(mtToma0);
//      drawToma();
//      glPopMatrix();
//  
//      glPushMatrix();
//      float mtToma1[] = {  0.35, 0   , 0   , 0.00,  
//                           0   , -0.2 , 0   , 0.00,  
//                           0   , 0   , 0.05, 0.00,  
//                           0   , 0.25, 0.01, 1.00 };
//      glMultMatrixf(mtToma1);
//      drawToma();
//     glPopMatrix();
//      
  
  //drawChasis(); // parte superior
    
      glPushMatrix();
        float mtChasis[] = { 0.6*cos(180/(M_PI*10)), 0   , 0   , 0.00,  
                             0  , 0.15 , 0   , 0.00,  
                             0  , 0   , 0.1*cos(180/(M_PI*10)), 0.00,  
                             0.2, 0, 0.05, 1.00 };
        glMultMatrixf(mtChasis);
        drawChasis();
       glPopMatrix();
  
  
  //drawAleron(); // delantero

  //drawAleron(); // trasero
  
  //SdrawCasco(lod);
  
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
