# Vulkan Engine
Projet réalisé à la base dans le cadre du Bureau d'étude de la troisième année de Licence à l'Université Paul Sabatier de Toulouse.
Cette version du projet est étendue.

# Requirements
 - [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
 - [tiny_obj_loader.h](https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h)
 - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
 
# Application
 Pour créer une application avec ce moteur :
 - Créer un fichier qui servira de **main** (main.cpp par exemple)
 - Inclure le moteur
 > #include "GraphicsEngine.h"
 - Créer une fonction main
 > int main() { }
 - Dans le main, créer une scène
 > Scene scene = Scene();
 - Y ajouter des lumières
 > DirectionalLight dirLight = DirectionalLight(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
 > PointLight point = PointLight(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
 > PointLight point2 = PointLight(14.0f, 20.0f, 1.0f, 1.0f, 0.0f, 0.0f);
 > scene.addDirectionalLight(&dirLight);
 > scene.addPointLight(&point);
 > scene.addPointLight(&point2);
 - Y ajouter des objets et construire le graphe de scène
 > Object ground = Object("ground_model.obj", "ground_diffuse_texture.jpg", 0.0f, 0.0f, -0.1f, 30.0f);
 > ground.setNormalTexturePath("ground_normal_texture.jpg");
 > SGNode groundNode = SGNode(&ground);
 > scene.getRoot()->addChild(&groundNode);
 
 > Object anotherObject = Object("object_model.obj", "object_diffuse_texture.jpg", 0.0f, 0.1f, 0.0f, 0.5f);
 > anotherObject.setName("Another Object");
 > anotherObject.setNormalTexturePath("object_normal_texture.jpg");
 > SGNode anotherObjectNode = SGNode(&anotherObject);
 > groundNode.addChild(&anotherObjectNode);
 - Lancer l'application
 > GraphicsEngine app;
 > app.setScene(&scene);
 > app.start();