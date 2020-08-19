#include "src/GraphicsEngine.h"

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
	Mesh groundMesh = Mesh("models/plan.obj");
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

	// Table
	Object table = Object(0.0f, 0.0f, 0.0f, 0.5f);
	SGNode tableNode = SGNode(&table);
	table.setMesh(&tableMesh);
	table.setMaterial(&metal2Material);
	scene.getRoot()->addChild(&tableNode);

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
	Object house = Object(5.0f, -0.05f, 3.0f, 1.0f);
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
	Object ground0 = Object(-10.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode0 = SGNode(&ground0);
	scene.getRoot()->addChild(&groundNode0);
	ground0.setMesh(&groundMesh);
	ground0.setMaterial(&groundMaterial);
	Object ground1 = Object(-10.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode1 = SGNode(&ground1);
	scene.getRoot()->addChild(&groundNode1);
	ground1.setMesh(&groundMesh);
	ground1.setMaterial(&groundMaterial);
	Object ground2 = Object(-10.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode2 = SGNode(&ground2);
	scene.getRoot()->addChild(&groundNode2);
	ground2.setMesh(&groundMesh);
	ground2.setMaterial(&groundMaterial);
	Object ground3 = Object(-10.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode3 = SGNode(&ground3);
	scene.getRoot()->addChild(&groundNode3);
	ground3.setMesh(&groundMesh);
	ground3.setMaterial(&groundMaterial);
	Object ground4 = Object(-10.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode4 = SGNode(&ground4);
	scene.getRoot()->addChild(&groundNode4);
	ground4.setMesh(&groundMesh);
	ground4.setMaterial(&groundMaterial);
	Object ground5 = Object(-10.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode5 = SGNode(&ground5);
	scene.getRoot()->addChild(&groundNode5);
	ground5.setMesh(&groundMesh);
	ground5.setMaterial(&groundMaterial);
	Object ground6 = Object(-10.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode6 = SGNode(&ground6);
	scene.getRoot()->addChild(&groundNode6);
	ground6.setMesh(&groundMesh);
	ground6.setMaterial(&groundMaterial);
	Object ground7 = Object(-10.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode7 = SGNode(&ground7);
	scene.getRoot()->addChild(&groundNode7);
	ground7.setMesh(&groundMesh);
	ground7.setMaterial(&groundMaterial);
	Object ground8 = Object(-10.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode8 = SGNode(&ground8);
	scene.getRoot()->addChild(&groundNode8);
	ground8.setMesh(&groundMesh);
	ground8.setMaterial(&groundMaterial);
	Object ground9 = Object(-10.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode9 = SGNode(&ground9);
	scene.getRoot()->addChild(&groundNode9);
	ground9.setMesh(&groundMesh);
	ground9.setMaterial(&groundMaterial);
	Object ground10 = Object(-8.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode10 = SGNode(&ground10);
	scene.getRoot()->addChild(&groundNode10);
	ground10.setMesh(&groundMesh);
	ground10.setMaterial(&groundMaterial);
	Object ground11 = Object(-8.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode11 = SGNode(&ground11);
	scene.getRoot()->addChild(&groundNode11);
	ground11.setMesh(&groundMesh);
	ground11.setMaterial(&groundMaterial);
	Object ground12 = Object(-8.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode12 = SGNode(&ground12);
	scene.getRoot()->addChild(&groundNode12);
	ground12.setMesh(&groundMesh);
	ground12.setMaterial(&groundMaterial);
	Object ground13 = Object(-8.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode13 = SGNode(&ground13);
	scene.getRoot()->addChild(&groundNode13);
	ground13.setMesh(&groundMesh);
	ground13.setMaterial(&groundMaterial);
	Object ground14 = Object(-8.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode14 = SGNode(&ground14);
	scene.getRoot()->addChild(&groundNode14);
	ground14.setMesh(&groundMesh);
	ground14.setMaterial(&groundMaterial);
	Object ground15 = Object(-8.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode15 = SGNode(&ground15);
	scene.getRoot()->addChild(&groundNode15);
	ground15.setMesh(&groundMesh);
	ground15.setMaterial(&groundMaterial);
	Object ground16 = Object(-8.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode16 = SGNode(&ground16);
	scene.getRoot()->addChild(&groundNode16);
	ground16.setMesh(&groundMesh);
	ground16.setMaterial(&groundMaterial);
	Object ground17 = Object(-8.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode17 = SGNode(&ground17);
	scene.getRoot()->addChild(&groundNode17);
	ground17.setMesh(&groundMesh);
	ground17.setMaterial(&groundMaterial);
	Object ground18 = Object(-8.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode18 = SGNode(&ground18);
	scene.getRoot()->addChild(&groundNode18);
	ground18.setMesh(&groundMesh);
	ground18.setMaterial(&groundMaterial);
	Object ground19 = Object(-8.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode19 = SGNode(&ground19);
	scene.getRoot()->addChild(&groundNode19);
	ground19.setMesh(&groundMesh);
	ground19.setMaterial(&groundMaterial);
	Object ground20 = Object(-6.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode20 = SGNode(&ground20);
	scene.getRoot()->addChild(&groundNode20);
	ground20.setMesh(&groundMesh);
	ground20.setMaterial(&groundMaterial);
	Object ground21 = Object(-6.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode21 = SGNode(&ground21);
	scene.getRoot()->addChild(&groundNode21);
	ground21.setMesh(&groundMesh);
	ground21.setMaterial(&groundMaterial);
	Object ground22 = Object(-6.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode22 = SGNode(&ground22);
	scene.getRoot()->addChild(&groundNode22);
	ground22.setMesh(&groundMesh);
	ground22.setMaterial(&groundMaterial);
	Object ground23 = Object(-6.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode23 = SGNode(&ground23);
	scene.getRoot()->addChild(&groundNode23);
	ground23.setMesh(&groundMesh);
	ground23.setMaterial(&groundMaterial);
	Object ground24 = Object(-6.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode24 = SGNode(&ground24);
	scene.getRoot()->addChild(&groundNode24);
	ground24.setMesh(&groundMesh);
	ground24.setMaterial(&groundMaterial);
	Object ground25 = Object(-6.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode25 = SGNode(&ground25);
	scene.getRoot()->addChild(&groundNode25);
	ground25.setMesh(&groundMesh);
	ground25.setMaterial(&groundMaterial);
	Object ground26 = Object(-6.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode26 = SGNode(&ground26);
	scene.getRoot()->addChild(&groundNode26);
	ground26.setMesh(&groundMesh);
	ground26.setMaterial(&groundMaterial);
	Object ground27 = Object(-6.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode27 = SGNode(&ground27);
	scene.getRoot()->addChild(&groundNode27);
	ground27.setMesh(&groundMesh);
	ground27.setMaterial(&groundMaterial);
	Object ground28 = Object(-6.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode28 = SGNode(&ground28);
	scene.getRoot()->addChild(&groundNode28);
	ground28.setMesh(&groundMesh);
	ground28.setMaterial(&groundMaterial);
	Object ground29 = Object(-6.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode29 = SGNode(&ground29);
	scene.getRoot()->addChild(&groundNode29);
	ground29.setMesh(&groundMesh);
	ground29.setMaterial(&groundMaterial);
	Object ground30 = Object(-4.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode30 = SGNode(&ground30);
	scene.getRoot()->addChild(&groundNode30);
	ground30.setMesh(&groundMesh);
	ground30.setMaterial(&groundMaterial);
	Object ground31 = Object(-4.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode31 = SGNode(&ground31);
	scene.getRoot()->addChild(&groundNode31);
	ground31.setMesh(&groundMesh);
	ground31.setMaterial(&groundMaterial);
	Object ground32 = Object(-4.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode32 = SGNode(&ground32);
	scene.getRoot()->addChild(&groundNode32);
	ground32.setMesh(&groundMesh);
	ground32.setMaterial(&groundMaterial);
	Object ground33 = Object(-4.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode33 = SGNode(&ground33);
	scene.getRoot()->addChild(&groundNode33);
	ground33.setMesh(&groundMesh);
	ground33.setMaterial(&groundMaterial);
	Object ground34 = Object(-4.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode34 = SGNode(&ground34);
	scene.getRoot()->addChild(&groundNode34);
	ground34.setMesh(&groundMesh);
	ground34.setMaterial(&groundMaterial);
	Object ground35 = Object(-4.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode35 = SGNode(&ground35);
	scene.getRoot()->addChild(&groundNode35);
	ground35.setMesh(&groundMesh);
	ground35.setMaterial(&groundMaterial);
	Object ground36 = Object(-4.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode36 = SGNode(&ground36);
	scene.getRoot()->addChild(&groundNode36);
	ground36.setMesh(&groundMesh);
	ground36.setMaterial(&groundMaterial);
	Object ground37 = Object(-4.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode37 = SGNode(&ground37);
	scene.getRoot()->addChild(&groundNode37);
	ground37.setMesh(&groundMesh);
	ground37.setMaterial(&groundMaterial);
	Object ground38 = Object(-4.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode38 = SGNode(&ground38);
	scene.getRoot()->addChild(&groundNode38);
	ground38.setMesh(&groundMesh);
	ground38.setMaterial(&groundMaterial);
	Object ground39 = Object(-4.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode39 = SGNode(&ground39);
	scene.getRoot()->addChild(&groundNode39);
	ground39.setMesh(&groundMesh);
	ground39.setMaterial(&groundMaterial);
	Object ground40 = Object(-2.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode40 = SGNode(&ground40);
	scene.getRoot()->addChild(&groundNode40);
	ground40.setMesh(&groundMesh);
	ground40.setMaterial(&groundMaterial);
	Object ground41 = Object(-2.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode41 = SGNode(&ground41);
	scene.getRoot()->addChild(&groundNode41);
	ground41.setMesh(&groundMesh);
	ground41.setMaterial(&groundMaterial);
	Object ground42 = Object(-2.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode42 = SGNode(&ground42);
	scene.getRoot()->addChild(&groundNode42);
	ground42.setMesh(&groundMesh);
	ground42.setMaterial(&groundMaterial);
	Object ground43 = Object(-2.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode43 = SGNode(&ground43);
	scene.getRoot()->addChild(&groundNode43);
	ground43.setMesh(&groundMesh);
	ground43.setMaterial(&groundMaterial);
	Object ground44 = Object(-2.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode44 = SGNode(&ground44);
	scene.getRoot()->addChild(&groundNode44);
	ground44.setMesh(&groundMesh);
	ground44.setMaterial(&groundMaterial);
	Object ground45 = Object(-2.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode45 = SGNode(&ground45);
	scene.getRoot()->addChild(&groundNode45);
	ground45.setMesh(&groundMesh);
	ground45.setMaterial(&groundMaterial);
	Object ground46 = Object(-2.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode46 = SGNode(&ground46);
	scene.getRoot()->addChild(&groundNode46);
	ground46.setMesh(&groundMesh);
	ground46.setMaterial(&groundMaterial);
	Object ground47 = Object(-2.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode47 = SGNode(&ground47);
	scene.getRoot()->addChild(&groundNode47);
	ground47.setMesh(&groundMesh);
	ground47.setMaterial(&groundMaterial);
	Object ground48 = Object(-2.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode48 = SGNode(&ground48);
	scene.getRoot()->addChild(&groundNode48);
	ground48.setMesh(&groundMesh);
	ground48.setMaterial(&groundMaterial);
	Object ground49 = Object(-2.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode49 = SGNode(&ground49);
	scene.getRoot()->addChild(&groundNode49);
	ground49.setMesh(&groundMesh);
	ground49.setMaterial(&groundMaterial);
	Object ground50 = Object(0.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode50 = SGNode(&ground50);
	scene.getRoot()->addChild(&groundNode50);
	ground50.setMesh(&groundMesh);
	ground50.setMaterial(&groundMaterial);
	Object ground51 = Object(0.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode51 = SGNode(&ground51);
	scene.getRoot()->addChild(&groundNode51);
	ground51.setMesh(&groundMesh);
	ground51.setMaterial(&groundMaterial);
	Object ground52 = Object(0.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode52 = SGNode(&ground52);
	scene.getRoot()->addChild(&groundNode52);
	ground52.setMesh(&groundMesh);
	ground52.setMaterial(&groundMaterial);
	Object ground53 = Object(0.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode53 = SGNode(&ground53);
	scene.getRoot()->addChild(&groundNode53);
	ground53.setMesh(&groundMesh);
	ground53.setMaterial(&groundMaterial);
	Object ground54 = Object(0.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode54 = SGNode(&ground54);
	scene.getRoot()->addChild(&groundNode54);
	ground54.setMesh(&groundMesh);
	ground54.setMaterial(&groundMaterial);
	Object ground55 = Object(0.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode55 = SGNode(&ground55);
	scene.getRoot()->addChild(&groundNode55);
	ground55.setMesh(&groundMesh);
	ground55.setMaterial(&groundMaterial);
	Object ground56 = Object(0.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode56 = SGNode(&ground56);
	scene.getRoot()->addChild(&groundNode56);
	ground56.setMesh(&groundMesh);
	ground56.setMaterial(&groundMaterial);
	Object ground57 = Object(0.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode57 = SGNode(&ground57);
	scene.getRoot()->addChild(&groundNode57);
	ground57.setMesh(&groundMesh);
	ground57.setMaterial(&groundMaterial);
	Object ground58 = Object(0.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode58 = SGNode(&ground58);
	scene.getRoot()->addChild(&groundNode58);
	ground58.setMesh(&groundMesh);
	ground58.setMaterial(&groundMaterial);
	Object ground59 = Object(0.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode59 = SGNode(&ground59);
	scene.getRoot()->addChild(&groundNode59);
	ground59.setMesh(&groundMesh);
	ground59.setMaterial(&groundMaterial);
	Object ground60 = Object(2.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode60 = SGNode(&ground60);
	scene.getRoot()->addChild(&groundNode60);
	ground60.setMesh(&groundMesh);
	ground60.setMaterial(&groundMaterial);
	Object ground61 = Object(2.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode61 = SGNode(&ground61);
	scene.getRoot()->addChild(&groundNode61);
	ground61.setMesh(&groundMesh);
	ground61.setMaterial(&groundMaterial);
	Object ground62 = Object(2.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode62 = SGNode(&ground62);
	scene.getRoot()->addChild(&groundNode62);
	ground62.setMesh(&groundMesh);
	ground62.setMaterial(&groundMaterial);
	Object ground63 = Object(2.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode63 = SGNode(&ground63);
	scene.getRoot()->addChild(&groundNode63);
	ground63.setMesh(&groundMesh);
	ground63.setMaterial(&groundMaterial);
	Object ground64 = Object(2.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode64 = SGNode(&ground64);
	scene.getRoot()->addChild(&groundNode64);
	ground64.setMesh(&groundMesh);
	ground64.setMaterial(&groundMaterial);
	Object ground65 = Object(2.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode65 = SGNode(&ground65);
	scene.getRoot()->addChild(&groundNode65);
	ground65.setMesh(&groundMesh);
	ground65.setMaterial(&groundMaterial);
	Object ground66 = Object(2.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode66 = SGNode(&ground66);
	scene.getRoot()->addChild(&groundNode66);
	ground66.setMesh(&groundMesh);
	ground66.setMaterial(&groundMaterial);
	Object ground67 = Object(2.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode67 = SGNode(&ground67);
	scene.getRoot()->addChild(&groundNode67);
	ground67.setMesh(&groundMesh);
	ground67.setMaterial(&groundMaterial);
	Object ground68 = Object(2.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode68 = SGNode(&ground68);
	scene.getRoot()->addChild(&groundNode68);
	ground68.setMesh(&groundMesh);
	ground68.setMaterial(&groundMaterial);
	Object ground69 = Object(2.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode69 = SGNode(&ground69);
	scene.getRoot()->addChild(&groundNode69);
	ground69.setMesh(&groundMesh);
	ground69.setMaterial(&groundMaterial);
	Object ground70 = Object(4.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode70 = SGNode(&ground70);
	scene.getRoot()->addChild(&groundNode70);
	ground70.setMesh(&groundMesh);
	ground70.setMaterial(&groundMaterial);
	Object ground71 = Object(4.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode71 = SGNode(&ground71);
	scene.getRoot()->addChild(&groundNode71);
	ground71.setMesh(&groundMesh);
	ground71.setMaterial(&groundMaterial);
	Object ground72 = Object(4.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode72 = SGNode(&ground72);
	scene.getRoot()->addChild(&groundNode72);
	ground72.setMesh(&groundMesh);
	ground72.setMaterial(&groundMaterial);
	Object ground73 = Object(4.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode73 = SGNode(&ground73);
	scene.getRoot()->addChild(&groundNode73);
	ground73.setMesh(&groundMesh);
	ground73.setMaterial(&groundMaterial);
	Object ground74 = Object(4.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode74 = SGNode(&ground74);
	scene.getRoot()->addChild(&groundNode74);
	ground74.setMesh(&groundMesh);
	ground74.setMaterial(&groundMaterial);
	Object ground75 = Object(4.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode75 = SGNode(&ground75);
	scene.getRoot()->addChild(&groundNode75);
	ground75.setMesh(&groundMesh);
	ground75.setMaterial(&groundMaterial);
	Object ground76 = Object(4.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode76 = SGNode(&ground76);
	scene.getRoot()->addChild(&groundNode76);
	ground76.setMesh(&groundMesh);
	ground76.setMaterial(&groundMaterial);
	Object ground77 = Object(4.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode77 = SGNode(&ground77);
	scene.getRoot()->addChild(&groundNode77);
	ground77.setMesh(&groundMesh);
	ground77.setMaterial(&groundMaterial);
	Object ground78 = Object(4.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode78 = SGNode(&ground78);
	scene.getRoot()->addChild(&groundNode78);
	ground78.setMesh(&groundMesh);
	ground78.setMaterial(&groundMaterial);
	Object ground79 = Object(4.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode79 = SGNode(&ground79);
	scene.getRoot()->addChild(&groundNode79);
	ground79.setMesh(&groundMesh);
	ground79.setMaterial(&groundMaterial);
	Object ground80 = Object(6.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode80 = SGNode(&ground80);
	scene.getRoot()->addChild(&groundNode80);
	ground80.setMesh(&groundMesh);
	ground80.setMaterial(&groundMaterial);
	Object ground81 = Object(6.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode81 = SGNode(&ground81);
	scene.getRoot()->addChild(&groundNode81);
	ground81.setMesh(&groundMesh);
	ground81.setMaterial(&groundMaterial);
	Object ground82 = Object(6.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode82 = SGNode(&ground82);
	scene.getRoot()->addChild(&groundNode82);
	ground82.setMesh(&groundMesh);
	ground82.setMaterial(&groundMaterial);
	Object ground83 = Object(6.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode83 = SGNode(&ground83);
	scene.getRoot()->addChild(&groundNode83);
	ground83.setMesh(&groundMesh);
	ground83.setMaterial(&groundMaterial);
	Object ground84 = Object(6.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode84 = SGNode(&ground84);
	scene.getRoot()->addChild(&groundNode84);
	ground84.setMesh(&groundMesh);
	ground84.setMaterial(&groundMaterial);
	Object ground85 = Object(6.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode85 = SGNode(&ground85);
	scene.getRoot()->addChild(&groundNode85);
	ground85.setMesh(&groundMesh);
	ground85.setMaterial(&groundMaterial);
	Object ground86 = Object(6.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode86 = SGNode(&ground86);
	scene.getRoot()->addChild(&groundNode86);
	ground86.setMesh(&groundMesh);
	ground86.setMaterial(&groundMaterial);
	Object ground87 = Object(6.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode87 = SGNode(&ground87);
	scene.getRoot()->addChild(&groundNode87);
	ground87.setMesh(&groundMesh);
	ground87.setMaterial(&groundMaterial);
	Object ground88 = Object(6.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode88 = SGNode(&ground88);
	scene.getRoot()->addChild(&groundNode88);
	ground88.setMesh(&groundMesh);
	ground88.setMaterial(&groundMaterial);
	Object ground89 = Object(6.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode89 = SGNode(&ground89);
	scene.getRoot()->addChild(&groundNode89);
	ground89.setMesh(&groundMesh);
	ground89.setMaterial(&groundMaterial);
	Object ground90 = Object(8.0, -0.1f, -10.0, 1.0f);
	SGNode groundNode90 = SGNode(&ground90);
	scene.getRoot()->addChild(&groundNode90);
	ground90.setMesh(&groundMesh);
	ground90.setMaterial(&groundMaterial);
	Object ground91 = Object(8.0, -0.1f, -8.0, 1.0f);
	SGNode groundNode91 = SGNode(&ground91);
	scene.getRoot()->addChild(&groundNode91);
	ground91.setMesh(&groundMesh);
	ground91.setMaterial(&groundMaterial);
	Object ground92 = Object(8.0, -0.1f, -6.0, 1.0f);
	SGNode groundNode92 = SGNode(&ground92);
	scene.getRoot()->addChild(&groundNode92);
	ground92.setMesh(&groundMesh);
	ground92.setMaterial(&groundMaterial);
	Object ground93 = Object(8.0, -0.1f, -4.0, 1.0f);
	SGNode groundNode93 = SGNode(&ground93);
	scene.getRoot()->addChild(&groundNode93);
	ground93.setMesh(&groundMesh);
	ground93.setMaterial(&groundMaterial);
	Object ground94 = Object(8.0, -0.1f, -2.0, 1.0f);
	SGNode groundNode94 = SGNode(&ground94);
	scene.getRoot()->addChild(&groundNode94);
	ground94.setMesh(&groundMesh);
	ground94.setMaterial(&groundMaterial);
	Object ground95 = Object(8.0, -0.1f, 0.0, 1.0f);
	SGNode groundNode95 = SGNode(&ground95);
	scene.getRoot()->addChild(&groundNode95);
	ground95.setMesh(&groundMesh);
	ground95.setMaterial(&groundMaterial);
	Object ground96 = Object(8.0, -0.1f, 2.0, 1.0f);
	SGNode groundNode96 = SGNode(&ground96);
	scene.getRoot()->addChild(&groundNode96);
	ground96.setMesh(&groundMesh);
	ground96.setMaterial(&groundMaterial);
	Object ground97 = Object(8.0, -0.1f, 4.0, 1.0f);
	SGNode groundNode97 = SGNode(&ground97);
	scene.getRoot()->addChild(&groundNode97);
	ground97.setMesh(&groundMesh);
	ground97.setMaterial(&groundMaterial);
	Object ground98 = Object(8.0, -0.1f, 6.0, 1.0f);
	SGNode groundNode98 = SGNode(&ground98);
	scene.getRoot()->addChild(&groundNode98);
	ground98.setMesh(&groundMesh);
	ground98.setMaterial(&groundMaterial);
	Object ground99 = Object(8.0, -0.1f, 8.0, 1.0f);
	SGNode groundNode99 = SGNode(&ground99);
	scene.getRoot()->addChild(&groundNode99);
	ground99.setMesh(&groundMesh);
	ground99.setMaterial(&groundMaterial);

	// Visualize hierarchy
	scene.viewSceneGraph();

	GraphicsEngine app;

	// Set the scene
	app.setScene(&scene);
	// Start the application
	app.start();
}
