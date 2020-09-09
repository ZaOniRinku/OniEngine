#include "Skybox.h"

Skybox::Skybox() {
	rightFacePath = "";
	rightFaceRValue = 0.0f;
	rightFaceGValue = 0.0f;
	rightFaceBValue = 0.0f;

	leftFacePath = "";
	leftFaceRValue = 0.0f;
	leftFaceGValue = 0.0f;
	leftFaceBValue = 0.0f;

	topFacePath = "";
	topFaceRValue = 0.0f;
	topFaceGValue = 0.0f;
	topFaceBValue = 0.0f;

	bottomFacePath = "";
	bottomFaceRValue = 0.0f;
	bottomFaceGValue = 0.0f;
	bottomFaceBValue = 0.0f;

	backFacePath = "";
	backFaceRValue = 0.0f;
	backFaceGValue = 0.0f;
	backFaceBValue = 0.0f;

	frontFacePath = "";
	frontFaceRValue = 0.0f;
	frontFaceGValue = 0.0f;
	frontFaceBValue = 0.0f;
}

Skybox::Skybox(std::string rightFPath, std::string leftFPath, std::string topFPath, std::string bottomFPath, std::string backFPath, std::string frontFPath) {
	rightFacePath = rightFPath;
	rightFaceRValue = 0.0f;
	rightFaceGValue = 0.0f;
	rightFaceBValue = 0.0f;

	leftFacePath = leftFPath;
	leftFaceRValue = 0.0f;
	leftFaceGValue = 0.0f;
	leftFaceBValue = 0.0f;

	topFacePath = topFPath;
	topFaceRValue = 0.0f;
	topFaceGValue = 0.0f;
	topFaceBValue = 0.0f;

	bottomFacePath = bottomFPath;
	bottomFaceRValue = 0.0f;
	bottomFaceGValue = 0.0f;
	bottomFaceBValue = 0.0f;

	backFacePath = backFPath;
	backFaceRValue = 0.0f;
	backFaceGValue = 0.0f;
	backFaceBValue = 0.0f;

	frontFacePath = frontFPath;
	frontFaceRValue = 0.0f;
	frontFaceGValue = 0.0f;
	frontFaceBValue = 0.0f;
}

std::string Skybox::getRightFacePath() {
	return rightFacePath;
}

float Skybox::getRightFaceRValue() {
	return rightFaceRValue;
}

float Skybox::getRightFaceGValue() {
	return rightFaceGValue;
}

float Skybox::getRightFaceBValue() {
	return rightFaceBValue;
}

void Skybox::setRightFaceValue(float newRightFaceRValue, float newRightFaceGValue, float newRightFaceBValue) {
	rightFaceRValue = newRightFaceRValue;
	rightFaceGValue = newRightFaceGValue;
	rightFaceBValue = newRightFaceBValue;
}

std::string Skybox::getLeftFacePath() {
	return leftFacePath;
}

float Skybox::getLeftFaceRValue() {
	return leftFaceRValue;
}

float Skybox::getLeftFaceGValue() {
	return leftFaceGValue;
}

float Skybox::getLeftFaceBValue() {
	return leftFaceBValue;
}

void Skybox::setLeftFaceValue(float newLeftFaceRValue, float newLeftFaceGValue, float newLeftFaceBValue) {
	leftFaceRValue = newLeftFaceRValue;
	leftFaceGValue = newLeftFaceGValue;
	leftFaceBValue = newLeftFaceBValue;
}

std::string Skybox::getTopFacePath() {
	return topFacePath;
}

float Skybox::getTopFaceRValue() {
	return topFaceRValue;
}

float Skybox::getTopFaceGValue() {
	return topFaceGValue;
}

float Skybox::getTopFaceBValue() {
	return topFaceBValue;
}

void Skybox::setTopFaceValue(float newTopFaceRValue, float newTopFaceGValue, float newTopFaceBValue) {
	topFaceRValue = newTopFaceRValue;
	topFaceGValue = newTopFaceGValue;
	topFaceBValue = newTopFaceBValue;
}

std::string Skybox::getBottomFacePath() {
	return bottomFacePath;
}

float Skybox::getBottomFaceRValue() {
	return bottomFaceRValue;
}

float Skybox::getBottomFaceGValue() {
	return bottomFaceGValue;
}

float Skybox::getBottomFaceBValue() {
	return bottomFaceBValue;
}

void Skybox::setBottomFaceValue(float newBottomFaceRValue, float newBottomFaceGValue, float newBottomFaceBValue) {
	bottomFaceRValue = newBottomFaceRValue;
	bottomFaceGValue = newBottomFaceGValue;
	bottomFaceBValue = newBottomFaceBValue;
}

std::string Skybox::getBackFacePath() {
	return backFacePath;
}

float Skybox::getBackFaceRValue() {
	return backFaceRValue;
}

float Skybox::getBackFaceGValue() {
	return backFaceGValue;
}

float Skybox::getBackFaceBValue() {
	return backFaceBValue;
}

void Skybox::setBackFaceValue(float newBackFaceRValue, float newBackFaceGValue, float newBackFaceBValue) {
	backFaceRValue = newBackFaceRValue;
	backFaceGValue = newBackFaceGValue;
	backFaceBValue = newBackFaceBValue;
}

std::string Skybox::getFrontFacePath() {
	return frontFacePath;
}

float Skybox::getFrontFaceRValue() {
	return frontFaceRValue;
}

float Skybox::getFrontFaceGValue() {
	return frontFaceGValue;
}

float Skybox::getFrontFaceBValue() {
	return frontFaceBValue;
}

void Skybox::setFrontFaceValue(float newFrontFaceRValue, float newFrontFaceGValue, float newFrontFaceBValue) {
	frontFaceRValue = newFrontFaceRValue;
	frontFaceGValue = newFrontFaceGValue;
	frontFaceBValue = newFrontFaceBValue;
}