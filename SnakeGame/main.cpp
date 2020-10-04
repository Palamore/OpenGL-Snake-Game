#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <time.h>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Snake.h"

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MakeTexture(unsigned int& tex, const char* resource);
glm::mat4 SetSnakeBodyPos(glm::vec3 tilePos[], int x, int y);
glm::vec3 SetPosToVec(Pos _pos);
Pos GenerateSnakeBody(std::vector<Pos>& snakePos);
bool CheckHeadCollision(glm::vec3 _pos, glm::vec3 headPos, SnakeDirection sd);
void GameOver(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const float FRAME_RATE = 1.0f / 60.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 6.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -60.0f);

Snake snake;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "FAILED TO CREATE GLFW WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	Shader shader("SnakeGameVertexShader.vs", "SnakeGameFragmentShader.fs");

	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f
	};

	float snakeVertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f
	};

	glm::vec3 tilePositions[] = {
		glm::vec3(-5.0f, -5.0f, -10.0f),
		glm::vec3(-4.0f, -5.0f, -10.0f),
		glm::vec3(-3.0f, -5.0f, -10.0f),
		glm::vec3(-2.0f, -5.0f, -10.0f),
		glm::vec3(-1.0f, -5.0f, -10.0f),
		glm::vec3( 0.0f, -5.0f, -10.0f),
		glm::vec3( 1.0f, -5.0f, -10.0f),
		glm::vec3( 2.0f, -5.0f, -10.0f),
		glm::vec3( 3.0f, -5.0f, -10.0f),
		glm::vec3( 4.0f, -5.0f, -10.0f),
		glm::vec3( 5.0f, -5.0f, -10.0f)
	};


	unsigned int VBO, VAO, snakeVBO, snakeVAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//-----------------------------------

	glGenVertexArrays(1, &snakeVAO);
	glGenBuffers(1, &snakeVBO);

	glBindVertexArray(snakeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, snakeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(snakeVertices), snakeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	stbi_set_flip_vertically_on_load(true);
	unsigned int boxTex1, boxTex2, snakeTex, addBodyTex;
	MakeTexture(boxTex1, "resources/tile1.jpg");
	MakeTexture(boxTex2, "resources/tile2.jpg");
	MakeTexture(snakeTex, "resources/brownBox.jpg");
	MakeTexture(addBodyTex, "resources/YellowBox.jpg");

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	shader.setInt("texture3", 2);
	shader.setInt("texture4", 3);
	shader.setInt("texFlag", 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boxTex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, boxTex2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, snakeTex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, addBodyTex);

	// Snake Initialization
	std::vector<Pos> snakeBodyContainer;
	snakeBodyContainer.reserve(50);
	SnakeDirection snakeDirection = SnakeDirection::UP;
	Pos pos = { 5, 5 };
	Pos genPos = { 6, 6 };
	Pos nextPos = { 5, 4 }, lastPos = pos, curPos = pos;
	lastPos.y += 1;
	float frame = 0.0f, oneSec = 0.0f;
	bool getBoxFlag = false;
	snakeBodyContainer.push_back(lastPos);



	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frame += deltaTime;
		oneSec += deltaTime;

		ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		int tileCnt = 0;
		glBindVertexArray(VAO);
		for (size_t i = 0; i < 11; i++)
		{
			for (size_t j = 0; j < 11; j++)
			{
				if (tileCnt % 2 == 0)
					shader.setInt("texFlag", 1);
				else
					shader.setInt("texFlag", 2);
				tileCnt++;

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, tilePositions[j] + glm::vec3(0.0f, 0.0f, 1.0f * (float)i));

				shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}

		glm::vec3 nextVec = SetPosToVec(nextPos);
		glm::vec3 curVec = SetPosToVec(curPos);
		glm::vec3 lastVec = SetPosToVec(lastPos);
		glm::vec3 tailVec;
		if (!snakeBodyContainer.empty())
		{
			tailVec = SetPosToVec(snakeBodyContainer[0]);
		}
		else
		{
			tailVec = curVec;
		}

		glBindVertexArray(snakeVAO);
		shader.setInt("texFlag", 3);
		for (size_t i = 0; i < snakeBodyContainer.size(); i++)
		{
			glm::mat4 model = SetSnakeBodyPos(tilePositions, snakeBodyContainer[i].x, snakeBodyContainer[i].y);
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		if (frame >= FRAME_RATE)
		{
			float t = oneSec;
			if (t >= 0.5f) t = 0.5f;
			frame -= FRAME_RATE;
			glm::vec3 headingVec = curVec + (nextVec - curVec) * t * 2.0f;

			glm::mat4 snakeHead = SetSnakeBodyPos(tilePositions, curPos.x, curPos.y);
			snakeHead = glm::translate(snakeHead, (nextVec - curVec) * t * 2.0f);
			shader.setMat4("model", snakeHead);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 snakeTail = SetSnakeBodyPos(tilePositions, lastPos.x, lastPos.y);
			snakeTail = glm::translate(snakeTail, (tailVec - lastVec) * t * 2.0f);
			shader.setMat4("model", snakeTail);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			if (nextPos.x < 0 || nextPos.x > 10 || nextPos.y < 0 || nextPos.y > 10)
			{
				GameOver(window);
			}
			// 部府 面倒眉农
			if (CheckHeadCollision(lastVec + ((tailVec - lastVec) * t * 2.0f), headingVec, snakeDirection))
			{
				GameOver(window);
			}
			// 个烹 面倒眉农
			for (size_t i = 0; i < snakeBodyContainer.size(); i++)
			{
				glm::vec3 bodyVec = SetPosToVec(snakeBodyContainer[i]);
				if (CheckHeadCollision(bodyVec, headingVec, snakeDirection))
				{
					GameOver(window);
				}
			}
			// 眠啊 个烹 面倒眉农 
			glm::vec3 genVec = SetPosToVec(genPos);
			if (CheckHeadCollision(genVec, headingVec, snakeDirection))
			{
				std::cout << (int)snakeDirection << std::endl;
				getBoxFlag = true;
				genPos = GenerateSnakeBody(snakeBodyContainer);
			}


			if (oneSec >= 0.5f)
			{
				oneSec -= 0.5f;

				if (snakeBodyContainer.empty())
					lastPos = curPos;
				else
					lastPos = snakeBodyContainer[0];
				curPos = nextPos;

				snakeDirection = snake.GetDirection();

				switch (snakeDirection)
				{
				case SnakeDirection::UP:
					nextPos.y -= 1;
					break;
				case SnakeDirection::DOWN:
					nextPos.y += 1;
					break;
				case SnakeDirection::LEFT:
					nextPos.x -= 1;
					break;
				case SnakeDirection::RIGHT:
					nextPos.x += 1;
					break;
				}

				snakeBodyContainer.push_back(curPos);
				if (!getBoxFlag)
				{
					snakeBodyContainer.erase(snakeBodyContainer.begin());
				}
				else
				{
					getBoxFlag = false;
				}
			}
		}

		shader.setInt("texFlag", 4);
		glm::mat4 model = SetSnakeBodyPos(tilePositions, genPos.x, genPos.y);
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &snakeVAO);
	glDeleteBuffers(1, &snakeVBO);


	glfwTerminate();
	return 0;
}


