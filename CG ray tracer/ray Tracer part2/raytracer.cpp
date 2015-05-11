#include <fstream>
#include <algorithm>
#include <cstdlib> 
#include <cstdio>
#include <cmath>
#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265

using namespace std;
unsigned width = 320, height = 320;



class Vector
{
public:
	float x, y, z;
	Vector() : x(float(0)), y(float(0)), z(float(0)) 
	{}
	Vector(float xx) : x(xx), y(xx), z(xx) 
	{}
	Vector(float xx, float yy, float zz) : x(xx), y(yy), z(zz)
	{}
	Vector& normalize()
	{
		float norSquare = lengthSquare();
		if (norSquare > 0) {
			float invNor = 1 / sqrt(norSquare);
			x *= invNor, y *= invNor, z *= invNor;
		}
		return *this;
	}

	Vector operator + (const Vector &v) const 
	{ 
		return Vector(x + v.x, y + v.y, z + v.z); 
	}

	Vector operator - (const Vector &v) const 
	{ 
		return Vector(x - v.x, y - v.y, z - v.z); 
	}

	float dot(const Vector &v) const 
	{
		return x * v.x + y * v.y + z * v.z; 
	}

	Vector operator * (const float &f) const 
	{
		return Vector(x * f, y * f, z * f); 
	}

	Vector operator * (const Vector &v) const 
	{ 
		return Vector(x * v.x, y * v.y, z * v.z); 
	}
	
	Vector& operator += (const Vector &v) 
	{
		x += v.x, y += v.y, z += v.z; 
		return *this; 
	}

	Vector& operator *= (const Vector &v) 
	{ 
		x *= v.x, y *= v.y, z *= v.z; 
		return *this; 
	}
	Vector operator - () const 
	{ 
		return Vector(-x, -y, -z); 
	}

	float lengthSquare() const 
	{
		return x * x + y * y + z * z;
	}

	float length() const 
	{ 
		return sqrt(lengthSquare()); 
	}
	// required for saving the images
	friend std::ostream & operator << (std::ostream &os, const Vector &v)
	{
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}

};
Vector *scene = new Vector[width * height], *pixel = scene; //array to store colour info for all pixels


class Tetrahedron
{
public:
	Vector vx1, vx2, vx3, vx4;
	Tetrahedron(const Vector &v1, const Vector &v2,const Vector &v3, const Vector &v4) : vx1(v1) , vx2(v2), vx3(v3), vx4(v4)
	{}

