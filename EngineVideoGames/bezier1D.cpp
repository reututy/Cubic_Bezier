#include "bezier1D.h"
#include <iostream>

#define CUBIC_BEZIER_MAT glm::mat4(glm::vec4(-1.0f, 3.0f,-3.0f,1.0f), glm::vec4(3.0f, -6.0f,3.0f,0.0f), glm::vec4(-3.0f, 3.0f,0.0f,1.0f), glm::vec4(1.0f, 0.0f,0.0f,0.0f))
#define BLUE glm::vec3(0.2667f, 0.3137f, 0.6196f)
#define BLUEL glm::vec3(0.0f, 0.0f, 1.0f)
#define CUBIC_BEZIER_DER_MAT glm::mat4(glm::vec4(0.0f, -3.0f,6.0f,-3.0f), glm::vec4(0.0f, 9.0f,-12.0f,3.0f), glm::vec4(0.0f, -9.0f,6.0f,0.0f), glm::vec4(0.0f, 3.0f,0.0f,0.0f))

Bezier1D::Bezier1D(void)
{
	AddSegment(glm::mat4(glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(1.0, 0.0, 0.0, 0.0),
		glm::vec4(1.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0)));

	//AddSegment(glm::mat4(glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(-1.0, 1.0, 0.0, 0.0),
		//glm::vec4(-1.0, 0.0, 0.0, 0.0), glm::vec4(-1.0, 1.0, 0.0, 0.0)));

	//AddSegment(glm::mat4(glm::vec4(6.0, 0.0, 0.0, 0.0), glm::vec4(7.0, 0.0, 0.0, 0.0),
		//glm::vec4(8.0, 0.0, 0.0, 0.0), glm::vec4(9.0, 0.0, 0.0, 0.0)));
}

Bezier1D::~Bezier1D(void)
{
	std::cout << "THE DESTRUCTOR WAS SUMMONED" << std::endl;
}

IndexedModel Bezier1D::GetLine(int resT)
{
	std::cout << "GET LINE IN BEZIER 1D" << std::endl;
	IndexedModel index_model;
	float t = 0.0;
	glm::vec4 vec_t;
	glm::vec3 vec_res;
	std::cout << "resT currently is: " << resT << std::endl;
	for (int j = 0; j < segments.size(); j++)
	{
		t = 0.0;
		
		for (int i = 1; i < resT+1; i++)
		{
			vec_res = *(GetVertex(j,t).GetPos());
			index_model.positions.push_back(vec_res);	//TODO: verify order of insertion
			index_model.colors.push_back(BLUEL);
			index_model.indices.push_back(j*resT + i);
			t += (float) 1 / resT;
		}
	}

	return index_model;
}

LineVertex Bezier1D::GetVertex(int segment, float t)
{
	std::cout << "GET VERTEX" << std::endl;
	glm::vec4 vec_t = glm::vec4(t*t*t, t*t, t, 1);
	//glm::vec4 vec_t = glm::vec4(1, t, t*t, t*t*t);
	glm::vec3 pos_vec = glm::vec3(vec_t*CUBIC_BEZIER_MAT*segments.at(segment));
	return LineVertex(pos_vec, BLUEL);
}

LineVertex Bezier1D::GetControlPoint(int segment, int indx)
{
	std::cout << "GET CONTROL POINT" << std::endl;
	glm::vec3 control_point = glm::vec3(segments.at(segment)[indx]);
	return LineVertex(control_point, BLUEL);
}

glm::vec3 Bezier1D::GetVelosity(int segment, float t)
{
	std::cout << "GET VELOCITY" << std::endl;
	glm::vec3 pos_vec = *GetVertex(segment, t).GetPos();
	glm::vec4 vec_t = glm::vec4(0, t*t, t, 1);
	return glm::vec3(vec_t*CUBIC_BEZIER_DER_MAT*segments.at(segment));
}

void Bezier1D::MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition)
{

}

void Bezier1D::AddSegment(glm::mat4 mat)
{
	std::cout << "ADD SEGMENT" << std::endl;
	segments.push_back(mat);
}