void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		snake.SetDirection(SnakeDirection::UP);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		snake.SetDirection(SnakeDirection::DOWN);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		snake.SetDirection(SnakeDirection::LEFT);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		snake.SetDirection(SnakeDirection::RIGHT);
				
}

void MakeTexture(unsigned int& tex, const char* resource)
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(resource, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}
	stbi_image_free(data);
}

glm::mat4 SetSnakeBodyPos(glm::vec3 tilePos[], int x, int y)
{
	glm::mat4 posMatrix = glm::mat4(1.0f);
	posMatrix = glm::translate(posMatrix, tilePos[x] + glm::vec3(0.0f, 0.0f, 1.0f * y));
	return posMatrix;
}

Pos GenerateSnakeBody(std::vector<Pos>& snakePos)
{
	Pos _pos;
	bool genFlag = true;
	while (1)
	{
		srand(static_cast<unsigned int>(time(NULL)));
		int xPos = rand() % 11;
		int yPos = rand() % 11;
		for (size_t i = 0; i < snakePos.size(); i++)
		{
			if (snakePos[i].x == xPos && snakePos[i].y == yPos)
			{
				genFlag = false;
				break;
			}
		}
		if (!genFlag)
		{
			genFlag = true;
		}
		else
		{
			_pos.x = xPos;
			_pos.y = yPos;
			return _pos;
		}
	}
}

glm::vec3 SetPosToVec(Pos _pos)
{
	return glm::vec3(static_cast<float>(_pos.x), 0.0f, static_cast<float>(_pos.y));
}

bool CheckHeadCollision(glm::vec3 _pos, glm::vec3 headPos, SnakeDirection sd)
{
	float minX = _pos.x - 0.5f, maxX = _pos.x + 0.5f;
	float minZ = _pos.z - 0.5f, maxZ = _pos.z + 0.5f;
	float headZ = headPos.z, headX = headPos.x;
	switch (sd)
	{
	case SnakeDirection::UP:
		headZ -= 0.5f;
		break;
	case SnakeDirection::DOWN:
		headZ += 0.5f;
		break;
	case SnakeDirection::LEFT:
		headX -= 0.5f;
		break;
	case SnakeDirection::RIGHT:
		headX += 0.5f;
		break;
	default: break;
	}
	if (headZ > minZ &&
		headZ < maxZ &&
		headX > minX &&
		headX < maxX)
		return true;
	return false;
}

void GameOver(GLFWwindow* window)
{
	std::cout << "GAME OVER!" << std::endl;
	glfwSetWindowShouldClose(window, true);
}