	//for barycentric co-ordinates of the ray triangle intersection
	bool intersect(const Vector &v1, const Vector &v2, const Vector &v3, const Vector &rayorig, const Vector &raydir, Vector* cg)
	{
		cg->x = (v2.y * raydir.z * rayorig.x + raydir.y * v2.x * rayorig.z - v2.z *
			raydir.y * rayorig.x - raydir.z * v2.x * rayorig.y - raydir.x * v2.y * rayorig.z + raydir.x *
			v2.z * rayorig.y - v2.y * raydir.z * v3.x - raydir.y * v2.x * v3.z +
			v2.z * raydir.y * v3.x + raydir.z * v2.x * v3.y + raydir.x * v2.y * v3.z
			- raydir.x * v2.z * v3.y + raydir.y * rayorig.x * v3.z - v3.y * raydir.z * rayorig.x
			- raydir.y * v3.x * rayorig.z + rayorig.y * raydir.z * v3.x + raydir.x * v3.y * rayorig.z
			- raydir.x * rayorig.y * v3.z) / (v2.y * raydir.z * v1.x - v2.y * raydir.z *
			v3.x + raydir.y * v2.x * v1.z + v1.y * raydir.z * v3.x - raydir.z * v2.x
			* v1.y + raydir.z * v2.x * v3.y - raydir.z * v1.x * v3.y - raydir.x *
			v1.y * v3.z + raydir.x * v1.z * v3.y - raydir.x * v2.y * v1.z + raydir.x
			* v2.y * v3.z - raydir.x * v2.z * v3.y - raydir.y * v2.x * v3.z +
			raydir.y * v1.x * v3.z - v1.z * raydir.y * v3.x + v2.z * raydir.y * v3.x
			- v2.z * raydir.y * v1.x + raydir.x * v2.z * v1.y); cg->y = -(raydir.x *
			v1.y * v3.z - raydir.x * v1.y * rayorig.z - raydir.x * v1.z * v3.y + raydir.x
			* v3.y * rayorig.z + raydir.x * v1.z * rayorig.y - raydir.x * rayorig.y * v3.z - v1.z
			* raydir.y * rayorig.x - raydir.y * v3.x * rayorig.z - v3.y * raydir.z * rayorig.x + v1.y
			* raydir.z * rayorig.x + raydir.y * v1.x * rayorig.z - v1.y * raydir.z * v3.x + raydir.y
			* rayorig.x * v3.z - raydir.y * v1.x * v3.z + raydir.z * v1.x * v3.y - raydir.z
			* v1.x * rayorig.y + rayorig.y * raydir.z * v3.x + v1.z * raydir.y * v3.x) /
			(v2.y * raydir.z * v1.x - v2.y * raydir.z * v3.x + raydir.y * v2.x *
			v1.z + v1.y * raydir.z * v3.x - raydir.z * v2.x * v1.y + raydir.z *
			v2.x * v3.y - raydir.z * v1.x * v3.y - raydir.x * v1.y * v3.z + raydir.x
			* v1.z * v3.y - raydir.x * v2.y * v1.z + raydir.x * v2.y * v3.z -
			raydir.x * v2.z * v3.y - raydir.y * v2.x * v3.z + raydir.y * v1.x * v3.z
			- v1.z * raydir.y * v3.x + v2.z * raydir.y * v3.x - v2.z * raydir.y *
			v1.x + raydir.x * v2.z * v1.y); cg->z = (-raydir.z * v2.x * v1.y +
			raydir.z * v2.x * rayorig.y - raydir.z * v1.x * rayorig.y + raydir.x * v2.y * rayorig.z +
			v1.y * raydir.z * rayorig.x + v2.y * raydir.z * v1.x - v2.y * raydir.z * rayorig.x +
			raydir.y * v2.x * v1.z - raydir.y * v2.x * rayorig.z + raydir.y * v1.x * rayorig.z
			- v1.z * raydir.y * rayorig.x + v2.z * raydir.y * rayorig.x - v2.z * raydir.y * v1.x
			- raydir.x * v2.y * v1.z - raydir.x * v1.y * rayorig.z + raydir.x * v1.z *
			rayorig.y + raydir.x * v2.z * v1.y - raydir.x * v2.z * rayorig.y) / (v2.y * raydir.z
			* v1.x - v2.y * raydir.z * v3.x + raydir.y * v2.x * v1.z + v1.y *
			raydir.z * v3.x - raydir.z * v2.x * v1.y + raydir.z * v2.x * v3.y - raydir.z
			* v1.x * v3.y - raydir.x * v1.y * v3.z + raydir.x * v1.z * v3.y -
			raydir.x * v2.y * v1.z + raydir.x * v2.y * v3.z - raydir.x * v2.z * v3.y
			- raydir.y * v2.x * v3.z + raydir.y * v1.x * v3.z - v1.z * raydir.y *
			v3.x + v2.z * raydir.y * v3.x - v2.z * raydir.y * v1.x + raydir.x * v2.z
			* v1.y);
	}
};

class Sphere
{
public:
	Vector center;                         
	float radius, radiusSquare;                      
	Vector surfaceColor;    
	float reflection;             
	Sphere(const Vector &c, const float &r, const Vector &sc,
		const float &refl = 0) :
		center(c), radius(r), radiusSquare(r * r), surfaceColor(sc),
		reflection(refl)
	{}

	bool intersect(const Vector &rayorig, const Vector &raydir, float *t0 = NULL, float *t1 = NULL) const
	{
		Vector l = center - rayorig;
		float temp1 = l.dot(raydir);
		if (temp1 < 0) return false;
		float d2 = l.dot(l) - temp1 * temp1;
		if (d2 > radiusSquare) return false;
		float temp2 = sqrt(radiusSquare - d2);
		if (t0 != NULL && t1 != NULL) {
			*t0 = temp1 - temp2;
			*t1 = temp1 + temp2;
		}

		return true;
	}
};

#define MAX_RAY_DEPTH 5 //limiting the recursive call to getPixelColour


