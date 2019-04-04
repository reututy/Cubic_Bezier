#include "bezier2D.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtx/perpendicular.hpp>

#define BLUE glm::vec3(0.2667f, 0.3137f, 0.6196f)
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
		if (main_curve_runner % (resT) == 0)
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
			if (second_curve_runner % (resS) == 0)
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
			index_model.indices.push_back((main_curve_runner*(resS*circularSubdivision - 1)*4 + second_curve_runner) * 1);
			index_model.indices.push_back((main_curve_runner*(resS*circularSubdivision - 1)*4 + second_curve_runner) * 1 + 1);
			index_model.indices.push_back((main_curve_runner*(resS*circularSubdivision - 1)*4 + second_curve_runner) * 1 + 2);
			index_model.indices.push_back((main_curve_runner*(resS*circularSubdivision - 1)*4 + second_curve_runner) * 1 + 3);
			index_model.texCoords.push_back(glm::vec2(t, s));
			index_model.texCoords.push_back(glm::vec2(t + t_inc, s));
			index_model.texCoords.push_back(glm::vec2(t, s + s_inc));
			index_model.texCoords.push_back(glm::vec2(t + t_inc, s + s_inc));
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
	glm::vec3 normal = GetNormal(segmentT, segmentS, t, s);
	return Vertex(glm::vec3(pos_vec_t), glm::vec2(0.0f, 0.0f), normal, BLUEL);
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s)
{
	glm::mat4 segment_coordT = this->main_curve.GetSegments()[segmentT];
	glm::vec3 rotate_axis = glm::vec3(this->main_curve.GetSegments()[main_curve.GetNumSegs() - 1][3] - this->main_curve.GetSegments()[0][0]);
	glm::mat4 rotateMat = glm::rotate(360.0f * s, rotate_axis);

	glm::vec3 b_vec = main_curve.GetVelosity(segmentT, t);
	glm::vec3 c_vec = glm::vec3(rotateMat*glm::vec4(glm::vec3(0, 0, 1), 1));
	float sign = (segmentT % 2 == 0) ? 1.0f : -1.0f;
	glm::vec3 normal = sign*glm::normalize(glm::cross(b_vec, c_vec));
	normal.y = sign*normal.y;

	return normal;

	/*
	float line_parameter = (t + segmentT);
	//std::cout << "line_parameter " << line_parameter << std::endl;
	glm::vec4 velT = glm::vec4(main_curve.GetVelosity(segmentT, t), 1);
	glm::vec4 posT = glm::vec4(*(main_curve.GetVertex(segmentT, t)).GetPos(), 1);
	glm::vec4 center = glm::vec4(axis,1)*line_parameter;
	glm::vec4 radius = center - posT;
	glm::vec3 velS = glm::cross(glm::vec3(radius), glm::vec3(axis));

	/*glm::vec3 center = axis*line_parameter;
	float radius_m = (posT.y - center.y) / (posT.x - center.x);
	float velS_m = - 1 / radius_m;
	glm::vec4 velS = velS_m + posT;*/

	//glm::vec4 velS = velT*glm::rotate(360 * s, axis);
	//glm::vec3 normal = glm::cross(glm::vec3(velT), glm::vec3(velS));
	/*glm::vec4 velS = glm::vec4(1);
	glm::vec4 radius = glm::vec4(axis, 1) - posT;
	if (t != 0) {
		velS = velS*glm::rotate(90.0f*segmentS, axis*t);
	}
	
	//glm::vec4 radius = glm::perp(posT,glm::vec4(axis,1))*glm::rotate(360 * s, axis);
	//glm::vec3 velS = glm::cross(glm::vec3(radius), axis);
	glm::vec3 normal = glm::cross(glm::vec3(velS), glm::vec3(velT));
	normal = glm::vec3(glm::vec4(normal,1)*glm::rotate(360 * s, axis));
	return glm::vec3(normal);*/
}