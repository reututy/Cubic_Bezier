#include "bezier1D.h"
#include <iostream>

#define CUBIC_BEZIER_MAT glm::mat4(glm::vec4(-1.0f, 3.0f,-3.0f,1.0f), glm::vec4(3.0f, -6.0f,3.0f,0.0f), glm::vec4(-3.0f, 3.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f,0.0f,0.0f))
#define BLUE glm:: vec3(0.2667f, 0.3137f, 0.6196f)
#define CUBIC_BEZIER_DER_MAT glm::mat4(glm::vec4(0.0f, -3.0f,6.0f,-3.0f), glm::vec4(0.0f, 9.0f,-12.0f,3.0f), glm::vec4(0.0f, -9.0f,6.0f,0.0f), glm::vec4(0.0f, 3.0f,0.0f,0.0f))

Bezier1D::Bezier1D(void)
{
	AddSegment(glm::mat4(glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(1.0, 0.0, 0.0, 0.0),
		glm::vec4(2.0, 0.0, 0.0, 0.0), glm::vec4(3.0, 0.0, 0.0, 0.0)));

	AddSegment(glm::mat4(glm::vec4(3.0, 0.0, 0.0, 0.0), glm::vec4(4.0, 0.0, 0.0, 0.0),
		glm::vec4(5.0, 0.0, 0.0, 0.0), glm::vec4(6.0, 0.0, 0.0, 0.0)));

	AddSegment(glm::mat4(glm::vec4(6.0, 0.0, 0.0, 0.0), glm::vec4(7.0, 0.0, 0.0, 0.0),
		glm::vec4(8.0, 0.0, 0.0, 0.0), glm::vec4(9.0, 0.0, 0.0, 0.0)));
}

Bezier1D::~Bezier1D(void)
{

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
			vec_t = glm::vec4(t*t*t, t*t, t, 1); //TODO: Check if the vector is column or row
			vec_res = glm::vec3(vec_t*CUBIC_BEZIER_MAT*segments.at(j));
			index_model.positions.push_back(vec_res);	//TODO: verify order of insertion
			index_model.indices.push_back(j*resT + i);
			t += (float)1 / resT;
		}
	}

	return index_model;
}

LineVertex Bezier1D::GetVertex(int segment, int t)
{
	glm::vec4 vec_t = glm::vec4(t*t*t, t*t, t, 1);
	glm::vec3 pos_vec = glm::vec3(vec_t*CUBIC_BEZIER_MAT*segments.at(segment));
	return LineVertex(pos_vec, BLUE);
}

LineVertex Bezier1D::GetControlPoint(int segment, int indx)
{
	glm::vec3 control_point = glm::vec3(segments.at(segment)[indx]);
	return LineVertex(control_point, BLUE);
}

glm::vec3 Bezier1D::GetVelosity(int segment, int t)
{
	glm::vec3 pos_vec = *GetVertex(segment, t).GetPos();
	glm::vec4 vec_t = glm::vec4(0, t*t, t, 1);
	return glm::vec3(vec_t*CUBIC_BEZIER_DER_MAT*segments.at(segment));
}

void Bezier1D::MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition)
{

}

void Bezier1D::AddSegment(glm::mat4 mat)
{
	segments.push_back(mat);
}