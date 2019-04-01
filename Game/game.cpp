#include "game.h"
#include <iostream>


#define CONTROL_POINT_SCALE 0.1
#define PURGATORY -5000000

int MAX_CTRL = 3;
int MIN_CTRL = 3;

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game():Scene()
{
	curve = 0;
}

Game::Game(glm::vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
{ 
	curve = new Bezier1D();
}

void Game::addShape(int type,int parent,unsigned int mode)
{
	chainParents.push_back(parent);
	if(type!=BezierLine && type!=BezierSurface)
		shapes.push_back(new Shape(type,mode));
	else
	{
		if(type == BezierLine)
			shapes.push_back(new Shape(curve,30,30,false,mode));
		else
			shapes.push_back(new Shape(curve,30,30,true,mode));
	}
}

void Game::Init()
{
	addShape(Axis, -1, LINES);
	addShape(BezierLine, -1, LINE_STRIP);

	//translate all scene away from camera
	myTranslate(glm::vec3(0, 0, -20), 0);

	//Axis scale:
	pickedShape = 0;
	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);

	pickedShape = 1;
	//shapeTransformation(zGlobalTranslate, 5);

	addShape(Cube, -1, TRIANGLES);
	pickedShape = 2;
	shapeTransformation(zGlobalTranslate, PURGATORY);

	int counter = MIN_CTRL;
	glm::vec3 control_point;
	for (int k = 0; k < curve->GetNumSegs(); k++) 
	{
		for (int i = 0; i < 4; i++)
		{
			addShapeCopy(2, -1, TRIANGLES);
			pickedShape = counter++;
			control_point = *(curve->GetControlPoint(k, i)).GetPos();
			//scaling the cube
			shapeTransformation(xScale, CONTROL_POINT_SCALE);
			shapeTransformation(yScale, CONTROL_POINT_SCALE);
			shapeTransformation(zScale, CONTROL_POINT_SCALE);
			//move the cube to the control point
			if (counter % 4 != MIN_CTRL || (i == 3 && k == curve->GetNumSegs()-1))
			{
				shapeTransformation(xGlobalTranslate, control_point.x / CONTROL_POINT_SCALE);
				shapeTransformation(yGlobalTranslate, control_point.y / CONTROL_POINT_SCALE);
				shapeTransformation(zGlobalTranslate, control_point.z / CONTROL_POINT_SCALE);
			}
			else
			{
				shapeTransformation(zGlobalTranslate, PURGATORY / CONTROL_POINT_SCALE);
			}
		}
	}
	MAX_CTRL = counter;
	pickedShape = -1;
}

void Game::Update(glm::mat4 MVP,glm::mat4 Normal,Shader *s)
{
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 1.0f);
	s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
}

void Game::WhenRotate()
{
	if(pickedShape>=0)
		printMat(GetShapeTransformation());
}

void Game::WhenTranslate()
{
	bool is_connect_segments = ((pickedShape) % 4 == 3 && pickedShape < MAX_CTRL) || (pickedShape == MAX_CTRL - 1);
	if (pickedShape >= 0)
	{
		glm::vec4 trans_vec = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		//if the picked shape is one of the control points between the segments
		//move the relevent control point
		curve->MoveControlPoint((pickedShape - MIN_CTRL), (pickedShape - MIN_CTRL), is_connect_segments, trans_vec);
		if (is_connect_segments)
		{
			//move the pre control point
			//curve->MoveControlPoint((-1 + pickedShape - MIN_CTRL) / 4, (-1 + pickedShape - MIN_CTRL) % 4, is_connect_segments, trans_vec);
		}

		//if (is_connect_segments)
		//{
		//	if (pickedShape != MIN_CTRL) {
		//		//move one before
		//		curve->MoveControlPoint((-2 + pickedShape - MIN_CTRL) / 4, (-2 + pickedShape - MIN_CTRL) % 4, false, trans_vec);
		//		/*if (pickedShape == MAX_CTRL - 1) {
		//			curve->MoveControlPoint((-1 + pickedShape - MIN_CTRL) / 4, (-1 + pickedShape - MIN_CTRL) % 4, false, trans_vec);
		//		}*/
		//	}
		//	if (pickedShape != MAX_CTRL - 1) {
		//		//move one after
		//		curve->MoveControlPoint((1 + pickedShape - MIN_CTRL) / 4, (1 + pickedShape - MIN_CTRL) % 4, false, trans_vec);
		//	}
		//}

		//if the picked shape is the curve so all the control points moves with it
	/*	if (pickedShape == 1)
		{
			for (int i = MIN_CTRL; i < MAX_CTRL; i++) {
				pickedShape = i;
				shapeTransformation(xGlobalTranslate, trans_vec.x);
				shapeTransformation(yGlobalTranslate, trans_vec.y);
				shapeTransformation(zGlobalTranslate, trans_vec.z);
			}
		}*/
		shapes[1]->GetMesh()->InitLine(curve->GetLine(30));
	}
}

Game::~Game(void)
{
	delete curve;
}
