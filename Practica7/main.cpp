/*
PROYECTO FINAL: CGEIHC 2026-2
Integrantes: Armando Adair Núñez Badillo & Itzel Hernández Hernández
Temática: Hora de Aventura, Machinarium, Alicia en el País de las Maravillas, Steampunk
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

// Iluminación y Materiales
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

// Texturas Base
Texture pisoTexture;

// Modelos Base
Model Mi_lamparita;
Model Pasto;
// === MODELOS DEL PROYECTO FINAL ===
// Alicia
Model Casa_Alicia, Comedor_Alicia, Puerta_Alicia, Taza_Alicia, Hongo, Sombrero;
Model Mesa_Alicia;
// Machinarium / Steampunk
Model Torre_Reloj, Reloj_Principal, Engranajes, Gear1, Tuberias, Torre_Agua, LightHouse, Pilares;
// Hora de Aventura & Extras
Model Pico_Helado, Banca;
//josef machinarium
Model cuerpo;
Model brazo;
Model pierna;
//finn
Model cuerpoFinn;
Model brazoFinn;
Model pieIzqFinn;
Model pieDerFinn;
//hongo que brilla
Model hongo1;
//tetera
Model plato;
Model taza;
Model tetera;
//personaje npc de mach
Model npcMachinarium;
//npc de bmo
Model bmo;
//zeppelin
Model zeppelin;
Model aspas;
//Trenecito
Model via_tren, carro_tren;
//Locomotora
Model locomotora;
Model planoHumo;
Texture texturaHumo;


Skybox skybox;
Skybox skyboxNoche;

Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//MOVIMIENTOS DE JOSEF
// Posición y rotación 
float posRobotX = 0.0f;
float posRobotZ = 0.0f;
float giroRobot = 0.0f;
// Animación de extremidades
float oscilacion = 0.0f;
float velocidadAnimacion = 20.0f;//movimiento del brazo, rapidez
float amplitudArticulacion = 30.0f; // Máximo ángulo de rotación 
bool caminando = false;

//PARA LA LUZ DEL HONGUITO QUE BRILLA
float intensidadHongo = 1.0f;
bool hongoPrendido = true;

//PARA LA TETERA
float inclinacionTetera = 0.0f;

//PARA EL ZEPPELIN
float rotZeppelin = 0.0f;
float rotAspas = 0.0f;

//PARA EL CARRO EN LAS VIAS
float movCarrito = 0.0f;   
static bool trenActivo = false;
static bool teclaTPresionada = false;

//PARA LA LOCOMOTORA
float movHumoY = 0.0f;    
float escalaHumo = 1.0f;   
static bool locoActiva = false;
static bool teclaLPresionada = false;





// Luces
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

// Función para normales
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() {
	// Plano para el piso
	unsigned int floorIndices[] = { 0, 2, 1, 1, 2, 3 };
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3); // meshList[0] ahora será el piso
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	// =======================================================
	// Carga de Textura del Piso (Empedrado)
	// =======================================================
	// Asegúrate de que esta ruta también apunte a donde tienes la imagen del piso
	pisoTexture = Texture("TexturasProject/skybox/textura_piso.png");
	pisoTexture.LoadTextureA();

	// Cargar Modelos
	Mi_lamparita = Model();
	Mi_lamparita.LoadModel("ModelosProject/Mi_lamparita.obj");


	// == = CARGA DE MODELOS DEL PROYECTO == =
		// Temática Alicia
	Puerta_Alicia = Model();   Puerta_Alicia.LoadModel("ModelosProject/puerta_alicia.obj");
	Taza_Alicia = Model();     Taza_Alicia.LoadModel("ModelosProject/Taza_alicia.obj");
	Hongo = Model();           Hongo.LoadModel("ModelosProject/hongo.obj");
	Sombrero = Model();		   Sombrero.LoadModel("ModelosProject/sombrero.obj");
	Mesa_Alicia = Model();         Mesa_Alicia.LoadModel("ModelosProject/comedor.obj");
	//hongo que brilla
	hongo1 = Model();
	hongo1.LoadModel("ModelosProject/hongo1.obj");
	//tetera
	plato = Model();
	plato.LoadModel("ModelosProject/plato.obj");
	taza = Model();
	taza.LoadModel("ModelosProject/taza.obj");
	tetera = Model();
	tetera.LoadModel("ModelosProject/tetera.obj");

	// Temática Steampunk / Machinarium
	Torre_Reloj = Model();     Torre_Reloj.LoadModel("ModelosProject/Torre_reloj.obj");
	Reloj_Principal = Model(); Reloj_Principal.LoadModel("ModelosProject/Reloj_principal.obj");
	Engranajes = Model();      Engranajes.LoadModel("ModelosProject/engranajes_extras.obj");
	Gear1 = Model();           Gear1.LoadModel("ModelosProject/gear1.obj");
	Tuberias = Model();        Tuberias.LoadModel("ModelosProject/tuberias.obj");
	Torre_Agua = Model();      Torre_Agua.LoadModel("ModelosProject/torre_agua.obj");
	LightHouse = Model();      LightHouse.LoadModel("ModelosProject/light_house.obj");
	Pilares = Model();         Pilares.LoadModel("ModelosProject/pilares.obj");
	Pasto = Model();           Pasto.LoadModel("ModelosProject/pasto.obj");
	//josef
	cuerpo = Model();
	cuerpo.LoadModel("ModelosProject/cuerpo.obj");
	brazo = Model();
	brazo.LoadModel("ModelosProject/brazo.obj");
	pierna = Model();
	pierna.LoadModel("ModelosProject/pierna.obj");
	//npc machinarium
	npcMachinarium = Model();
	npcMachinarium.LoadModel("ModelosProject/secundario_mach.obj");
	//zeppelin
	zeppelin = Model();
	zeppelin.LoadModel("ModelosProject/zeppelin.obj");
	aspas = Model();
	aspas.LoadModel("ModelosProject/aspas.obj");
	//mini carrito en vias de tren
	via_tren = Model();
	via_tren.LoadModel("ModelosProject/via_tren.obj");
	carro_tren = Model();
	carro_tren.LoadModel("ModelosProject/carro_tren.obj");
	// Locomotora
	locomotora = Model();
	locomotora.LoadModel("ModelosProject/locomotora.obj");

	// Humo 
	planoHumo = Model();
	planoHumo.LoadModel("ModelosProject/plano.obj"); 
	texturaHumo = Texture("TexturasProject/humote.png");
	texturaHumo.LoadTextureA(); 

	// Temática Hora de Aventura & Extras
	Pico_Helado = Model();     Pico_Helado.LoadModel("ModelosProject/PicoHelado.obj");
	Banca = Model();           Banca.LoadModel("ModelosProject/banca.obj");
	//finn
	cuerpoFinn.LoadModel("ModelosProject/cuerpo_finn.obj");
	brazoFinn.LoadModel("ModelosProject/brazo_finn.obj");
	pieIzqFinn.LoadModel("ModelosProject/pie_izquierdo_finn.obj");
	pieDerFinn.LoadModel("ModelosProject/pie_derecho_finn.obj");
	//bmo adventure
	bmo = Model();
	bmo.LoadModel("ModelosProject/bmo.obj");
	
	
	
	
	
	

	// =======================================================
	// CONFIGURACIÓN DE SKYBOX (Alicia en el País de las Maravillas)
	// =======================================================
	std::vector<std::string> skyboxFaces;
	// ¡Rutas corregidas basándome en tu carpeta exacta!
	skyboxFaces.push_back("TexturasProject/skybox/textura_pared_hongos.png"); // Right
	skyboxFaces.push_back("TexturasProject/skybox/textura_pared_hongos.png"); // Left
	skyboxFaces.push_back("TexturasProject/skybox/textura_cielo.png");        // Up (Cielo)
	skyboxFaces.push_back("TexturasProject/skybox/textura_cielo.png");         // Down (Piso)
	skyboxFaces.push_back("TexturasProject/skybox/textura_pared_reloj.png");  // Back
	skyboxFaces.push_back("TexturasProject/skybox/textura_pared_farolas.png");// Front
	skybox = Skybox(skyboxFaces);

	// =======================================================
	// CONFIGURACIÓN DE SKYBOX NOCTURNO
	// =======================================================
	std::vector<std::string> skyboxNocheFaces;
	// Cambia estos nombres por los de las imágenes de noche que descargues
	skyboxNocheFaces.push_back("TexturasProject/skybox/noche_estrellada.png"); // Right
	skyboxNocheFaces.push_back("TexturasProject/skybox/noche_estrellada.png");  // Left
	skyboxNocheFaces.push_back("TexturasProject/skybox/noche_estrellada.png");   // Up (Cielo estrellado)
	skyboxNocheFaces.push_back("TexturasProject/skybox/noche_estrellada.png");  // Down (Piso)
	skyboxNocheFaces.push_back("TexturasProject/skybox/noche_estrellada.png");  // Back
	skyboxNocheFaces.push_back("TexturasProject/skybox/noche_estrellada.png"); // Front
	skyboxNoche = Skybox(skyboxNocheFaces);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// =======================================================
	// ILUMINACIÓN INICIAL
	// =======================================================
	// 1. Luz Direccional (El Sol)
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.3f, 0.3f, 0.0f, -1.0f, -1.0f);

	// 2. Linterna de la cámara (SpotLight 0)
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f);
	spotLightCount++;

	// 3. Spotlight Rotatorio del Faro (SpotLight 1)
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 30.0f,                 // INTENSIDAD: Subimos a 30.0f para más fuerza
		-235.0f, 50.0f, 0.0f,
		1.0f, -0.5f, 0.0f,
		1.0f, 0.0005f, 0.00005f,     // RANGO: Le metimos más ceros para que viaje más lejos
		60.0f);                      // CONO: Abierto a 45 grados (antes era 20)
	spotLightCount++;

	// 3. Luz de la Farola (PointLight 0)
	glm::vec3 posFarola = glm::vec3(-15.0f, 0.0f, 220.0f);
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, 0.0f, 5.0f, posFarola.x + 2.4f, posFarola.y + 7.6f, posFarola.z - 2.4f, 5.0f, 0.05f, 0.012f);
	pointLightCount++;

	// 4. Luz del Faro (PointLight 1)
	pointLights[1] = PointLight(1.0f, 0.95f, 0.8f,
		0.2f,   // AMBIENTAL en 0.0f: Ya no iluminará mágicamente las espaldas de los modelos.
		1.5f,   // DIFUSA en 0.5f: Un fogonazo súper débil (antes estaba en 2.0 y 10.0).
		-235.0f, 50.0f, 0.0f,
		1.0f, 0.001f, 0.002f); // ATENUACIÓN: Aumenté estos dos últimos números para que el radio de la luz sea mucho más corto.
	pointLightCount++;

	// Luz de la lampara (itzel)
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 2.0f,
		12.0f, 2.0f, 4.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//luz del hongo
	pointLights[3] = PointLight(0.8f, 0.0f, 1.0f,
		0.2f, 3.0f,       // Intensidad ambiental y difusa
		-5.0f, 5.0f, 2.0f, // Posición (ajustar dependiendo de donde se poga el hongo en el main oficial)
		0.3f, 0.01f, 0.002f); // Atenuación
	pointLightCount++;

	// luz del segundo honguito
	pointLights[4] = PointLight(0.8f, 0.0f, 1.0f,
		0.2f, 3.0f,
		-60.0f, 5.0f, 100.0f,
		0.3f, 0.01f, 0.002f);
	pointLightCount++; 


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Rotacion y movimiento del sombrero de copa

	float movSombrero = 0.0f;
	float rotSombrero = 0.0f;

	// Variables para la animación del Engranaje (Tik-Tak)
	bool animacionGearActiva = false;
	bool teclaGPresionada = false;
	bool teclaHPresionada = false;
	float rotGearActual = 0.0f;
	float rotGearObjetivo = 0.0f;
	float tiempoUltimoPaso = 0.0f;

	//Variables para el faro
	// Variables para el control del Faro (Tecla P)
	bool faroEncendido = true;
	bool teclaPPresionada = false;

	//// =======================================================
	// Variables para el Sistema de Cámaras (4 Modos)
	// =======================================================
	int modoCamara = 1;
	bool tecla1Presionada = false;
	bool tecla2Presionada = false;
	bool tecla3Presionada = false;
	bool tecla4Presionada = false;
	float cam3X = 0.0f;
	float cam3Y = 250.0f;
	float cam3Z = 0.0f;

	//// Variables para el Spotlight del Faro
	bool spotFaroEncendido = true;
	bool teclaFPresionada = false;


	// BUCLE PRINCIPAL
	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//AQUI VOY A PONER LO RELACIONADO CON VARIABLES DE LAS ANIMACIONES
		///PARA EL HONGO QUE BRILLA
		glm::vec3 posHongo = glm::vec3(60.0f, -0.5f, 150.0f);
		pointLights[3].SetPos(glm::vec3(posHongo.x, posHongo.y + 0.5f, posHongo.z));
		//PARA EL SEGUNDO HONGO
		// Actualizar el segundo hongo
		glm::vec3 posHongo2 = glm::vec3(-60.0f, -1.0f, 100.0f);
		pointLights[4].SetPos(glm::vec3(posHongo2.x, posHongo2.y + 6.0f, posHongo2.z)); 
		pointLights[4].SetDiffuseIntensity(intensidadHongo);

		//PARA LA TETERA
		// posiciones de las 4 tazas sobre el plato
		glm::vec3 posTazas[] = {
			glm::vec3(-0.2f, 0.1f, 0.2f),  //frontal izq
			glm::vec3(0.2f, 0.1f, 0.2f),   //(frontal der
			glm::vec3(0.2f, 0.1f, -0.2f),  //trasera der
			glm::vec3(-0.2f, 0.1f, -0.2f)  //trasera izq
		};

		float movTeteraX = 0.0f;
		float movTeteraZ = 0.0f;

		//PARA EL ZEPPELIN
		float movZepX = 0.0f;
		float movZepZ = 0.0f;
		float anguloZep = 0.0f;
		rotAspas += 500.0f * deltaTime;
		if (rotAspas >= 360.0f) rotAspas = 0.0f;


		////////////////////////////////////////////
		// MOVIMIENTO DE JOSEF
		////////////////////////////////////////////
		caminando = false;
		//se ajusta la velocidad
		float velocidadCaminar = 0.5f * deltaTime;
		float velocidadGiro = 1.2f * deltaTime;
		// Flecha ARRIBA: Avanzar
		if (mainWindow.getsKeys()[GLFW_KEY_UP]) {
			posRobotX += sin(giroRobot * toRadians) * velocidadCaminar;
			posRobotZ += cos(giroRobot * toRadians) * velocidadCaminar;
			caminando = true;
		}
		// Flecha ABAJO: Retroceder
		if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) {
			posRobotX -= sin(giroRobot * toRadians) * velocidadCaminar;
			posRobotZ -= cos(giroRobot * toRadians) * velocidadCaminar;
			caminando = true;
		}
		// Flecha IZQUIERDA: Girar sobre su propio eje
		if (mainWindow.getsKeys()[GLFW_KEY_LEFT]) {
			giroRobot += velocidadGiro;
		}
		// Flecha DERECHA: Girar hacia el otro lado
		if (mainWindow.getsKeys()[GLFW_KEY_RIGHT]) {
			giroRobot -= velocidadGiro;
		}
		if (caminando) {
			oscilacion = sin(glfwGetTime() * 6.0f) * 25.0f;
		}
		else {
			oscilacion = 0.0f;
		}




		//glfwPollEvents();
		//Camara anterior
		//camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		glfwPollEvents();





		// =======================================================
		// CONTROLES DE LUCES (TECLADO)
		// =======================================================
		// Boton OFF/ON para la lamparita (Tecla O)
		static bool lamparaEncendida = true;
		static bool teclaOPresionada = false;

		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			if (!teclaOPresionada) {
				lamparaEncendida = !lamparaEncendida;
				teclaOPresionada = true;
			}
		}
		else {
			teclaOPresionada = false;
		}

		if (lamparaEncendida) {
			
			pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, 0.0f, 5.0f, posFarola.x + 2.4f, posFarola.y + 7.6f, posFarola.z - 2.4f, 5.0f, 0.05f, 0.012f);
		}
		else {
			
			pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, posFarola.x + 2.4f, posFarola.y + 7.6f, posFarola.z - 2.4f, 5.0f, 0.05f, 0.012f);
		}

		// =======================================================
		// Botón OFF/ON para el Faro (Tecla P)
		// =======================================================
		if (mainWindow.getsKeys()[GLFW_KEY_P]) {
			if (!teclaPPresionada) {
				faroEncendido = !faroEncendido; // Cambia el estado
				teclaPPresionada = true;
			}
		}
		else {
			teclaPPresionada = false;
		}

		// Actualizamos la luz del Faro (PointLight 1) en cada fotograma
		if (faroEncendido) {
			// INTENSO: Ambiental a 1.0f, Difusa a 15.0f, y Exponencial bajísimo (0.0001f) para que llegue al piso
			pointLights[1] = PointLight(1.0f, 0.95f, 0.8f,
				1.0f, 15.0f,
				-235.0f, 50.0f, 0.0f,
				1.0f, 0.002f, 0.0001f);
		}
		else {
			// APAGADO: Multiplicadores de luz en 0.0f
			pointLights[1] = PointLight(1.0f, 0.95f, 0.8f,
				0.0f, 0.0f,
				-235.0f, 50.0f, 0.0f,
				1.0f, 0.002f, 0.0001f);
		}

		// =======================================================
		// Botón OFF/ON para el Spotlight Rotatorio (Tecla F)
		// =======================================================
		if (mainWindow.getsKeys()[GLFW_KEY_F]) {
			if (!teclaFPresionada) {
				spotFaroEncendido = !spotFaroEncendido;
				teclaFPresionada = true;
			}
		}
		else {
			teclaFPresionada = false;
		}

		// Matemática de rotación del cono de luz
		if (spotFaroEncendido) {
			float velocidadGiro = 1.5f;
			float anguloFaro = now * velocidadGiro;

			// Mantenemos el -0.5f para que siga apuntando en diagonal hacia el piso
			glm::vec3 dirFaro = glm::vec3(sin(anguloFaro), -0.5f, cos(anguloFaro));

			spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
				0.0f, 30.0f,            // Más fuerza
				-235.0f, 50.0f, 0.0f,
				dirFaro.x, dirFaro.y, dirFaro.z,
				1.0f, 0.0005f, 0.00005f, // Más rango
				60.0f);                  // Cono abierto
		}
		else {
			// Apagado completo
			spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -235.0f, 50.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0005f, 0.00005f, 60.0f);
		}



		// =======================================================
		// CICLO DE DÍA Y NOCHE (60 Segundos)
		// =======================================================
		float tiempoGlobal = fmod(now, 60.0f);
		bool esNoche = (tiempoGlobal >= 30.0f); // Del 0 al 29 es Día, del 30 al 59 es Noche

		if (esNoche) {
			// 1. LUNA: Bajamos la intensidad de la Directional Light y le damos un tono azulado oscuro
			mainLight = DirectionalLight(0.2f, 0.2f, 0.4f, 0.1f, 0.1f, 0.0f, -1.0f, -1.0f);

			// 2. ENCENDIDO AUTOMÁTICO DE LUMINARIAS
			lamparaEncendida = true;
			//faroEncendido = true; Esta linea la comento para que podamos apagar y prender el faro de noche 
			spotFaroEncendido = true;
		}
		else {
			// 1. SOL A MÁXIMA POTENCIA: 
			// Subimos la ambiental a 0.6f para aclarar las sombras 
			// y la difusa a 1.0f para que pegue con todo el brillo.
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.6f, 1.0f, 0.0f, -1.0f, -1.0f);

			// 2. APAGADO AUTOMÁTICO DE LUMINARIAS
			lamparaEncendida = false;
			faroEncendido = false;
			spotFaroEncendido = false;
		}

		//ANIMACIONES BASICAS:
		
		

		// =======================================================
		// Control del Engranaje (G Inicia / H Detiene)
		// =======================================================

		// Tecla G: INICIAR
		if (mainWindow.getsKeys()[GLFW_KEY_G]) {
			if (!teclaGPresionada) {
				animacionGearActiva = !animacionGearActiva; // Activa/Desactiva

				// Sincronizamos el reloj exactamente al momento de encenderlo
				// para que no intente dar giros acumulados del pasado.
				if (animacionGearActiva) {
					tiempoUltimoPaso = now;
				}
				animacionGearActiva = true;
				tiempoUltimoPaso = now; // Sincronizamos el reloj para que no se vuelva loco
				teclaGPresionada = true;
			}
		}
		else {
			teclaGPresionada = false;
		}

		// Tecla H: DETENER
		if (mainWindow.getsKeys()[GLFW_KEY_H]) {
			if (!teclaHPresionada) {
				animacionGearActiva = false;
				teclaHPresionada = true;
			}
		}
		else {
			teclaHPresionada = false;
		}

		// Lógica del "Tik-Tak"
		if (animacionGearActiva) {
			// Se espera exactamente 1.5 segundos
			if (now - tiempoUltimoPaso > 1.5f) {
				rotGearObjetivo += 45.0f; // Los grados que avanza en cada "Tak"
				tiempoUltimoPaso = now;   // Reinicia el reloj
			}
		}

		// El multiplicador en 2.0f hace que el movimiento sea súper pesado, lento y robusto
		rotGearActual += (rotGearObjetivo - rotGearActual) * 2.0f * deltaTime;
		
		////////////////////////////////////////
		// CARRITO EN LAS VIAS (Tecla T)
		//////////////////////////////////////////
		// --- control del tren (TECLA T) ---
		if (mainWindow.getsKeys()[GLFW_KEY_T]) {
			if (!teclaTPresionada) {
				trenActivo = !trenActivo;
				teclaTPresionada = true;
			}
		}
		else {
			teclaTPresionada = false;
		}

		rotGearActual += (rotGearObjetivo - rotGearActual) * 1.5f * deltaTime;
		if (trenActivo) {
			movCarrito += 5.0f * deltaTime; 
			if (movCarrito > 150.0f) movCarrito = -150.0f; 
		}
		
		// LOCOMOTORA (LETRA L)
		if (mainWindow.getsKeys()[GLFW_KEY_L]) {
			if (!teclaLPresionada) {
				locoActiva = !locoActiva;
				teclaLPresionada = true;
			}
		}
		else {
			teclaLPresionada = false;
		}

		//animacion humo
		if (locoActiva) {
			movHumoY += 2.0f * deltaTime;    
			escalaHumo += 1.5f * deltaTime; 

			if (movHumoY > 12.0f) { 
				movHumoY = 0.0f;
				escalaHumo = 1.0f;
			}
		}

		//ANIMACIONES COMPLEJAS:
		// 
		// 
		//Animacion SOMBRERO
		float tiempoCiclo = fmod(glfwGetTime(), 4.0f);
		float movSombrero = 0.0f;
		float rotSombrero = 0.0f;

		if (tiempoCiclo < 1.0f) {
			float progreso = tiempoCiclo;
			movSombrero = progreso * 15.0f;
			rotSombrero = 0.0f;
		}
		else if (tiempoCiclo < 2.0f) {
			float progreso = tiempoCiclo - 1.0f;
			movSombrero = 15.0f;
			rotSombrero = progreso * 85.0f;
		}
		else if (tiempoCiclo < 3.0f) {
			float progreso = tiempoCiclo - 2.0f;
			movSombrero = 15.0f;
			rotSombrero = 85.0f - (progreso * 85.0f);
		}
		else {
			float progreso = tiempoCiclo - 3.0f;
			movSombrero = 15.0f - (progreso * 15.0f);
			rotSombrero = 0.0f;
		}
		//ANIMACION COMPLEJA DEL HONGO QUE BRILLA
		intensidadHongo = 3.0f + (sin(glfwGetTime() * 3.0f) * 2.0f);
		pointLights[3].SetDiffuseIntensity(intensidadHongo);

		//ANIMACION COMPLEJA DE LA TETERA QUE SIRVA
		float tiempoTotal = glfwGetTime();
		float duracionCiclo = 5.0f; // cada taza toma 5 seg
		int indiceActual = (int)(tiempoTotal / duracionCiclo) % 4;
		int indiceSiguiente = (indiceActual + 1) % 4;
		float progreso = fmod(tiempoTotal, duracionCiclo);

		if (progreso < 2.0f) {
			float t = progreso / 2.0f;
			movTeteraX = posTazas[indiceActual].x + (posTazas[indiceSiguiente].x - posTazas[indiceActual].x) * t;
			movTeteraZ = posTazas[indiceActual].z + (posTazas[indiceSiguiente].z - posTazas[indiceActual].z) * t;
			inclinacionTetera = 0.0f;
		}
		else {
			movTeteraX = posTazas[indiceSiguiente].x;
			movTeteraZ = posTazas[indiceSiguiente].z;

			if (progreso > 3.0f && progreso < 4.0f) {
				float tServido = (progreso - 3.0f);
				inclinacionTetera = sin(3.14159f * tServido) * 60.0f;
			}
			else {
				inclinacionTetera = 0.0f;
			}
		}

		//ANIMACION COMPLEJA PARA EL ZEPPELIN
		float tiempoZep = glfwGetTime() * 0.3f;
		float radioZep = 200.0f; // qué tan grande es el círculo
		movZepX = radioZep * sin(tiempoZep);
		movZepZ = radioZep * cos(tiempoZep);
		anguloZep = atan2(sin(tiempoZep), cos(tiempoZep)) * (180.0f / 3.14159f);
		float flotadoY = sin(glfwGetTime() * 0.8f) * 0.5f;
		rotAspas += 600.0f * deltaTime;








		// =======================================================
		// RENDERIZADO
		// =======================================================
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		// =======================================================
		// RENDERIZADO DEL SKYBOX (Día o Noche)
		// =======================================================
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (esNoche) {
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}



		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Actualizar linterna de cámara
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Mandar luces al shader
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color(1.0f, 1.0f, 1.0f);

		// 1. DIBUJAR PISO BASE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh(); // meshList[0] es el piso en el código limpio

		// 2. DIBUJAR FAROLA
		model = glm::mat4(1.0);
		model = glm::translate(model, posFarola);
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mi_lamparita.RenderModel();




		// =========================================================
		// ZONA 1: ALICIA EN EL PAÍS DE LAS MARAVILLAS (Esquina X+, Z+)
		// =========================================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.0f, 5.0f));
		// model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Descomenta si necesitas hacerlos más grandes/chicos
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Casa_Alicia.RenderModel();



		//SECCION HONGOS 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -1.0f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hongo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -1.0f, 200.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hongo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -1.0f, 250.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hongo.RenderModel();

		//hacemos otro hongo brilloso
		posHongo2 = glm::vec3(-60.0f, -1.0f, 100.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, posHongo2);
		model = glm::scale(model, glm::vec3(10.0f, 14.0f, 10.0f));
		if (intensidadHongo > 0.1f) {
			color = glm::vec3(1.0f, 0.5f, 1.0f);
		}
		else {
			color = glm::vec3(0.3f, 0.3f, 0.3f);
		}

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hongo1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -1.0f, 150.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hongo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -1.0f, 200.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hongo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -1.0f, 250.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hongo.RenderModel();

		//pasto entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 150.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pasto.RenderModel();

		//pasto derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pasto.RenderModel();


		//pasto izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pasto.RenderModel();


		//pasto trasero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -150.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pasto.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_Alicia.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -1.0f, 25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Taza_Alicia.RenderModel();

		// =========================================================
		// ZONA 2: MACHINARIUM / STEAMPUNK (Esquina X-, Z-)
		// =========================================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 11.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Torre_Reloj.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

		// APLICAMOS LA ROTACIÓN EN Y
		model = glm::rotate(model, glm::radians(rotGearActual), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gear1.RenderModel();

		model = glm::mat4(1.0);
		// El engranaje y reloj principal los pongo cerca de la torre
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -110.0f));
		// APLICAMOS LA ROTACIÓN EN EL EJE Y
		model = glm::rotate(model, glm::radians(rotGearActual), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gear1.RenderModel();
		
	
		//Engranajes pequenos decorativos 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 1.0f, 150.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranajes.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 7.0f, -200.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranajes.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 7.0f, 220.0f));
		model = glm::scale(model, glm::vec3(4.0f, 8.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranajes.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -1.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reloj_Principal.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 0.0f, -120.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Torre_Agua.RenderModel();

		// =========================================================
		// Faro
		// =========================================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LightHouse.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -1.0f, 100.0f));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tuberias.RenderModel();
		//Pilar Dderecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.0f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		Pilares.RenderModel();

		//Pilar izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -1.0f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		Pilares.RenderModel();
		//Pilar TRASERO IZQ
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -1.0f, -45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		Pilares.RenderModel();
		//PILAR TRASERO DERECHO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.0f, -45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		Pilares.RenderModel();



		//YA arregle 
		// =========================================================
		// ZONA 3: HORA DE AVENTURA & EXTRAS (Esquina X-, Z+)
		// =========================================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -5.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pico_Helado.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, -1.0f, 55.0f)); // La banca cerquita del centro
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca.RenderModel();


		//sombrero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f + movSombrero + 15.0f, -100.0f));
		model = glm::rotate(model, glm::radians(rotSombrero), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sombrero.RenderModel();


		////////////////////////////////////////////////
		// JOSEF
		// ////////////////////////////////////////////////
		
		//base
		glm::mat4 modelRobotBase = glm::mat4(1.0);
		modelRobotBase = glm::translate(modelRobotBase, glm::vec3(posRobotX+0.0f, 5.0f, posRobotZ+230.0f));
		modelRobotBase = glm::rotate(modelRobotBase, giroRobot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelRobotBase = glm::scale(modelRobotBase, glm::vec3(10.0f, 10.0f, 10.0f));
		// cuerpo
		model = modelRobotBase;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo.RenderModel();
		// brazo izq
		modelaux = modelRobotBase;
		modelaux = glm::translate(modelaux, glm::vec3(-0.25f, 0.4f, 0.0f));
		modelaux = glm::rotate(modelaux, oscilacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		brazo.RenderModel();
		//brazo derecho
		modelaux = modelRobotBase;
		modelaux = glm::translate(modelaux, glm::vec3(0.25f, 0.4f, 0.0f));
		modelaux = glm::rotate(modelaux, -oscilacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		brazo.RenderModel();
		// pierna izq
		modelaux = modelRobotBase;
		modelaux = glm::translate(modelaux, glm::vec3(-0.1f, -0.33f, -0.03f));
		modelaux = glm::rotate(modelaux, -oscilacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		pierna.RenderModel();
		// pierna derecha
		modelaux = modelRobotBase;
		modelaux = glm::translate(modelaux, glm::vec3(0.1f, -0.33f, -0.03f));
		modelaux = glm::rotate(modelaux, oscilacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		pierna.RenderModel();

		///////////////////////////////////////////
		// FINN
		//////////////////////////////////////////
		glm::mat4 modelFinnBase = glm::mat4(1.0f);
		modelFinnBase = glm::translate(modelFinnBase, glm::vec3(-100.0f, 8.4f, 60.0f));
		modelFinnBase = glm::rotate(modelFinnBase, 45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelFinnBase = glm::scale(modelFinnBase, glm::vec3(2.0f, 2.0f, 2.0f));
		// cuerpo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFinnBase));
		cuerpoFinn.RenderModel();
		// brazo izq
		modelaux = modelFinnBase;
		modelaux = glm::translate(modelaux, glm::vec3(0.7f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		brazoFinn.RenderModel();
		// brazo derecho
		modelaux = modelFinnBase;
		modelaux = glm::translate(modelaux, glm::vec3(-0.7f, 3.0f, 0.0f));
		modelaux = glm::scale(modelaux, glm::vec3(-1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		brazoFinn.RenderModel();
		// pie derecho
		modelaux = modelFinnBase;
		modelaux = glm::translate(modelaux, glm::vec3(1.15f, -3.7f, -0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		pieIzqFinn.RenderModel();
		// pie izq
		modelaux = modelFinnBase;
		modelaux = glm::translate(modelaux, glm::vec3(-1.3f, -3.4f, -0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		pieDerFinn.RenderModel();

		////////////////////////////////////////////
		// HONGO QUE BRILLA
		//////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, posHongo);
		model = glm::scale(model, glm::vec3(10.0f, 14.0f, 10.0f));
		if (intensidadHongo > 0.1f) {
			color = glm::vec3(1.0f, 0.5f, 1.0f);
		}
		else {
			color = glm::vec3(0.3f, 0.3f, 0.3f);
		}
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hongo1.RenderModel();


		//////////////////////////////////////////////////////
		// TETERA Y SUS TAZAS
		///////////////////////////////////////////////////
		//plato
		glm::mat4 modelPlato = glm::mat4(1.0f);
		modelPlato = glm::translate(modelPlato, glm::vec3(-145.0f, 12.0f, 105.0f));
		modelPlato = glm::scale(modelPlato, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPlato));
		plato.RenderModel();
		//tazas
		for (int i = 0; i < 4; i++) {
			modelaux = modelPlato;
			modelaux = glm::translate(modelaux, posTazas[i]);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
			taza.RenderModel();
		}
		//tetera
		modelaux = modelPlato;
		modelaux = glm::translate(modelaux, glm::vec3(movTeteraX + 0.3f, 0.5f, movTeteraZ));
		modelaux = glm::rotate(modelaux, inclinacionTetera * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		tetera.RenderModel();

		//COMEDOR ALICIA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, 0.0f, 120.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa_Alicia.RenderModel();


		/////////////////////////////////////////////////7
		// NPC MACHINARIUM
		////////////////////////////////////////////
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, -1.0f, -120.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		npcMachinarium.RenderModel();


		/////////////////////////////////////////////////////////////
		// NPC BMO
		///////////////////////////////////////////////////////////
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, 1.0f, 75.0f));
		model = glm::rotate(model, -30.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bmo.RenderModel();

		/////////////////////////////////////////////////////////////77
		// ZEPPELIN
		////////////////////////////////////////////////////////////
		glm::mat4 modelZep = glm::mat4(1.0f);
		modelZep = glm::translate(modelZep, glm::vec3(movZepX, 45.0f + flotadoY + 105.0f, movZepZ));
		modelZep = glm::rotate(modelZep, anguloZep * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelZep = glm::scale(modelZep, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelZep));
		zeppelin.RenderModel();
		//aspas
		modelaux = modelZep;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, -4.7f, -2.7f));
		modelaux = glm::rotate(modelaux, rotAspas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		aspas.RenderModel();
		modelaux = modelZep;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, -4.7f, 0.8f));
		modelaux = glm::rotate(modelaux, rotAspas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		aspas.RenderModel();

		//////////////////////////////////////////////////////////////////////
		// CARRO Y VIAS
		///////////////////////////////////////////////////////////////////
		// vias
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 1.5f, -190.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		via_tren.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, 1.5f, -190.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		via_tren.RenderModel();
		// carrito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCarrito, 1.7f, -192.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		carro_tren.RenderModel();


		// =======================================================
		// SELECTOR DE CÁMARAS (Teclas 1, 2, 3, 4)
		// =======================================================

		// TECLA 1: Cámara Libre (Restauramos su velocidad al activarla)
		if (mainWindow.getsKeys()[GLFW_KEY_1]) {
			if (!tecla1Presionada) {
				modoCamara = 1;
				camera = Camera(camera.getCameraPosition(), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.5f, 0.5f);
				tecla1Presionada = true;
			}
		}
		else {
			tecla1Presionada = false;
		}

		// TECLA 2: 3ra Persona
		if (mainWindow.getsKeys()[GLFW_KEY_2]) {
			if (!tecla2Presionada) { modoCamara = 2; tecla2Presionada = true; }
		}
		else { tecla2Presionada = false; }

		// TECLA 3: Cámara Aérea
		if (mainWindow.getsKeys()[GLFW_KEY_3]) {
			if (!tecla3Presionada) {
				modoCamara = 3;
				// Al encenderla, la centramos y la mandamos súper alto (Y=250)
				cam3X = 0.0f; cam3Z = 0.0f; cam3Y = 250.0f;
				tecla3Presionada = true;
			}
		}
		else { tecla3Presionada = false; }

		// TECLA 4: Recorrido Cinemático
		if (mainWindow.getsKeys()[GLFW_KEY_4]) {
			if (!tecla4Presionada) { modoCamara = 4; tecla4Presionada = true; }
		}
		else { tecla4Presionada = false; }


		// =======================================================
		// EJECUCIÓN DEL MODO DE CÁMARA SELECCIONADO
		// =======================================================
		if (modoCamara == 1) {
			// MODO 1: LIBRE
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		else if (modoCamara == 2) {
			// MODO 2: 3ra PERSONA (Anclada al movimiento y rotación de Josef)
			float distTrasera = 40.0f;
			float alturaCam = 20.0f;
			float realJosefX = posRobotX;
			float realJosefZ = posRobotZ + 230.0f;

			// Mantiene la posición a las espaldas
			float camX = realJosefX - sin(giroRobot * toRadians) * distTrasera;
			float camZ = realJosefZ - cos(giroRobot * toRadians) * distTrasera;

			// ¡LA MAGIA AQUÍ!: El YAW (tercer parámetro) ahora es '90.0f - giroRobot'
			// Esto obliga a la cámara a rotar en sincronía perfecta con las flechas Izq/Der
			camera = Camera(glm::vec3(camX, alturaCam, camZ), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f - giroRobot, -15.0f, 0.0f, 0.0f);
		}
		else if (modoCamara == 3) {
			float velMapa = 2.0f * deltaTime;

			if (mainWindow.getsKeys()[GLFW_KEY_A]) cam3X -= velMapa;
			if (mainWindow.getsKeys()[GLFW_KEY_D]) cam3X += velMapa;

		////////////////////////////////////////////////////////////
		// LOCOMOTORA Y HUMITO
		////////////////////////////////////////////////////////
		// loco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, -90.0f)); 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		locomotora.RenderModel();

		//// dibujar humito
		// --- ANIMACIÓN BÁSICA DEL HUMO (MEJORADA) ---
		// 2. DIBUJAR HUMITO (¡Ahora sí va a funcionar!)
		if (locoActiva) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			model = glm::mat4(1.0);
			// Posición: La locomotora está en 140, el humo sale de su chimenea
			model = glm::translate(model, glm::vec3(140.0f, 5.0f + movHumoY, -180.0f));

			// Rotación: Hacemos que el humo gire un poco mientras sube (Animación Simple)
			model = glm::rotate(model, (movHumoY * 50.0f) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model, glm::vec3(escalaHumo, escalaHumo, escalaHumo));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			texturaHumo.UseTexture();
			planoHumo.RenderModel();

			glDisable(GL_BLEND);
		}
			if (mainWindow.getsKeys()[GLFW_KEY_W]) cam3Z -= velMapa;
			if (mainWindow.getsKeys()[GLFW_KEY_S]) cam3Z += velMapa;

			if (cam3X < -280.0f) cam3X = -280.0f;
			if (cam3X > 280.0f) cam3X = 280.0f;
			if (cam3Z < -280.0f) cam3Z = -280.0f;
			if (cam3Z > 280.0f) cam3Z = 280.0f;

			camera = Camera(glm::vec3(cam3X, cam3Y, cam3Z), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -80.0f, 0.0f, 0.0f);
		}
		else if (modoCamara == 4) {
			// MODO 4: CINEMÁTICA
			float tiempoTour = fmod(glfwGetTime(), 15.0f);
			if (tiempoTour < 5.0f) {
				// 1. Pico Helado: Subimos la Y de 75 a 170.0f para llegar a la verdadera punta
				camera = Camera(glm::vec3(150.0f, 170.0f, 100.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -20.0f, 0.0f, 0.0f);
			}
			else if (tiempoTour < 10.0f) {
				// 2. Torre del Reloj
				camera = Camera(glm::vec3(0.0f, 120.0f, 80.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f, 0.0f, 0.0f);
			}
			else {
				// 3. Zepelín
				float alturaZep = 45.0f + flotadoY + 105.0f;
				camera = Camera(glm::vec3(movZepX + 40.0f, alturaZep + 20.0f, movZepZ + 40.0f), glm::vec3(0.0f, 1.0f, 0.0f), anguloZep + 200.0f, -20.0f, 0.0f, 0.0f);
			}
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}