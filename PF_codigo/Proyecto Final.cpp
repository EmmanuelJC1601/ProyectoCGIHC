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
void processInput(GLFWwindow* window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));
Camera cameraCohete(glm::vec3(0.0f, 0.0f, 0.0f));
Camera cameraPlanetario(glm::vec3(0.0f, 0.0f, 0.0f));

bool camaraCoheteActiva = false;
bool personajeMoviendose = false;

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
glm::vec3 position(-1.1156f, 0.5f, -13.627f);
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

float cohete_height = 0.00f;

// Shaders
Shader* mLightsShader;
Shader* proceduralShader;
Shader* wavesShader;

Shader* cubemapShader;
Shader* dynamicShader;

Shader* proceduralSatelite;
Shader* proceduralPlaneta;

Shader* fresnelMetalShader;
Shader* fresnelVidrioShader;

// CARGAR MODELOS
Model* PF_MATE;
Model* PF_METAL;
Model* PF_CRISTAL;
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
Model* satelite3;
Model* satelite4;
Model* walle;
Model* amongUs;
Model* humo;

// MODELOS ANIMADOS
AnimatedModel* character01;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap* mainCubeMap;

// Light gLight;
std::vector<Light> gLights;

// Materiales
Material material01;
Material material;

float proceduralTime = 0.0f;
float wavesTime = 0.0f;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

// selección de cámara
int    activeCamera = 1; // activamos la primera cámara

Light lightM;
Light lightSN1;
Light lightSN2;
Light lightSN3;
Light lightEXC1;
glm::vec3 pivoteEXC1 = glm::vec3(-60.4081f, 16.4927f, -60.1588f);
Light lightCUL;

int meteorLightIndex;
float anguloexcavadora = 1.0f;
int excaLightIndex;
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

//Parte de colisiones

struct BoundingBox {
	glm::vec3 min;
	glm::vec3 max;

	// Función para detectar colisión con otra caja
	bool checkCollision(const BoundingBox& other) {
		return (min.x <= other.max.x && max.x >= other.min.x) &&
			(min.y <= other.max.y && max.y >= other.min.y) &&
			(min.z <= other.max.z && max.z >= other.min.z);
	}
};

struct Obstacle {
	glm::vec3 position;
	glm::vec3 size;
	BoundingBox box;
};

// Creamos una lista de obstáculos
std::vector<Obstacle> obstaculos;

// Variables globales de colisión
BoundingBox boxCharacter;
// Shader para debug de colisiones
Shader* debugShader;
bool showHitbox = false;
// VAO y VBO para las cajas de colisión
unsigned int boxVAO, boxVBO, boxEBO;

glm::vec3 characterSize = glm::vec3(0.5f, 1.8f, 0.5f); // 0.5m ancho, 1.8m alto

struct InteractionZone {
	std::string message; // El mensaje que quieres mostrar ("Presiona E para entrar")
	BoundingBox box;     // La caja que delimita la zona
	bool isPlayerInside; // Flag para saber si el jugador está adentro actualmente
	int zoneID;          // Un identificador por si quieres saber a qué casa/lugar entra
};

// Creamos la lista global de zonas de interacción
std::vector<InteractionZone> zonasInteraccion;

