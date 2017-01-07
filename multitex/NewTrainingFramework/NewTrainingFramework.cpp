// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#include <vector>
#include "model.h"
#include "camera.h"


GLuint vboId;
Shaders myShaders;
Matrix m;
float m_time=0, ampFactor=0.02;
model model1;
GLuint texture[4];
camera cam;
int count = 0;
int Init ( ESContext *esContext )
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	m.SetIdentity();

	model1.loadModel("../Resources/Models/Terrain.nfg");

	model1.loadTexture("../Resources/Textures/Grass.tga",&texture[0]);
	model1.loadTexture("../Resources/Textures/Terrain_blend_map.tga", &texture[1]);
	model1.loadTexture("../Resources/Textures/Dirt.tga", &texture[2]);
	model1.loadTexture("../Resources/Textures/Rock.tga", &texture[3]);
	cam.setCursorToCentre();
	
	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw(ESContext *esContext)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(myShaders.program);
	glBindBuffer(GL_ARRAY_BUFFER, model1.m_hVertexBuffer); //binded model's vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model1.m_hIndexBuffer); //binded indices

	unsigned short textureUnit = 0;// make active a texture unit 
	glActiveTexture(GL_TEXTURE0 + textureUnit);// bind the texture to the currently active texture unit 
	glBindTexture(GL_TEXTURE_2D, texture[0]);// set the uniform sampler 
	glUniform1i(myShaders.uniformTexture, textureUnit++);

	glActiveTexture(GL_TEXTURE0 + textureUnit);// bind the texture to the currently active texture unit 
	glBindTexture(GL_TEXTURE_2D, texture[1]);// set the uniform sampler 
	glUniform1i(myShaders.uniformMask, textureUnit++);

	glActiveTexture(GL_TEXTURE0 + textureUnit);// bind the texture to the currently active texture unit 
	glBindTexture(GL_TEXTURE_2D, texture[2]);// set the uniform sampler 
	glUniform1i(myShaders.uniformTexture1, textureUnit++);

	glActiveTexture(GL_TEXTURE0 + textureUnit);// bind the texture to the currently active texture unit 
	glBindTexture(GL_TEXTURE_2D, texture[3]);// set the uniform sampler 
	glUniform1i(myShaders.uniformTexture2, textureUnit);

	GLfloat* ptr = (GLfloat *)0;
	if (myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
	}
	if (myShaders.uvAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.uvAttribute);
		glVertexAttribPointer(myShaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr+3);
	}
	if (myShaders.matrixTransform != -1) {
		glUniformMatrix4fv(myShaders.matrixTransform, 1, false, (GLfloat *)&m);
	}

	glDrawElements(GL_TRIANGLES, model1.nrIndices, GL_UNSIGNED_INT, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	m_time += deltaTime;
	m = model1.modelMatrix * cam.getViewM() * cam.getProjectionM();
	/*count++;
	if (count == 10) {
		cam.setCursorToCentre();
		count = 0;
	}*/

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	cam.ProcessKeyboard(CameraMovement(key));
}
void Mouse(ESContext *, int posX, int posY) {
	cam.mouseMove(posX, posY);
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("open complite");
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

