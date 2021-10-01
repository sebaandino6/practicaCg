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
    /// @TODO: implementar algun algoritmo de rasterizacion de segmentos
    glBegin(GL_POINTS);
//    
    glColor4f(0,0,0,.5);
//    
    ///Bresenham
//    float dy = (p1.y-p0.y);
//    float dx = (p1.x-p0.x);
//    float di;
//    float m;
//    float pasoDi;
//    
//    //Como se si la curva es suficientemente suave?
//    
//    if(abs(dx) < abs(dy)) {
//        
//        if(dy < 0){
//            std::swap(p0,p1);
//        }
//        
//        for( Point p = p0; p.y <= p1.y; ++p.y) {
//            //acá debería tener un while
//        }       
//        
//    } else {
//        if (dx >=0) {
//            m = dx/dy;
//            
//            for( Point p = p0; p.y <= p1.y; ++p.y) {
//                if(dx == 0) {
//                    glVertex2f(p0.x,p0.y);
//                    
//                } 
//                
//                glVertex2f(p0.x,p0.y);
//                di = 0;
//                while(p.x < p1.x){
//                    
//                    float h = di + m;
//                    if( ((2*dx*di) + (2*dy) - dx) < 0) {
//                        //Pinto E
//                        glVertex2f(p.x+1, p.y);
//                        di += h;
//                        pasoDi = 2*dx*di + 2*dy;  
//                    }else{
//                        //Pinto NE
//                        glVertex2f(p.x+1, p.y+1);
//                        //di += h - 1;
//                        pasoDi = 2*dx*di + 2*dy - 2*dx;
//                    }
//                    
//                    if(di == 0){
//                        glVertex2f(p.x,p.y);
//                        pasoDi = 0;
//                        
//                    }
//                    
//                    di = pasoDi;
//                }
//                
//            }
//            
//        }   
//    }
    
    
    ///DDA
        float dy = (p1.y-p0.y);
        float dx = (p1.x-p0.x);
    ///Como se si la curva es suficientemente suave?
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
    
    if( abs( distance( puntoC.p, puntoM) ) < 0.5 ){ ///si la distancia de C a M es menor a medio pixel
        ///dibujo la linea entre el punto A y el punto B
        glBegin(GL_LINES);
        glVertex2f( redon(puntoA.p.x) , redon(puntoA.p.y) );   ///el punto A
        glVertex2f( redon(puntoB.p.x) , redon(puntoB.p.y) );   ///el punto B
        glEnd();
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

void draw_curve(curve_func_t f) {
    glBegin(GL_POINTS);
    /// @TODO: implementar algun algoritmo de rasterizacion de curvas
    
    // f es la función de la curva, recibe un t entre 0 y 1 y retorna
    // el punto y la derivada en ese punto, por ej:
    //    auto r = f(0.5); // calcular en el punto "medio" de la curva
    // entonces r.p es el punto (tipo Point) y r.d la derivada (tipo Vector)
    
    ///SUBDIVISION
    subdivision(f,0,1);
        
    ///BEZIER RASTERING
//    float t=0;
//    while( t < 1 ){
//        
//        auto puntoA = f(t); ///punto 0
//        float diferencial = 1 / abs(std::max( puntoA.d.x , puntoA.d.y ));    ///aumento de t
//        t+=diferencial; ///aumento t
//        auto puntoB = f(t); ///punto siguiente
//        
//        ///si alguno de los dos valores es igual se estaria pintando el mismo lugar dos veces
//        if( redon(puntoB.p.x) != redon(puntoA.p.x) ){ 
//            glVertex2f( redon(puntoA.p.x) , redon(puntoB.p.y) );///pinto el puntoA           
//        }else if( redon(puntoB.p.y) != redon(puntoA.p.y) ){
//            glVertex2f( redon(puntoA.p.x) , redon(puntoB.p.y) );
//        }
//        ///no salto mas de un pixel, evito que se genere una desconexion de un pixel
//        if( abs(redon(puntoB.p.x) - redon(puntoA.p.x) ) > 1 ){
//            t = t - diferencial/2;
//        }else if( abs(redon(puntoB.p.y) - redon(puntoA.p.y)) > 1){
//            t = t - diferencial/2;
//        }
//    }
    
    
    float t=0;
    auto r = f(t);
    auto d = r.d;
    int subdivisiones = curve_degree;
    
    for(int i = 0; i < subdivisiones - 1; i++ ) {
        vector puntos[i] = f(t/i)
    }
    
    for(i = 0; i < subdivisiones - 1; i++) {
        float diferencial = std::max( puntos[i].x, puntos[i].y ); 
        auto puntoA = puntos[i];
        auto puntoB = puntos(i+1); //aumento el t y me da el punto que sigue
        float tm = ( i + (i+1/diferencial)) / 2; 
        auto puntoC = punto(m);
        auto puntoM = (puntoA+puntoB)/2;

        if( abs(distance(puntoC, puntoM)) < .5 ){
            glVertex2f(M.x,M.y);
            
        }else{
            
        }
        
        gl

        
        
    }
    
    while( t < 1 ){
            
           float diferencial = 1 / std::max( r.p.x, r.p.y ); ; 
            auto puntoA = f(t);
            float tm = ( t + (t+1/diferencial)) / 2; 
            t=+diferencial;
            auto puntoB = f(t); //aumento el t y me da el punto que sigue
            auto puntoC = f(tm);
    /*        auto puntoM = (puntoA+puntoB)/2;*/
            
            glVertex2f(M.x,M.y);
    
            if( abs(distance(puntoC, puntoM)) < .5 ){
                
                
            }else{
                
            }
    
    glEnd();
}






