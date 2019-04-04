#include "game.h"
#include <iostream>

#define CONTROL_POINT_SCALE 0.1
#define PURGATORY -5000000

int MAX_CTRL = 3;
int MIN_CTRL = 3;
bool once = false;

extern bool bezier_surface_flag;


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

Game::~Game(void)
{
	delete curve;
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
	int prev_shape = pickedShape;
	if (!once) { 
		MoveControlCubes(); 
	}
	if (bezier_surface_flag && !once) {
		for (int i = MIN_CTRL - 2; i < MAX_CTRL; i++) {
			pickedShape = i;
			shapeTransformation(zGlobalTranslate, PURGATORY);
		}
		once = true;
	}
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
		GetShapeTransformation();
}

void Game::WhenTranslate()
{
	bool no_preservation = !(pickedShape <= 4 || pickedShape >= MAX_CTRL - 2);
	if (pickedShape >= 3 && pickedShape < MAX_CTRL) //Make sure that it only happens in the case of the cubes
	{
		glm::vec4 trans_vec = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		//if the picked shape is one of the control points between the segments
		//move the relevent control point
		curve->MoveControlPoint((pickedShape - MIN_CTRL), (pickedShape - MIN_CTRL), no_preservation, trans_vec);
		//move the relevent cube incident to that specific control point
		shapes[1]->GetMesh()->InitLine(curve->GetLine(30));
	}
}

void Game::MoveControlCubes()
{
	int old_picked_shape = pickedShape;
	glm::vec3 control_point;
	
	for (int i = MIN_CTRL; i < MAX_CTRL; i++)
	{
		if (!((i - 2) % 4) == 0 && i != MAX_CTRL - 1)
		{
			pickedShape = i;
			glm::vec4 curr_pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
			control_point = *(curve->GetControlPoint((i - MIN_CTRL) / 4, (i - MIN_CTRL) % 4)).GetPos();
			control_point = (control_point) - glm::vec3(curr_pos);
			shapeTransformation(xGlobalTranslate, control_point.x);
			shapeTransformation(yGlobalTranslate, control_point.y);
			shapeTransformation(zGlobalTranslate, control_point.z);
		}
	}
	pickedShape = old_picked_shape;
}
