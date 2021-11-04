#ifndef P4F_H
#define P4F_H


///////////////////////////////////////////////////////////
// clase: puntos de cuatro floats
// MANEJESE CON CUIDADO!!!
// 4 para poder hacer racionales y porque GLU pide 3D(x,y,z) o 4D(x,y,z,w) pero no (x,y,w)
// floats porque GLU pide floats
// tiene solo lo necesario para usarlos aca
// z siempre es 0
// Cuidado con w=0: los puntos visbles son <wx,w>
const size_t SZF=sizeof(float);
class p4f{
	float c[4];
public:
	p4f(double x=0, double y=0, double z=0, double w=1){
		c[0]=x; c[1]=y; c[2]=z; c[3]=w;
	}
	p4f(const p4f &p){memcpy(c,p.c,4*SZF);}
	
	operator const float*() const {return c;}
	
	p4f& operator=(const p4f &p){memcpy(c,p.c,4*SZF); return *this;}
	
	float& operator[](int i) {return c[i];}
	const float& operator[](int i) const {return c[i];}
	
	// modifica el peso pero mantiene el punto en el mismo lugar de R3
	// ==> OJO: no admite peso 0 !!!!!!!!!!!!!!!!!
	// (para hacer peso=0 poner p[3]=0 y ya esta)
	p4f& peso(float w){
		if (fabs(w)<1e-7) {if (w<0) w=-1e-7; else w=1e-7;}
		float f=w/c[3];
		c[0]*=f; c[1]*=f; c[2]*=f; c[3]=w; return *this;
	}
		
		bool cerca2D(const p4f &p, float r=1){
			float cw=c[3],pw=p[3];
			return (fabs(c[0]/cw-p[0]/pw)<=r && fabs(c[1]/cw-p[1]/pw)<=r);
		}
			
			p4f& neg(){c[0]=-c[0]; c[1]=-c[1]; c[2]=-c[2]; c[3]=-c[3]; return *this;}
			p4f operator -() const {return p4f(-c[0],-c[1],-c[2],-c[3]);}
			
			// asigna el punto interpolado
			p4f& lerp(const p4f& p0,const p4f& p1,float u){
				c[0]=(1-u)*p0[0]+u*p1[0];
				c[1]=(1-u)*p0[1]+u*p1[1];
				c[2]=(1-u)*p0[2]+u*p1[2];
				c[3]=(1-u)*p0[3]+u*p1[3];
				return *this;
			}
				
				float mod() const {
					if (c[3]!=0) return sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2])/c[3]; // punto: distancia al origen
					else return sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]); // vector: modulo
				}
				p4f& dir() {return operator/=(mod());}
				
				p4f operator -(const p4f &p) const{
					return p4f(c[0]-p[0],c[1]-p[1],c[2]-p[2],c[3]-p[3]);
				}
				p4f& operator -=(const p4f &p) {
					c[0]-=p[0];c[1]-=p[1];c[2]-=p[2];c[3]-=p[3]; return *this;
				}
				p4f operator +(const p4f &p) const{
					return p4f(c[0]+p[0],c[1]+p[1],c[2]+p[2],c[3]+p[3]);
				}
				p4f& operator +=(const p4f &p) {
					c[0]+=p[0];c[1]+=p[1];c[2]+=p[2];c[3]+=p[3]; return *this;
				}
				p4f operator *(float f) const{
					return p4f(c[0]*f,c[1]*f,c[2]*f,c[3]*f);
				}
				p4f &operator *=(float f) {
					c[0]*=f;c[1]*=f;c[2]*=f;c[3]*=f; return *this;
				}
				friend p4f operator*(float f,const p4f &p) {return (p*f);}
				p4f operator /(float f) const{
					return p4f(c[0]/f,c[1]/f,c[2]/f,c[3]/f);
				}
				p4f &operator /=(float f) {
					c[0]/=f;c[1]/=f;c[2]/=f;c[3]/=f; return *this;
				}
				friend p4f operator/(float f,const p4f &p) {return (p/f);}
				
				p4f &giro_z_90(){float t=c[0]; c[0]=-c[1]; c[1]=t; return *this;}
};

#endif
