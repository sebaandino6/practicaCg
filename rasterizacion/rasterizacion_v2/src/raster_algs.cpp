#include <forward_list>
#include <iostream>
#include <cmath>
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glut.h>
#endif
#include "raster_algs.hpp"

void draw_line(Point p0, Point p1) {
    glBegin(GL_POINTS);
    glColor4f(0,0,0,.5);
    int n = p1.x - p0.x;
    float dy = (1.0/n)*(p1-p0).y;
    
    for( Point p = p0; p.x <= p1.x; ++p.x) {
        glVertex2f(p.x,p.y);
        p.y+= dy;
    }
    
    
    /// @TODO: implementar algun algoritmo de rasterizacion de segmentos
    
    glEnd();
}


void draw_curve(curve_func_t f) {
    glBegin(GL_POINTS);
    
    /// @TODO: implementar algun algoritmo de rasterizacion de curvas
    
    // f es la función de la curva, recibe un t entre 0 y 1 y retorna
    // el punto y la derivada en ese punto, por ej:
    //    auto r = f(0.5); // calcular en el punto "medio" de la curva
    // entonces r.p es el punto (tipo Point) y r.d la derivada (tipo Vector)
    
    glEnd();
}






