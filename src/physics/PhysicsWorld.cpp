//
//  PhysicsWorld.cpp
//  Game Engine
//
//  Created by isaiah ballah on 2016-03-28.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "PhysicsWorld.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

btVector3 toVector3(const vec3 &vec) {
	return btVector3(vec.x, vec.y, vec.z);
}

vec3 toVec3(const btVector3 &vec) {
	return vec3(vec.getX(), vec.getY(), vec.getZ());
}

void PhysicsWorld::addBlock(Block *block) {
	vec3 halfSize = block->getSize();
	vec3 position = block->getPosition();
	btBoxShape *boxShape = new btBoxShape(toVector3(halfSize));
	
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), toVector3(position)));
	btScalar mass = 10 * glm::length(block->getSize());
	btVector3 inertia(0, 0, 0);
	boxShape->calculateLocalInertia(mass, inertia);
	
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState,  boxShape, inertia);
	btRigidBody* body = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(body);
	
	block->setBody(body);
	blocks.push_back(tuple<btRigidBody*, Block*>(body, block));
	
	body->setFriction(0);
	body->setRestitution(0);
	body->setDamping(0.995, 0);
}

void PhysicsWorld::update(float delta) {
	dynamicsWorld->stepSimulation(delta, 10);
	
	for (int i = 0; i < blocks.size(); i++) {
		btTransform trans;
		btRigidBody *rigidBody = get<0>(blocks[i]);
		Block *block = get<1>(blocks[i]);

		rigidBody->getMotionState()->getWorldTransform(trans);
		btQuaternion quaternion(trans.getRotation());
		vec3 position = toVec3(trans.getOrigin());
		vec3 axis = toVec3(quaternion.getAxis());
		float angle = quaternion.getAngle();
		
		block->setPosition(position, false);
		//block->setRotation(angle, axis, false);
	}
}