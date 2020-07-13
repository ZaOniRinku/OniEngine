#include "Material.h"

Material::Material(std::string dPath, std::string nPath, std::string mPath, std::string rPath, std::string aPath) {
	diffusePath = dPath;
	normalPath = nPath;
	metallicPath = mPath;
	roughnessPath = rPath;
	AOPath = aPath;
	constructed = false;
}

std::string Material::getDiffusePath() {
	return diffusePath;
}

void Material::setDiffusePath(std::string newDiffusePath) {
	diffusePath = newDiffusePath;
}

float Material::getDiffuseRValue() {
	return diffuseRValue;
}

float Material::getDiffuseGValue() {
	return diffuseGValue;
}

float Material::getDiffuseBValue() {
	return diffuseBValue;
}

void Material::setDiffuseValues(float newDiffuseRValue, float newDiffuseGValue, float newDiffuseBValue) {
	diffuseRValue = newDiffuseRValue;
	diffuseGValue = newDiffuseGValue;
	diffuseBValue = newDiffuseBValue;
}

std::string Material::getNormalPath() {
	return normalPath;
}

void Material::setNormalPath(std::string newNormalPath) {
	normalPath = newNormalPath;
}

float Material::getNormalXValue() {
	return normalXValue;
}

float Material::getNormalYValue() {
	return normalYValue;
}

float Material::getNormalZValue() {
	return normalZValue;
}

void Material::setNormalValues(float newNormalXValue, float newNormalYValue, float newNormalZValue) {
	normalXValue = newNormalXValue;
	normalYValue = newNormalYValue;
	normalZValue = newNormalZValue;
}

std::string Material::getMetallicPath() {
	return metallicPath;
}

void Material::setMetallicPath(std::string newMetallicPath) {
	metallicPath = newMetallicPath;
}

float Material::getMetallicValue() {
	return metallicValue;
}

void Material::setMetallicValue(float newMetallicValue) {
	metallicValue = newMetallicValue;
}

std::string Material::getRoughnessPath() {
	return roughnessPath;
}

void Material::setRoughnessPath(std::string newRoughnessPath) {
	roughnessPath = newRoughnessPath;
}

float Material::getRoughnessValue() {
	return roughnessValue;
}

void Material::setRoughnessValue(float newRoughnessValue) {
	roughnessValue = newRoughnessValue;
}

std::string Material::getAOPath() {
	return AOPath;
}

void Material::setAOPath(std::string newAOPath) {
	AOPath = newAOPath;
}

float Material::getAOValue() {
	return AOValue;
}

void Material::setAOValue(float newAOValue) {
	AOValue = newAOValue;
}

uint32_t Material::getDiffuseMipLevel() {
	return diffuseMipLevel;
}

void Material::setDiffuseMipLevel(uint32_t newDiffuseMipLevel) {
	diffuseMipLevel = newDiffuseMipLevel;
}

uint32_t Material::getNormalMipLevel() {
	return normalMipLevel;
}

void Material::setNormalMipLevel(uint32_t newNormalMipLevel) {
	normalMipLevel = newNormalMipLevel;
}

uint32_t Material::getMetallicMipLevel() {
	return metallicMipLevel;
}

void Material::setMetallicMipLevel(uint32_t newMetallicMipLevel) {
	metallicMipLevel = newMetallicMipLevel;
}

uint32_t Material::getRoughnessMipLevel() {
	return roughnessMipLevel;
}

void Material::setRoughnessMipLevel(uint32_t newRoughnessMipLevel) {
	roughnessMipLevel = newRoughnessMipLevel;
}

uint32_t Material::getAOMipLevel() {
	return AOMipLevel;
}

void Material::setAOMipLevel(uint32_t newAOMipLevel) {
	AOMipLevel = newAOMipLevel;
}

// Diffuse

VkImage* Material::getDiffuseTextureImage() {
	return &diffuseTextureImage;
}

void Material::setDiffuseTextureImage(VkImage newDiffuseTextureImage) {
	diffuseTextureImage = newDiffuseTextureImage;
}

VkDeviceMemory* Material::getDiffuseTextureImageMemory() {
	return &diffuseTextureImageMemory;
}

void Material::setDiffuseTextureImageMemory(VkDeviceMemory newDiffuseTextureImageMemory) {
	diffuseTextureImageMemory = newDiffuseTextureImageMemory;
}

VkImageView* Material::getDiffuseTextureImageView() {
	return &diffuseTextureImageView;
}

void Material::setDiffuseTextureImageView(VkImageView newDiffuseTextureImageView) {
	diffuseTextureImageView = newDiffuseTextureImageView;
}

