#ifndef DRAWOBJECTS_H
#define DRAWOBJECTS_H

float lAmbient[] ={.2,.2,.2}, // luz ambiente
	  lDiffuse[] ={.7,.7,.7}, // luz difusa
	  lSpecular[]={.9,.9,.9}, // luz especular
	  cTeapotAmbient[] = {.3,.25,.1},
	  cTeapotDiffuse[] = {.6,.5,.2},
	  cTeapotSpecular[]= {.1,.1,.1},
	  cTeapotShininess = 16,
	  cFloorAmbient[] = {.3,.3,.3,.75},
	  cFloorDiffuse[] = {.6,.6,.6,.75},
	  cFloorSpecular[]= {.9,.9,.9,.75},
	  cFloorShininess = 32,
	  cLineAmbient[] = {.1,.1,.1, 0},
	  cLineDiffuse[] = {.2,.2,.2, 0},
	  cLineSpecular[]= {.2,.2,.2, 0},
	  cLineShininess = 8,
	  floorLod=24.;

void drawTeapots(float tiempo){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMaterialf(GL_FRONT, GL_SHININESS, cTeapotShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, cTeapotAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cTeapotDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cTeapotSpecular);
	
	glPushMatrix();
	glTranslatef(1.f,.45f,0.f);
	glRotatef(tiempo/50, 0.0, 1.0, 0.0);
	glRotatef(10*sin(tiempo/500)-10, 0.0, 0.0, 1.0);
	glutSolidTeapot(.45);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.f,.45f,0.f);
	glRotatef(tiempo/50, 0.0, 1.0, 0.0);
	glRotatef(10*sin(tiempo/500)-10, 0.0, 0.0, 1.0);
	glutSolidTeapot(.45);
	glPopMatrix();
	
	glPopAttrib();
}

void drawLight(float* lpos){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	
	glDisable(GL_LIGHTING);
	glColor3f(1.,1.,1.);
	glTranslatef(lpos[0], lpos[1], lpos[2]);
	
	glutSolidSphere(.05f, 10, 10);
	
	glPopMatrix();
	glPopAttrib();
}

void drawFloor(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(glIsEnabled(GL_LIGHTING)) {
		glMaterialf(GL_FRONT, GL_SHININESS, cFloorShininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, cFloorAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, cFloorDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, cFloorSpecular);
	} else
		glColor4fv(cFloorAmbient);
	
	float step = 4 / floorLod;
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
	for(float x=-2; x<2-step/2; x+=step) {
		for(float z=-2; z<2-step/2; z+=step) {
			glNormal3f(0,1,0); glVertex3f(x,	  	0, z    );
			glNormal3f(0,1,0); glVertex3f(x+step,	0, z    );
			glNormal3f(0,1,0); glVertex3f(x+step,	0, z+step);
			glNormal3f(0,1,0); glVertex3f(x,	  	0, z+step);
		}
	}
	glEnd();
	
	if(glIsEnabled(GL_LIGHTING)) {
		glMaterialf(GL_FRONT, GL_SHININESS, cLineShininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, cLineAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, cLineDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, cLineSpecular);
	} else
		glColor4fv(cLineAmbient);
		
	glDisable(GL_BLEND);
	for(float x=-2; x<2-step/2; x+=step) {
		for(float z=-2; z<2-step/2; z+=step) {
			glBegin(GL_LINE_LOOP);
			glNormal3f(0,1,0); glVertex3f(x,	  	0, z    );
			glNormal3f(0,1,0); glVertex3f(x+step,	0, z    );
			glNormal3f(0,1,0); glVertex3f(x+step,	0, z+step);
			glNormal3f(0,1,0); glVertex3f(x,	  	0, z+step);
			glEnd();
		}
	}
	glPopAttrib();
}

#endif
