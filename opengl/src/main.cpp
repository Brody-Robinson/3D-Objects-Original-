#define GLEW_STATIC

#include "error.h"
#include "Light.h"
#include "Camera.h"
#include "Funtions.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Mouse_Scroll_enum
{
	bool increase;
	bool decrease;
};


using namespace std;

bool GameLoop = true;

freeCamera Camera;

Light light;

bool is_perspective = true;
 

bool  isMouseActive = false;
float xyCusorPos[2] = { 0, 0 };
float lastXYCusorPos[2] = { 0, 0 };

float lastX = 1000.0f / 2.0f;
float lastY = 1000.0f / 2.0f;
bool firstMouse = true;

double scrollY = 0.0f;
double _previous_scrolly_ = 0.0f;

bool Increase = 0;
bool Decrease = 0;


static void setCusorPos(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	xyCusorPos[0] = xoffset;
	xyCusorPos[1] = yoffset;
}

bool MoveCursor()
{
	if (lastXYCusorPos[0] != xyCusorPos[0])
	{
		lastXYCusorPos[0] = xyCusorPos[0];
		lastXYCusorPos[1] = xyCusorPos[1];

		return true;
	}
	else
	{
		return false;
	}
}

static void scrollCallBack(GLFWwindow* window, double x, double y)
{
	scrollY = y;

	if (scrollY > _previous_scrolly_)
	{
		Increase = true;
		Decrease = false;
	}
	else if (scrollY < _previous_scrolly_)
	{
		Decrease = true;
		Increase = false;
	}

	_previous_scrolly_ = y;
}

Mouse_Scroll_enum mouse_scroll_return()
{
	Mouse_Scroll_enum f;
	f.increase = Increase;
	f.decrease = Decrease;

	Increase = false;
	Decrease = false;
	return f;
}

void Uinput(float deltaTime, GLFWwindow** Window)
{
	GLFWwindow* m_window = *Window;

	if (mouse_scroll_return().increase)
	{
		Camera.MovementSpeed += 1.25f;
	}

	else if (mouse_scroll_return().decrease)
	{
		Camera.MovementSpeed -= 1.25f;
	}

	if (MoveCursor())
	{
		Camera.ProcessMouseMovement(xyCusorPos[0], xyCusorPos[1]);
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime * 100);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime * 100);
	}

	else if (glfwGetKey(m_window, GLFW_KEY_W))
	{
		Camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime * 100);
	}

	else if (glfwGetKey(m_window, GLFW_KEY_S))
	{
		Camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime * 100);
	}

	else if (glfwGetKey(m_window, GLFW_KEY_Q))
	{
		Camera.ProcessKeyboard(Camera_Movement::UP, deltaTime * 100);
	}

	else if (glfwGetKey(m_window, GLFW_KEY_E))
	{
		Camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime * 100);
	}

	else if (glfwGetKey(m_window, GLFW_KEY_P))
	{
		is_perspective = !is_perspective;
	}


	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
	{
		GameLoop = false;
	}
}


void display(GLsizei count)
{
	GLcall(glDrawArrays(GL_TRIANGLES, 0, count));
}

