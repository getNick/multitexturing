#include <stdafx.h>
#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	//colorAttribute = glGetAttribLocation(program, "a_color");
	matrixTransform = glGetUniformLocation(program, "u_matT");
	uvAttribute= glGetAttribLocation(program, "a_uv");
	uniformTexture = glGetUniformLocation(program, "u_texture_0");
	uniformMask = glGetUniformLocation(program, "u_texture_mask");
	uniformTexture1 = glGetUniformLocation(program, "u_texture_1");
	uniformTexture2 = glGetUniformLocation(program, "u_texture_2");
	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}