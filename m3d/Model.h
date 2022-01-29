#ifndef MODEL_H
#define MODEL_H
#include <cstdio>
#include <cstdint>

struct DBV {
	double x;
	double y;
	double z;
};

struct DBM {
	double a[9];
};

struct Vertex {
	char x_8,y_8,z_8;
	unsigned char yscr_8;
	unsigned char xscr_8;
	unsigned char zscr_8;
};

struct Normal {
	char x,y,z;
	unsigned char I_8,n_power;
};

struct Polygon {
	Vertex* vertices[3];
	Normal* normals[3];
	unsigned char color_id;
	char middle_x,middle_y,middle_z;
};

struct Model {
	int num_vert;
	Vertex* vertices;
	int num_norm;
	Normal* normals;
	int num_poly;
	Polygon* polygons;

	int xmax,ymax,zmax;
	int xmin,ymin,zmin;
	int x_off,y_off,z_off;
	int rmax;
	int memory_allocation_method;

	double volume;
	DBV rcm;
	DBM J;
};

template<typename T>
void read(std::FILE* f, T& value){
	fread(&value, sizeof(T), 1, f);
}

template<>
void read<DBM>(std::FILE* f, DBM& M);

template<>
void read<DBV>(std::FILE* f, DBV& D);

template<>
void read<Vertex>(std::FILE* f, Vertex& vertex);

template<>
void read<Normal>(std::FILE* f, Normal& normal);

template<>
void read<Model>(std::FILE* f, Model& model);

#endif // MODEL_H
