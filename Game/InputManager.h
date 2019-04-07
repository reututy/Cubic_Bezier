#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include <iostream>

#define NO_OF_MODES 6

int curr_mode = 7;
bool bezier_surface_flag = 0;
char modes_names[6][20] =  { "POINTS", "LINES" , "LINE_LOOP", "LINE_STRIP", "TRIANGLES", "QUADS"};
void mouse_callback(GLFWwindow* window,int button, int action, int mods)
{	
	if(action == GLFW_PRESS )
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		double x2,y2;
		glfwGetCursorPos(window,&x2,&y2);
		scn->picking((int)x2,(int)y2);
	}
}
	
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);
	scn->shapeTransformation(scn->zCameraTranslate,yoffset);
		
}
	
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);

	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_SPACE:
			if (bezier_surface_flag == 0) 
			{
				scn->addShape(5, -1, curr_mode);
				bezier_surface_flag++;
			}
			break;
		case GLFW_KEY_W: {
			curr_mode = (curr_mode + 1) % NO_OF_MODES;
			std::cout << "Current mode is: " << modes_names[curr_mode] << std::endl;
			scn->change_shape_mode(15, curr_mode);
		}
		default:
			break;
		}
	}
}
	

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);

	scn->updatePosition((float)xpos,(float)ypos);
	if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		scn->mouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
	}
	else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		scn->mouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
	}

}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);

	scn->resize(width,height);
	//relation = (float)width/(float)height;
}

void init(Display &display)
{
	display.addKeyCallBack(key_callback);
	display.addMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
	display.addResizeCallBack(window_size_callback);

}
