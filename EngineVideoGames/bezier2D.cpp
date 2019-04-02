#include "bezier2D.h"

#define BLUEL glm::vec3(0.0f, 0.0f, 1.0f)
#define WINDOW 4

Bezier2D::Bezier2D(void)
{
	circularSubdivision = 4;
}

Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision)
{
	this->circularSubdivision = circularSubdivision;

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

	for (int i = 0; i < resT; i++)
	{
		for (int j = 0; j < resS; j++)
		{
			k = j + WINDOW;
			h = k - 1;

		}
	}

	for (int i = 0; i < main_curve.GetNumSegs(); i++)
	{
		for (int k = 0; k < second_curve.GetNumSegs(); k++)
		{
			t = 0.0;
			for (int j = 0; j < resT; j++)
			{
				s = 0.0;
				for (int h = 0; h < resS; h++)
				{
					vec_pos = *GetVertex(i, k, j, h).GetPos();
					index_model.positions.push_back(vec_pos);
					index_model.colors.push_back(BLUEL);
					index_model.normals.push_back(GetNormal(i, k, j, h));
					index_model.indices.push_back(j*resT + i);
					s += s_inc;
				}//end of resS loop
				t += t_inc;
			}//end of resT loop
		}//end of second_curve loop
	}//end of main_curve loop
	return IndexedModel();
}

Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s)
{
	glm::vec3 pos_vec_t = *(main_curve.GetVertex(segmentT,t)).GetPos();
	glm::vec3 pos_vec_s = *(second_curve.GetVertex(segmentT, t)).GetPos();
	glm::vec3 result = pos_vec_t * pos_vec_s;
	glm::vec3 normal = GetNormal(segmentT, segmentS, t, s);
	return Vertex(result, glm::vec2(0.0f, 0.0f), normal, BLUEL);
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s)
{
	glm::vec3 velT = main_curve.GetVelosity(segmentT, t);
	glm::vec3 velS = second_curve.GetVelosity(segmentS, s);
	//glm::vec3 vel1 = velT*(*(second_curve.GetVertex(segmentT, t)).GetPos());
	//glm::vec3 vel2 = velS*(*(second_curve.GetVertex(segmentS, s)).GetPos());
	return glm::cross(velT, velS);
}