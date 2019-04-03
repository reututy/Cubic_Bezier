#include "bezier2D.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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
	float t = 0.0;
	float s = 0.0;
	float t_inc = (float)1 / (resT - 1);
	float s_inc = (float)1 / (resS - 1);
	int k, h;
	glm::vec3 vec_pos;

	for (int main_curve_runner = 0; main_curve_runner < main_curve.GetNumSegs(); main_curve_runner++)
	{
		for (int resT_Runner = 0; resT_Runner < resT; resT_Runner++)
		{
			t = 0.0;
			for (int resS_Runner = 0; resS_Runner < resS*circularSubdivision; resS_Runner++)
			{
				if (resS_Runner % resS == 0)
				{
					s = 0.0;
				}
				
				//vec_pos = *GetVertex(i, k, j, h).GetPos();
				index_model.positions.push_back(vec_pos);
				index_model.colors.push_back(BLUEL);
				//index_model.normals.push_back(GetNormal(i, k, j, h));
				//index_model.indices.push_back(j*resT + i);
				s += s_inc;
				t += t_inc;
			}//end of resS loop
		}//end of resT loop
	}//end of main_curve loop
	return IndexedModel();
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