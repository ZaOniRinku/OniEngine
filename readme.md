# ONIEngine
ONIEngine est un moteur graphique développé en C++ avec l'API Vulkan.

Projet réalisé à la base dans le cadre du Bureau d'étude de la troisième année de Licence à l'Université Paul Sabatier de Toulouse.
Cette version du projet est étendue.

# Requirements
 - [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
 - [glfw](https://www.glfw.org/)
 - [tiny_obj_loader.h](https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h)
 - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
 
# Application
 Pour créer une application avec ce moteur :
 - Créer un fichier qui servira de **main** (main.cpp par exemple)
 - Inclure le moteur
 ```C++
 #include "GraphicsEngine.h"
```
 - Créer une fonction main
 ```C++
 int main() { }
 ```
 - Dans le main, créer une scène
 ```C++
 Scene scene = Scene();
 ```
 - Y ajouter des lumières
 ```C++
 DirectionalLight dirLight = DirectionalLight(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
 PointLight point = PointLight(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
 PointLight point2 = PointLight(14.0f, 20.0f, 1.0f, 1.0f, 0.0f, 0.0f);
 scene.addDirectionalLight(&dirLight);
 scene.addPointLight(&point);
 scene.addPointLight(&point2);
 ```
 - Ajouter des Meshes
 ```C++
 Mesh objetMesh = Mesh("objet.obj");
 ```
 - Ajouter des Matériaux
 ```C++
 // Nécessite 5 textures (PBR) : Diffuse Map, Normal Map, Metallic Map, Roughness Map et Ambient Occlusion Map.
 Material mat = Material("diffuse_texture.jpg", "normal_texture.jpg", "metallic_texture.jpg", "roughness_texture.jpg", "ao_texture.jpg");
 ```
 - Y ajouter des objets et construire le graphe de scène
 ```C++
 Object ground = Object(0.0f, 0.0f, -0.1f, 30.0f);
 ground.setMesh(&objetMesh);
 ground.setMaterial(&mat);
 SGNode groundNode = SGNode(&ground);
 scene.getRoot()->addChild(&groundNode);
 ```
 
 ```C++
 Object anotherObject = Object(0.0f, 0.1f, 0.0f, 0.5f);
 anotherObject.setMesh(&anotherMesh);
 anotherObject.setMaterial(&mat);
 anotherObject.setName("Another Object");
 SGNode anotherObjectNode = SGNode(&anotherObject);
 groundNode.addChild(&anotherObjectNode);
 ```
 - Configurer les contrôles
 ```C++
 // Fonction de contrôle pour la caméra :
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
 ```
 
 ```C++
 // Assigner la fonction (même fonctionnement pour les objets) :
 scene.getCamera()->controls = cameraControls;
 ```
 - Lancer l'application
 ```C++
 GraphicsEngine app;
 app.setScene(&scene);
 app.start();
 ```