Vector getPixelColour(const Vector &rayorig, const Vector &raydir,
	const Sphere* sphere1, const Sphere * sphere2, const int &depth)
{

	float tnear = INFINITY;
	const Sphere *sphere = NULL;


	float t0 = INFINITY, t1 = INFINITY;
	if (sphere1->intersect(rayorig, raydir, &t0, &t1)) {
		if (t0 < 0) t0 = t1;
		if (t0 < tnear) {
			tnear = t0;
			sphere = sphere1;
		}
	}

	if (sphere2->intersect(rayorig, raydir, &t0, &t1)) {
		if (t0 < 0) t0 = t1;
		if (t0 < tnear) {
			tnear = t0;
			sphere = sphere2;
		}
	}

	if (!sphere) return Vector(2);
	Vector surfaceColor = 0;
	Vector pointHit = rayorig + raydir * tnear;
	Vector surfNormal = pointHit - sphere->center;
	surfNormal.normalize();



	if (raydir.dot(surfNormal) > 0) surfNormal = -surfNormal;
	if (sphere->reflection > 0 && depth < MAX_RAY_DEPTH) {

		float viewAngle = -raydir.dot(surfNormal);
		
		float reflectIndex = (pow(1 - viewAngle, 3))*0.9 + 0.1;//reflection index
		Vector reflectDir = raydir - surfNormal * 2 * raydir.dot(surfNormal);
		reflectDir.normalize();

		Vector reflection = getPixelColour(pointHit + surfNormal, reflectDir, sphere1, sphere2, depth + 1);


		Vector lightDirection(1, 1, 0);
		lightDirection.normalize();

		Vector surfaceColor1 = sphere->surfaceColor * std::max(float(0), surfNormal.dot(lightDirection));// lambertian shading

		Vector h = (lightDirection + raydir);
		h.normalize();
		Vector surfaceColor2 = sphere->surfaceColor * pow(std::max(float(0), surfNormal.dot(h)), 9); //blinnphong effect

		surfaceColor = (reflection * reflectIndex) * sphere->surfaceColor + surfaceColor1 + surfaceColor2;
	}
	else {


		Vector lightDirection(1, 1, 0);
		lightDirection.normalize();

		Vector h = (lightDirection + raydir);
		h.normalize();
		Vector surfaceColor2 = sphere->surfaceColor * pow(std::max(float(0), surfNormal.dot(h)), 64); //blinnphong effect


		Vector lightDirection2(1, 1, 0);
		lightDirection2.normalize();
		surfaceColor = sphere->surfaceColor * std::max(float(0), surfNormal.dot(lightDirection2)) + surfaceColor2;// lambertian shading

	}

	return surfaceColor + surfaceColor*Vector(0.4); //adding ambient light
}


void fillScene(const Sphere* sphere1, const Sphere* sphere2)
{


	for (unsigned y = 0; y<height; ++y) {
		for (unsigned x = 0; x <width; ++x, ++pixel) {
			float angle = -90;
			Vector raydir(cos(angle*PI / 180), 0, sin(angle*PI / 180));
			raydir.normalize();
			*pixel = getPixelColour(Vector(x + (400 * -(cos(angle * PI / 180))), y, 400 * -(sin(angle * PI / 180))), raydir, sphere1, sphere2, 0);
		}
	}


	// for saving the images for making the animation
	/*std::ofstream ofs("./nonambient.JPEG", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	
	
	for (unsigned i = 0 ; i < width * height; ++i) {
		ofs << (unsigned char)(std::min(float(1), scene[i].x) * 255) <<
			(unsigned char)(std::min(float(1), scene[i].y) * 255) <<
			(unsigned char)(std::min(float(1), scene[i].z) * 255);
	}
	ofs.close();*/


}




void mydisplay()
{
	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT);


	glDrawPixels(width, height, GL_RGB, GL_FLOAT, scene);

	glutSwapBuffers();


}

int main(int argc, char **argv)
{
	Sphere* sphere1 = new Sphere(Vector(200, 180, -20), 80, Vector(1.00, 0.32, 0.36), 0.5);
	Sphere* sphere2 = new Sphere(Vector(60, 180, -15), 60, Vector(0.10, 0.76, 0.26), 0.0);
	//Tetrahedron * tetrahedron = new Tetrahedron(Vector(120,270,-5), Vector(),Vector(),Vector());

	fillScene(sphere1, sphere2);
	delete sphere1, sphere2;


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);


	glutInitWindowSize(width, height);


	glutInitWindowPosition(0, 0);


	glutCreateWindow("ray tracer!");

	glutDisplayFunc(mydisplay);

	glutMainLoop();


	return 0;
}
