#include "game.h"
#include <iostream>

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


Game::Game():Scene(){curve = 0;}

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
	addShape(Axis,-1,LINES);
	addShape(Cube,-1,TRIANGLES);
	addShapeCopy(1,-1,TRIANGLES);
	addShapeFromFile("../res/objs/testBoxNoUV.obj",-1,TRIANGLES);
	
	//translate all scene away from camera
	myTranslate(glm::vec3(0,0,-20),0);

	pickedShape = 0;

	shapeTransformation(yScale,10);
	shapeTransformation(xScale,10);
	shapeTransformation(zScale,10);

	
	pickedShape = 1;
	shapeTransformation(yGlobalTranslate,5);
	
	pickedShape = 2;
	shapeTransformation(yGlobalRotate,45);	

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
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0,0,0,1);
		std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
	}
}

Game::~Game(void)
{
	delete curve;
}
