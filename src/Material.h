#pragma once
#include <vulkan/vulkan.hpp>
#include <string>

class Material {
public:
	Material(std::string dPath, std::string nPath, std::string mPath, std::string rPath, std::string aPath);

	std::string getDiffusePath();
	void setDiffusePath(std::string newDiffusePath);
	float getDiffuseRValue();
	float getDiffuseGValue();
	float getDiffuseBValue();
	float getDiffuseAValue();
	void setDiffuseValues(float newDiffuseRValue, float newDiffuseGValue, float newDiffuseBValue, float newDiffuseAValue);

	std::string getNormalPath();
	void setNormalPath(std::string newNormalPath);
	float getNormalXValue();
	float getNormalYValue();
	float getNormalZValue();
	void setNormalValues(float newNormalXValue, float newNormalYValue, float newNormalZValue);

	std::string getMetallicPath();
	void setMetallicPath(std::string newMetallicPath);
	float getMetallicValue();
	void setMetallicValue(float newMetallicValue);

	std::string getRoughnessPath();
	void setRoughnessPath(std::string newRoughnessPath);
	float getRoughnessValue();
	void setRoughnessValue(float newRoughnessValue);

	std::string getAOPath();
	void setAOPath(std::string newAOPath);
	float getAOValue();
	void setAOValue(float newAOValue);

	uint32_t getDiffuseMipLevel();
	void setDiffuseMipLevel(uint32_t newDiffuseMipLevel);
	uint32_t getNormalMipLevel();
	void setNormalMipLevel(uint32_t newNormalMipLevel);
	uint32_t getMetallicMipLevel();
	void setMetallicMipLevel(uint32_t newMetallicMipLevel);
	uint32_t getRoughnessMipLevel();
	void setRoughnessMipLevel(uint32_t newRoughnessMipLevel);
	uint32_t getAOMipLevel();
	void setAOMipLevel(uint32_t newAOMipLevel);

	VkDeviceMemory* getImageMemory();
	void setImageMemory(VkDeviceMemory newImageMemory);

	VkImage* getDiffuseTextureImage();
	void setDiffuseTextureImage(VkImage newDiffuseTextureImage);
	VkImageView* getDiffuseTextureImageView();
	void setDiffuseTextureImageView(VkImageView newDiffuseTextureImageView);
	VkSampler* getDiffuseTextureSampler();
	void setDiffuseTextureSampler(VkSampler newDiffuseTextureSampler);

	VkImage* getNormalTextureImage();
	void setNormalTextureImage(VkImage newNormalTextureImage);
	VkImageView* getNormalTextureImageView();
	void setNormalTextureImageView(VkImageView newNormalTextureImageView);
	VkSampler* getNormalTextureSampler();
	void setNormalTextureSampler(VkSampler newNormalTextureSampler);

	VkImage* getMetallicTextureImage();
	void setMetallicTextureImage(VkImage newMetallicTextureImage);
	VkImageView* getMetallicTextureImageView();
	void setMetallicTextureImageView(VkImageView newMetallicTextureImageView);
	VkSampler* getMetallicTextureSampler();
	void setMetallicTextureSampler(VkSampler newMetallicTextureSampler);

	VkImage* getRoughnessTextureImage();
	void setRoughnessTextureImage(VkImage newRoughnessTextureImage);
	VkImageView* getRoughnessTextureImageView();
	void setRoughnessTextureImageView(VkImageView newRoughnessTextureImageView);
	VkSampler* getRoughnessTextureSampler();
	void setRoughnessTextureSampler(VkSampler newRoughnessTextureSampler);

	VkImage* getAOTextureImage();
	void setAOTextureImage(VkImage newAOTextureImage);
	VkImageView* getAOTextureImageView();
	void setAOTextureImageView(VkImageView newAOTextureImageView);
	VkSampler* getAOTextureSampler();
	void setAOTextureSampler(VkSampler newAOTextureSampler);

	bool isConstructed();
	void constructedTrue();
	bool isDestructed();
	void destructedTrue();
private:
	VkDeviceMemory imageMemory;

	std::string diffusePath;
	float diffuseRValue;
	float diffuseGValue;
	float diffuseBValue;
	float diffuseAValue;

	VkImage diffuseTextureImage;
	VkImageView diffuseTextureImageView;
	VkSampler diffuseTextureSampler;
	uint32_t diffuseMipLevel;

	std::string normalPath;
	float normalXValue;
	float normalYValue;
	float normalZValue;

	VkImage normalTextureImage;
	VkImageView normalTextureImageView;
	VkSampler normalTextureSampler;
	uint32_t normalMipLevel;

	std::string metallicPath;
	float metallicValue;

	VkImage metallicTextureImage;
	VkImageView metallicTextureImageView;
	VkSampler metallicTextureSampler;
	uint32_t metallicMipLevel;

	std::string roughnessPath;
	float roughnessValue;

	VkImage roughnessTextureImage;
	VkImageView roughnessTextureImageView;
	VkSampler roughnessTextureSampler;
	uint32_t roughnessMipLevel;

	std::string AOPath;
	float AOValue;

	VkImage AOTextureImage;
	VkImageView AOTextureImageView;
	VkSampler AOTextureSampler;
	uint32_t AOMipLevel;

	bool constructed;
	bool destructed;
};

