/*
* 
* Equipo 3 CGIHC
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <animatedmodel.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>

#include <irrKlang.h>
using namespace irrklang;

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

// Declaración global ajustada para la posición inicial del astronauta
glm::vec3 position(-1.1156f, 0.0f, -13.627f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     trdpersonOffset = 1.5f;
float     scaleV = 0.025f;
float     rotateCharacter = 44.078f;


float	  door_offset = 0.0f;
float	  door_rotation = 0.0f;
float     window_rotation = 0.0f;
float	  meteor_size = 1.0f;
float	  meteor_offset = 0.0f;

float   excavadora_rotation = 0.0f;
float supernova_rotation = 0.0f;

float capsule_offset = 0.0f;
float capsule_speed = 0.009f;
float capsule_max = 2.0f;
bool going_up = true;



// Shaders
Shader *mLightsShader;
Shader *proceduralShader;
Shader *wavesShader;

Shader *cubemapShader;
Shader *dynamicShader;

Shader *proceduralSatelite;
Shader *proceduralPlaneta;

// CARGAR MODELOS
Model* MAESTRO;
Model* meteoro;
Model* lightDummy;
Model* excavadora;	
Model* supernova;
Model* camion;
Model* capsula;
Model* bandera;

Model* planeta0;
Model* planeta1;
Model* planeta2;
Model* planeta3;
Model* cohete;
Model* nave;
Model* satelite0;
Model* satelite1;
Model* satelite2;
Model* walle;
Model* amongUs;
Model* humo;

// MODELOS ANIMADOS
AnimatedModel   *character01;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap *mainCubeMap;

// Light gLight;
std::vector<Light> gLights;

// Materiales
Material material01;
Material material;

float proceduralTime = 0.0f;
float wavesTime = 0.0f;

// Audio
ISoundEngine *SoundEngine = createIrrKlangDevice();

// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara

Light lightM;
Light lightSN1;
Light lightSN2;
Light lightSN3;
Light lightEXC1;
Light lightCUL;

int meteorLightIndex;
int farolIzqIndex;
int farolDerIndex;

// Sistema de disparo laser
glm::vec3 posicionesKiko[5] = {
	glm::vec3(21.0f, 0.1f, -10.0f),
	glm::vec3(25.0f, 0.3f, -15.0f),
	glm::vec3(30.0f, 0.8f, -10.0f),
	glm::vec3(34.0f, 1.0f, -15.0f),
	glm::vec3(40.0f, 1.2f, -10.0f)
};

// Rotacios modelos marcianos
float rotacionBaseKiko[5] = {
	60.0f, 30.0f, 180.0f, -30.0f, -60.0f
};

bool laserActivo = false;
glm::vec3 posicionLaser(0.0f);
glm::vec3 colorLaser(1.0f);
float distanciaLaser = 0.0f;
float velocidadLaser = 25.0f;
float anguloLaser = 0.0f;



// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PROYECTO FINAL - Vida sustentable en el espacio", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	mLightsShader = new Shader("shaders/11_PhongShaderMultLightsPuntual.vs", "shaders/11_PhongShaderMultLightsPuntual.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation2.vs", "shaders/12_ProceduralAnimation2.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	dynamicShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");

	proceduralSatelite = new Shader("shaders/chema_ProceduralSat.vs", "shaders/chema_ProceduralSat.fs");
	proceduralPlaneta = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");

	// Máximo número de huesos: 100
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// CARPETA MODELOS ******
	MAESTRO = new Model("models/ModelosFbx/PF.fbx");
	meteoro = new Model("models/ModelosFbx/meteorito.fbx");
	excavadora = new Model("models/ModelosFbx/ExcavadoraUp.fbx");
	supernova = new Model("models/ModelosFbx/Supernova.fbx");
	camion = new Model("models/ModelosFbx/Camion.fbx");
	capsula = new Model("models/ModelosFbx/capsula.fbx");
	bandera = new Model("models/ModelosFbx/bandera.fbx");

	planeta0 = new Model("models/ModelosFbx/planeta.fbx");
	planeta1 = new Model("models/ModelosFbx/planeta1.fbx");
	planeta2 = new Model("models/ModelosFbx/planeta2.fbx");
	planeta3 = new Model("models/ModelosFbx/planeta3.fbx");
	cohete = new Model("models/ModelosFbx/coheteEspacial.fbx");
	nave = new Model("models/ModelosFbx/nave.fbx");
	satelite0 = new Model("models/ModelosFbx/satelite.fbx");
	satelite1 = new Model("models/ModelosFbx/satelite1.fbx");
	satelite2 = new Model("models/ModelosFbx/satelite2.fbx");
	walle = new Model("models/ModelosFbx/walle.fbx");
	amongUs = new Model("models/ModelosFbx/amongUs.fbx");
	humo = new Model("models/ModelosFbx/humo.fbx");

	lightDummy = new Model("models/ModelosFbx/lightDummy.fbx");

	character01 = new AnimatedModel("models/ModelosFbx/astronauta_walk.fbx");

	// Cubemap 
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.jpg", 
		"textures/cubemap/01/negx.jpg", 
		"textures/cubemap/01/posy.jpg", 
		"textures/cubemap/01/negy.jpg", 
		"textures/cubemap/01/posz.jpg", 
		"textures/cubemap/01/negz.jpg"  
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= trdpersonOffset * forwardView;
	camera3rd.Front = forwardView;

	// Lights configuration
	Light light;
	light.type = 2; // Sol
	light.Direction = glm::vec3(-0.2f, -1.0f, -0.3f); 
	light.Power = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gLights.push_back(light);

	Light light2;
	light2.Position = glm::vec3(-7.23106f, 2.43386f, -12.3568f);
	light2.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	light2.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light2.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	light2.alphaIndex = 128;
	light2.distance = 3.0f;
	light2.cutOff = glm::cos(glm::radians(89.0f)); 
	light2.outerCutOff = glm::cos(glm::radians(85.0f)); 
	light2.type = 1;
	gLights.push_back(light2);

	Light light3;
	light3.Position = glm::vec3(12.492, 3.0, 30.4636f);
	light3.Color = glm::vec4(0.5f, 1.0f, 0.0f, 1.0f);
	light3.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	light3.alphaIndex = 256;
	light3.distance = 3.0f;
	light3.type = 0;
	gLights.push_back(light3);


	//LightM
	lightM.Position = glm::vec3(-17.4583f, 21.3668f, 36.7405f);
	lightM.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lightM.Power = glm::vec4(80.0f, 80.0f, 80.0f, 1.0f);
	lightM.alphaIndex = 128;
	lightM.distance = 1.0f;
	lightM.type = 0;
	gLights.push_back(lightM);
	meteorLightIndex = gLights.size() - 1;

	//LightSN1
	lightSN1.Position = glm::vec3(-106.488f, 46.5978f, 31.1536f);
	lightSN1.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSN1.Power = glm::vec4(200.0f, 200.0f, 200.0f, 1.0f);
	lightSN1.alphaIndex = 128;
	lightSN1.distance = 1.0f;
	lightSN1.type = 0;
	gLights.push_back(lightSN1);


	//LightSN2
	lightSN2.Position = glm::vec3(-117.842f, 46.5978f, 23.2239f);
	lightSN2.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSN2.Power = glm::vec4(200.0f, 200.0f, 200.0f, 1.0f);
	lightSN2.alphaIndex = 128;
	lightSN2.distance = 1.0f;
	lightSN2.type = 0;
	gLights.push_back(lightSN2);

	//LightSN3
	lightSN3.Position = glm::vec3(-118.563f, -39.0834f, 51.644f);
	lightSN3.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSN3.Power = glm::vec4(200.0f, 200.0f, 200.0f, 1.0f);
	lightSN3.alphaIndex = 128;
	lightSN3.distance = 1.0f;
	lightSN3.type = 0;
	gLights.push_back(lightSN3);

	//lightEXC1
	lightEXC1.Position = glm::vec3(-26.3512f, 11.98f, -9.3925f);
	lightEXC1.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightEXC1.Power = glm::vec4(50.0f, 50.0f, 50.0f, 1.0f);
	lightEXC1.alphaIndex = 128;
	lightEXC1.distance = 1.0f;
	lightEXC1.type = 0;
	gLights.push_back(lightEXC1);

	//lightCUL
	lightCUL.Position = glm::vec3(18.0336f, 5.0f, 7.49889f);
	lightCUL.Color = glm::vec4(1.0f, 0.698f, 0.0f, 1.0f);
	lightCUL.Power = glm::vec4(200.0f, 200.0f, 200.0f, 1.0f);
	lightCUL.alphaIndex = 128;
	lightCUL.distance = 1.0f;
	lightCUL.type = 1;
	gLights.push_back(lightCUL);

	// LightTorre1
	Light lightT1;
	lightT1.Position = glm::vec3(-44.674f, 8.785f, 0.17336f);
	lightT1.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); // cyan neón
	lightT1.Power = glm::vec4(80.0f, 80.0f, 80.0f, 1.0f);
	lightT1.alphaIndex = 128;
	lightT1.distance = 2.0f;
	lightT1.type = 0;
	gLights.push_back(lightT1);

	// LightTorre2
	Light lightT2;
	lightT2.Position = glm::vec3(-39.608f, 10.767f, 4.6277f);
	lightT2.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	lightT2.Power = glm::vec4(80.0f, 80.0f, 80.0f, 1.0f);
	lightT2.alphaIndex = 128;
	lightT2.distance = 2.0f;
	lightT2.type = 0;
	gLights.push_back(lightT2);

	// LightTorre3
	Light lightT3;
	lightT3.Position = glm::vec3(-45.358f, 14.004f, 9.6701f);
	lightT3.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	lightT3.Power = glm::vec4(80.0f, 80.0f, 80.0f, 1.0f);
	lightT3.alphaIndex = 128;
	lightT3.distance = 2.0f;
	lightT3.type = 0;
	gLights.push_back(lightT3);

	// Luces plataforma
	Light lightP1;
	lightP1.Position = glm::vec3(20.172f, 1.1019f, -31.257f);
	lightP1.Direction = glm::normalize(glm::vec3(23.7f - 20.172f, 3.0f, -31.0f - (-31.257f)));
	lightP1.Color = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	lightP1.Power = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
	lightP1.alphaIndex = 128;
	lightP1.distance = 5.0f;
	lightP1.cutOff = glm::cos(glm::radians(15.0f));
	lightP1.outerCutOff = glm::cos(glm::radians(20.0f));
	lightP1.type = 1;
	gLights.push_back(lightP1);

	Light lightP2;
	lightP2.Position = glm::vec3(25.631f, 1.1019f, -34.489f);
	lightP2.Direction = glm::normalize(glm::vec3(23.7f - 25.631f, 3.0f, -31.0f - (-34.489f)));
	lightP2.Color = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	lightP2.Power = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
	lightP2.alphaIndex = 128;
	lightP2.distance = 5.0f;
	lightP2.cutOff = glm::cos(glm::radians(15.0f));
	lightP2.outerCutOff = glm::cos(glm::radians(20.0f));
	lightP2.type = 1;
	gLights.push_back(lightP2);

	Light lightP3;
	lightP3.Position = glm::vec3(25.563f, 1.1019f, -27.408f);
	lightP3.Direction = glm::normalize(glm::vec3(23.7f - 25.563f, 3.0f, -31.0f - (-27.408f)));
	lightP3.Color = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	lightP3.Power = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
	lightP3.alphaIndex = 128;
	lightP3.distance = 5.0f;
	lightP3.cutOff = glm::cos(glm::radians(15.0f));
	lightP3.outerCutOff = glm::cos(glm::radians(20.0f));
	lightP3.type = 1;
	gLights.push_back(lightP3);

	// Luces escudos
	Light lightE1;
	lightE1.Position = glm::vec3(-28.578f, 5.7313f, 41.991f);
	lightE1.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lightE1.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	lightE1.alphaIndex = 128;
	lightE1.distance = 5.0f;
	lightE1.type = 0;
	gLights.push_back(lightE1);

	Light lightE2;
	lightE2.Position = glm::vec3(-33.252f, 3.9654f, 33.067f);
	lightE2.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lightE2.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	lightE2.alphaIndex = 128;
	lightE2.distance = 5.0f;
	lightE2.type = 0;
	gLights.push_back(lightE2);
	Light farolIzq;
	farolIzq.type = 1; 
	farolIzq.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	farolIzq.Power = glm::vec4(20.0f, 16.0f, 16.0f, 16.0f); 
	farolIzq.cutOff = glm::cos(glm::radians(15.0f)); 
	farolIzq.outerCutOff = glm::cos(glm::radians(24.5f)); 
	gLights.push_back(farolIzq);
	farolIzqIndex = gLights.size() - 1;

	Light farolDer = farolIzq;
	gLights.push_back(farolDer);
	farolDerIndex = gLights.size() - 1;

	return true;
}


void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection;
	glm::mat4 view;

	if (activeCamera) {
		// Cámara en primera persona
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
	}
	else {
		// cámara en tercera persona
		projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera3rd.GetViewMatrix();
	}

	// Cubemap (fondo)
	{
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}
	
	 {
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
			SetLightUniformInt(mLightsShader, "type", i, gLights[i].type);
			SetLightUniformFloat(mLightsShader, "cutOff", i, gLights[i].cutOff);
			SetLightUniformFloat(mLightsShader, "outerCutOff", i, gLights[i].outerCutOff);
		}
		
		mLightsShader->setVec3("eye", camera.Position);

		material.ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		material.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		material.transparency = 1.0f;
		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		// MAESTRO
		MAESTRO->Draw(*mLightsShader);

		//Meteorito
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.6288f + meteor_offset, 23.8857f - meteor_offset, 36.3215f )); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f + meteor_size, 0.1f + meteor_size, 0.1f + meteor_size));
		mLightsShader->setMat4("model", model);
		meteoro->Draw(*mLightsShader);

		// Excavadora
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-29.4625f, 13.2426f, -12.8524f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(excavadora_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		excavadora ->Draw(*mLightsShader);

		// Supernova
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-113.741f, 49.8045f, 31.1289f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(supernova_rotation), glm::vec3(-0.407f, -0.816f, 0.411f));
		model = glm::rotate(model, glm::radians(29.1885f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-26.0396f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		supernova->Draw(*mLightsShader);


		//Light dummy
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightM.Position); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));// it's a bit too big for our scene, so scale it down

		mLightsShader->setMat4("model", model);
		lightDummy->Draw(*mLightsShader);

		// CAPSULA
		if (going_up) {
			capsule_offset += capsule_speed;
			if (capsule_offset >= capsule_max) going_up = false;
		}
		else {
			capsule_offset -= capsule_speed;
			if (capsule_offset <= 0.0f) going_up = true;
		}
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.252f, capsule_offset + 0.0f, -28.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		capsula->Draw(*mLightsShader);

		// WALLE
		{
			model = glm::mat4(1.0f);

			float tiempo = currentFrame;
			float offsetX = (sin(tiempo * 0.7f) * 3.0f) + (cos(tiempo * 1.3f) * 2.0f);
			float offsetZ = (cos(tiempo * 0.5f) * 3.0f) + (sin(tiempo * 1.1f) * 2.0f);
			float offsetY = abs(sin(tiempo * 4.0f)) * 0.2f + sin(tiempo * 1.5f) * 0.1f;
			float velX = (0.7f * cos(tiempo * 0.7f) * 3.0f) - (1.3f * sin(tiempo * 1.3f) * 2.0f);
			float velZ = -(0.5f * sin(tiempo * 0.5f) * 3.0f) + (1.1f * cos(tiempo * 1.1f) * 2.0f);
			float angulo_orientacion = atan2(velX, velZ);
			model = glm::translate(model, glm::vec3(10.0f + offsetX, 0.0f + offsetY, -25.0f + offsetZ));
			model = glm::rotate(model, angulo_orientacion, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

			mLightsShader->setMat4("model", model);
			walle->Draw(*mLightsShader);
		}

		// AMONGUS
		{
			model = glm::mat4(1.0f);

			float tiempo = currentFrame;
			float offsetX = (sin(tiempo * 0.6f) * 1.0f) + (cos(tiempo * 1.7f) * 0.0f);
			float offsetZ = (cos(tiempo * 0.3f) * 2.0f) + (sin(tiempo * 1.1f) * 1.0f);
			float offsetY = abs(sin(tiempo * 4.0f)) * 0.2f + sin(tiempo * 1.5f) * 0.1f;
			float velX = (0.7f * cos(tiempo * 0.7f) * 3.0f) + (1.3f * sin(tiempo * 1.3f) * 2.0f);
			float velZ = -(0.5f * sin(tiempo * 0.5f) * 3.0f) - (1.1f * cos(tiempo * 1.1f) * 2.0f);
			float angulo_orientacion = atan2(velX, velZ);
			model = glm::translate(model, glm::vec3(5.0f + offsetX, 0.0f + offsetY, -15.0f + offsetZ));
			model = glm::rotate(model, angulo_orientacion, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

			mLightsShader->setMat4("model", model);
			amongUs->Draw(*mLightsShader);
		}

	}
		

	glUseProgram(0);


	// CAMION
	{
		// Activamos el shader 
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(17.7995f, 0.770269f, 7.29547f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 14.0f);
		proceduralShader->setFloat("height", 0.0f);

		camion->Draw(*proceduralShader);
		proceduralTime += 0.01;

	}

	glUseProgram(0);

	// NAVE
	{
		// Activamos el shader 
		proceduralShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 20.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 20.0f);
		proceduralShader->setFloat("height", 10.0f);

		nave->Draw(*proceduralShader);
		proceduralTime += 0.01;

	}

	glUseProgram(0);

	// PLANETA
	{
		// Activamos el shader 
		proceduralPlaneta->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralPlaneta->setMat4("projection", projection);
		proceduralPlaneta->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		proceduralPlaneta->setMat4("model", model);

		proceduralPlaneta->setFloat("time", proceduralTime);
		proceduralPlaneta->setFloat("a", 15.0f);
		proceduralPlaneta->setFloat("m", 1.0f);

		planeta0->Draw(*proceduralPlaneta);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);

	// PLANETA 1
	{
		// Activamos el shader 
		proceduralPlaneta->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralPlaneta->setMat4("projection", projection);
		proceduralPlaneta->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		proceduralPlaneta->setMat4("model", model);

		proceduralPlaneta->setFloat("time", proceduralTime);
		proceduralPlaneta->setFloat("a", 15.0f);
		proceduralPlaneta->setFloat("m", 1.0f);

		planeta1->Draw(*proceduralPlaneta);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);

	// PLANETA 2
	{
		// Activamos el shader 
		proceduralPlaneta->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralPlaneta->setMat4("projection", projection);
		proceduralPlaneta->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.5f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		proceduralPlaneta->setMat4("model", model);

		proceduralPlaneta->setFloat("time", proceduralTime);
		proceduralPlaneta->setFloat("a", 15.0f);
		proceduralPlaneta->setFloat("m", 1.0f);

		planeta2->Draw(*proceduralPlaneta);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);

	// PLANETA 3
	{
		// Activamos el shader 
		proceduralPlaneta->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralPlaneta->setMat4("projection", projection);
		proceduralPlaneta->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.25f, 0.75f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		proceduralPlaneta->setMat4("model", model);

		proceduralPlaneta->setFloat("time", proceduralTime);
		proceduralPlaneta->setFloat("a", 15.0f);
		proceduralPlaneta->setFloat("m", 1.0f);

		planeta3->Draw(*proceduralPlaneta);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);

	// SATELITE
	{
		// Activamos el shader 
		proceduralSatelite->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);

		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("horizontalSpeed", 0.5f);
		proceduralSatelite->setFloat("radiusY", 60.0f);
		proceduralSatelite->setFloat("radiusZ", 50.0f);
		proceduralSatelite->setFloat("startX", -50.0f);

		satelite0->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);
	/*
	// SATELITE 1
	{
		// Activamos el shader 
		proceduralSatelite->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);

		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("horizontalSpeed", 0.5f);
		proceduralSatelite->setFloat("radiusY", 60.0f);
		proceduralSatelite->setFloat("radiusZ", 45.0f);
		proceduralSatelite->setFloat("startX", -40.0f);

		satelite1->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);

	// SATELITE 2
	{
		// Activamos el shader 
		proceduralSatelite->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);

		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("horizontalSpeed", 0.5f);
		proceduralSatelite->setFloat("radiusY", 60.0f);
		proceduralSatelite->setFloat("radiusZ", 75.0f);
		proceduralSatelite->setFloat("startX", -30.0f);

		satelite2->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);
	*/
	// BANDERA
	{
		// Activamos el shader
		wavesShader->use();
		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);
		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0935f, 7.5397f, -45.266f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		wavesShader->setMat4("model", model);
		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);
		bandera->Draw(*wavesShader);
		wavesTime += 0.01;
	}

	glUseProgram(0);
	
	// Astronauta
	{
		character01->UpdateAnimation(deltaTime);

		// Activación del shader del personaje
		dynamicShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		dynamicShader->setMat4("projection", projection);
		dynamicShader->setMat4("view", view);

		// Código corregido (Dinámico)
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position); // Aplica el vector de posición global
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f)); // Aplica la rotación calculada
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

		dynamicShader->setMat4("model", model);

		dynamicShader->setMat4("gBones", MAX_RIGGING_BONES, character01->gBones);

		// Dibujamos el modelo
		character01->Draw(*dynamicShader);
	}

	glUseProgram(0); 

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}
// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	static bool keyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	// Nuevo control para ascender con la barra espaciadora
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	// Opcional: Control para descender (ej. tecla Shift Izquierdo)
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		excavadora_rotation += 1.f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		excavadora_rotation -= 1.f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		supernova_rotation += 1.f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		supernova_rotation -= 1.f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		if (window_rotation <= 130.0f)
			window_rotation += 1.f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		if (window_rotation >= 0.0f)
			window_rotation -= 1.f;

	// Control del meteorito
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !keyPressed) {
		meteor_offset += 1.0f;
		meteor_size += 0.01f;
		gLights[meteorLightIndex].Position.x += 1.0f;
		gLights[meteorLightIndex].Position.y -= 1.0f;
		keyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
		keyPressed = false;
	}

	// --- NUEVO: Activador del Láser con Tecla K ---
	static bool kKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !kKeyPressed) {
		kKeyPressed = true;

		if (!laserActivo) {
			laserActivo = true;
			distanciaLaser = 0.0f;

			// 1. Selección del emisor
			int alienShooter = rand() % 5;
			posicionLaser = posicionesKiko[alienShooter];
			posicionLaser.y += 0.8f;

			// 2. Asignación aleatoria de color
			int colorPicker = rand() % 3;
			if (colorPicker == 0) colorLaser = glm::vec3(1.0f, 0.0f, 0.0f);      // Rojo
			else if (colorPicker == 1) colorLaser = glm::vec3(0.0f, 1.0f, 0.0f); // Verde
			else colorLaser = glm::vec3(0.1f, 0.7f, 1.0f);                       // Azul Cielo

			// --- NUEVO: Cálculo del ángulo de disparo ---
			// Simulamos el mismo tiempo desfasado que usa el renderizado
			float tiempoActual = (float)glfwGetTime();
			float tiempoDesfasado = tiempoActual + (alienShooter * 0.8f);
			float idle_rotation = sin(tiempoDesfasado * 1.5f) * 20.0f;

			// Sumamos la base + la animación y lo convertimos a radianes
			anguloLaser = glm::radians(rotacionBaseKiko[alienShooter] + idle_rotation);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
		kKeyPressed = false;
	}


	// 1. Definimos constantes de velocidad (ajusta estos valores a tu gusto)
	float movementSpeed = 2.0f;    // Velocidad de traslación
	float turnSpeed = 60.0f;      // Velocidad de rotación en grados por segundo

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		// Multiplicamos por deltaTime para que el movimiento sea fluido y no dependa del framerate
		position = position + (movementSpeed * deltaTime) * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position = position - (movementSpeed * deltaTime) * forwardView;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		// Aumentamos significativamente el incremento de giro
		rotateCharacter += turnSpeed * deltaTime;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		// Reducimos el ángulo basado en el tiempo transcurrido
		rotateCharacter -= turnSpeed * deltaTime;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		camera3rd.Position = position;
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= trdpersonOffset * forwardView;
	}

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		activeCamera = 0;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		activeCamera = 1;

}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; 

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
