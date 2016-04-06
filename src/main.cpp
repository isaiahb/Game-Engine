#include "Window.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Buffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "FreeCam.h"
#include "ShapeGenerator.h"
#include "SOIL.h"
#include "Block.h"
#include "Renderer.h"
#include "PhysicsWorld.hpp"

#include "bullet/btBulletDynamicsCommon.h"
#include "BlockCam.hpp"
#include "Model.hpp"
//#include <freetype/ft2build.h>
//#include FT_FREETYPE_H

using namespace ballah;
using namespace graphics;
using namespace glm;

//Camera camera;

Window window(960, 540, "Ballah");

Shader shader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/basic.vert",
			  "/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/basic.frag");
Shader modelShader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/mesh.vert",
				   "/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/mesh.frag");

Shader skyboxShader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/skybox.vert",
					"/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/skybox.frag");

Renderer renderer(&shader, &skyboxShader);

int frameCount;
float currentTime, previousTime;
float lastFrameTime;
float fps;

float calculateFPS() {
    frameCount++;
    currentTime = glfwGetTime();
    float delta = currentTime - lastFrameTime;

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;
	float delay = 20;
    if(timeInterval > delay) {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1.0f);
        previousTime = currentTime;
        frameCount = 0;
        std::cout<<"FPS "<<fps<<std::endl;
    }

    lastFrameTime = currentTime;
    return delta;
}
GLuint createBlockTexture() {
	shader.enable();
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    int width, height;
    unsigned char* image = SOIL_load_image("/Users/isaiah/Documents/computer science/Game Engine/src/assets/lava.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
	SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

mat4 projectionMatrix = glm::perspective(30.0f, (float)window.getWidth() / window.getHeight(), 0.10f, -10.0f);
void init() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    window.setClearColor(0.035f, 0.035f, 0.15f, 1.0f);
    
    
	
    shader.enable();
    shader.setUniform1i("ourShader", 0);
    shader.setUniform4f("colour", vec4(0.2f, 0.3f, 0.8f, 1.0f));
    shader.setUniformMat4("projectionMatrix", projectionMatrix);
	shader.disable();
	
	modelShader.enable();
	modelShader.setUniformMat4("projection", projectionMatrix);
	
    skyboxShader.enable();
    skyboxShader.setUniformMat4("projectionMatrix", projectionMatrix);
	skyboxShader.disable();
    
}

void print(const char* text, vec3 vec) {
	std::cout<<text<<" "<<vec.x<<" ,"<<vec.y<<" ,"<<vec.z<<std::endl;
}
void updateCamera(float delta, BlockCam &camera, Block &block0) {
	vec3 moveVector(0,0,0);
	if(window.isKeyPressed(GLFW_KEY_W)) moveVector.z += 1;
	if(window.isKeyPressed(GLFW_KEY_S)) moveVector.z -= 1;
	if(window.isKeyPressed(GLFW_KEY_D)) moveVector.x -= 1;
	if(window.isKeyPressed(GLFW_KEY_A)) moveVector.x += 1;
	if(window.isKeyPressed(GLFW_KEY_Q)) {
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if(window.isKeyPressed(GLFW_KEY_E)) {
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	//if(window.isMouseButtonPressed(0)) camera.reset();
	if (window.isMouseButtonPressed(0))
		camera.update(window.mouse);
	else
		camera.setLastMousePosition(window.mouse);
	
	//camera.move(moveVector);
	btVector3 velo = block0.getBody()->getLinearVelocity();
	vec3 v(-moveVector.x * 20, velo.getY(), -moveVector.z * 20);
	camera.move(moveVector, v);
	//print("velo ", v);
	
	//block0.getBody()->setLinearVelocity(btVector3(v.x, v.y, v.z));
	block0.getBody()->applyForce(btVector3(v.x, v.y, v.z) * 50, btVector3());
	//float angle = (camera.getRotation() - 3.14159f / 2.0f);
	//vec3 axis(0, 1, 0);
	
	//block0.setRotation(angle, axis);
	
	if(glm::length(moveVector)>0) block0.getBody()->activate();
}
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.3f, -1.6f, -3.0f),
	glm::vec3(-1.7f, 0.9f, 1.0f)
};

int main(void){
    init();
	
	modelShader.enable();
	Model model("/Users/isaiah/Desktop/green dragon/dragon.blend");
//	Model model("/Users/isaiah/Desktop/space/ship.blend");

	GLuint blockTexture = createBlockTexture();
	renderer.setBlockTexture(blockTexture);
    Block block0( 0,  2, -4, 1, 1, 1);
    Block block2(-2, -1, -4, 1,1,1);
    Block block3( 2, -1, -4, 1, 1, 1);
	
	Block plane(2, -2.5, -4, 20, 1, 20);
	
	modelShader.enable();
	modelShader.setUniformMat4("model", block2.getModelMatrix());

	BlockCam camera(&block0);
	
	PhysicsWorld physicsWorld;
	physicsWorld.addBlock(&block0);
	physicsWorld.addBlock(&block2);
	physicsWorld.addBlock(&block3);
	physicsWorld.addBlock(&plane);
	plane.getBody()->setMassProps(0, btVector3(0,0,0));

	mat4 scale = glm::scale(mat4(), vec3(.21,.21,.21));
//	mat4 scale = glm::scale(mat4(), vec3(1.21,1.21,1.21));

    while(window.isRunning()) {
        if(window.isKeyPressed(GLFW_KEY_ESCAPE)) {
           window.~Window();
            break;
        }
        float delta = calculateFPS();
		
		updateCamera(delta, camera, block0);
		physicsWorld.update(delta);
		
        mat4 viewMatrix = camera.getWorldToViewMatrix();
		shader.enable();
        shader.setUniformMat4("viewMatrix", viewMatrix);
        shader.setUniform3f("light_pos", camera.getPosition());
        shader.setUniform3f("camera_position", camera.getPosition());
		shader.setUniformMat4("instanceMatrix", block0.getModelMatrix());
		
		glm::mat4 viewDirection = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.enable();
        skyboxShader.setUniformMat4("viewMatrix", viewDirection);
		
		modelShader.enable();
		modelShader.setUniformMat4("view", viewMatrix);
		modelShader.setUniform3f("light_pos", camera.getPosition());
		modelShader.setUniformMat4("model",block2.getModelMatrix() * scale);
		modelShader.setUniform3f("view", camera.getPosition());
		modelShader.setUniform3f("light_pos", camera.getPosition());
		modelShader.setUniform3f("camera_position", camera.getPosition());
		
		
		float rotation = block0.getBody()->getWorldTransform().getRotation().z();
		std::cout<<"rotation "<<rotation<<std::endl;

        window.clear();
        renderer.renderSkybox();
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		model.Draw(&modelShader);
		glFrontFace(GL_CCW);
        renderer.render(&Block::blocks);
		


        window.update();
        
    }
}

