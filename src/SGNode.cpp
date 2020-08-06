#include "SGNode.h"

SGNode::SGNode() {
	object = nullptr;
	parent = nullptr;
}

SGNode::SGNode(Object* obj) {
	object = obj;
	parent = nullptr;
	obj->setNode(this);
}

void SGNode::addChild(SGNode* child) {
	children.push_back(child);
}

std::vector<SGNode*> SGNode::getChildren() {
	return children;
}

SGNode* SGNode::getParent() {
	return parent;
}
void SGNode::setParent(SGNode newParent) {
	parent = &newParent;
}

Object* SGNode::getObject() {
	return object;
}

void SGNode::setObject(Object newObject) {
	*object = newObject;
}

void SGNode::flatten(SGNode* node, std::vector<Object*>* elements) {
	if (object) {
		elements->push_back(object);
	}
	for (SGNode* child : children) {
		child->flatten(child, elements);
	}
}

void SGNode::flattenFrameEvent(SGNode* node, std::vector<Object*>* elementsFE) {
	if (object && object->frameEvent) {
		elementsFE->push_back(object);
	}
	for (SGNode* child : children) {
		child->flattenFrameEvent(child, elementsFE);
	}
}

void SGNode::viewSceneNode(int level) {
	for (SGNode* child : children) {
		for (size_t i = 0; i < level; i++) {
			std::cout << "-";
		}
		if (child->object != nullptr) {
			if (child->object->getName() != "") {
				std::cout << " " << child->object->getName() << std::endl;
			}
			else {
				std::cout << " " << child->object->getMesh()->getModelPath() << std::endl;
			}
		}
		child->viewSceneNode(level + 1);
	}
}

int SGNode::nbElements() {
	int somme = 0;
	for (SGNode* child : children) {
		somme += child->nbElements();
	}
	return (int) children.size() + somme;
}
