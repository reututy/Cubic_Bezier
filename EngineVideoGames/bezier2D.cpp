#include "bezier2D.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#define BLUEL glm::vec3(0.0f, 0.0f, 1.0f)
#define CIRCLE_CONST 0.55228475

Bezier2D::Bezier2D(void)
{
	circularSubdivision = 4;
}

Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision)
{
	this->circularSubdivision = circularSubdivision;
	this->main_curve = b;
	this->axis = axis;
}

Bezier2D::~Bezier2D(void)
{

}

IndexedModel Bezier2D::GetSurface(int resT, int resS)
{
	IndexedModel index_model;
	int segS = 0, segT = 0;
	float t = 0.0;
	float s = 0.0;
	float t_inc = (float)1 / (resT - 1);
	float s_inc = (float)1 / (resS - 1);
	glm::vec3 vec_pos_top_left;
	glm::vec3 vec_pos_top_right;
	glm::vec3 vec_pos_bottom_right;
	glm::vec3 vec_pos_bottom_left;

	for (int main_curve_runner = 0; main_curve_runner < main_curve.GetNumSegs()*resT - 1 ; main_curve_runner++)
	{

		if (main_curve_runner != 0 && main_curve_runner % resT == 0)
		{
			segT++;
		}
		if (main_curve_runner % (resT - 1) == 0)
		{
			t = 0.0;
		}
		//std::cout << "SegT is: " << segT << "  Iteration No: " << main_curve_runner << std::endl;
		for (int second_curve_runner = 0; second_curve_runner < resS*circularSubdivision - 1; second_curve_runner++)
		{
			if (main_curve_runner != 0 && main_curve_runner % resS == 0)
			{
				segS++;
			}
			if (second_curve_runner % (resS - 1) == 0)
			{
				s = 0.0;
			}

			vec_pos_top_left = *GetVertex(segT, segS, t, s).GetPos();
			vec_pos_top_right = *GetVertex(segT, segS, t + t_inc, s).GetPos();
			vec_pos_bottom_left = *GetVertex(segT, segS, t, s + s_inc).GetPos();
			vec_pos_bottom_right = *GetVertex(segT, segS, t + t_inc, s + s_inc).GetPos();
			index_model.positions.push_back(vec_pos_top_left);
			index_model.positions.push_back(vec_pos_top_right);
			index_model.positions.push_back(vec_pos_bottom_left);
			index_model.positions.push_back(vec_pos_bottom_right);
			index_model.colors.push_back(BLUEL);
			index_model.colors.push_back(BLUEL);
			index_model.colors.push_back(BLUEL);
			index_model.colors.push_back(BLUEL);
			index_model.normals.push_back(GetNormal(segT, segS, t, s));
			index_model.normals.push_back(GetNormal(segT, segS, t + t_inc, s));
			index_model.normals.push_back(GetNormal(segT, segS, t, s + s_inc));
			index_model.normals.push_back(GetNormal(segT, segS, t + t_inc, s + s_inc));
			//std::cout << "index num is:  " << main_curve_runner*resT + second_curve_runner * 4 << std::endl;
			index_model.indices.push_back((main_curve_runner*resT + second_curve_runner) * 16);
			index_model.indices.push_back((main_curve_runner*resT + second_curve_runner) * 16 + 1);
			index_model.indices.push_back((main_curve_runner*resT + second_curve_runner) * 16 + 2);
			index_model.indices.push_back((main_curve_runner*resT + second_curve_runner) * 16 + 3);
			index_model.texCoords.push_back(glm::vec2(0, 0));
			index_model.texCoords.push_back(glm::vec2(0, 0));
			index_model.texCoords.push_back(glm::vec2(0, 0));
			index_model.texCoords.push_back(glm::vec2(0, 0));
			s += s_inc;
		}
		t += t_inc;
	}
	return index_model;
}

Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s)
{
	glm::vec4 pos_vec_t = glm::vec4(*(main_curve.GetVertex(segmentT,t)).GetPos(),1);
	pos_vec_t = pos_vec_t*glm::rotate(360*s, axis);
	//glm::vec3 pos_vec_s = *(second_curve.GetVertex(segmentT, t)).GetPos();
	//glm::vec3 result = pos_vec_t * pos_vec_s;
	glm::vec3 normal = GetNormal(segmentT, segmentS, t, s);
	return Vertex(glm::vec3(pos_vec_t), glm::vec2(0.0f, 0.0f), normal, BLUEL);
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s)
{
	glm::vec4 velT = glm::vec4(main_curve.GetVelosity(segmentT, t), 1);
	//glm::vec4 pos_vec_t = glm::vec4(*(main_curve.GetVertex(segmentT, t)).GetPos(), 1);
	velT = velT*glm::rotate(360 * s, axis);
	return glm::vec3(velT);
}