//Práctica 4: Modelado Jerárquico.
//Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
//heredar entre diversas instancias para que estén unidas
//Teclas de la F a la K para rotaciones de articulaciones

#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(15, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();



	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	GLfloat giroRuedas = 0.0f;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();

		// SI  U, SUMA. SI  I, RESTA.
		if (mainWindow.getsKeys()[GLFW_KEY_U]) {
			giroRuedas += 2.0f;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_I]) {
			giroRuedas -= 2.0f;
		}
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		// =========================================================
		//            MODELADO DEL PERRO ROBOT (Robustecido)
		// =========================================================

		// --- ANCLA PRINCIPAL DEL PERRO ---
		model = glm::mat4(1.0);
		// Lo movemos a la derecha (X=9.0), altura ajustada y misma profundidad
		model = glm::translate(model, glm::vec3(9.0f, 1.5f, -4.0f));
		// Escala global para que el perro completo sea más grande
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		glm::mat4 modelPerro = model; // Guardamos la matriz base del perro

		// --- CUERPO (TORSO) ---
		model = glm::scale(modelPerro, glm::vec3(4.5f, 2.2f, 2.5f)); // Torso más ancho y largo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.54f, 0.27f, 0.07f); // Café Medio para el torso
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// --- CABEZA ---
		model = modelPerro;
		model = glm::translate(model, glm::vec3(2.8f, 1.2f, 0.0f)); // Ajustado al nuevo torso
		glm::mat4 cabezaAux = model;

		model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f)); // Cabeza más grande
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.82f, 0.55f, 0.28f); // Café Clarito para la cara
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// --- HOCICO ---
		model = cabezaAux;
		model = glm::translate(model, glm::vec3(1.2f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.7f, 0.8f)); // Hocico más alargado
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.15f, 0.08f, 0.04f); // Café muy oscuro (casi negro) para la nariz
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// --- OREJAS ---
		for (float i : {-0.7f, 0.7f}) { // Más separadas por la cabeza grande
			model = cabezaAux;
			model = glm::translate(model, glm::vec3(-0.2f, 1.0f, i));
			model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.9f, 0.3f)); // Orejas más largas
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			color = glm::vec3(0.35f, 0.16f, 0.04f); // Café oscuro
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMeshGeometry();
		}

		// --- PATAS DELANTERAS ---
		for (float i : {-1.0f, 1.0f}) { // Más separadas por el ancho del pecho
			// HOMBRO
			model = modelPerro;
			model = glm::translate(model, glm::vec3(1.8f, -0.8f, i));
			model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 hombroDel = model;

			model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.35f, 0.16f, 0.04f); // Articulaciones oscuras
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// MUSLO
			model = hombroDel;
			model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 1.6f, 0.5f)); // Más largas y gruesas
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.82f, 0.55f, 0.28f); // Café Clarito
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMeshGeometry();

			// RODILLA
			model = hombroDel;
			model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f)); // Bajamos la rodilla
			model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 rodillaDel = model;

			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.35f, 0.16f, 0.04f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// PANTORRILLA
			model = rodillaDel;
			model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
			model = glm::scale(model, glm::vec3(0.4f, 1.4f, 0.4f)); // Pantorrillas fuertes
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.54f, 0.27f, 0.07f); // Café Medio
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMeshGeometry();
		}

		// --- COLA ---
		model = modelPerro;
		model = glm::translate(model, glm::vec3(-2.2f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.6f, 2.0f, 0.6f)); // Cola más larga y vistosa
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.82f, 0.55f, 0.28f); // Café clarito
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMeshGeometry();


		// --- PATAS TRASERAS ---
		for (float z_pos : {-1.0f, 1.0f}) { // Más separadas
			// CADERA
			model = modelPerro;
			model = glm::translate(model, glm::vec3(-1.8f, -0.8f, z_pos));
			model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 caderaAux = model;

			model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.35f, 0.16f, 0.04f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// MUSLO TRASERO
			model = caderaAux;
			model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 1.6f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.82f, 0.55f, 0.28f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMeshGeometry();

			// RODILLA TRASERA
			model = caderaAux;
			model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
			model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 rodillaAux = model;

			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.35f, 0.16f, 0.04f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();

			// PANTORRILLA TRASERA
			model = rodillaAux;
			model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
			model = glm::scale(model, glm::vec3(0.4f, 1.4f, 0.4f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.54f, 0.27f, 0.07f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMeshGeometry();
		}



		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
