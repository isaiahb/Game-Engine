#pragma once
#include <iostream>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <btBulletDynamicsCommon.h>
#include "Block.h"
#include <tuple>
#include <vector>


class PhysicsWorld {
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	
	//list of all physics bodies with their coupled graphics bodies
	vector<tuple<btRigidBody*, Block*>> blocks;
	
	
public:
	PhysicsWorld() {
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
		dynamicsWorld->setGravity(btVector3(0,-40,0));
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -2.5, 0)));
		
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
//		dynamicsWorld->addRigidBody(groundRigidBody);
		
		groundRigidBody->setFriction(0.9);
		groundRigidBody->setRestitution(0);
	}
	~PhysicsWorld() {
		// Clean up behind ourselves like good little programmers
		delete dynamicsWorld;
		delete solver;
		delete dispatcher;
		delete collisionConfiguration;
		delete broadphase;
	}
	void update(float delta);
	void addBlock(Block*);
};