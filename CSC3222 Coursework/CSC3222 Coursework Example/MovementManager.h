#pragma once
#include "Components/PhysicsNode.h"
#include "Components/VelocityNode.h"
#include <vector>
#include "Entity.h"
#include "Pathfinding.h"

/*
 * based of: https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-movement-manager--gamedev-4278
 */
struct MovementNode{
	PhysicsNode& physicsNode;
	VelocityNode&  velocityNode;
};

class MovementManager
{
public:
	const float MAX_LOOKAHEAD;
	const float SEPERATION_RADIUS;
	const float MAX_SEPERATION = 40.0f;
	const float MIN_LEADER_BEHIND = Map::GRID_SIZE * 4;
	MovementManager(VelocityNode& host, CollidableNode& hostCollider, float maxForce = 100.0f, float lookahead = 100.0f, float seperationRadius = Map::GRID_SIZE * 1.3f) : 
	host(host), hostCollider(hostCollider), steeringForce(), maxForce(maxForce), MAX_LOOKAHEAD(lookahead), SEPERATION_RADIUS(seperationRadius){
		this->count = rand();
	};
	~MovementManager();

	MovementManager(const MovementManager& src) : host(src.host), hostCollider(src.hostCollider),
		maxForce(src.maxForce), MAX_LOOKAHEAD(src.MAX_LOOKAHEAD),
		SEPERATION_RADIUS(src.SEPERATION_RADIUS), obstacles(src.obstacles), flock(src.flock),
		path(src.path), pathIsSet(src.pathIsSet), currentPathLocation(src.currentPathLocation) {};

	MovementManager operator=(const MovementManager& rhs){
		return MovementManager(rhs);
	}

	void setHost(VelocityNode& host, CollidableNode& hostCollider){


	}


	void update();
	void setMaxForce(float maxForce){
		this->maxForce = maxForce;
	}

	float getMaxForce() const{
		return maxForce;
	}

	void applySeek(Vector2 target, float arrivalRadius = 0);
	void applyFlee(Vector2 target);
	void applyEvade(VelocityNode& target);
	void applyObstacleAvoidance();
	void applyFollowPath(float pathRadius = 10.0f);

	void addObstacles(std::vector<Entity>* obstacles){
		this->obstacles = obstacles;
	}
	void setFlock(std::vector<Entity*> flock){
		this->flock = flock;
	};
	void applyFollowLeader(VelocityNode& leader, float leaderSightDistance = 30.0f, float leaderBehindDistance = 20.0f);

	void setPath(vector<Node>& nodePath) {
		resetPath();
		pathIsSet = true;
		for(auto node : nodePath){
			path.push_back(node.position);
		}	
	}

	void resetPath(){
		pathIsSet = false;
		path.clear();
		currentPathLocation = 0;
	}

	void reversePathDirection(){
		pathDirection = -pathDirection;
	}

	bool atPathEnd(float radius){
		if (currentPathLocation != path.size() - 1 || pathDirection < 0) return false;

		return Vector2::Distance(host.getPosition(), path[currentPathLocation]) < radius;
	}

	bool atPathStart(float radius) {
		if (currentPathLocation != 0 || pathDirection > 0) return false;

		return Vector2::Distance(host.getPosition(), path[currentPathLocation]) < radius;
	}

	int getPathDirection(){
		return pathDirection;
	}

protected:
	VelocityNode& host;
	CollidableNode& hostCollider;
	Vector2 steeringForce;
	float maxForce;
	std::vector<Entity>* obstacles;
	std::vector<Entity*> flock;
	std::vector<Vector2> path;
	int currentPathLocation;
	bool pathIsSet;
	int pathDirection = 1;
	int count = 0;
	

	Vector2 seek(Vector2 target, float arrivalRadius = 0);
	Vector2 flee(Vector2 target);
	Vector2 evade(VelocityNode& target);
	Vector2 obstacleAvoidance();
	Vector2 obstacleAvoidanceRaycast(Vector2 ahead, Vector2 ahead2);
	Vector2 followLeader(VelocityNode& leader, float leaderSightDistance, float leaderBehindDistance);
	Vector2 seperation();
	Vector2 followPath(float pathRadius);


	void addSteeringForce(Vector2 force, float weight = 1.0f){
		steeringForce += force * weight;
	}

	bool lineCircleIntersection(Vector2 ahead, Vector2 ahead2, Entity& obstacle, CollisionData& outCollisionData);
	Entity* findNearestObstacle(Vector2 hostPosition, Vector2 ahead, Vector2 ahead2, CollisionData& outCollisionData);

};

