#include "Model.h"
#include <ctype.h>

template<>
void read<DBM>(std::FILE* f, DBM& M) {
	for(int i = 0; i < 9; i++){
		read(f, M.a[i]);
	}
}



template<>
void read<DBV>(std::FILE* f, DBV& D) {
	read(f, D.x);
	read(f, D.y);
	read(f, D.z);
}

template<>
void read<Vertex>(std::FILE* f, Vertex& vertex) {
	float tf;
	int sort_info;
	read(f, tf);
	read(f, tf);
	read(f, tf);
	read(f, vertex.x_8);
	read(f, vertex.y_8);
	read(f, vertex.z_8);
	read(f, sort_info);
}

template<>
void read<Normal>(std::FILE* f, Normal& normal) {
	int sort_info;
	read(f, normal.x);
	read(f, normal.y);
	read(f, normal.z);
	read(f, normal.n_power);
	read(f, sort_info);
}


template<>
void read<Model>(std::FILE* f,  Model& model) {
	//	Vertex* vertices, Normal* normals;
	int32_t num_vert_total, version;
	int phi,psi,tetta;

	read(f, version);
	read(f, model.num_vert);
	read(f, model.num_norm);
	read(f, model.num_poly);
	read(f, num_vert_total);
	read(f, model.xmax);
	read(f, model.ymax);
	read(f, model.zmax);
	read(f, model.xmin);
	read(f, model.ymin);
	read(f, model.zmin);
	read(f, model.x_off);
	read(f, model.y_off);
	read(f, model.z_off);
	read(f, model.rmax);
	read(f, phi);
	read(f, psi);
	read(f, tetta);
	read(f, model.volume);
	read(f, model.rcm);
	read(f, model.J);

	model.vertices = new Vertex[model.num_vert];
	model.normals = new Normal[model.num_norm];
	model.polygons = new Polygon[model.num_poly];

	for(int i = 0; i < model.num_vert; i++){
		read(f, model.vertices[i]);
	}

	for(int i = 0; i < model.num_norm; i++){
		read(f, model.normals[i]);
	}

	for(int i = 0; i < model.num_poly; i++){
		Polygon& polygon = model.polygons[i];

		int32_t num;
		int32_t sort_info;
		int8_t tchar[4];
		uint32_t color_shift;
		uint32_t color_id;

		read(f, num );
		read(f, sort_info);
		read(f, color_id);
		polygon.color_id = color_id;
		read(f, color_shift);

		read(f, tchar[0] );

		read(f, tchar[1] );

		read(f, tchar[2] );

		read(f, tchar[3]);

		read(f, polygon.middle_x);
		read(f, polygon.middle_y);
		read(f, polygon.middle_z);

		for(int index = 0; index < num; index++){
			int vert_ind;
			int norm_ind;
			read(f, vert_ind);
			read(f, norm_ind);
			polygon.vertices[index] = &model.vertices[vert_ind];
			polygon.normals[index] = &model.normals[norm_ind];
		}
	}

	fseek(f, 3 * model.num_poly * 4, SEEK_CUR);
}

template<>
void read<Wheel>(FILE* f, Wheel& wheel)
{
	read(f, wheel.steer);
	read(f, wheel.r);
	read(f, wheel.width);
	read(f, wheel.radius);
	read(f, wheel.bound_index);
	if(wheel.steer){
		read(f, wheel.model);
	}
	wheel.dZ = 0;
}

template<>
void read<Object>(FILE* f, Object& object)
{
	read(f, object.body);
	read(f, object.xmax);
	read(f, object.ymax);
	read(f, object.zmax);
	read(f, object.rmax);
	read(f, object.n_wheels);
	read(f, object.n_debris);
	read(f, object.body_color_offset);
	read(f, object.body_color_shift);
	if(object.n_wheels){
		object.wheels = new Wheel[object.n_wheels];
		for(int i = 0; i < object.n_wheels; i++){
			read(f, object.wheels[i]);
		}
	}
}
