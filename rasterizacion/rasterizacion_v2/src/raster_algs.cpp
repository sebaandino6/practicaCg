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

void dda_line(Point p0, Point p1) {
    
    float dy = (p1.y-p0.y);
    float dx = (p1.x-p0.x);
    //     Como se si la curva es suficientemente suave?
    if(abs(dx) < abs(dy)) {
        if(dy!=0){
            if(dy < 0){
                std::swap(p0,p1);
            }
            
            float m = dx/dy;
            
            for( Point p = p0; p.y <= p1.y; ++p.y) {
                glVertex2f(p.x,redon(p.y));
                p.x+= m;
            }    
        }    
    }else{
        if(dx!=0){
            if(dx < 0){
                std::swap(p0,p1);
            }
            
            float m = dy/dx;
            
            for( Point p = p0; p.x <= p1.x; ++p.x) {
                glVertex2f(redon(p.x),p.y);
                p.y+= m;
            }
        }
    }
}

void bresenham_line(Point p0, Point p1) {
    
    float dy; 
    float dx;
    float di;
    float pasoDi;
    float m;
    dy = abs(p1.y-p0.y);
    dx = (p1.x-p0.x);
    bool prueba = abs(dx) < abs(dy);
    Point p;
    
    if(prueba == true) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
    }
    
    if(p0.x > p1.x) {
        std::swap(p0, p1);        
    }
    
    if(p0.y > p1.y) {
        di = -1; 
    }else{
        di = 1;
    }
    
    dy = abs(p1.y-p0.y);
    dx = (p1.x-p0.x);
    p.y = p0.y;
    pasoDi = 0;
    for (p.x = p0.x; p.x < p1.x; p.x++) {
        if(prueba == true) {
            glVertex2f(p.y,p.x);
        }else{
            glVertex2f(p.x,p.y);
        }
        
        pasoDi+= dy;
        
        if(2*pasoDi >= dx) {
            p.y+= di; 
            pasoDi = pasoDi - dx;
        }
    }
}


void draw_line(Point p0, Point p1) {
    glBegin(GL_POINTS);
    glColor4f(0,0,0,.5);
      
    //dda_line(p0, p1);
    bresenham_line(p0, p1);
    glEnd();
}


static float distance(Point a, Point b) {
    int dx = a.x-b.x, dy = a.y-b.y;
    return sqrt(dx*dx+dy*dy);
}

void subdivision(curve_func_t f, float A,float B){
 
    auto puntoA = f(A); ///puntoA
    auto puntoB = f(B); ///puntoB
    float tm = ( A + B ) / 2; ///el valor medio entre t y el aumento de t
    auto puntoC = f(tm);    ///punto de la curva
    Point puntoM = (puntoA.p + puntoB.p) / 2; ///punto medio 
    
    float distancia = distance( puntoC.p, puntoM) ;
    if( abs( distancia) < 0.5 ){ ///si la distancia de C a M es menor a medio pixel
        ///dibujo la linea entre el punto A y el punto B
//        glBegin(GL_LINES);
//        glVertex2f( redon(puntoA.p.x) , redon(puntoA.p.y) );   ///el punto A
//        glVertex2f( redon(puntoB.p.x) , redon(puntoB.p.y) );   ///el punto B
//        glEnd();
        bresenham_line(puntoA.p, puntoB.p);
    }else{
        /*se almacena el par (C,tm) entremedio de A y B: next(A,ta) = (C,tm) y 
        next(C,tm) = (B,tb) repitiendo el tramo. El proceso termina
        cuando no hay tramo siguiente (next(B,tb) == NULL). Finalmente, 
        se rasteriza (Bresenham) la secuencia de tramos rectos consecutivos.        */
        /// llamo recursivamente la funcion entre los puntos de puntoA y puntoC
        /// llamo recursivamente la funcion entre los puntos de puntoC y puntoB
        subdivision( f, A, tm );
        subdivision( f, tm, B );
    }
}
    
    
void dda_curva(curve_func_t f) {
    //BEZIER RASTERING
    float t=0;
    while( t < 1 ){
            
        auto puntoA = f(t); //punto 0
        float diferencial = 1 / abs(std::max( puntoA.d.x , puntoA.d.y ));    //aumento de t
        t+=diferencial; //aumento t
        auto puntoB = f(t); ///punto siguiente
            
            //si alguno de los dos valores es igual se estaria pintando el mismo lugar dos veces
        if( redon(puntoB.p.x) != redon(puntoA.p.x) ){ 
            glVertex2f( redon(puntoA.p.x) , redon(puntoB.p.y) );///pinto el puntoA           
        }else if( redon(puntoB.p.y) != redon(puntoA.p.y) ){
            glVertex2f( redon(puntoA.p.x) , redon(puntoB.p.y) );
        }
            //no salto mas de un pixel, evito que se genere una desconexion de un pixel
        if( abs(redon(puntoB.p.x) - redon(puntoA.p.x) ) > 1 ){
            t = t - diferencial/2;
            }else if( abs(redon(puntoB.p.y) - redon(puntoA.p.y)) > 1){
                t = t - diferencial/2;
            }
        }
}

void draw_curve(curve_func_t f) {
    glBegin(GL_POINTS);
    //subdivision(f,0,1);
    dda_curva(f);
   
    glEnd();
}






