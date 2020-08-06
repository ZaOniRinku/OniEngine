#pragma once
#include <vector>
#include <iostream>
#include "Object.h"

class Object;

class SGNode {
public:
	SGNode();
	SGNode(Object* obj);
	void addChild(SGNode* child);
	std::vector<SGNode*> getChildren();
	SGNode* getParent();
	void setParent(SGNode parent);
	Object* getObject();
	void setObject(Object newObject);
	void flatten(SGNode* node, std::vector<Object*>* elements);
	void flattenFrameEvent(SGNode* node, std::vector<Object*>* elements);
	void viewSceneNode(int level);
	int nbElements();
private:
	std::vector<SGNode*> children;
	Object* object;
	SGNode* parent;
};
