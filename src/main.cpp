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
#include "Input.hpp"

//#include <freetype/ft2build.h>
//#include FT_FREETYPE_H

using namespace ballah;
using namespace graphics;
using namespace glm;

//Camera camera;

Window window(960, 540, "Ballah");
Input *input;// = window.getInput();
Shader shader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/basic.vert",
			  "/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/basic.frag");
Shader modelShader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/mesh.vert",
				   "/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/mesh.frag");
Shader skyboxShader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/skybox.vert",
					"/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/skybox.frag");
Renderer renderer(&shader, &skyboxShader);
Block block0( 0,  2, -4, 1, 1, 1);
Block block2(-2, -1, -4, 1, 1, 1);
Block block3( 2, -1, -4, 1, 1, 1);
Block plane(2, -2.5, -4, 20, 1, 20);
Block mouseBlock(0,0,0, 0.5,0.5,0.5);


BlockCam camera(&block0);
PhysicsWorld physicsWorld;

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

mat4 projectionMatrix = glm::perspective(30.0f, (float)window.getWidth() / window.getHeight(), .10f, -10.0f);
void init() {
	input = window.getInput();
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

void print(const char* text, vec4 vec) {
	std::cout<<text<<" x: "<<vec.x<<" ,y: "<<vec.y<<" ,z: "<<vec.z<<std::endl;
}
void updateCamera(float delta, BlockCam &camera, Block &block0) {
	vec3 moveVector(0,0,0);

	if(input->keys.keyDown[GLFW_KEY_W]) moveVector.z += 1;
	if(input->keys.keyDown[GLFW_KEY_S]) moveVector.z -= 1;
	if(input->keys.keyDown[GLFW_KEY_D]) moveVector.x -= 1;
	if(input->keys.keyDown[GLFW_KEY_A]) moveVector.x += 1;
	
	if(input->keys.keyDown[GLFW_KEY_Q]) {
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if(input->keys.keyDown[GLFW_KEY_E]) {
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	if (window.isMouseButtonPressed(0))
		camera.update(input->mouse.position);//camera.update(window.mouse);
	else
		camera.setLastMousePosition(input->mouse.position);//camera.setLastMousePosition(window.mouse);
	
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

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
Input::Mouse *mouse;

//if mouse hits something returns <true, hitPosition> if not <false, 0 vec>
tuple<bool, vec3> getWorldMousePosition() {
	mouse = &input->mouse;
	
	mat4 viewMatrix = camera.getWorldToViewMatrix();
	
	GLint viewport[4];
	GLfloat winX, winY;
	glm::mat4x4 mMatrix;
	glm::mat4x4 vMatrix = viewMatrix;
	glm::mat4x4 pMatrix = projectionMatrix;
	
	glm::mat4x4 cameraTransformation;
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = ((float)mouse->position.x/viewport[2]*2)-1;
	winY = ((float)(viewport[3]-mouse->position.y)/viewport[3]*2)-1;
	
	glm::mat4x4 mat =  glm::inverse(vMatrix) * glm::inverse(mMatrix) * glm::inverse(pMatrix);
	glm::vec4 wSC1 = (mat) * glm::vec4(winX,winY,-1,1);
	wSC1 /= wSC1[3];
	glm::vec4 wSC2 = (mat) * glm::vec4(winX,winY,1,1);
	wSC2 /= wSC2[3];
	print("1 ", wSC1);
	print("2 ", wSC2);
	vec4 dir = wSC1 - wSC2;
	print("dir", dir);
	
	vec3 startRay = camera.getPosition();//vStart;//camera.getPosition();
	vec3 endRay = startRay + vec3(dir) * 50.0f;//End; //startRay + (vec3(world) * rayLength);
	print("start ", startRay);
	print("end ", endRay);
	
	btVector3 start = PhysicsWorld::toVector3(startRay);
	btVector3 end = PhysicsWorld::toVector3(endRay);
	
	
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	
	// Perform raycast
	physicsWorld.getWorld()->rayTest(start, end, RayCallback);
	
	if(RayCallback.hasHit()) {
		vec3 hit = PhysicsWorld::toVec3(RayCallback.m_hitPointWorld);
		vec3 normal = PhysicsWorld::toVec3(RayCallback.m_hitNormalWorld);
		
		// Do some clever stuff here
		print("hit ", hit);
		print("normal ", normal);
		tuple<bool, vec3> t (true, hit);
		return t;
	} else {
		std::cout<<"Ray not hit"<<std::endl;
	}
	tuple<bool, vec3> t (false, vec3());
	return t;
}

void pressed(glm::vec2 position) {
	mouse = &input->mouse;

	mat4 viewMatrix = camera.getWorldToViewMatrix();

	GLint viewport[4];
	GLfloat winX, winY;
	glm::mat4x4 mMatrix;
	glm::mat4x4 vMatrix = viewMatrix;
	glm::mat4x4 pMatrix = projectionMatrix;

	glm::mat4x4 cameraTransformation;
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = ((float)mouse->position.x/viewport[2]*2)-1;
	winY = ((float)(viewport[3]-mouse->position.y)/viewport[3]*2)-1;
	
	glm::mat4x4 mat =  glm::inverse(vMatrix) * glm::inverse(mMatrix) * glm::inverse(pMatrix);
	glm::vec4 wSC1 = (mat) * glm::vec4(winX,winY,-1,1);
	wSC1 /= wSC1[3];
	glm::vec4 wSC2 = (mat) * glm::vec4(winX,winY,1,1);
	wSC2 /= wSC2[3];
	print("1 ", wSC1);
	print("2 ", wSC2);
	vec4 dir = wSC1 - wSC2;
	print("dir", dir);

	vec3 startRay = camera.getPosition();//vStart;//camera.getPosition();
	vec3 endRay = startRay + vec3(dir) * 50.0f;//End; //startRay + (vec3(world) * rayLength);
	print("start ", startRay);
	print("end ", endRay);
	
	btVector3 start = PhysicsWorld::toVector3(startRay);
	btVector3 end = PhysicsWorld::toVector3(endRay);
	

	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	
	// Perform raycast
	physicsWorld.getWorld()->rayTest(start, end, RayCallback);
	
	if(RayCallback.hasHit()) {
		vec3 hit = PhysicsWorld::toVec3(RayCallback.m_hitPointWorld);
		vec3 normal = PhysicsWorld::toVec3(RayCallback.m_hitNormalWorld);
		
		// Do some clever stuff here
		print("hit ", hit);
		print("normal ", normal);
		mouseBlock.setPosition(hit, false);
	} else {
		std::cout<<"Ray not hit"<<std::endl;
	}
}


int main(void){
    init();
	
	modelShader.enable();
	Model model("/Users/isaiah/Desktop/green dragon/dragon.blend");

	GLuint blockTexture = createBlockTexture();
	renderer.setBlockTexture(blockTexture);

	
	modelShader.enable();
	modelShader.setUniformMat4("model", block2.getModelMatrix());
	
	physicsWorld.addBlock(&block0);
	physicsWorld.addBlock(&block2);
	physicsWorld.addBlock(&block3);
	physicsWorld.addBlock(&plane);
	plane.getBody()->setMassProps(0, btVector3(0,0,0));

	mat4 scale = glm::scale(mat4(), vec3(.21,.21,.21));
//	mat4 scale = glm::scale(mat4(), vec3(1.21,1.21,1.21));
	////////////////////////////////////////
	/////////////FRAME BUFFER///////////////
	////////////////////////////////////////
	
	// Framebuffers
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	GLuint textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.getWidth(), window.getHeight()); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	//input->mouse.addMousePressedListener(0,pressed);
	
	KeyPressed moveBlock = []() {
//		std::cout<<"OMG DOES IT WORKS"<<std::endl;
		tuple<bool, vec3> mouseHit = getWorldMousePosition();
		if (get<0>(mouseHit)) {
			mouseBlock.setPosition(get<1>(mouseHit), false);
		}
		
	};
	input->keys.addKeyPressedListener(GLFW_KEY_ENTER, moveBlock);

	
    while(window.isRunning()) {
        if(window.isKeyPressed(GLFW_KEY_ESCAPE)) {
           window.~Window();
            break;
        }
        float delta = calculateFPS();
		moveBlock();
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
		//std::cout<<"rotation "<<rotation<<std::endl;

        window.clear();
        //renderer.renderSkybox();
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		model.Draw(&modelShader);
		glFrontFace(GL_CCW);
        renderer.render(&Block::blocks);
		GLvoid	*data;
						  

		window.update();
		
    }
}


// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type = 0;
	
	if(!depth && !stencil)
		attachment_type = GL_RGB;
	else if(depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if(!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;
	
	//Generate texture ID and load texture data
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if(!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, window.getWidth(), window.getHeight(), 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window.getWidth(), window.getHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return textureID;
}

