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
       
    glColor4f(0,0,0,.5);
    
//    //Bresenham
//        float dy = (p1.y-p0.y);
//        float dx = (p1.x-p0.x);
//        float di;
//        float m;
//        float pasoDi;
//    
//    //Como se si la curva es suficientemente suave?
//    
//        if(abs(dx) < abs(dy)) {
//                    
//            if(dy < 0){
//                std::swap(p0,p1);
//            }
//            
//            for( Point p = p0; p.y <= p1.y; ++p.y) {
//                //acá debería tener un while
//            }       
//          
//        } else {
//            
//          if (dx >=0) {
//            m = dx/dy;
//              
//            
//            if(dx == 0) {
//                glVertex2f(p0.x,p0.y);
//                
//            } 
//                  
//            glVertex2f(p0.x,p0.y);
//            di = 0;
//            while(p.x < p1.x){
//                      
//                float h = di + m;
//                if( ((2*dx*di) + (2*dy) - dx) < 0) {
//                        //Pinto E
//                        glVertex2f(p.x+1, p.y);
//                        di += h;
//                        pasoDi = 2*dx*di + 2*dy;  
//                    }else{
//                        //Pinto NE
//                        glVertex2f(p.x+1, p.y+1);
//                        //di += h - 1;
//                       /* pasoDi = 2*dx*di + 2*dy - 2*dx;*/
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
        
//    
//    //DDA
//    float dy = (p1.y-p0.y);
//    float dx = (p1.x-p0.x);
// 
//    //Como se si la curva es suficientemente suave?
//    
//    if(abs(dx) < abs(dy)) {
//        if(dy!=0){
//            if(dy < 0){
//                std::swap(p0,p1);
//            }
//                
//            float m = dx/dy;
//            for( Point p = p0; p.y <= p1.y; ++p.y) {
//                glVertex2f(p.x,redon(p.y));
//                p.x+= m;
//            }    
//        }    
//    }else{
//        if(dx!=0){
//            if(dx < 0){
//                std::swap(p0,p1);
//            }
//        
//            float m = dy/dx;
//    
//            for( Point p = p0; p.x <= p1.x; ++p.x) {
//                glVertex2f(redon(p.x),p.y);
//                p.y+= m;
//            }
//        }
//    }
    glEnd();
}


void draw_curve(curve_func_t f) {
    glBegin(GL_POINTS);
    
    /// @TODO: implementar algun algoritmo de rasterizacion de curvas
    
    // f es la función de la curva, recibe un t entre 0 y 1 y retorna
    // el punto y la derivada en ese punto, por ej:
    //    auto r = f(0.5); // calcular en el punto "medio" de la curva
    // entonces r.p es el punto (tipo Point) y r.d la derivada (tipo Vector)
    
    
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






