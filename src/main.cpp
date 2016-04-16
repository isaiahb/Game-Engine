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
#include <math.h>

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
Shader colorShader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/basic_color.vert",
				   "/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/basic_color.frag");
Shader colorModelShader("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/mesh_color.vert",
						"/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/mesh_color.frag");

Shader test("/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/spot.vert",
			"/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/spot.frag",
			"/Users/isaiah/Documents/computer science/Game Engine/src/graphics/shaders/spot.geo");


Renderer renderer(&shader, &skyboxShader, &colorShader);
Block block0( 0.5, 2, -4, 1, 1, 1);
Block block2(-2, -1, -4, 1, 1, 1);
Block block3( 2, -1, -4, 1, 1, 1);
Block plane(0.5, -2.5, 0.5, 20, 1, 20);
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


float radians(float degrees) {
	float radians = ( degrees * 3.14159 ) / 180;
	return radians;
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
//    window.setClearColor(0.035f, 0.035f, 0.15f, 1.0f);
	window.setClearColor(1, 1, 1, 1.0f);

	
    shader.enable();
    shader.setUniform1i("ourShader", 0);
    shader.setUniform4f("colour", vec4(0.2f, 0.3f, 0.8f, 1.0f));
    shader.setUniformMat4("projectionMatrix", projectionMatrix);
	shader.disable();
	
	modelShader.enable();
	modelShader.setUniformMat4("projection", projectionMatrix);
	
	colorModelShader.enable();
	colorModelShader.setUniformMat4("projection", projectionMatrix);

	colorShader.enable();
	colorShader.setUniform1i("ourShader", 0);
	colorShader.setUniform4f("colour", vec4(0.2f, 0.3f, 0.8f, 1.0f));
	colorShader.setUniformMat4("projectionMatrix", projectionMatrix);
	colorShader.disable();
	
    skyboxShader.enable();
    skyboxShader.setUniformMat4("projectionMatrix", projectionMatrix);
	skyboxShader.disable();
	
	test.enable();
	test.setUniformMat4("projectionMatrix", projectionMatrix);
	test.disable();
	
	
	
}

void print(const char* text, vec3 vec) {
	std::cout<<text<<" "<<vec.x<<" ,"<<vec.y<<" ,"<<vec.z<<std::endl;
}

void print(const char* text, vec4 vec) {
	std::cout<<text<<" x: "<<vec.x<<" ,y: "<<vec.y<<" ,z: "<<vec.z<<std::endl;
}

void print(const char* text, vec2 vec) {
	std::cout<<text<<" x: "<<vec.x<<" ,y: "<<vec.y<<std::endl;
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
	
	block0.getBody()->applyForce(btVector3(v.x, v.y, v.z) * 50, btVector3());

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
	//print("1 ", wSC1);
	//print("2 ", wSC2);
	vec4 dir = wSC1 - wSC2;
	//print("dir", dir);
	
	vec3 startRay = camera.getPosition();//vStart;//camera.getPosition();
	vec3 endRay = startRay + vec3(dir) * 50.0f;//End; //startRay + (vec3(world) * rayLength);
	//print("start ", startRay);
	//print("end ", endRay);
	
	btVector3 start = PhysicsWorld::toVector3(startRay);
	btVector3 end = PhysicsWorld::toVector3(endRay);
	
	
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	
	// Perform raycast
	physicsWorld.getWorld()->rayTest(start, end, RayCallback);
	
	if(RayCallback.hasHit()) {
		vec3 hit = PhysicsWorld::toVec3(RayCallback.m_hitPointWorld);
		vec3 normal = PhysicsWorld::toVec3(RayCallback.m_hitNormalWorld);
		
		// Do some clever stuff here
		//print("hit ", hit);
		//print("normal ", normal);
		tuple<bool, vec3> t (true, hit);
		return t;
	} else {
		//std::cout<<"Ray not hit"<<std::endl;
	}
	tuple<bool, vec3> t (false, vec3());
	return t;
}

vec3 spot(0,0,0);
void pressed(glm::vec2 position) {
	tuple<bool, vec3> mouseHit = getWorldMousePosition();
	if (get<0>(mouseHit)) {
		spot = get<1>(mouseHit);
		print("spot", spot);
	}
}

vec2 roundVec(vec2 vec) {
	return vec2(round(vec.x), round(vec.y));
}
vec3 roundVec(vec3 vec) {
	return vec3(round(vec.x), round(vec.y), round(vec.z));
}
vec4 roundVec(vec4 vec) {
	return vec4(round(vec.x), round(vec.y), round(vec.z), round(vec.w));
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

	mat4 scale = glm::scale(mat4(), vec3(.21, .21, .21));
	mat4 scale2 = glm::scale(mat4(), vec3(.21, .21, .21) * 1.05f);


	KeyPressed moveBlock = []() {
		tuple<bool, vec3> mouseHit = getWorldMousePosition();
		if (get<0>(mouseHit)) {
			mouseBlock.setPosition(get<1>(mouseHit), false);
		}
		
	};
	
	MouseScrolled scrolled = [](vec2 delta) {
		camera.addRadius(delta.y);
		camera.addRotation(radians(delta.x));

	};
	
	input->mouse.addMousePressedListener(1, pressed);
	input->mouse.addMouseScrolledListener(scrolled);
	input->keys.addKeyPressedListener(GLFW_KEY_ENTER, moveBlock);
	
	// Vertex data
	GLfloat points[] = {
		0, 0 // Top-left
	};
	
	test.enable();
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glBindVertexArray(0);
	test.setUniform3f("uColor", vec3(1,0,0));
	
    while(window.isRunning()) {
        if(window.isKeyPressed(GLFW_KEY_ESCAPE)) {
           //window.~Window();
			glfwTerminate();
            break;
        }
        float delta = calculateFPS();
		//moveBlock();
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
		
		colorModelShader.enable();
		colorModelShader.setUniformMat4("view", viewMatrix);
		colorModelShader.setUniform3f("light_pos", camera.getPosition());
		colorModelShader.setUniformMat4("model",block2.getModelMatrix() * scale2);
		colorModelShader.setUniform3f("view", camera.getPosition());
		colorModelShader.setUniform3f("light_pos", camera.getPosition());
		colorModelShader.setUniform3f("camera_position", camera.getPosition());
		
		colorShader.enable();
		colorShader.setUniformMat4("viewMatrix", viewMatrix);
		colorShader.setUniform3f("light_pos", camera.getPosition());
		colorShader.setUniform3f("camera_position", camera.getPosition());
		colorShader.setUniformMat4("instanceMatrix", block0.getModelMatrix());
		
        window.clear();
		test.enable();
		test.setUniformMat4("viewMatrix", viewMatrix);
		mat4 modelMatrix = glm::translate(vec3(round(spot.x), spot.y + 0.0001, round(spot.z)));
		test.setUniformMat4("modelMatrix", modelMatrix);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		//window.update();

		//continue;
		
		glEnable(GL_DEPTH_TEST);
        //renderer.renderSkybox();
		
		//Render Model
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		model.Draw(&modelShader);
		glFrontFace(GL_CCW);
		
		//Render Shadow
		//glDisable(GL_DEPTH_TEST);
		GLfloat scale = 1.05;
		glCullFace(GL_FRONT);
		renderer.render(&Block::blocks, scale);
		
//		glFrontFace(GL_CW);
//		model.Draw(&colorModelShader);
//		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		renderer.render(&Block::blocks);

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

