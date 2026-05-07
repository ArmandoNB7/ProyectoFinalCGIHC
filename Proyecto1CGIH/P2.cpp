//Practica 2
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
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crear an ac 
static const char* vShaderRojo = "shaders/shaderRojo.vert";
static const char* fShaderRojo = "shaders/shaderRojo.frag";
static const char* vShaderAzulito = "shaders/shaderAzulito.vert";
static const char* fShaderAzulito = "shaders/shaderAzulito.frag";
static const char* vShaderCafesin = "shaders/shaderCafesin.vert";
static const char* fShaderCafesin = "shaders/shaderCafesin.frag";
static const char* vShaderVerdecito = "shaders/shaderVerdesito.vert";
static const char* fShaderVerdecito = "shaders/shaderVerdesito.frag";
static const char* vShaderVerdeObs = "shaders/shaderVerdeObs.vert";
static const char* fShaderVerdeObs = "shaders/shaderVerdeObs.frag";


float angulo = 0.0f;

//color caf  en RGB : 0.478, 0.255, 0.067

//Pir mide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//V rtices de un cubo
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

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		// LETRA A  
		// =========================

		// barra izquierda
		-0.91f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.84f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.84f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.90f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.84f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.90f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,

		// barra derecha
		-0.66f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.66f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.66f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,

		// barra superior
		-0.90f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.24f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.90f, -0.30f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.24f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.90f, -0.24f, 0.0f, 1.0f, 1.0f, 1.0f,

		// barra media
		-0.90f, -0.52f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.52f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.46f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.90f, -0.52f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.60f, -0.46f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.90f, -0.46f, 0.0f, 1.0f, 1.0f, 1.0f,


		// LETRA N  


		// barra izquierda
		-0.20f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,
		-0.14f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,
		-0.14f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,

		-0.20f, -0.10f, 0.0f,  0.6f, 0.0f, 1.0f,
		-0.14f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,
		-0.20f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,

		// barra derecha
		 0.02f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,
		 0.08f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,
		 0.08f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,

		 0.02f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,
		 0.08f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,
		 0.02f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,

		 // diagonal 
		 -0.14f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,
		 -0.08f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,
		  0.08f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,

		 -0.14f,  0.30f, 0.0f, 0.6f, 0.0f, 1.0f,
		  0.08f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,
		  0.02f, -0.10f, 0.0f, 0.6f, 0.0f, 1.0f,


		  // LETRA B  


		  // barra izquierda 
		   0.25f,  0.25f, 0.0f, 1.0f, 1.0f,0.0f,
		   0.31f,  0.25f, 0.0f, 1.0f, 1.0f,0.0f,
		   0.31f,  0.75f, 0.0f,1.0f, 1.0f,0.0f,

		   0.25f,  0.25f, 0.0f, 1.0f, 1.0f,0.0f,
		   0.31f,  0.75f, 0.0f, 1.0f, 1.0f,0.0f,
		   0.25f,  0.75f, 0.0f, 1.0f, 1.0f,0.0f,

		   // barra superior 
			0.25f,  0.69f, 0.0f, 1.0f, 1.0f,0.0f,
			0.52f,  0.69f, 0.0f, 1.0f, 1.0f,0.0f,
			0.52f,  0.75f, 0.0f, 1.0f, 1.0f,0.0f,

			0.25f,  0.69f, 0.0f, 1.0f, 1.0f,0.0f,
			0.52f,  0.75f, 0.0f, 1.0f, 1.0f,0.0f,
			0.25f,  0.75f, 0.0f, 1.0f, 1.0f,0.0f,

			// barra media 
			 0.25f,  0.50f, 0.0f, 1.0f, 1.0f,0.0f,
			 0.50f,  0.50f, 0.0f, 1.0f, 1.0f,0.0f,
			 0.50f,  0.56f, 0.0f, 1.0f, 1.0f,0.0f,

			 0.25f,  0.50f, 0.0f, 1.0f, 1.0f,0.0f,
			 0.50f,  0.56f, 0.0f, 1.0f, 1.0f,0.0f,
			 0.25f,  0.56f, 0.0f, 1.0f, 1.0f,0.0f,

			 // barra inferior 
			  0.25f,  0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
			  0.52f,  0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
			  0.52f,  0.31f, 0.0f, 1.0f, 1.0f, 0.0f,

			  0.25f,  0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
			  0.52f,  0.31f, 0.0f, 1.0f, 1.0f, 0.0f,
			  0.25f,  0.31f, 0.0f, 1.0f, 1.0f, 0.0f,

			  // columna derecha 
			   0.46f,  0.56f, 0.0f, 1.0f, 1.0f,0.0f,
			   0.52f,  0.56f, 0.0f, 1.0f, 1.0f, 0.0f,
			   0.52f,  0.75f, 0.0f, 1.0f, 1.0f, 0.0f,

			   0.46f,  0.56f, 0.0f, 1.0f, 1.0f, 0.0f,
			   0.52f,  0.75f, 0.0f, 1.0f, 1.0f, 0.0f,
			   0.46f,  0.75f, 0.0f, 1.0f, 1.0f, 0.0f,

			   // columna derecha (
				0.46f,  0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
				0.52f,  0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
				0.52f,  0.50f, 0.0f, 1.0f, 1.0f, 0.0f,

				0.46f,  0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
				0.52f,  0.50f, 0.0f, 1.0f, 1.0f, 0.0f,
				0.46f,  0.50f, 0.0f, 1.0f, 1.0f, 0.0f,


	};
	/*MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);*/

	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, sizeof(vertices_letras) / sizeof(GLfloat));
	meshColorList.push_back(letras);



}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar  ndices: objetos: cubo y  pir mide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); //shader rojo
	shader3->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); //azulito
	shader4->CreateFromFiles(vShaderAzulito, fShaderAzulito);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader(); //cafesin
	shader5->CreateFromFiles(vShaderCafesin, fShaderCafesin);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader(); //verdecito
	shader6->CreateFromFiles(vShaderVerdecito, fShaderVerdecito);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader(); //verdeObs
	shader7->CreateFromFiles(vShaderVerdeObs, fShaderVerdeObs);
	shaderList.push_back(*shader7);

}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); // ndice 0 en MeshList
	CrearCubo();// ndice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensi n 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensi n 4x4 en la cual se almacena la multiplicaci n de las transformaciones geom tricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con  ndice 1 en ShaderList 
		/*shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();*/

		//letras
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.2f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();

		//shader rojo de la casaa
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, -5.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// shader azul del techoo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, -3.0f));
		model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(2.7f, 1.4f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		//shaders verdes para ambas ventans
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// ventana izquierda posicion
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.4f, -0.1f, -3.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// ventana derecha posicion
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, -0.1f, -3.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// puerta posicion
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, -4.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		// shader cafe para las macetas
		shaderList[4].useShader(); //  indice 4 = Caf 
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.8f, -1.65f, -4.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // 

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.8f, -1.65f, -4.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// shader verdeobs para los arbustps
		shaderList[6].useShader(); //  ndice 6 = Verde Oscuro
		uniformModel = shaderList[6].getModelLocation();

		uniformProjection = shaderList[6].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.75f, -0.75f, -4.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.75f, -0.75f, -4.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();




		////Para el cubo y la pir mide se usa el primer set de shaders con  ndice 0 en ShaderList
		//shaderList[0].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.01;
		////Inicializar matriz de dimensi n 4x4 que servir  como matriz de modelo para almacenar las transformaciones geom tricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh();
		//
		////piramide
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-1.25f, 1.25f, -4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslaci n
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACI N //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/