void displayIndex(GLsizei count)
{
	GLcall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

void display(GLsizei start, GLsizei count)
{
	GLcall(glDrawArrays(GL_TRIANGLE_STRIP, start, count));
}

// main function
int main(int argc, char** argv)
{
	//............................................................................
	if (!glfwInit()) {
		std::cout << "initialization failed!" << std::endl;
		return 0;
	}
	//................................................................................
	GLFWwindow* m_window = glfwCreateWindow(1280, 720, "opengl", nullptr, nullptr);

	int max, min, r;
	glfwGetVersion(&max, &min, &r);
	//..............................................................................
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, max);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);

	std::cout << "version >> " << max << "." << min << std::endl;
	//...............................................................
	glfwMakeContextCurrent(m_window);

	glfwSetErrorCallback((GLFWerrorfun)[](int error_code, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
		});

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, (GLFWcursorposfun)setCusorPos);
	glfwSetScrollCallback(m_window, scrollCallBack);
	//.............................................................
	if (glewInit() != GLEW_OK)
	{
		std::cout << "OPENGL ERROR" << std::endl;
	}

	//...........................................................
	//std::cout << glGetString(GL_VERSION) << std::endl;
	init();

	Camera.Position = glm::vec3(20.0f,  70.0f,  20.0f);
	
	//...........................................................
	glm::mat4 projection = glm::infinitePerspective(glm::radians(60.0f), 16.0f/ 9.0f, 0.1f);
	glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), spotModel = glm::mat4(1.0f);
	//.............................................................
	_2DTex Keys = _2DTex("asset/Keys.png");
	_2DTex White = _2DTex("asset/White.png");
	_2DTex Screen = _2DTex("asset/Screen.png");


	//.........................................................
	light = Light(glm::vec3(0.9f, 0.9f, 0.9f), "light[0]", 10500.0f, 0.25f, 15250.51f);
	
	float Angle = 0.0f;
	glm::vec3 lightPos = glm::vec3(glm::sin(Angle) * 150, 100, glm::cos(Angle) * 150);


	light.set_position(glm::vec3(80.0f, 200.0f, 0.0f));
	light.set_Spotdirection(glm::vec3(-50.0f, -200.0f, -50.0f));
	light.set_spotlight_cutoff(glm::cos(3.14f / 2.0f));
	light.set_spotlight_innercutoff(glm::cos(3.14f / 10.0f));
	light.set_attenuation_constants(4.0f, 1.0f, 0.5f, 0.44f);

	Shader1.setUniformMat4("projection", projection);
	Shader1.Bind();

	float deltaTime, currentFrame, lastFrame = 0;

	while (!glfwWindowShouldClose(m_window) && GameLoop)
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Uinput(deltaTime, &m_window);

	
		view = Camera.GetViewMatrix();
		projection = (is_perspective ? glm::infinitePerspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f) : 
			glm::orthoRH(-100.0f, 100.0f, -100.0f, 100.0f, 0.01f, 10000.0f));


		light.render( Camera.Position,  Shader1.getRendererId(), false, true );
		
		Shader1.Bind();
		Shader1.setUniformMat4("view", view);
		Shader1.setUniformMat4("projection", projection);

		GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLcall(glClearColor(0.7f, 0.7f, 0.7f, 1.0f));

		GLcall(glViewport(0, 0, 1280, 720));
		
		{
			//Bind Shaders
			Shader1.Bind();

	
			//sets model and color uiniforms
			Shader1.setUniformMat4("projection", projection);
			Shader1.setUniformMat4("view", view);

			Shader1.setUniformMat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(600.0f, 3.5f, 600.0f)));

			Shader1.setUniformVec3("color", glm::vec3(0.45f));
			PlaneBuffer.Bind();
			PlaneVao.Bind();

			display(36);
		}


		{
			VAO.Bind();
			ibo.Bind();
		
			//tip of pen
			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(22.0f, 3.0f, 20.0f) )
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)) 
				* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 3.5f)));

			Shader1.setUniformVec3("color", glm::vec3(0.1f));
			
			displayIndex(cylinder.getIndexCount());


			//MUG Handle
			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(94.5f, 6.0f, -75.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(325.0f), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)));

			Shader1.setUniformVec3("color", glm::vec3(0.6f));

			display(cylinder.getVertexCount(), obj.getCount() / 2);

			//PLATE EDGE

			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 1.0f, -75.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))*
				glm::scale(glm::mat4(1.0f), glm::vec3(5.8f)));

			Shader1.setUniformVec3("color", glm::vec3(0.6f));

			display(cylinder.getVertexCount(), obj.getCount());

			/// 
			VAO_2.Bind();
			ibo_2.Bind();

			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(94.5f, 4.0f, -75.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0)) *
				 glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(0, 1, 0)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(0.45f, 0.45f, 4.5f)));

			Shader1.setUniformVec3("color", glm::vec3(0.6f));

			displayIndex(cylinder_2.getIndexCount());
		}

		//TEACUP
		{
			VAO_2.Bind();
			ibo_2.Bind();

			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 5.0f, -75.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 7.0f)));

			Shader1.setUniformVec3("color", glm::vec3(0.6f));

			displayIndex(cylinder_2.getIndexCount() - cylinder_2.getBaseIndexCount());
		}
		
		{						
			VAO_2.Bind();
			ibo_2.Bind();
			
			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 3.0f, 20.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 25.5f) ));

			Shader1.setUniformVec3("color", glm::vec3(0.6f));

			displayIndex(cylinder_2.getIndexCount());

		}

		{
			Shader1.Bind();
			PlaneVao.Bind();


			//Monitor
			Screen.bindTextureToSlot(2);
			White.bindTextureToSlot(3);

			Shader1.setTextureSampler("_MainTex", Screen.GetTextureSlot());
			Shader1.setTextureSampler("texture_2", White.GetTextureSlot());

			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 30.2f, -81.0f))*
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 0.2f, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(80.2f, 1.0f, 60.2f)));

			Shader1.setUniformVec3("color", glm::vec3(0.0f));

			display(36);

			//keyboard

			Keys.bindTextureToSlot(2);
			White.bindTextureToSlot(3);
			Shader1.setTextureSampler("_MainTex", Keys.GetTextureSlot());
			Shader1.setTextureSampler("texture_2", White.GetTextureSlot());

			Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 0.2f, -50.0f))*
				glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 1, 0))* glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 0.2f, 0.0f))*
				glm::scale(glm::mat4(1.0f), glm::vec3(80.2f, 3.0f, 60.2f)));

			Shader1.setUniformVec3("color", glm::vec3(0.0f));

			display(36);

			Shader1.setUniformVec3("color", glm::vec3(1.0f));


			//Plate
			{
				Shader1.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 1.0f, -75.0f))*
					glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 1, 0))*
					glm::scale(glm::mat4(1.0f), glm::vec3(10, 1.0f, 10)));

				Shader1.setUniformVec3("color", glm::vec3(0.6f, 0.6f, 0.6f));

				display(36);
			}
		}

		
		model = glm::mat4(1.0f);


		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	return 0;
}

