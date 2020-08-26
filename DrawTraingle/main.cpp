#include <iostream>

#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include<SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "Camera.h"




const GLuint WIDTH = 800, HEIGHT = 600;

int SCREEN_WIDTH, SCREEN_HEIGHT;


void KeyCallBack(GLFWwindow* window,int 
	key,int scancode,int actions ,int mode);



void
MouseCallBackFunction(GLFWwindow* window,double xPos,double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void DoMovementFunction();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;

bool keys[1024];
bool FirstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


glm::vec3 lightPos(1.2f,1.0f,-2.0f);



int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Create Open GL", nullptr, nullptr);

	

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (nullptr == window)
	{
		std::cout << "window initialize has to been failured" << std::endl;

		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window,KeyCallBack);
	glfwSetCursorPosCallback(window, MouseCallBackFunction);
  

	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);



	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Glew Initialitate has been init failure" << std::endl;


		return EXIT_FAILURE;
	}

	glViewport(0, 0,SCREEN_WIDTH, SCREEN_HEIGHT);


	glEnable(GL_DEPTH_TEST);


    // Build and compile our shader program
    Shader lightingShader("lighting.vs", "lighting.frag");
   // Shader lampShader("lamp.vs","lamp.frag");
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        // Positions            // Normals              // Texture Coords
      -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
      0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
      -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

      -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
      0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
      -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

      -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
      -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
      0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
      0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
      0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
      0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
      0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

      -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
      0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
      0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
    };


    // Positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,   0.0f,   0.0f),
        glm::vec3(2.0f,   5.0f,   -15.0f),
        glm::vec3(-1.5f,  -2.2f,  -2.5f),
        glm::vec3(-3.8f,  -2.0f,  -12.3f),
        glm::vec3(2.4f,   -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,   -7.5f),
        glm::vec3(1.3f,   -2.0f,  -2.5f),
        glm::vec3(1.5f,   2.0f,   -2.5f),
        glm::vec3(1.5f,   0.2f,   -1.5f),
        glm::vec3(-1.3f,  1.0f,   -1.5f)
    };



    // First, set the container's VAO (and VBO)
    GLuint VBO, boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(boxVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat) ));
    glEnableVertexAttribArray(1);

    //Texture Attribute
    glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    
    glBindVertexArray(0);

    ////// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    //GLuint lightVAO;
    //glGenVertexArrays(1, &lightVAO);
    //glBindVertexArray(lightVAO);
    //// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //// Set the vertex attributes (only position data for the lamp))
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(0);
    //glBindVertexArray(0);


    GLuint diffuseMap, specularMap,emissionMap;

    glGenTextures(1, &diffuseMap);
    glGenTextures(1, &specularMap);

    glGenTextures(1, &emissionMap);


    int width, height;
    unsigned char* image;


    // Diffuse map
    image = SOIL_load_image("container2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    // Specular map
    image = SOIL_load_image("container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    lightingShader.Use();

    glUniform1i(glGetUniformLocation(lightingShader.Program,"material.diffuse"),0);
  glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);


    glm::mat4 projection = glm::perspective(camera.GetZoomFunction(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {


        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovementFunction();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
     
        GLint lightSpotLightDir = glGetUniformLocation
        (lightingShader.Program,"light.direction");
    
        GLint lightSpotLightCutoffLoc = glGetUniformLocation
        (lightingShader.Program, "light.Cutoff");

        GLint lightSpotLightCutOuterLoc = glGetUniformLocation
        (lightingShader.Program, "light.outerCutoff");



        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(lightSpotLightDir, camera.GetFrontFunction().x, camera.GetFrontFunction().y, camera.GetFrontFunction().z);
       
        glUniform1f(lightSpotLightCutoffLoc, 
            glm::cos(glm::radians(12.5f)));
     
        glUniform1f(lightSpotLightCutOuterLoc,
            glm::cos(glm::radians(17.5f)));
       
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);


        glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewmatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);


        // Draw the container (using container's vertex attributes)
        glm::mat4 model;
        glBindVertexArray(boxVAO);
        for (GLuint i = 0; i < 10; i++)
        {
            model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        ////// Also draw the lamp object, again binding the appropriate shader
        //lampShader.Use();
        //// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        //modelLoc = glGetUniformLocation(lampShader.Program, "model");
        //viewLoc = glGetUniformLocation(lampShader.Program, "view");
        //projLoc = glGetUniformLocation(lampShader.Program, "projection");

        //// Set matrices
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //model = glm::mat4();
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //// Draw the light object (using light's vertex attributes)
        //glBindVertexArray(lightVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &boxVAO);
   // glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

	glfwTerminate();



	return EXIT_SUCCESS;
}




void KeyCallBack(GLFWwindow* window, int
	key, int scancode, int actions, int mode)
{
	if (key == GLFW_KEY_ESCAPE && actions ==GLFW_PRESS )
	{
		glfwSetWindowShouldClose(window,GL_TRUE);

	}

	if (key>= 0 && key<1024)
	{
		if (GLFW_PRESS ==actions )
		{
			keys[key] = true
				;
		}
		else if(GLFW_RELEASE == actions)
		{
			keys[key] = false
				;
		}


	}

}


void
MouseCallBackFunction(GLFWwindow* window, double xPos, double yPos)
{
	if (FirstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		FirstMouse = false;
	}
	GLfloat xoffset = xPos -lastX;
	GLfloat yoffset = lastY - yPos;


	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovmentFunction(xoffset, yoffset);

}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessMouseScrollFunction(yOffset);


}


void DoMovementFunction()
{
	if (keys[GLFW_KEY_W]||
		keys[GLFW_KEY_UP])
	{
		camera.ProcessInputFunction(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] ||
		keys[GLFW_KEY_DOWN])
	{
		camera.ProcessInputFunction(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] ||
		keys[GLFW_KEY_LEFT])
	{
		camera.ProcessInputFunction(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] ||
		keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessInputFunction(RIGHT, deltaTime);
	}

}
