#pragma once
#include <vulkan/vulkan.hpp>
#include <string>

class Material {
public:
	Material(std::string dPath, std::string nPath, std::string mPath, std::string rPath, std::string aPath);

	std::string getDiffusePath();
	void setDiffusePath(std::string newDiffusePath);
	std::string getNormalPath();
	void setNormalPath(std::string newNormalPath);
	std::string getMetallicPath();
	void setMetallicPath(std::string newMetallicPath);
	std::string getRoughnessPath();
	void setRoughnessPath(std::string newRoughnessPath);
	std::string getAOPath();
	void setAOPath(std::string newAOPath);

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

	VkImage* getDiffuseTextureImage();
	void setDiffuseTextureImage(VkImage newDiffuseTextureImage);
	VkDeviceMemory* getDiffuseTextureImageMemory();
	void setDiffuseTextureImageMemory(VkDeviceMemory newDiffuseTextureImageMemory);
	VkImageView* getDiffuseTextureImageView();
	void setDiffuseTextureImageView(VkImageView newDiffuseTextureImageView);
	VkSampler* getDiffuseTextureSampler();
	void setDiffuseTextureSampler(VkSampler newDiffuseTextureSampler);

	VkImage* getNormalTextureImage();
	void setNormalTextureImage(VkImage newNormalTextureImage);
	VkDeviceMemory* getNormalTextureImageMemory();
	void setNormalTextureImageMemory(VkDeviceMemory newNormalTextureImageMemory);
	VkImageView* getNormalTextureImageView();
	void setNormalTextureImageView(VkImageView newNormalTextureImageView);
	VkSampler* getNormalTextureSampler();
	void setNormalTextureSampler(VkSampler newNormalTextureSampler);

	VkImage* getMetallicTextureImage();
	void setMetallicTextureImage(VkImage newMetallicTextureImage);
	VkDeviceMemory* getMetallicTextureImageMemory();
	void setMetallicTextureImageMemory(VkDeviceMemory newMetallicTextureImageMemory);
	VkImageView* getMetallicTextureImageView();
	void setMetallicTextureImageView(VkImageView newMetallicTextureImageView);
	VkSampler* getMetallicTextureSampler();
	void setMetallicTextureSampler(VkSampler newMetallicTextureSampler);

	VkImage* getRoughnessTextureImage();
	void setRoughnessTextureImage(VkImage newRoughnessTextureImage);
	VkDeviceMemory* getRoughnessTextureImageMemory();
	void setRoughnessTextureImageMemory(VkDeviceMemory newRoughnessTextureImageMemory);
	VkImageView* getRoughnessTextureImageView();
	void setRoughnessTextureImageView(VkImageView newRoughnessTextureImageView);
	VkSampler* getRoughnessTextureSampler();
	void setRoughnessTextureSampler(VkSampler newRoughnessTextureSampler);

	VkImage* getAOTextureImage();
	void setAOTextureImage(VkImage newAOTextureImage);
	VkDeviceMemory* getAOTextureImageMemory();
	void setAOTextureImageMemory(VkDeviceMemory newAOTextureImageMemory);
	VkImageView* getAOTextureImageView();
	void setAOTextureImageView(VkImageView newAOTextureImageView);
	VkSampler* getAOTextureSampler();
	void setAOTextureSampler(VkSampler newAOTextureSampler);

	bool isConstructed();
	void constructedTrue();
private:
	std::string diffusePath;

	VkImage diffuseTextureImage;
	VkDeviceMemory diffuseTextureImageMemory;
	VkImageView diffuseTextureImageView;
	VkSampler diffuseTextureSampler;
	uint32_t diffuseMipLevel;

	std::string normalPath;

	VkImage normalTextureImage;
	VkDeviceMemory normalTextureImageMemory;
	VkImageView normalTextureImageView;
	VkSampler normalTextureSampler;
	uint32_t normalMipLevel;

	std::string metallicPath;

	VkImage metallicTextureImage;
	VkDeviceMemory metallicTextureImageMemory;
	VkImageView metallicTextureImageView;
	VkSampler metallicTextureSampler;
	uint32_t metallicMipLevel;

	std::string roughnessPath;

	VkImage roughnessTextureImage;
	VkDeviceMemory roughnessTextureImageMemory;
	VkImageView roughnessTextureImageView;
	VkSampler roughnessTextureSampler;
	uint32_t roughnessMipLevel;

	std::string AOPath;

	VkImage AOTextureImage;
	VkDeviceMemory AOTextureImageMemory;
	VkImageView AOTextureImageView;
	VkSampler AOTextureSampler;
	uint32_t AOMipLevel;

	bool constructed;
};