VkSampler* Material::getDiffuseTextureSampler() {
	return &diffuseTextureSampler;
}

void Material::setDiffuseTextureSampler(VkSampler newDiffuseTextureSampler) {
	diffuseTextureSampler = newDiffuseTextureSampler;
}

// Normal

VkImage* Material::getNormalTextureImage() {
	return &normalTextureImage;
}

void Material::setNormalTextureImage(VkImage newNormalTextureImage) {
	normalTextureImage = newNormalTextureImage;
}

VkDeviceMemory* Material::getNormalTextureImageMemory() {
	return &normalTextureImageMemory;
}

void Material::setNormalTextureImageMemory(VkDeviceMemory newNormalTextureImageMemory) {
	normalTextureImageMemory = newNormalTextureImageMemory;
}

VkImageView* Material::getNormalTextureImageView() {
	return &normalTextureImageView;
}

void Material::setNormalTextureImageView(VkImageView newNormalTextureImageView) {
	normalTextureImageView = newNormalTextureImageView;
}

VkSampler* Material::getNormalTextureSampler() {
	return &normalTextureSampler;
}

void Material::setNormalTextureSampler(VkSampler newNormalTextureSampler) {
	normalTextureSampler = newNormalTextureSampler;
}

// Metallic

VkImage* Material::getMetallicTextureImage() {
	return &metallicTextureImage;
}

void Material::setMetallicTextureImage(VkImage newMetallicTextureImage) {
	metallicTextureImage = newMetallicTextureImage;
}

VkDeviceMemory* Material::getMetallicTextureImageMemory() {
	return &metallicTextureImageMemory;
}

void Material::setMetallicTextureImageMemory(VkDeviceMemory newMetallicTextureImageMemory) {
	metallicTextureImageMemory = newMetallicTextureImageMemory;
}

VkImageView* Material::getMetallicTextureImageView() {
	return &metallicTextureImageView;
}

void Material::setMetallicTextureImageView(VkImageView newMetallicTextureImageView) {
	metallicTextureImageView = newMetallicTextureImageView;
}

VkSampler* Material::getMetallicTextureSampler() {
	return &metallicTextureSampler;
}

void Material::setMetallicTextureSampler(VkSampler newMetallicTextureSampler) {
	metallicTextureSampler = newMetallicTextureSampler;
}

// Roughness

VkImage* Material::getRoughnessTextureImage() {
	return &roughnessTextureImage;
}

void Material::setRoughnessTextureImage(VkImage newRoughnessTextureImage) {
	roughnessTextureImage = newRoughnessTextureImage;
}

VkDeviceMemory* Material::getRoughnessTextureImageMemory() {
	return &roughnessTextureImageMemory;
}

void Material::setRoughnessTextureImageMemory(VkDeviceMemory newRoughnessTextureImageMemory) {
	roughnessTextureImageMemory = newRoughnessTextureImageMemory;
}

VkImageView* Material::getRoughnessTextureImageView() {
	return &roughnessTextureImageView;
}

void Material::setRoughnessTextureImageView(VkImageView newRoughnessTextureImageView) {
	roughnessTextureImageView = newRoughnessTextureImageView;
}

VkSampler* Material::getRoughnessTextureSampler() {
	return &roughnessTextureSampler;
}

void Material::setRoughnessTextureSampler(VkSampler newRoughnessTextureSampler) {
	roughnessTextureSampler = newRoughnessTextureSampler;
}

// AO

VkImage* Material::getAOTextureImage() {
	return &AOTextureImage;
}

void Material::setAOTextureImage(VkImage newAOTextureImage) {
	AOTextureImage = newAOTextureImage;
}

VkDeviceMemory* Material::getAOTextureImageMemory() {
	return &AOTextureImageMemory;
}

void Material::setAOTextureImageMemory(VkDeviceMemory newAOTextureImageMemory) {
	AOTextureImageMemory = newAOTextureImageMemory;
}

VkImageView* Material::getAOTextureImageView() {
	return &AOTextureImageView;
}

void Material::setAOTextureImageView(VkImageView newAOTextureImageView) {
	AOTextureImageView = newAOTextureImageView;
}

VkSampler* Material::getAOTextureSampler() {
	return &AOTextureSampler;
}

void Material::setAOTextureSampler(VkSampler newAOTextureSampler) {
	AOTextureSampler = newAOTextureSampler;
}

// Constructed

bool Material::isConstructed() {
	return constructed;
}

void Material::constructedTrue() {
	constructed = true;
}