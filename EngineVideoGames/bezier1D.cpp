#include "bezier1D.h"
#include <iostream>



#define CUBIC_BEZIER_MAT glm::mat4(glm::vec4(-1.0f, 3.0f,-3.0f,1.0f), glm::vec4(3.0f, -6.0f,3.0f,0.0f), glm::vec4(-3.0f, 3.0f,0.0f,0.0f), glm::vec4(1.0f, 0.0f,0.0f,0.0f))
#define BLUE glm::vec3(0.2667f, 0.3137f, 0.6196f)
#define BLUEL glm::vec3(0.0f, 0.0f, 1.0f)
#define CUBIC_BEZIER_DER_MAT glm::mat4(glm::vec4(0.0f, -3.0f,6.0f,-3.0f), glm::vec4(0.0f, 9.0f,-12.0f,3.0f), glm::vec4(0.0f, -9.0f,6.0f,0.0f), glm::vec4(0.0f, 3.0f,0.0f,0.0f))

void print_mat(glm::mat4 mat_to_print) {
	for (int i = 0; i < 4; i++) {
		std::cout << mat_to_print[i].x << "   " << mat_to_print[i].y << "   "
			<< mat_to_print[i].z << "   " << mat_to_print[i].w << std::endl;
	}
}

Bezier1D::Bezier1D(void)
{
	glm::mat4 mat = glm::mat4(glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 5.0, 0.0, 1.0),
		glm::vec4(5.0, 5.0, 0.0, 1.0), glm::vec4(5.0, 0.0, 0.0, 1.0));
	AddSegment(glm::transpose(mat));

	AddSegment(glm::transpose(glm::mat4(glm::vec4(5.0, 0.0, 0.0, 1.0), glm::vec4(5.0, -5.0, 0.0, 1.0),
		glm::vec4(10.0, -5.0, 0.0, 1.0), glm::vec4(10.0, 0.0, 0.0, 1.0))));
}

Bezier1D::~Bezier1D(void)
{
	std::cout << "THE DESTRUCTOR WAS SUMMONED" << std::endl;
}

IndexedModel Bezier1D::GetLine(int resT)
{
	IndexedModel index_model;
	float t = 0.0;
	glm::vec4 vec_t;
	glm::vec3 vec_res;
	
	for (int j = 0; j < segments.size(); j++)
	{
		t = 0.0;
		
		for (int i = 0; i < resT; i++)
		{
			vec_res = *(GetVertex(j,t).GetPos());
			index_model.positions.push_back(vec_res);	//TODO: verify order of insertion
			index_model.colors.push_back(BLUEL);
			index_model.indices.push_back(j*resT + i);
			t += (float) 1 / (resT-1);
		}
	}
	return index_model;
}

LineVertex Bezier1D::GetVertex(int segment, float t)
{
	glm::vec4 vec_t = glm::vec4(t*t*t, t*t, t, 1);
	glm::vec3 pos_vec = glm::vec3(vec_t*CUBIC_BEZIER_MAT*segments.at(segment));
	return LineVertex(pos_vec, BLUEL);
}

LineVertex Bezier1D::GetControlPoint(int segment, int indx)
{
	glm::vec3 control_point = glm::vec3(glm::transpose(segments.at(segment))[indx]);
	return LineVertex(control_point, BLUEL);
}

glm::vec3 Bezier1D::GetVelosity(int segment, float t)
{
	glm::vec3 pos_vec = *GetVertex(segment, t).GetPos();
	glm::vec4 vec_t = glm::vec4(0, t*t, t, 1);
	return glm::vec3(vec_t*CUBIC_BEZIER_DER_MAT*segments.at(segment));
}

void Bezier1D::MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition)
{
	//std::cout << "before" << std::endl << std::endl;
	//print_mat(segments.at(segment));

	segments.at(segment) = glm::transpose(segments.at(segment));
	segments.at(segment)[indx] = newPosition;
	segments.at(segment) = glm::transpose(segments.at(segment));

	//std::cout << "after" << std::endl;
	//print_mat(segments.at(segment));

	/*if (!preserveC1)
	{ 
		segments.at(segment)[indx] = newPosition;

	}
	else 
	{

	}*/
}

void Bezier1D::AddSegment(glm::mat4 mat)
{
	segments.push_back(mat);
}

int Bezier1D::GetNumSegs() 
{
	return segments.size();
}