// Variable global para saber qué mensaje mostrar en el loop de renderizado
std::string mensajePantallaActual = "";
bool mostrarMensaje = false;

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

	fresnelMetalShader = new Shader("shaders/11_fresnel.vs", "shaders/11_fresnel_metal.fs");
	fresnelVidrioShader = new Shader("shaders/11_fresnel.vs", "shaders/11_fresnel_vidrio.fs");

	// Shader de debug (líneas de colisión)
	debugShader = new Shader("shaders/colisiones.vs", "shaders/colisiones.fs");

	float boxVertices[] = {
		// 8 vértices: X, Y, Z — todos entre 0.0 y 1.0
		0.0f, 0.0f, 0.0f,  // 0: min
		1.0f, 0.0f, 0.0f,  // 1
		1.0f, 1.0f, 0.0f,  // 2
		0.0f, 1.0f, 0.0f,  // 3
		0.0f, 0.0f, 1.0f,  // 4
		1.0f, 0.0f, 1.0f,  // 5
		1.0f, 1.0f, 1.0f,  // 6: max
		0.0f, 1.0f, 1.0f,  // 7
	};

	unsigned int boxIndices[] = {
		// 12 aristas del cubo (líneas)
		0,1, 1,2, 2,3, 3,0, // cara trasera
		4,5, 5,6, 6,7, 7,4, // cara delantera
		0,4, 1,5, 2,6, 3,7  // aristas laterales
	};

	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);
	glGenBuffers(1, &boxEBO);

	glBindVertexArray(boxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);



	// Máximo número de huesos: 100
	dynamicShader->setBonesIDs(MAX_RIGGING_BONES);

	// CARPETA MODELOS ******
	PF_MATE = new Model("models/ModelosFbx/PF_MATE.fbx");
	PF_METAL = new Model("models/ModelosFbx/PF_METAL.fbx");
	PF_CRISTAL = new Model("models/ModelosFbx/PF_CRISTAL.fbx");
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
	satelite3 = new Model("models/ModelosFbx/satelite3.fbx");
	satelite4 = new Model("models/ModelosFbx/satelite4.fbx");
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


	//Camara
	camera3rd.Position = position;
	camera3rd.Position.y += 1.7f;
	camera3rd.Position -= trdpersonOffset * forwardView;
	camera3rd.Front = forwardView;

	cameraCohete.Position = glm::vec3(92.0144f, 20.0f, -68.9319f);
	cameraCohete.Front = glm::normalize(glm::vec3(-12.1405f, 20.0f, -70.1337f) - cameraCohete.Position);

	cameraPlanetario.Position = glm::vec3(338.523f, 17.6387f, 109.238f);
	cameraPlanetario.Front = glm::normalize(glm::vec3(456.48f, 51.0941f, 109.589f) - cameraPlanetario.Position);



	// Lights configuration

	//Luz global
	Light light;
	light.type = 2; // Sol
	light.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	light.Power = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	gLights.push_back(light);


	/*	//Estacionamiento
		Light light2;
		light2.Position = glm::vec3(-12.9147f, 5.0f, -42.8472f);
		light2.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		light2.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		light2.Power = glm::vec4(15.0f, 15.0f, 15.0f, 1.0f);
		light2.alphaIndex = 128;
		light2.distance = 300.0f;
		light2.cutOff = glm::cos(glm::radians(10.0f));
		light2.outerCutOff = glm::cos(glm::radians(60.0f));
		light2.type = 1;
		gLights.push_back(light2);*/

		//Residuos
	Light light3;
	light3.Position = glm::vec3(32.789f, -10.0f, 126.602f);
	light3.Color = glm::vec4(0.5f, 1.0f, 0.0f, 1.0f);
	light3.Power = glm::vec4(50.0f, 50.0f, 50.0f, 1.0f);
	light3.alphaIndex = 256;
	light3.type = 0;
	gLights.push_back(light3);


	//LightM
	lightM.Position = glm::vec3(-143.02f, 57.1525f, 126.602f);
	lightM.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lightM.Power = glm::vec4(50.0f, 50.0f, 50.0f, 1.0f);
	lightM.alphaIndex = 128;
	lightM.distance = 100.0f;
	lightM.type = 0;
	gLights.push_back(lightM);
	meteorLightIndex = gLights.size() - 1;


	//LightSN1
	lightSN1.Position = glm::vec3(448.315f, 51.0941f, 109.589f);
	lightSN1.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightSN1.Power = glm::vec4(100.0f, 100.0f, 100.0f, 1.0f);
	lightSN1.alphaIndex = 128;
	lightSN1.distance = 100.0f;
	lightSN1.type = 0;
	gLights.push_back(lightSN1);


	//lightEXC1
	lightEXC1.Position = glm::vec3(-60.1957f, 15.9777f, -45.158f);
	lightEXC1.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightEXC1.Power = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);
	lightEXC1.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightEXC1.cutOff = glm::cos(glm::radians(5.0f));
	lightEXC1.outerCutOff = glm::cos(glm::radians(30.0f));
	lightEXC1.alphaIndex = 254;
	lightEXC1.distance = 100.0f;
	lightEXC1.type = 1;
	gLights.push_back(lightEXC1);
	excaLightIndex = gLights.size() - 1;


	//lightCUL
	lightCUL.Position = glm::vec3(78.3205f, 21.9512f, 6.45334f);
	lightCUL.Color = glm::vec4(1.0f, 0.698f, 0.0f, 1.0f);
	lightCUL.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	lightCUL.alphaIndex = 64;
	lightCUL.distance = 100.0f;
	lightCUL.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightCUL.cutOff = glm::cos(glm::radians(10.0f));
	lightCUL.outerCutOff = glm::cos(glm::radians(80.0f));
	lightCUL.type = 1;
	gLights.push_back(lightCUL);


	// LightTorre1
	Light lightT1;
	lightT1.Position = glm::vec3(-72.5525f, 67.3527f, -5.91642f);
	lightT1.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	lightT1.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	lightT1.alphaIndex = 128;
	lightT1.type = 0;
	gLights.push_back(lightT1);
	/*
	// LightTorre2
	Light lightT2;
	lightT2.Position = glm::vec3(-52.799f, 21.013f, -15.166f);
	lightT2.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	lightT2.Power = glm::vec4(8.0f, 8.0f, 8.0f, 1.0f);
	lightT2.alphaIndex = 500;
	lightT2.type = 0;
	gLights.push_back(lightT2);

	// LightTorre3
	Light lightT3;
	lightT3.Position = glm::vec3(-54.001f, 32.973f, 11.304f);
	lightT3.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	lightT3.Power = glm::vec4(8.0f, 8.0f, 8.0f, 1.0f);
	lightT3.alphaIndex = 500;
	lightT3.type = 0;
	gLights.push_back(lightT3);*/

	// Plataforma
	Light lightP1;
	lightP1.Position = glm::vec3(-13.4876f, 1.45712f, -71.1623f);
	lightP1.Color = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	lightP1.Power = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
	lightP1.alphaIndex = 512;
	lightP1.distance = 10.0f;
	lightP1.type = 0;
	gLights.push_back(lightP1);
	/*
	Light lightP2;
	lightP2.Position = glm::vec3(-19.059f, 0.9f, -69.796f);
	lightP2.Direction = glm::normalize(glm::vec3(-14.954f - (-19.059f), 3.0f, -69.898f - (-69.796f)));
	lightP2.Color = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	lightP2.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	lightP2.alphaIndex = 128;
	lightP2.distance = 5.0f;
	lightP2.cutOff = glm::cos(glm::radians(15.0f));
	lightP2.outerCutOff = glm::cos(glm::radians(20.0f));
	lightP2.type = 1;
	gLights.push_back(lightP2);

	Light lightP3;
	lightP3.Position = glm::vec3(-14.204f, 0.9f, -73.826f);
	lightP3.Direction = glm::normalize(glm::vec3(-14.954f - (-14.204f), 3.0f, -69.898f - (-73.826f)));
	lightP3.Color = glm::vec4(1.0f, 1.0f, 0.8f, 1.0f);
	lightP3.Power = glm::vec4(40.0f, 40.0f, 40.0f, 1.0f);
	lightP3.alphaIndex = 128;
	lightP3.distance = 5.0f;
	lightP3.cutOff = glm::cos(glm::radians(15.0f));
	lightP3.outerCutOff = glm::cos(glm::radians(20.0f));
	lightP3.type = 1;
	gLights.push_back(lightP3); */

	// Luces escudos
	Light lightE1;
	lightE1.Position = glm::vec3(-67.9001f, 33.5283f, 106.74f);
	lightE1.Color = glm::vec4(0.0f, 0.9333f, 1.0f, 1.0f);
	lightE1.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	lightE1.alphaIndex = 128;
	lightE1.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightE1.cutOff = glm::cos(glm::radians(70.0f));
	lightE1.outerCutOff = glm::cos(glm::radians(75.0f));
	lightE1.type = 1;
	gLights.push_back(lightE1);

	Light lightE2;
	lightE2.Position = glm::vec3(-40.115f, 33.5283f, 126.035f);
	lightE2.Color = glm::vec4(0.7098f, 0.0f, 0.7098f, 1.0f);
	lightE2.Power = glm::vec4(30.0f, 30.0f, 30.0f, 1.0f);
	lightE2.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightE2.cutOff = glm::cos(glm::radians(70.0f));
	lightE2.outerCutOff = glm::cos(glm::radians(75.0f));
	lightE2.alphaIndex = 128;
	lightE2.distance = 5.0f;
	lightE2.type = 1;
	gLights.push_back(lightE2);

	//Obstaculos planetario
	obstaculos.push_back({ glm::vec3(-2.85093f, 0.0f, 65.1156f), glm::vec3(2.60531f, 2.0f,  1.43633f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-4.40584f, 0.0f, 66.6129f), glm::vec3(1.43633f, 2.0f, 1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-5.61521f, 0.0f, 68.1678f), glm::vec3(1.43633f, 2.0f, 1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-6.43436f, 0.0f, 73.9239f), glm::vec3(1.82875f, 2.0f,  9.97436f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-4.92414f, 0.0f, 79.5704f), glm::vec3(1.43633f, 2.0f, 1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-3.56853f, 0.0f, 81.0786f), glm::vec3(1.43633f, 2.0f, 1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(1.67209f, 0.0f, 82.5759f), glm::vec3(9.55517f, 2.0f,  1.43633f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(6.73994f, 0.0f, 81.0786f), glm::vec3(1.43633f, 2.0f,  1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(8.2062f, 0.0f, 79.5129f), glm::vec3(1.43633f, 2.0f,  1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(10.0937f, 0.0f, 77.4944f), glm::vec3(1.82875f, 2.0f,   3.6367f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(10.0943f, 0.0f, 69.3991f), glm::vec3(1.82875f, 2.0f,   3.6367f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(8.26379f, 0.0f, 66.9584f), glm::vec3(1.43633f, 2.0f,  1.82875f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(6.50958f, 0.0f, 65.3567f), glm::vec3(1.43633f, 2.0f,  1.82875f) });//Planetario 1

	obstaculos.push_back({ glm::vec3(-0.61501f, 0.0f, 46.7535f), glm::vec3(1.82875f, 2.0f,  37.8324f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(4.52655f, 0.0f, 46.6705f), glm::vec3(1.82875f, 2.0f,  37.8324f) });//Planetario 1

	obstaculos.push_back({ glm::vec3(-5.87091f, 0.0f, 24.2595f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-4.54635f, 0.0f, 26.1599f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-2.53073f, 0.0f, 27.6572f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(6.5223f, 0.0f, 28.4865f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(8.37667f, 0.0f, 26.5054f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(10.4034f, 0.0f, 20.5445f), glm::vec3(1.82875f, 2.0f,  9.97435f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(8.37667f, 0.0f, 26.5054f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(8.67037f, 0.0f, 14.3153f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(6.9234f, 0.0f, 12.5339f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-1.21024f, 0.0f, 12.1612f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-3.32312f, 0.0f, 13.3777f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-4.79572f, 0.0f, 15.2985f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-5.75612f, 0.0f, 17.3473f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1

	obstaculos.push_back({ glm::vec3(-30.1803f, 0.0f, 17.5368f), glm::vec3(46.5689f, 2.0f,  1.43633f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-54.1659f, 0.0f, 27.0338f), glm::vec3(1.2189f, 2.0f,   20.4599f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-26.7806f, 0.0f, 22.5231f), glm::vec3(43.626f, 2.0f,  1.43633f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-48.0664f, 0.0f, 30.115f), glm::vec3(1.2189f, 2.0f,   13.5506f) });//Planetario 1

	obstaculos.push_back({ glm::vec3(-55.024f, 0.0f, 38.2554f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-57.0363f, 0.0f, 39.4502f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-58.1681f, 0.0f, 50.6432f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-59.2371f, 0.0f, 44.6694f), glm::vec3(2.07271f, 2.0f,  10.1896f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-56.8476f, 0.0f, 52.5925f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-50.9367f, 0.0f, 54.5418f), glm::vec3(11.4497f, 2.0f,  1.91568f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-45.4031f, 0.0f, 52.5925f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-44.0826f, 0.0f, 50.706f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-43.3909f, 0.0f, 45.1095f), glm::vec3(2.07271f, 2.0f,  9.42948f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-45.5288f, 0.0f, 39.3244f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-47.5411f, 0.0f, 37.8781f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1

	obstaculos.push_back({ glm::vec3(0.857785f, 0.0f, 3.26237f), glm::vec3(1.82875f, 2.0f,  17.723f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(4.8147f, 0.0f, 2.00773f), glm::vec3(1.82875f, 2.0f,  20.4436f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-8.5247f, 0.0f, -4.8925f), glm::vec3(16.7017f, 2.0f,  1.43633f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-3.6907f, 0.0f, -9.321f), glm::vec3(18.7882f, 2.0f,  1.43633f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-16.8135f, 0.0f, -15.7919f), glm::vec3(1.82875f, 2.0f,  20.3304f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-12.1651f, 0.0f, -18.199f), glm::vec3(1.82875f, 2.0f,  16.4527f) });//Planetario 1

	obstaculos.push_back({ glm::vec3(-10.1374f, 0.0f, -26.4652f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-8.03876f, 0.0f, -27.8293f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-5.99262f, 0.0f, -33.653f), glm::vec3(2.07271f, 2.0f,  11.8756f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-6.98946f, 0.0f, -40.5784f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-9.0356f, 0.0f, -41.8375f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-14.1772f, 0.0f, -42.6245f), glm::vec3(8.24983f, 2.0f,  1.91568f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-19.4237f, 0.0f, -41.3129f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-20.9977f, 0.0f, -39.4766f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-22.5716f, 0.0f, -34.2825f), glm::vec3(2.07271f, 2.0f,  8.60824f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-21.0501f, 0.0f, -28.6688f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1
	obstaculos.push_back({ glm::vec3(-19.0565f, 0.0f, -26.4128f), glm::vec3(1.91568f, 2.0f,  2.07271f) });//Planetario 1


	//Obstaculos paneles
	obstaculos.push_back({ glm::vec3(-45.9112f, 0.0f, 97.5499f), glm::vec3(14.4062f, 2.0f, 10.6256f) });//Planetario 10
	//Antena
	obstaculos.push_back({ glm::vec3(-76.3634f, 0.0f, 86.8662f), glm::vec3(8.58289f, 2.0f, 10.6256f) });//Excavadora
	//Residuos
	obstaculos.push_back({ glm::vec3(16.7307f, 0.0f, 126.671f), glm::vec3(1.28499f, 2.0f, 29.6728f) });//Excavadora
	obstaculos.push_back({ glm::vec3(47.7538f, 0.0f, 126.837f), glm::vec3(1.33974f, 2.0f, 29.7264f) });//Excavadora
	obstaculos.push_back({ glm::vec3(32.1979f, 0.0f, 142.26f), glm::vec3(29.7264f, 2.0f,  1.33974f) });//Excavadora
	obstaculos.push_back({ glm::vec3(22.7381f, 0.0f, 111.148f), glm::vec3(10.8113f, 2.0f,1.33974f) });//Excavadora
	obstaculos.push_back({ glm::vec3(41.6597f, 0.0f, 111.148f), glm::vec3(10.8113f, 2.0f, 1.33974f) });//Excavadora

	//Cultivos
	obstaculos.push_back({ glm::vec3(78.2396f, 0.0f, 5.2665f), glm::vec3(57.3512f, 2.0f, 60.2532f) });//Excavadora
	//Reactor
	obstaculos.push_back({ glm::vec3(-119.372f, 0.0f, 35.7895f), glm::vec3(63.3587f, 2.0f, 66.8346f) });//Excavadora
	obstaculos.push_back({ glm::vec3(-71.8282f, 0.0f, -5.55289f), glm::vec3(39.7158f, 2.0f, 41.2301f) });//Excavadora
	obstaculos.push_back({ glm::vec3(-112.137f, 0.0f, -41.383f), glm::vec3(39.7158f, 2.0f, 41.2301f) });//Excavadora

	//Excavadora 
	obstaculos.push_back({ glm::vec3(-60.1359f, 0.0f, -59.9148f), glm::vec3(8.18161f, 2.0f, 8.49356f) });//Excavadora
	//Arbol en cupula
	obstaculos.push_back({ glm::vec3(-25.202f, 0.0f, 4.13061f), glm::vec3(8.18161f, 2.0f, 8.49356f) });//Excavadora

	//Bandera
	obstaculos.push_back({ glm::vec3(-22.3551f, 0.0f, 52.9398f), glm::vec3(7.4517f, 2.0f, 7.4517f) });//Excavadora

	//Cupulas
	obstaculos.push_back({ glm::vec3(-39.7822f, 0.0f, 125.916f), glm::vec3(36.9811f, 2.0f, 36.9811f) });//Excavadora
	obstaculos.push_back({ glm::vec3(-67.23f, 0.0f, 107.835f), glm::vec3(24.0168f, 2.0f, 24.0168f) });//Excavadora

	//Base espacial
	obstaculos.push_back({ glm::vec3(-36.4891f, 0.0f, -114.936f), glm::vec3(9.88254f, 4.0f, 32.4808f) });//Excavadora
	obstaculos.push_back({ glm::vec3(-20.9832f, 0.0f, -126.534f), glm::vec3(26.7246f, 4.0f, 9.4f) });//Excavadora


	// Zonas de interacción
	InteractionZone zonaPlanetario;
	zonaPlanetario.message = "Presiona E para hacer despegar al cohete";
	zonaPlanetario.isPlayerInside = false;
	zonaPlanetario.zoneID = 1; // ID 1 para la cápsula
	// Definimos su caja de colisión (Min y Max)
	zonaPlanetario.box.min = glm::vec3(-2.926f, 0.0f, 69.5525f);
	zonaPlanetario.box.max = glm::vec3(5.5065f, 5.0f, 77.5928f);
	zonasInteraccion.push_back(zonaPlanetario);

	// Zona 2: Cohete
	InteractionZone zonaCoheteE;
	zonaCoheteE.message = "Presiona T para ver las estrellas";
	zonaCoheteE.isPlayerInside = false;
	zonaCoheteE.zoneID = 2;
	zonaCoheteE.box.min = glm::vec3(-27.726f, 0.0f, -82.6192f);
	zonaCoheteE.box.max = glm::vec3(3.52916f, 4.0f, -57.1342f);
	zonasInteraccion.push_back(zonaCoheteE);

	SoundEngine->play2D("sound/musicaPF.wav", true);
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

void DrawBoundingBox(const BoundingBox& box, glm::vec3 color,
	const glm::mat4& view, const glm::mat4& projection)
{
	debugShader->use();

	glm::vec3 size = box.max - box.min;
	glm::vec3 center = box.min; // el cubo ya arranca desde min

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, center);
	model = glm::scale(model, size);

	debugShader->setMat4("model", model);
	debugShader->setMat4("view", view);
	debugShader->setMat4("projection", projection);
	debugShader->setVec3("debugColor", color);

	glBindVertexArray(boxVAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::vec3 oldPosition = position;

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// ------ Dentro de tu función bool Update() ------

// 1. Actualizas la caja de tu personaje con su posición real
	boxCharacter.min = position - (characterSize / 2.0f);
	boxCharacter.max = position + (characterSize / 2.0f);

	// 2. Revisamos las zonas de interacción
	mostrarMensaje = false; // Asumimos que no está en ninguna por defecto
	mensajePantallaActual = "";

	for (size_t i = 0; i < zonasInteraccion.size(); i++) {
		if (boxCharacter.checkCollision(zonasInteraccion[i].box)) {
			zonasInteraccion[i].isPlayerInside = true;
			mostrarMensaje = true;
			mensajePantallaActual = zonasInteraccion[i].message;
		}
		else {
			zonasInteraccion[i].isPlayerInside = false;
		}
	}


	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection;
	glm::mat4 view;

	if (activeCamera == 1) {
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();
	}
	if (activeCamera == 2) {
		projection = glm::perspective(glm::radians(cameraCohete.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = cameraCohete.GetViewMatrix();
	}
	if (activeCamera == 0) {
		if (personajeMoviendose) {
			float yawObjetivo = -90.0f + rotateCharacter;
			float pitchObjetivo = 0.0f; // Queremos que mire recto horizontalmente al caminar
			float deltaYaw = yawObjetivo - camera3rd.Yaw;
			float deltaPitch = pitchObjetivo - camera3rd.Pitch;

			camera3rd.ProcessMouseMovement(deltaYaw / camera3rd.MouseSensitivity, deltaPitch / camera3rd.MouseSensitivity);

			camera3rd.Position = position;
			camera3rd.Position.y += 1.7f;
			camera3rd.Position -= trdpersonOffset * forwardView;
			camera3rd.Front = forwardView;
		}
		projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera3rd.GetViewMatrix();
	}
	if (activeCamera == 3) {
		projection = glm::perspective(glm::radians(cameraPlanetario.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = cameraPlanetario.GetViewMatrix();
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

		// PF
		PF_MATE->Draw(*mLightsShader);

		//Meteorito
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-161.929f + meteor_offset, 70.1912f - meteor_offset, 123.585f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f + meteor_size, 0.1f + meteor_size, 0.1f + meteor_size));
		mLightsShader->setMat4("model", model);
		meteoro->Draw(*mLightsShader);

		// Excavadora
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.1686f, 17.6388f, -59.638f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(excavadora_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		excavadora->Draw(*mLightsShader);

		// Supernova
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(456.48f, 51.0941f, 109.589f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(supernova_rotation), glm::vec3(-0.407f, -0.816f, 0.411f));
		model = glm::rotate(model, glm::radians(29.1885f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-26.0396f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		supernova->Draw(*mLightsShader);

		//COHETE

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-13.7122f, 15.7166f + cohete_height, -70.8137f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mLightsShader->setMat4("model", model);
		cohete->Draw(*mLightsShader);

		//Light dummy
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(32.789f, 10.0f, 126.602f)); // translate it down so it's at the center of the scene
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
		model = glm::translate(model, glm::vec3(23.18f, 2.75701 + capsule_offset, -121.201f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		capsula->Draw(*mLightsShader);

		// WALLE
		{
			model = glm::mat4(1.0f);

			float tiempo = currentFrame;
			float offsetX = (sin(tiempo * 0.6f) * 1.0f) + (cos(tiempo * 1.7f) * 0.0f);
			float offsetZ = (cos(tiempo * 0.3f) * 2.0f) + (sin(tiempo * 1.1f) * 1.0f);
			float offsetY = abs(sin(tiempo * 4.0f)) * 0.2f + sin(tiempo * 1.5f) * 0.1f;
			float velX = (0.7f * cos(tiempo * 0.7f) * 3.0f) + (1.3f * sin(tiempo * 1.3f) * 2.0f);
			float velZ = -(0.5f * sin(tiempo * 0.5f) * 3.0f) - (1.1f * cos(tiempo * 1.1f) * 2.0f);
			float angulo_orientacion = atan2(velX, velZ);
			model = glm::translate(model, glm::vec3(25.0f + offsetX, 1.5f + offsetY, -25.0f + offsetZ));
			model = glm::rotate(model, angulo_orientacion, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

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
			model = glm::translate(model, glm::vec3(5.0f + offsetX, 1.5f + offsetY, 0.0f + offsetZ));
			model = glm::rotate(model, angulo_orientacion, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

			mLightsShader->setMat4("model", model);
			amongUs->Draw(*mLightsShader);
		}

	}
	glUseProgram(0);

	{
		fresnelMetalShader->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		fresnelMetalShader->setMat4("projection", projection);
		fresnelMetalShader->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		fresnelMetalShader->setMat4("model", model);
		fresnelMetalShader->setVec3("cameraPosition", camera.Position);
		fresnelMetalShader->setFloat("mRefractionRatio", 1.0f / 1.0f);
		fresnelMetalShader->setFloat("_Bias", 0.1f);
		fresnelMetalShader->setFloat("_Scale", 1.0f);
		fresnelMetalShader->setFloat("_Power", 3.0f);
		fresnelMetalShader->setInt("texture_diffuse1", 0);
		fresnelMetalShader->setInt("cubetex", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mainCubeMap->textureID);
		PF_METAL->Draw(*fresnelMetalShader);
	}
	glUseProgram(0);

	{
		fresnelVidrioShader->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		fresnelVidrioShader->setMat4("projection", projection);
		fresnelVidrioShader->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		fresnelVidrioShader->setMat4("model", model);
		fresnelVidrioShader->setVec3("cameraPosition", camera.Position);
		fresnelVidrioShader->setFloat("mRefractionRatio", 1.0f / 1.5f);
		fresnelVidrioShader->setFloat("_Bias", 0.5f);
		fresnelVidrioShader->setFloat("_Scale", 0.5f);
		fresnelVidrioShader->setFloat("_Power", 2.0f);
		fresnelVidrioShader->setInt("texture_diffuse1", 0);
		fresnelVidrioShader->setInt("cubetex", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mainCubeMap->textureID);
		PF_CRISTAL->Draw(*fresnelVidrioShader);
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
		model = glm::translate(model, glm::vec3(78.4547f, 2.60694f, 6.85619f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 48.704f);
		proceduralShader->setFloat("height", 0.0f);

		camion->Draw(*proceduralShader);
		proceduralTime += 0.001;

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
		model = glm::translate(model, glm::vec3(0.0f, 28.131f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 165.261f);
		proceduralShader->setFloat("height", 28.131f);

		nave->Draw(*proceduralShader);
		proceduralTime += 0.001;

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
		proceduralPlaneta->setFloat("a", 30.0f);
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
		proceduralPlaneta->setFloat("a", 300.0f);
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
		proceduralPlaneta->setFloat("a", 30.0f);
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
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralPlaneta->setMat4("model", model);

		proceduralPlaneta->setFloat("time", proceduralTime);
		proceduralPlaneta->setFloat("a", 300.0f);
		proceduralPlaneta->setFloat("m", 1.0f);

		planeta3->Draw(*proceduralPlaneta);
		proceduralTime += 0.00001;

	}

	glUseProgram(0);

	// SATELITE
	{
		proceduralSatelite->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);
		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("a", 2.0f);
		proceduralSatelite->setFloat("b", 2.0f);
		proceduralSatelite->setFloat("n", 10.0f);
		proceduralSatelite->setFloat("m", 4.0f);
		satelite0->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;
	}

	glUseProgram(0);

	// SATELITE 1
	{
		proceduralSatelite->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);
		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("a", 2.0f);
		proceduralSatelite->setFloat("b", 2.0f);
		proceduralSatelite->setFloat("n", 10.0f);
		proceduralSatelite->setFloat("m", 4.0f);
		satelite1->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;
	}

	glUseProgram(0);

	// SATELITE 2
	{
		proceduralSatelite->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);
		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("a", 2.0f);
		proceduralSatelite->setFloat("b", 2.0f);
		proceduralSatelite->setFloat("n", 10.0f);
		proceduralSatelite->setFloat("m", 5.0f);
		satelite2->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;
	}

	glUseProgram(0);

	// SATELITE 3
	{
		proceduralSatelite->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);
		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("a", 2.0f);
		proceduralSatelite->setFloat("b", 2.0f);
		proceduralSatelite->setFloat("n", 10.0f);
		proceduralSatelite->setFloat("m", 6.0f);
		satelite3->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;
	}

	glUseProgram(0);

	// SATELITE 4
	{
		proceduralSatelite->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		proceduralSatelite->setMat4("projection", projection);
		proceduralSatelite->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralSatelite->setMat4("model", model);
		proceduralSatelite->setFloat("time", proceduralTime);
		proceduralSatelite->setFloat("a", 2.0f);
		proceduralSatelite->setFloat("b", 2.0f);
		proceduralSatelite->setFloat("n", 10.0f);
		proceduralSatelite->setFloat("m", 7.0f);
		satelite4->Draw(*proceduralSatelite);
		proceduralTime += 0.00001;
	}

	glUseProgram(0);

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
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		wavesShader->setMat4("model", model);
		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 5.0f);
		wavesShader->setFloat("height", 5.0f);
		bandera->Draw(*wavesShader);
		wavesTime += 0.001;
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


	// --- Lógica de Colisiones ---
	{
		{
			// 1. Actualizar caja del personaje
			boxCharacter.min = position - glm::vec3(characterSize.x / 2.0f, 0.0f, characterSize.z / 2.0f);
			boxCharacter.max = position + glm::vec3(characterSize.x / 2.0f, characterSize.y, characterSize.z / 2.0f);

			bool colisionDetectada = false;

			// 2. Verificar contra TODOS los planetarios
			for (auto& p : obstaculos) {
				// Actualizamos la caja del planetario actual
				p.box.min = p.position - (p.size / 2.0f);
				p.box.max = p.position + (p.size / 2.0f);

				// Comprobar colisión
				if (boxCharacter.checkCollision(p.box)) {
					colisionDetectada = true;
					break; // Si choca con uno, no hace falta revisar los demás
				}
			}

			// 3. Respuesta a la colisión
			if (colisionDetectada) {
				position = oldPosition; // Revertir posición

				// Actualizar cámara de tercera persona
				camera3rd.Position = position;
				camera3rd.Position.y += 1.7f;
				camera3rd.Position -= trdpersonOffset * forwardView;
			}
		}
	}

	glUseProgram(0);


	//Dibujar colisiones
	{
		if (showHitbox) {
			glm::mat4 projection_dbg;
			glm::mat4 view_dbg;

			if (activeCamera == 1) {
				projection_dbg = glm::perspective(glm::radians(camera.Zoom),
					(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
				view_dbg = camera.GetViewMatrix();
			}
			if (activeCamera == 0) {
				projection_dbg = glm::perspective(glm::radians(camera3rd.Zoom),
					(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
				view_dbg = camera3rd.GetViewMatrix();
			}
			if (activeCamera == 2) {
				projection_dbg = glm::perspective(glm::radians(cameraCohete.Zoom),
					(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
				view_dbg = cameraCohete.GetViewMatrix();
			}
			if (activeCamera == 3) {
				projection_dbg = glm::perspective(glm::radians(cameraPlanetario.Zoom),
					(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
				view_dbg = cameraPlanetario.GetViewMatrix();
			}

			// Cajas de obstáculos (amarillo)
			for (auto& obs : obstaculos) {
				BoundingBox b;
				b.min = obs.position - (obs.size / 2.0f);
				b.max = obs.position + (obs.size / 2.0f);
				DrawBoundingBox(b, glm::vec3(1.0f, 1.0f, 0.0f), view_dbg, projection_dbg);
			}

			// Coloca esto en tu función Update() si usaste el método directo de Min/Max
			for (auto& obs : zonasInteraccion) {
				// Si el jugador está dentro es verde, si no, amarilla
				glm::vec3 colorCaja = obs.isPlayerInside ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 1.0f, 0.0f);

				// Pasamos directamente la caja que ya vive dentro del objeto
				DrawBoundingBox(obs.box, colorCaja, view, projection);
			}

			// Caja del personaje (rojo)
			DrawBoundingBox(boxCharacter, glm::vec3(1.0f, 0.0f, 0.0f), view_dbg, projection_dbg);
		}
		glUseProgram(0);
	}

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}
// Procesamos entradas del teclado
void processInput(GLFWwindow* window) {
	static bool keyPressed = false;
	static bool keyPressed2 = false;
	personajeMoviendose = false;


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Bloque de  camara 
	if (camaraCoheteActiva) {
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			camaraCoheteActiva = false;
			activeCamera = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			cohete_height += 0.05f;
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) supernova_rotation += 1.f;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) supernova_rotation -= 1.f;

		return;
	}


	// Controles de cámara primera persona
	if (activeCamera == 1) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}


	//Animacion cohete
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		for (size_t i = 0; i < zonasInteraccion.size(); i++) {
			if (zonasInteraccion[i].isPlayerInside) {

				if (zonasInteraccion[i].zoneID == 1) {
					camaraCoheteActiva = true;
					activeCamera = 3;
				}

				if (zonasInteraccion[i].zoneID == 2) {
					// 1. Activamos banderas de estado
					camaraCoheteActiva = true;
					activeCamera = 2;

					// 2. Teletransportamos la posición base del astronauta
					position = glm::vec3(73.0513f, 0.0f, -70.3005f);

					// 4. Actualizamos la cámara interna (1ra persona) por si acaso
					camera.Position = position;
					camera.Position.y += 1.7f;
				}

				break;
			}
		}

	}

	// Controles de la excavadora, supernova y meteorito
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		glm::vec3 lightPos = glm::vec3(gLights[excaLightIndex].Position);
		anguloexcavadora = glm::radians(1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, pivoteEXC1);
		transformation = glm::rotate(transformation, anguloexcavadora, glm::vec3(0.0f, 1.0f, 0.0f));
		transformation = glm::translate(transformation, -pivoteEXC1);
		lightPos = glm::vec3(transformation * glm::vec4(lightPos, 1.0f));
		gLights[excaLightIndex].Position = lightPos;
		excavadora_rotation += 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		glm::vec3 lightPos = glm::vec3(gLights[excaLightIndex].Position);
		anguloexcavadora = glm::radians(-1.0f);
		glm::mat4 transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, pivoteEXC1);
		transformation = glm::rotate(transformation, anguloexcavadora, glm::vec3(0.0f, 1.0f, 0.0f));
		transformation = glm::translate(transformation, -pivoteEXC1);
		lightPos = glm::vec3(transformation * glm::vec4(lightPos, 1.0f));
		gLights[excaLightIndex].Position = lightPos;
		excavadora_rotation -= 1.f;
	}


	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !keyPressed) {
		meteor_offset -= 1.0f;
		meteor_size -= 0.01f;
		gLights[meteorLightIndex].Position.x -= 1.0f;
		gLights[meteorLightIndex].Position.y += 1.0f;
		keyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
		keyPressed = false;
	}

	// Constant speed definitions
	float movementSpeed = 15.0f;
	float turnSpeed = 60.0f;

	// Movimiento del personaje (Flechas)
	if (activeCamera == 0) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			personajeMoviendose = true;
			position = position + (movementSpeed * deltaTime) * forwardView;
			camera3rd.Front = forwardView;
			camera3rd.ProcessKeyboard(FORWARD, deltaTime);
			camera3rd.Position = position;
			camera3rd.Position.y += 1.7f;
			camera3rd.Position -= trdpersonOffset * forwardView;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			personajeMoviendose = true;
			position = position - (movementSpeed * deltaTime) * forwardView;
			camera3rd.Front = forwardView;
			camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
			camera3rd.Position = position;
			camera3rd.Position.y += 1.7f;
			camera3rd.Position -= trdpersonOffset * forwardView;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			personajeMoviendose = true;
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
			personajeMoviendose = true;
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
	}
	// Selectores manuales de cámara
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) activeCamera = 0;
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) activeCamera = 1;
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) activeCamera = 2;
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) activeCamera = 3;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !keyPressed2) {
		showHitbox = !showHitbox;
		keyPressed2 = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		keyPressed2 = false;
	}
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

	if (activeCamera == 0) {
		if (!personajeMoviendose) { // Primera persona
			camera3rd.ProcessMouseMovement(xoffset, yoffset);
		}
	}
	if (activeCamera == 1) {
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}