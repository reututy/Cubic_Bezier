#include "game.h"
#include <iostream>


#define CONTROL_POINT_SCALE 0.1

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
	shapeTransformation(xScale, 0.1);
	shapeTransformation(yScale, 0.1);
	shapeTransformation(zScale, 0.1);

	int counter = 3;
	glm::vec3 control_point;
	for (int k = 0; k < curve->GetNumSegs(); k++) 
	{
		for (int i = 0; i < 4; i++)
		{
			addShapeCopy(2, -1, TRIANGLES);
			pickedShape = counter++;
			control_point = *(curve->GetControlPoint(k, i)).GetPos();
			shapeTransformation(xScale, CONTROL_POINT_SCALE);
			shapeTransformation(yScale, CONTROL_POINT_SCALE);
			shapeTransformation(zScale, CONTROL_POINT_SCALE);
			shapeTransformation(xGlobalTranslate, control_point.x / CONTROL_POINT_SCALE);
			shapeTransformation(yGlobalTranslate, control_point.y / CONTROL_POINT_SCALE);
			shapeTransformation(zGlobalTranslate, control_point.z / CONTROL_POINT_SCALE);
			std::cout << "control_point.x: " << control_point.x << std::endl;
			std::cout << "control_point.y: " << control_point.y << std::endl;
			std::cout << "control_point.z: " << control_point.z << std::endl;
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
	if(pickedShape>=0)
	{
		if (pickedShape >= MIN_CTRL && pickedShape <= MAX_CTRL) 
		{
			curve->MoveControlPoint((pickedShape - MIN_CTRL)/4, (pickedShape - MIN_CTRL)%4, false, GetShapeTransformation()*glm::vec4(0, 0, 0, 1));
		}
		//glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		//std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
	}
}

Game::~Game(void)
{
	delete curve;
}
