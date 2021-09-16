#ifndef FAS_SETTINGS_H
#define FAS_SETTINGS_H
#include <GL/gl.h>
#include "fas_settings.h"

enum class FogMode { Linear, Exp, Exp2 };

struct fog_settings_t {
	bool enable = true;
	FogMode mode = FogMode::Linear;
	GLfloat color[4] = {0.2, 0.2, 0.2, 1.0};
	GLfloat start = 0.f;
	GLfloat end = 20.f;
	GLfloat density = 0.1f;
	
	void Apply();
	const char *GetModeName() const;
};

struct spot_settings_t {
	
	bool enable = true;
	GLfloat cutoff = 50.f;
	GLfloat exponent = 8.f;
	GLfloat constant_attenuation = 1.f;
	GLfloat linear_attenuation= 0.f;
	GLfloat quadratic_attenuation= 0.f;
	
	void Apply(GLint light_num);
	
};

// agrupar todas las configuraciones en un solo gran struct
struct settings_struct_t {
	fog_settings_t fog;
	spot_settings_t spot;
	void Apply(GLint light_num);
};

extern settings_struct_t settings;


#endif

