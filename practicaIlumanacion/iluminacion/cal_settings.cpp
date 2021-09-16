#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif

#include "cal_settings.h"

cal_settings_t settings;


void general_settings_t::Apply ( ) const {
  
  if (enable_lighting) {
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local_viewer?GL_TRUE:GL_FALSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
    
  } else {
    glDisable(GL_LIGHTING);
  }
  
  if (enable_color_material) {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  } else {
    glDisable(GL_COLOR_MATERIAL);
  }
  
  glColor4fv(color);
  
}

void light_settings_t::Apply (GLenum light_num) const {
  if (enable) {
    glLightfv(light_num,GL_POSITION,position);
    glEnable(light_num);
    glLightfv(light_num,GL_AMBIENT, ambient);
    glLightfv(light_num,GL_DIFFUSE, diffuse);
    glLightfv(light_num,GL_SPECULAR,specular);
  } else {
    glDisable(light_num);
  }
  
}

void material_settings_t::Apply ( ) const {
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,emission);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
  glMateriali (GL_FRONT_AND_BACK,GL_SHININESS,shininess);
}


void cal_settings_t::Apply ( ) const {
  general.Apply();
	if (general.enable_lighting) {
    light0.Apply(GL_LIGHT0);
    light1.Apply(GL_LIGHT1);
    material.Apply();
  }
}

