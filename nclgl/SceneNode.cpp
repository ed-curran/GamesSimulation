#include "SceneNode.h"

SceneNode::SceneNode(Mesh*mesh, Vector4 colour) {
	awake = true;
	this->mesh = mesh;
	this->colour = colour;
	parent = NULL;
	boundingRadius = 100.0f;
	distanceFromCamera = 0.0f;

	modelScale = Vector3(1, 1, 1);
}

SceneNode::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}

bool	SceneNode::CompareByCameraDistance(SceneNode*a, SceneNode*b) {
	return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
}

bool	SceneNode::CompareByCameraDistanceReverse(SceneNode*a, SceneNode*b) {
	return (a->distanceFromCamera < b->distanceFromCamera) ? false : true;
}

bool	SceneNode::CompareByZ(SceneNode*a, SceneNode*b) {
	return (a->GetWorldTransform().GetPositionVector().z < b->GetWorldTransform().GetPositionVector().z) ? true : false;
}

void	SceneNode::Update(float msec) {
	if (parent && inheritParentTransform) {
		if (physicsNode != nullptr) {
			SetModelScale(physicsNode->getScale());
			SetColour(physicsNode->colour);

			SetTransform(Matrix4::Translation(physicsNode->getPosition3d())* physicsNode->getQrotation().ToMatrix());
		}
		
		//we don't want node to inherit depth of parent, not sure how to do this
		worldTransform = parent->worldTransform * transform  * Matrix4::Rotation(baseRotation, Vector3(0, 0, 1));
	}
	else {
		worldTransform = transform;
	}

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

bool SceneNode::RemoveChild(SceneNode* s, bool recursive) {

	for (auto i = children.begin(); i != children.end(); ++i) {
		if ((*i) == s) {
			i = children.erase(i);
			return true;
		}
	}

	if (recursive) {
		for (auto i = children.begin(); i != children.end(); ++i) {
			if ((*i)->RemoveChild(s, recursive)) {
				return true;
			}
		}
	}
	return false;
}