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
	float movementObject = 1.0f * (float)deltaTime;

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

	// Models
	Model groundModel = Model("models/ground.obj");
	Model tableModel = Model("models/table.obj");
	Model swordModel = Model("models/Big_Sword_OBJ.obj");
	Model sphereModel = Model("models/uvsphere.obj");
	Model diceModel = Model("models/dice.obj");
	Model arrowModel = Model("models/arrow.obj");
	Model houseModel = Model("models/maison.obj");
	Model roomModel = Model("models/viking_room.obj");
	Model sponzaModel = Model("models/sponza.obj");

	Model triangleModel = Model();
	triangleModel.addParametricMeshVertex(0.0f, 0.0f, 1.0f);
	triangleModel.addParametricMeshIndex(0);
	triangleModel.addParametricMeshVertex(0.0f, 1.0f, 0.0f);
	triangleModel.addParametricMeshIndex(1);
	triangleModel.addParametricMeshVertex(0.0f, 0.0f, -1.0f);
	triangleModel.addParametricMeshIndex(2);

	// Materials
	Material emptyMaterial = Material("", "", "", "", "");
	Material groundMaterial = Material("textures/Ground037_2K_Color.jpg", "textures/Ground037_2K_Normal.jpg", "", "textures/Ground037_2K_Roughness.jpg", "textures/Ground037_2K_AmbientOcclusion.jpg");
	//groundMaterial.setMetallicValue(1.0f);
	Material ground2Material = Material("textures/grass_basecolor.png", "textures/grass_normal.png", "", "textures/grass_roughness.png", "textures/grass_ambientOcclusion.png");
	ground2Material.setMetallicValue(1.0f);
	Material metal1Material = Material("textures/Metal012_2K_Color.jpg", "textures/Metal012_2K_Normal.jpg", "textures/Metal012_2K_Metalness.jpg", "textures/Metal012_2K_Roughness.jpg", "");
	Material metal2Material = Material("textures/Metal006_2K_Color.jpg", "textures/Metal006_2K_Normal.jpg", "textures/Metal006_2K_Metalness.jpg", "textures/Metal006_2K_Roughness.jpg", "");
	Material metal3Material = Material("textures/Metal022_2K_Color.jpg", "textures/Metal022_2K_Normal.jpg", "textures/Metal022_2K_Metalness.jpg", "textures/Metal022_2K_Roughness.jpg", "");
	Material metal4Material = Material("textures/Metal032_2K_Color.jpg", "textures/Metal032_2K_Normal.jpg", "textures/Metal032_2K_Metalness.jpg", "textures/Metal032_2K_Roughness.jpg", "");
	Material metal5Material = Material("textures/Metal035_2K_Color.jpg", "textures/Metal035_2K_Normal.jpg", "textures/Metal035_2K_Metalness.jpg", "textures/Metal035_2K_Roughness.jpg", "");
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
	Skybox skybox = Skybox("textures/Yokohama3/posx.jpg", "textures/Yokohama3/negx.jpg", "textures/Yokohama3/posy.jpg", "textures/Yokohama3/negy.jpg", "textures/Yokohama3/posz.jpg", "textures/Yokohama3/negz.jpg");
	scene.setSkybox(skybox);

	Object triangle = Object(2.0f, 1.5f, 1.0f, 1.0f);
	SGNode triangleNode = SGNode(&triangle);
	triangle.setModel(&triangleModel);
	triangle.setMaterial(&emptyMaterial);
	scene.getRoot()->addChild(&triangleNode);

	// Spheres
	Object sphere1 = Object(2.0f, 1.0f, 2.0f, 0.3f);
	SGNode sphere1Node = SGNode(&sphere1);
	sphere1.setModel(&sphereModel);
	sphere1.setMaterial(&marbleMaterial);
	scene.getRoot()->addChild(&sphere1Node);
	sphere1.frameEvent = objectControls;

	Object sphere2 = Object(2.0f, 1.0f, 1.0f, 0.3f);
	SGNode sphere2Node = SGNode(&sphere2);
	sphere2.setModel(&sphereModel);
	sphere2.setMaterial(&metal1Material);
	scene.getRoot()->addChild(&sphere2Node);
	sphere2.frameEvent = objectControls;

	Object sphere3 = Object(2.0f, 1.0f, 0.0f, 0.3f);
	SGNode sphere3Node = SGNode(&sphere3);
	sphere3.setModel(&sphereModel);
	sphere3.setMaterial(&metal3Material);
	scene.getRoot()->addChild(&sphere3Node);
	sphere3.frameEvent = objectControls;

	Object sphere4 = Object(2.0f, 1.0f, -1.0f, 0.3f);
	SGNode sphere4Node = SGNode(&sphere4);
	sphere4.setModel(&sphereModel);
	sphere4.setMaterial(&carbonMaterial);
	scene.getRoot()->addChild(&sphere4Node);
	sphere4.frameEvent = objectControls;

	Object sphere5 = Object(2.0f, 1.0f, -2.0f, 0.3f);
	SGNode sphere5Node = SGNode(&sphere5);
	sphere5.setModel(&sphereModel);
	sphere5.setMaterial(&metal4Material);
	scene.getRoot()->addChild(&sphere5Node);
	sphere5.frameEvent = objectControls;

	Object sphere6 = Object(2.0f, 1.0f, -3.0f, 0.3f);
	SGNode sphere6Node = SGNode(&sphere6);
	sphere6.setModel(&sphereModel);
	sphere6.setMaterial(&metal5Material);
	scene.getRoot()->addChild(&sphere6Node);
	sphere6.frameEvent = objectControls;

	// House
	Object house = Object(5.0f, -0.11f, 3.0f, 1.0f);
	SGNode houseNode = SGNode(&house);
	house.setModel(&houseModel);
	house.setMaterial(&marbleMaterial);
	scene.getRoot()->addChild(&houseNode);

	Object room = Object(-7.0f, 0.0f, -3.0f, 1.5f, 270.0f, 0.0f, 90.0f);
	SGNode roomNode = SGNode(&room);
	room.setModel(&roomModel);
	room.setMaterial(&roomMaterial);
	scene.getRoot()->addChild(&roomNode);

	// Ground
	Object ground = Object(0.0, -0.1f, 0.0, 10.0f);
	SGNode groundNode = SGNode(&ground);
	scene.getRoot()->addChild(&groundNode);
	ground.setModel(&groundModel);
	ground.setMaterial(&groundMaterial);

	// Table
	Object table = Object(0.0f, 0.0f, 0.0f, 0.5f);
	SGNode tableNode = SGNode(&table);
	table.setModel(&tableModel);
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