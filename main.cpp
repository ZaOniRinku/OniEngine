#include "src/GraphicsEngine.h"

void cameraControls(Camera* camera, GLFWwindow* window, double deltaTime) {
	float movementSpeed = camera->getMovementSpeed() * deltaTime;
	glm::vec3 camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	glm::vec3 camFront = { camera->getFrontX(), camera->getFrontY(), camera->getFrontZ() };
	glm::vec3 camUp = { camera->getUpX(), camera->getUpY(), camera->getUpZ() };

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec3 newPos = camPos + (camFront * movementSpeed);
		camera->setPosition(newPos.x, newPos.y, newPos.z);
		camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 newPos = camPos - (camFront * movementSpeed);
		camera->setPosition(newPos.x, newPos.y, newPos.z);
		camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 newPos = camPos - (glm::normalize(glm::cross(camFront, camUp)) * movementSpeed);
		camera->setPosition(newPos.x, newPos.y, newPos.z);
		camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 newPos = camPos + (glm::normalize(glm::cross(camFront, camUp)) * movementSpeed);
		camera->setPosition(newPos.x, newPos.y, newPos.z);
		camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glm::vec3 newPos = camPos + (camUp * movementSpeed);
		camera->setPosition(newPos.x, newPos.y, newPos.z);
		camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		glm::vec3 newPos = camPos - (camUp * movementSpeed);
		camera->setPosition(newPos.x, newPos.y, newPos.z);
		camPos = { camera->getPositionX(), camera->getPositionY(), camera->getPositionZ() };
	}
}

void objectControls(Object* obj, GLFWwindow* window, double deltaTime) {
	float movementObject = 1.0 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		obj->move(movementObject, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		obj->move(-movementObject, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		obj->move(0.0f, 0.0f, -movementObject);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		obj->move(0.0f, 0.0f, movementObject);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		obj->move(0.0f, movementObject, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		obj->move(0.0f, -movementObject, 0.0f);
	}

}

int main() {
	// DEMO
	// In this demo, Scene Graph Nodes must be created explicitly,
	// but in an editor with a UI, SG Nodes can be created automatically
	// when an instance is placed inside a scene, according to the instances hierarchy.

	// Scene creation (default Camera)
	Scene scene = Scene();

	// Camera controls
	scene.getCamera()->frameEvent = cameraControls;

	DirectionalLight ambient = DirectionalLight(0.0f, -1.0f, -1.5f, 1.0f, 1.0f, 1.0f);
	DirectionalLight ambient2 = DirectionalLight(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	DirectionalLight ambient3 = DirectionalLight(0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	PointLight point = PointLight(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	PointLight point2 = PointLight(0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	PointLight point3 = PointLight(0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f);
	PointLight point4 = PointLight(0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f);
	PointLight point5 = PointLight(0.25f, 1.0f, 0.25f, 1.0f, 1.0f, 0.0f);
	scene.addDirectionalLight(&ambient2);
	scene.addPointLight(&point);
	scene.addPointLight(&point2);
	scene.addPointLight(&point3);
	scene.addPointLight(&point4);
	scene.addPointLight(&point5);
	
	// Meshes
	Mesh skyboxMesh = Mesh("models/skybox.obj");
	Mesh groundMesh = Mesh("models/plan.obj");

	// Materials
	Material skyboxMaterial = Material("textures/skybox.png", "", "", "", "");
	skyboxMaterial.setNormalValues(1.0f, 1.0f, 1.0f);
	skyboxMaterial.setMetallicValue(0.0f);
	skyboxMaterial.setRoughnessValue(1.0f);
	skyboxMaterial.setAOValue(1.0f);
	Material groundMaterial = Material("textures/Ground037_2K_Color.jpg", "textures/Ground037_2K_Normal.jpg", "", "textures/Ground037_2K_Roughness.jpg", "textures/Ground037_2K_AmbientOcclusion.jpg");
	groundMaterial.setMetallicValue(0.0f);
	

	// Skybox
	Object skybox = Object(0.0f, 0.0f, 0.0f, 50.0f, 90.0f, 0.0f, 0.0f);
	skybox.setMesh(&skyboxMesh);
	skybox.setMaterial(&skyboxMaterial);
	scene.setSkybox(&skybox);

	// Ground
	Object ground0 = Object(-10.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode0 = SGNode(&ground0);
	scene.getRoot()->addChild(&groundNode0);
	ground0.setMesh(&groundMesh);
	ground0.setMaterial(&groundMaterial);

	// Visualize hierarchy
	scene.viewSceneGraph();

	GraphicsEngine app;

	// Set the scene
	app.setScene(&scene);
	// Start the application
	app.start();
}
