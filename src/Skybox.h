#pragma once
#include <string>

class Skybox {
public:
	Skybox();
	Skybox(std::string rightFPath, std::string leftFPath, std::string topFPath, std::string bottomFPath, std::string backFPath, std::string frontFPath);
	std::string getRightFacePath();
	float getRightFaceRValue();
	float getRightFaceGValue();
	float getRightFaceBValue();
	void setRightFaceValue(float newRightFaceRValue, float newRightFaceGValue, float newRightFaceBValue);

	std::string getLeftFacePath();
	float getLeftFaceRValue();
	float getLeftFaceGValue();
	float getLeftFaceBValue();
	void setLeftFaceValue(float newLeftFaceRValue, float newLeftFaceGValue, float newLeftFaceBValue);

	std::string getTopFacePath();
	float getTopFaceRValue();
	float getTopFaceGValue();
	float getTopFaceBValue();
	void setTopFaceValue(float newTopFaceRValue, float newTopFaceGValue, float newTopFaceBValue);

	std::string getBottomFacePath();
	float getBottomFaceRValue();
	float getBottomFaceGValue();
	float getBottomFaceBValue();
	void setBottomFaceValue(float newBottomFaceRValue, float newBottomFaceGValue, float newBottomFaceBValue);

	std::string getBackFacePath();
	float getBackFaceRValue();
	float getBackFaceGValue();
	float getBackFaceBValue();
	void setBackFaceValue(float newBackFaceRValue, float newBackFaceGValue, float newBackFaceBValue);

	std::string getFrontFacePath();
	float getFrontFaceRValue();
	float getFrontFaceGValue();
	float getFrontFaceBValue();
	void setFrontFaceValue(float newFrontFaceRValue, float newFrontFaceGValue, float newFrontFaceBValue);
private:
	std::string rightFacePath;
	float rightFaceRValue;
	float rightFaceGValue;
	float rightFaceBValue;

	std::string leftFacePath;
	float leftFaceRValue;
	float leftFaceGValue;
	float leftFaceBValue;

	std::string topFacePath;
	float topFaceRValue;
	float topFaceGValue;
	float topFaceBValue;

	std::string bottomFacePath;
	float bottomFaceRValue;
	float bottomFaceGValue;
	float bottomFaceBValue;

	std::string backFacePath;
	float backFaceRValue;
	float backFaceGValue;
	float backFaceBValue;

	std::string frontFacePath;
	float frontFaceRValue;
	float frontFaceGValue;
	float frontFaceBValue;
};

