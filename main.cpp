#include "src/Renderer.h"

void cameraControls(Camera* camera, GLFWwindow* window, double deltaTime) {
	float movementSpeed = camera->getMovementSpeed() * (float)deltaTime;
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
	float movementObject = 1.0 * (float)deltaTime;

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

	DirectionalLight ambient = DirectionalLight(-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	DirectionalLight ambient2 = DirectionalLight(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	DirectionalLight ambient3 = DirectionalLight(0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	PointLight point = PointLight(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	PointLight point2 = PointLight(0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	PointLight point3 = PointLight(0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f);
	PointLight point4 = PointLight(0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f);
	PointLight point5 = PointLight(0.25f, 1.0f, 0.25f, 1.0f, 1.0f, 0.0f);
	SpotLight spot = SpotLight(0.0f, 5.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0, 1.0, 1.0, 5.0f, 20.0f);
	//spot.actAsTorchlight(true);
	scene.addDirectionalLight(&ambient);
	scene.addDirectionalLight(&ambient2);
	scene.addPointLight(&point);
	scene.addPointLight(&point2);
	scene.addPointLight(&point3);
	scene.addPointLight(&point4);
	scene.addPointLight(&point5);
	scene.addSpotLight(&spot);

	// Meshes
	Mesh skyboxMesh = Mesh("models/skybox.obj");
	Mesh groundMesh = Mesh("models/ground.obj");
	Mesh tableMesh = Mesh("models/table.obj");
	Mesh swordMesh = Mesh("models/Big_Sword_OBJ.obj");
	Mesh sphereMesh = Mesh("models/uvsphere.obj");
	Mesh diceMesh = Mesh("models/dice.obj");
	Mesh arrowMesh = Mesh("models/arrow.obj");
	Mesh houseMesh = Mesh("models/maison.obj");
	Mesh roomMesh = Mesh("models/viking_room.obj");
	Mesh sponzaMesh = Mesh("models/sponza.obj");

	// Materials
	Material emptyMaterial = Material("", "", "", "", "");
	Material skyboxMaterial = Material("textures/skybox.png", "", "", "", "");
	Material groundMaterial = Material("textures/Ground037_2K_Color.jpg", "textures/Ground037_2K_Normal.jpg", "", "textures/Ground037_2K_Roughness.jpg", "textures/Ground037_2K_AmbientOcclusion.jpg");
	groundMaterial.setMetallicValue(1.0f);
	Material ground2Material = Material("textures/grass_basecolor.png", "textures/grass_normal.png", "", "textures/grass_roughness.png", "textures/grass_ambientOcclusion.png");
	ground2Material.setMetallicValue(1.0f);
	Material metal1Material = Material("textures/Metal012_2K_Color.jpg", "textures/Metal012_2K_Normal.jpg", "textures/Metal012_2K_Metalness.jpg", "textures/Metal012_2K_Roughness.jpg", "");
	Material metal2Material = Material("textures/Metal006_2K_Color.jpg", "textures/Metal006_2K_Normal.jpg", "textures/Metal006_2K_Metalness.jpg", "textures/Metal006_2K_Roughness.jpg", "");
	Material metal3Material = Material("textures/Metal022_2K_Color.jpg", "textures/Metal022_2K_Normal.jpg", "textures/Metal022_2K_Metalness.jpg", "textures/Metal022_2K_Roughness.jpg", "");
	Material woodMaterial = Material("textures/WoodSiding009_2K_Color.jpg", "textures/WoodSiding009_2K_Normal.jpg", "textures/WoodSiding009_2K_Metalness.jpg", "textures/WoodSiding009_2K_Roughness.jpg", "textures/WoodSiding009_2K_AmbientOcclusion.jpg");
	Material marbleMaterial = Material("textures/Marble009_2K_Color.jpg", "textures/Marble009_2K_Normal.jpg", "", "textures/Metal006_2K_Roughness.jpg", "");
	marbleMaterial.setMetallicValue(1.0f);
	Material deMaterial = Material("textures/texturede.png", "textures/normal.jpg", "textures/Metal022_2K_Metalness.jpg", "textures/Metal022_2K_Roughness.jpg", "");
	Material swordMaterial = Material("textures/Big_Sword_Base_Color_Map.jpg", "textures/Big_Sword_Normal_Map.jpg", "textures/Big_Sword_Metalness.jpg", "textures/Big_Sword_Roughness_Map.jpg", "textures/Big_Sword_AO_Map.jpg");
	Material roomMaterial = Material("textures/viking_room.png", "", "", "", "");
	roomMaterial.setDiffuseValues(1.0f, 0.0f, 0.0f, 0.01f);
	Material carbonMaterial = Material("textures/Fabric004_2K_Color.jpg", "textures/Fabric004_2K_Normal.jpg", "textures/Fabric004_2K_Metalness.jpg", "textures/Fabric004_2K_Roughness.jpg", "textures/emptymaterial.png");
	Material simpleWaterMaterial = Material("", "", "", "", "");
	simpleWaterMaterial.setDiffuseValues(0.0f, 0.0f, 0.75f, 0.25f);
	simpleWaterMaterial.setMetallicValue(0.75f);
	simpleWaterMaterial.setRoughnessValue(0.25f);

	// Skybox
	Object skybox = Object(0.0f, 0.0f, 0.0f, 50.0f, 90.0f, 0.0f, 0.0f);
	skybox.setMesh(&skyboxMesh);
	skybox.setMaterial(&skyboxMaterial);
	scene.setSkybox(&skybox);

	// Spheres
	Object sphere1 = Object(2.0f, 1.0f, 2.0f, 0.3f);
	SGNode sphere1Node = SGNode(&sphere1);
	sphere1.setMesh(&sphereMesh);
	sphere1.setMaterial(&marbleMaterial);
	scene.getRoot()->addChild(&sphere1Node);
	sphere1.frameEvent = objectControls;

	Object sphere2 = Object(2.0f, 1.0f, 1.0f, 0.3f);
	SGNode sphere2Node = SGNode(&sphere2);
	sphere2.setMesh(&sphereMesh);
	sphere2.setMaterial(&metal1Material);
	scene.getRoot()->addChild(&sphere2Node);
	sphere2.frameEvent = objectControls;

	Object sphere3 = Object(2.0f, 1.0f, 0.0f, 0.3f);
	SGNode sphere3Node = SGNode(&sphere3);
	sphere3.setMesh(&sphereMesh);
	sphere3.setMaterial(&metal3Material);
	scene.getRoot()->addChild(&sphere3Node);
	sphere3.frameEvent = objectControls;

	Object sphere4 = Object(2.0f, 1.0f, -1.0f, 0.3f);
	SGNode sphere4Node = SGNode(&sphere4);
	sphere4.setMesh(&sphereMesh);
	sphere4.setMaterial(&carbonMaterial);
	scene.getRoot()->addChild(&sphere4Node);
	sphere4.frameEvent = objectControls;

	// House
	Object house = Object(5.0f, -0.1f, 3.0f, 1.0f);
	SGNode houseNode = SGNode(&house);
	house.setMesh(&houseMesh);
	house.setMaterial(&marbleMaterial);
	scene.getRoot()->addChild(&houseNode);

	Object room = Object(-7.0f, 0.0f, -3.0f, 1.5f, 270.0f, 0.0f, 90.0f);
	SGNode roomNode = SGNode(&room);
	room.setMesh(&roomMesh);
	room.setMaterial(&roomMaterial);
	scene.getRoot()->addChild(&roomNode);

	// Ground
	Object ground = Object(0.0, -0.1f, 0.0, 10.0f);
	SGNode groundNode = SGNode(&ground);
	scene.getRoot()->addChild(&groundNode);
	ground.setMesh(&groundMesh);
	ground.setMaterial(&groundMaterial);

	// Table
	Object table = Object(0.0f, 0.0f, 0.0f, 0.5f);
	SGNode tableNode = SGNode(&table);
	table.setMesh(&tableMesh);
	table.setMaterial(&simpleWaterMaterial);
	scene.getRoot()->addChild(&tableNode);

	// Visualize hierarchy
	scene.viewSceneGraph();

	Renderer app;

	// Set the scene
	app.setScene(&scene);
	// Start the application
	app.start();
}
