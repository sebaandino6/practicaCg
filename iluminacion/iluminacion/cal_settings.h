#ifndef CAL_SETTINGS_H
#define CAL_SETTINGS_H

// configuraciones generales del modelo de color e iluminación
struct general_settings_t {
  bool enable_lighting = true;
  bool local_viewer = true;
  float ambient_light[4] = {.2f,.2f,.2f,1.f};
  bool enable_color_material = true;
  float color[4] = {0.f,1.0f,0.0f,1.f};
  void Apply() const;
};

// configuraciones por luz
struct light_settings_t {
	bool enable = false;
	float position[4] = { 0.f, 0.f, 0.f, 1.f };
	float ambient[4]  = { 1.f, 1.f, 0.f, 1.f};
	float diffuse[4]  = { 0.f, 0.f, 0.f, 1.f};
	float specular[4] = { 0.f, 0.f, 0.f, 1.f};
  void Apply(GLenum light_num) const;
};

// configuración de un material
struct material_settings_t {
	float emission[4] = {0.0f,0.0f, 0.f ,1.f};
	float ambient[4]  = {1.0f,1.0f, 0.f ,1.f};
	float diffuse[4]  = {1.0f,1.0f, 0.f ,1.f};
	float specular[4] = {0.0f,0.0f ,1.0f,1.f};
	int shininess = 50;//q de brillo
  void Apply() const;
};

// agrupar todas las configuraciones en un solo gran struct
struct cal_settings_t {
  general_settings_t general;
  light_settings_t light0;
  light_settings_t light1;
  material_settings_t material;
	void Apply() const;
};

extern cal_settings_t settings;

#endif

