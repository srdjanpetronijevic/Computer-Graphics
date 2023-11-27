#define CRES 360 
#define _CRT_SECURE_NO_WARNINGS
#define PI 3.14159265358979323846
#define ROTATION_SPEED_INCREMENT 0.01
#define TEXTURE_COUNT 4

#include "glfw_utils.h"
#include "shader_utils.h"
#include "texture_utils.h"
#include "drawing_utils.h"


int main(void)
{
	GLFWwindow* window = initializeGLFW(1920, 1080, "Louvre Exhibition");
	initializeGLEW();

	// Shader programs initialization
	unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
	unsigned int basicShader = createShader("basic.vert", "basic.frag");
	unsigned int circleShader = createShader("basic.vert", "circle.frag");
	unsigned int textureShader = createShader("texture.vert", "texture.frag");
	unsigned int progressBarShader = createShader("progress_bar.vert", "progress_bar.frag");

	// Textures loading and setup
	unsigned monalisaTexture = loadAndSetupTexture("mona_lisa.jpg");
	unsigned medusaTexture = loadAndSetupTexture("the_raft_of_medusa.png");
	unsigned libertyTexture = loadAndSetupTexture("liberty_leading_the_people.jpg");
	unsigned napoleonTexture = loadAndSetupTexture("crowning_of_napoleon.jpg");
	unsigned signatureTexture = loadAndSetupTexture("name.png");
	
	unsigned textures[TEXTURE_COUNT] = { monalisaTexture, medusaTexture, libertyTexture, napoleonTexture };

	// Vertex Array Object(for eeach element on screen. 0 - frames, 1 - circles, 3 - images, 4 - progress bar)
	unsigned int VAO[4];
	glGenVertexArrays(4, VAO);
	unsigned int VBO[4];
	glGenBuffers(4, VBO);

	// Vertex data for frames, circles, textures, and progress bar
	float vertices[] = {
		// First Frame
		-0.9,  0.4,  1.0, 1.0, 0.0, 1.0,
		-0.9, -0.4,  1.0, 1.0, 0.0, 1.0,
		-0.6,  0.4,  1.0, 1.0, 0.0, 1.0,
		-0.6, -0.4,  1.0, 1.0, 0.0, 1.0,

		// Second Frame
		-0.4,  0.4,  1.0, 0.0, 0.0, 1.0,
		-0.4, -0.4,  1.0, 0.0, 0.0, 1.0,
		-0.1,  0.4,  1.0, 0.0, 0.0, 1.0,
		-0.1, -0.4,  1.0, 0.0, 0.0, 1.0,

		// Third Frame
		 0.1,  0.4,  1.0, 0.0, 1.0, 1.0,
		 0.1, -0.4,  1.0, 0.0, 1.0, 1.0,
		 0.4,  0.4,  1.0, 0.0, 1.0, 1.0,
		 0.4, -0.4,  1.0, 0.0, 1.0, 1.0,

		 // Fourth Frame
		  0.6,  0.4,  0.0, 0.0, 1.0, 1.0,
		  0.6, -0.4,  0.0, 0.0, 1.0, 1.0,
		  0.9,  0.4,  0.0, 0.0, 1.0, 1.0,
		  0.9, -0.4,  0.0, 0.0, 1.0, 1.0,
	};

	float textureVertices[] = {
		// Mona Lisa
		-0.85, -0.3,  1.0, 0.0,
		-0.85, 0.3,  1.0, 1.0,
		-0.65, -0.3,  0.0, 0.0,
		-0.65, 0.3,  0.0, 1.0,

		// The Raft of Medusa
		-0.35, -0.3,   1.0, 0.0,
		-0.35, 0.3,   1.0, 1.0,
		-0.15, -0.3,   0.0, 0.0,
		-0.15, 0.3,   0.0, 1.0,

		// Liberty Leading the People
		0.15, -0.3,  1.0, 0.0,
		0.15, 0.3,  1.0, 1.0,
		0.35, -0.3,  0.0, 0.0,
		0.35, 0.3,  0.0, 1.0,

		// Crowning of Napoleon
		0.65, -0.3,  1.0, 0.0,
		0.65, 0.3,  1.0, 1.0,
		0.85, -0.3,  0.0, 0.0,
		0.85, 0.3,  0.0, 1.0,

		// Signature
		-0.6, -0.7,  1.0, 0.0,
		-0.6, -0.5,  1.0, 1.0,
		-0.9, -0.7,  0.0, 0.0,
		-0.9, -0.5,  0.0, 1.0,
	};

	float progressBarVertices[] =
	{
		-1.0, -1.0,
		-1.0, -0.8,
		 1.0, -0.8,
		 1.0, -0.8,
		 1.0, -1.0,
		-1.0, -1.0
	};

	// For some reason this draws elipse instead of circle
	float circle[CRES * 2 + 4]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
	float radius = 0.05;

	circle[0] = 0;
	circle[1] = 0.8;
	for (int i = 0; i <= CRES; i++)
	{
		float angle = (PI / 180) * i;
		circle[2 + 2 * i] = circle[0] + radius * cos(angle);  // Xi
		circle[2 + 2 * i + 1] = circle[1] + radius * sin(angle);  // Yi
	}


	unsigned int stride = (2 + 4) * sizeof(float);
	unsigned int textureStride = (2 + 2) * sizeof(float);
	unsigned int progressBarStride = 2 * sizeof(float);


	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertices), textureVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(progressBarVertices), progressBarVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, progressBarStride, (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int uPosLoc = glGetUniformLocation(unifiedShader, "uPos");
	unsigned int tPosLoc = glGetUniformLocation(textureShader, "uPos");
	glBindVertexArray(0);

	unsigned uTexLoc = glGetUniformLocation(textureShader, "uTex");
	glUniform1i(uTexLoc, 0);

	// Arrays for rotation radii and speeds
	float radii[] = { 0.09, 0.1, 0.07, 0.08 };
	float rotationSpeeds[] = { 5, 1, 4, 0.1 };
	bool changeCircleColor = false;
	bool rotateFrames = true;
	float progressBar = 0;

	double startTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && progressBar < 1920) {
			progressBar += 5;
			for (int i = 0; i < TEXTURE_COUNT; ++i) {
				rotationSpeeds[i] += ROTATION_SPEED_INCREMENT;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && progressBar > 0) {
			progressBar -= 5;
			for (int i = 0; i < TEXTURE_COUNT; ++i) {
				rotationSpeeds[i] -= ROTATION_SPEED_INCREMENT;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(unifiedShader);
		glUseProgram(basicShader);
		glBindVertexArray(VAO[0]);
		double currentTime = glfwGetTime();
		double elapsedTime = currentTime - startTime;

		// Change frame colour every 5 seconds
		float cycleDuration = 5.0;
		float timeVar = fmod(elapsedTime, cycleDuration) / cycleDuration;  // Normalize to the range [0, 1]
		float timeColorRed = 0.5 + 0.5 * sin(timeVar * 2.0 * PI);
		float timeColorGreen = 0.5 + 0.5 * sin(timeVar * 2.0 * PI + 2.0);
		float timeColorBlue = 0.5 + 0.5 * sin(timeVar * 2.0 * PI + 4.0);


		if (elapsedTime > 5) {
			glUniform1f(glGetUniformLocation(basicShader, "timeColorRed"), timeColorRed);
			glUniform1f(glGetUniformLocation(basicShader, "timeColorGreen"), timeColorGreen);
			glUniform1f(glGetUniformLocation(basicShader, "timeColorBlue"), timeColorBlue);
		}

		if (rotateFrames) {
			glUniform1i(glGetUniformLocation(basicShader, "rotate"), 1);
			for (int i = 0; i < TEXTURE_COUNT; ++i) {
				drawRotatedSquare(i, radii[i], rotationSpeeds[i], uPosLoc);
			}
		}
		else {
			glUniform1i(glGetUniformLocation(basicShader, "rotate"), 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			for (int i = 0; i < TEXTURE_COUNT; ++i) {
				glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Yellow Button
		glUseProgram(circleShader);
		glBindVertexArray(VAO[1]);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			changeCircleColor = true;
			rotateFrames = false;
		}
		else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			changeCircleColor = false;
			rotateFrames = true;
		}
		if (changeCircleColor) {
			glClearColor(0.3, 0.3, 0.3, 1.0);
			glUniform4f(glGetUniformLocation(circleShader, "circleColor"), 0.0, 0.0, 0.0, 1.0);
		}
		else {
			glClearColor(0.7, 0.7, 0.7, 1.0);
			glUniform4f(glGetUniformLocation(circleShader, "circleColor"), 1.0, 1.0, 0.0, 1.0);
		}

		glUniform2f(uPosLoc, 0, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));

		// Textures
		glUseProgram(textureShader);
		glBindVertexArray(VAO[2]);

		glBindTexture(GL_TEXTURE_2D, signatureTexture);


		glUniform2f(tPosLoc, 0, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);

		for (int i = 0; i < TEXTURE_COUNT; ++i) {
			// Set rotation uniform based on the rotate condition
			glUniform1i(glGetUniformLocation(textureShader, "rotation"), rotateFrames ? 1 : 0);

			// Bind texture
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			// Set texture position uniform
			glUniform2f(tPosLoc, radii[i] * cos(glfwGetTime() * rotationSpeeds[i]), radii[i] * sin(glfwGetTime() * rotationSpeeds[i]));

			// Draw arrays
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);

			// Unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Proggress bar
		glUseProgram(progressBarShader);
		glBindVertexArray(VAO[3]);
		glUniform1f(glGetUniformLocation(progressBarShader, "barColor"), progressBar);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 3, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Cleanup
	glDeleteBuffers(4, VBO);
	glDeleteVertexArrays(4, VAO);
	glDeleteProgram(unifiedShader);
	glfwTerminate();
	return 0;
}