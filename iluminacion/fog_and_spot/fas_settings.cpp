#include "fas_settings.h"


settings_struct_t settings;

void fog_settings_t::Apply() {
	
	glClearColor(color[0], color[1], color[2], color[3]);
	
	if (enable) {
		
		glEnable(GL_FOG);
		
		switch (mode) {
		case FogMode::Linear:
				glFogi (GL_FOG_MODE, GL_LINEAR);
				break;
		case FogMode::Exp:
				glFogi (GL_FOG_MODE, GL_EXP);
				break;
		case FogMode::Exp2:
				glFogi (GL_FOG_MODE, GL_EXP2);
				break;
		}
		
		glFogfv (GL_FOG_COLOR, color);
		
		glFogf (GL_FOG_START, start);
		glFogf (GL_FOG_DENSITY, density);
		glFogf (GL_FOG_END, end);
		
	} else {
		
		glDisable(GL_FOG);
		
	}
}

void spot_settings_t::Apply (GLint light_num) {
	if (enable) {
		
		// acá no se configura color, posición y dirección...
		// solo lo relacionado a la forma y atenuación del spot
		glLightf(light_num, GL_SPOT_CUTOFF, cutoff);
		glLightf(light_num, GL_SPOT_EXPONENT, exponent);
		glLightf(light_num, GL_CONSTANT_ATTENUATION, constant_attenuation);
		glLightf(light_num, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);
		glLightf(light_num, GL_LINEAR_ATTENUATION, linear_attenuation);
	
	} else {
		glLightf(light_num, GL_SPOT_CUTOFF, 180.f);
		glLightf(light_num, GL_SPOT_EXPONENT, 0.f);
		glLightf(light_num, GL_CONSTANT_ATTENUATION, 1.f);
		glLightf(light_num, GL_QUADRATIC_ATTENUATION, 0.f);
		glLightf(light_num, GL_LINEAR_ATTENUATION, 0.f);
		
	}
}


void settings_struct_t::Apply (GLint light_num) {
	fog.Apply();
	spot.Apply(light_num);
}
