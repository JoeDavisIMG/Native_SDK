/******************************************************************************

 @File         OGLES3Texturing.cpp

 @Title        Texturing

 @Version      

 @Copyright    Copyright (c) Imagination Technologies Limited.

 @Platform     Independant

 @Description  Shows how to use textures in OpenGL ES 3.0

******************************************************************************/
#include <stdio.h>
#include <string.h>
#if defined(__APPLE__) && defined (TARGET_OS_IPHONE)
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#else
#include <GLES3/gl3.h>
#endif
#include "PVRShell.h"

/******************************************************************************
 Defines
******************************************************************************/

// Index to bind the attributes to vertex shaders
#define VERTEX_ARRAY	0
#define TEXCOORD_ARRAY	1

// Size of the texture we create
#define TEX_SIZE		128

/*!****************************************************************************
 Class implementing the PVRShell functions.
******************************************************************************/
class OGLES3Texturing : public PVRShell
{
	// The vertex and fragment shader OpenGL handles
	GLuint m_uiVertexShader, m_uiFragShader;

	// The program object containing the 2 shader objects
	GLuint m_uiProgramObject;

	// Texture handle
	GLuint	m_uiTexture;

	// VBO handle
	GLuint m_ui32Vbo;

	//
	unsigned int m_ui32VertexStride;

public:
	virtual bool InitApplication();
	virtual bool InitView();
	virtual bool ReleaseView();
	virtual bool QuitApplication();
	virtual bool RenderScene();
};


/*!****************************************************************************
 @Function		InitApplication
 @Return		bool		true if no error occured
 @Description	Code in InitApplication() will be called by PVRShell once per
				run, before the rendering context is created.
				Used to initialize variables that are not dependant on it
				(e.g. external modules, loading meshes, etc.)
				If the rendering context is lost, InitApplication() will
				not be called again.
******************************************************************************/
bool OGLES3Texturing::InitApplication()
{
	return true;
}

/*!****************************************************************************
 @Function		QuitApplication
 @Return		bool		true if no error occured
 @Description	Code in QuitApplication() will be called by PVRShell once per
				run, just before exiting the program.
				If the rendering context is lost, QuitApplication() will
				not be called.
******************************************************************************/
bool OGLES3Texturing::QuitApplication()
{
    return true;
}

/*!****************************************************************************
 @Function		InitView
 @Return		bool		true if no error occured
 @Description	Code in InitView() will be called by PVRShell upon
				initialization or after a change in the rendering context.
				Used to initialize variables that are dependant on the rendering
				context (e.g. textures, vertex buffers, etc.)
******************************************************************************/
bool OGLES3Texturing::InitView()
{
	// Fragment and vertex shaders code
	const char* pszFragShader = "\
		#version 300 es\n\
		uniform sampler2D sampler2d;\
		in mediump vec2	myTexCoord;\
		layout (location = 0) out lowp vec4 oColour;\
		void main (void)\
		{\
		    oColour = texture(sampler2d,myTexCoord);\
		}";
	const char* pszVertShader = "\
		#version 300 es\n\
		#define VERTEX_ARRAY 0\n\
		#define TEXCOORD_ARRAY 1\n\
		layout (location = VERTEX_ARRAY) in highp vec4	myVertex;\
		layout (location = TEXCOORD_ARRAY) in highp vec2	myUV;\
		uniform mediump mat4	myPMVMatrix;\
		out mediump vec2	myTexCoord;\
		void main(void)\
		{\
			gl_Position = myPMVMatrix * myVertex;\
			myTexCoord = myUV;\
		}";

	// Create the fragment shader object
	m_uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load the source code into it
	glShaderSource(m_uiFragShader, 1, (const char**)&pszFragShader, NULL);

	// Compile the source code
	glCompileShader(m_uiFragShader);

	// Check if compilation succeeded
	GLint bShaderCompiled;
    glGetShaderiv(m_uiFragShader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled)
	{
		// An error happened, first retrieve the length of the log message
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(m_uiFragShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		// Allocate enough space for the message and retrieve it
		char* pszInfoLog = new char[i32InfoLogLength];
        glGetShaderInfoLog(m_uiFragShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

		/*
			Displays the message in a dialog box when the application quits
			using the shell PVRShellSet function with first parameter prefExitMessage.
		*/
		char* pszMsg = new char[i32InfoLogLength+256];
		strcpy(pszMsg, "Failed to compile fragment shader: ");
		strcat(pszMsg, pszInfoLog);
		PVRShellSet(prefExitMessage, pszMsg);

		delete [] pszMsg;
		delete [] pszInfoLog;
		return false;
	}

	// Loads the vertex shader in the same way
	m_uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_uiVertexShader, 1, (const char**)&pszVertShader, NULL);
	glCompileShader(m_uiVertexShader);
    glGetShaderiv(m_uiVertexShader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled)
	{
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(m_uiVertexShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
		char* pszInfoLog = new char[i32InfoLogLength];
        glGetShaderInfoLog(m_uiVertexShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength+256];
		strcpy(pszMsg, "Failed to compile vertex shader: ");
		strcat(pszMsg, pszInfoLog);
		PVRShellSet(prefExitMessage, pszMsg);

		delete [] pszMsg;
		delete [] pszInfoLog;
		return false;
	}

	// Create the shader program
    m_uiProgramObject = glCreateProgram();

	// Attach the fragment and vertex shaders to it
    glAttachShader(m_uiProgramObject, m_uiFragShader);
    glAttachShader(m_uiProgramObject, m_uiVertexShader);

	// Bind the custom vertex attribute "myVertex" to location VERTEX_ARRAY
    glBindAttribLocation(m_uiProgramObject, VERTEX_ARRAY, "myVertex");
	// Bind the custom vertex attribute "myUV" to location TEXCOORD_ARRAY
    glBindAttribLocation(m_uiProgramObject, TEXCOORD_ARRAY, "myUV");

	// Link the program
    glLinkProgram(m_uiProgramObject);

	// Check if linking succeeded in the same way we checked for compilation success
    GLint bLinked;
    glGetProgramiv(m_uiProgramObject, GL_LINK_STATUS, &bLinked);

	if (!bLinked)
	{
		int i32InfoLogLength, i32CharsWritten;
		glGetProgramiv(m_uiProgramObject, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
		char* pszInfoLog = new char[i32InfoLogLength];
		glGetProgramInfoLog(m_uiProgramObject, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		
		char* pszMsg = new char[i32InfoLogLength+256];
		strcpy(pszMsg, "Failed to link program: ");
		strcat(pszMsg, pszInfoLog);
		PVRShellSet(prefExitMessage, pszMsg);
		delete [] pszMsg;
		delete [] pszInfoLog;
		return false;
	}

	// Actually use the created program
	glUseProgram(m_uiProgramObject);

	// Sets the sampler2D variable to the first texture unit
	glUniform1i(glGetUniformLocation(m_uiProgramObject, "sampler2d"), 0);

	// Sets the clear color
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	/*
		Creates the texture
	*/

	// Allocates one texture handle
	glGenTextures(1, &m_uiTexture);

	// Binds this texture handle so we can load the data into it
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TEX_SIZE, TEX_SIZE);

	/*	---------
		PBO write
		---------
		Map a GL allocated buffer, write the proceedural texture straight into it.
		Without the PBO, glTexImage2D() has to memcpy() from the application allocated pTexData
		to a driver allocated buffer, wasting CPU cycles and bandwidth
	*/
#define PBO_WRITE
	GLuint* pTexData(NULL);
#ifdef PBO_WRITE
	GLuint pbo;
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	const GLuint buffer_size(TEX_SIZE*TEX_SIZE * 4);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, buffer_size, 0, GL_STATIC_DRAW);
	pTexData = (GLuint*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, buffer_size, GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
#else
	// Creates the data as a 32bits integer array (8bits per component)
	pTexData = new GLuint[TEX_SIZE*TEX_SIZE];
#endif

	for (int i=0; i<TEX_SIZE; i++)
	for (int j=0; j<TEX_SIZE; j++)
	{
		// Fills the data with a fancy pattern
		GLuint col = (255<<24) + ((255-j*2)<<16) + ((255-i)<<8) + (255-i*2);
		if ( ((i*j)/8) % 2 ) col = (GLuint) (255<<24) + (255<<16) + (0<<8) + (255);
		pTexData[j*TEX_SIZE+i] = col;
	}
#ifdef PBO_WRITE
	GLboolean unmap_error = glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	pTexData = NULL;
	if (unmap_error == GL_FALSE) printf("glUnmapBuffer failed!\n");
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TEX_SIZE, TEX_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, 0);
#else
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TEX_SIZE, TEX_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, pTexData);
	// Deletes the texture data, it's now in OpenGL memory
	delete[] pTexData;
#endif
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Create VBO for the triangle from our data

	// Interleaved vertex data
	GLfloat afVertices[] = {-0.4f,-0.4f,0.0f, // Pos
							 0.0f,0.0f ,	  // UVs
							 0.4f,-0.4f,0.0f,
							 1.0f,0.0f ,
							 0.0f,0.4f ,0.0f,
							 0.5f,1.0f};

	glGenBuffers(1, &m_ui32Vbo);

	m_ui32VertexStride = 5 * sizeof(GLfloat); // 3 floats for the pos, 2 for the UVs

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_ui32Vbo);

	// Set the buffer's data
	glBufferData(GL_ARRAY_BUFFER, 3 * m_ui32VertexStride, afVertices, GL_STATIC_DRAW);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Enable culling
	glEnable(GL_CULL_FACE);
	return true;
}

/*!****************************************************************************
 @Function		ReleaseView
 @Return		bool		true if no error occured
 @Description	Code in ReleaseView() will be called by PVRShell when the
				application quits or before a change in the rendering context.
******************************************************************************/
bool OGLES3Texturing::ReleaseView()
{
	// Frees the texture
	glDeleteTextures(1, &m_uiTexture);

	// Release Vertex buffer object.
	glDeleteBuffers(1, &m_ui32Vbo);

	// Frees the OpenGL handles for the program and the 2 shaders
	glDeleteProgram(m_uiProgramObject);
	glDeleteShader(m_uiVertexShader);
	glDeleteShader(m_uiFragShader);
	return true;
}

/*!****************************************************************************
 @Function		RenderScene
 @Return		bool		true if no error occured
 @Description	Main rendering loop function of the program. The shell will
				call this function every frame.
				eglSwapBuffers() will be performed by PVRShell automatically.
				PVRShell will also manage important OS events.
				Will also manage relevent OS events. The user has access to
				these events through an abstraction layer provided by PVRShell.
******************************************************************************/
bool OGLES3Texturing::RenderScene()
{
	// Clears the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	/*
		Bind the projection model view matrix (PMVMatrix) to
		the associated uniform variable in the shader
	*/

	// Matrix used for projection model view
	float afIdentity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	// First gets the location of that variable in the shader using its name
	int i32Location = glGetUniformLocation(m_uiProgramObject, "myPMVMatrix");

	// Then passes the matrix to that variable
	glUniformMatrix4fv(i32Location, 1, GL_FALSE, afIdentity);

	/*
		Draw a triangle.
		Please refer to HelloTriangle or IntroducingPVRShell for a detailed explanation.
	*/

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_ui32Vbo);

	// Pass the vertex data
	glEnableVertexAttribArray(VERTEX_ARRAY);
	glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, m_ui32VertexStride, 0);

	// Pass the texture coordinates data
	glEnableVertexAttribArray(TEXCOORD_ARRAY);
	glVertexAttribPointer(TEXCOORD_ARRAY, 2, GL_FLOAT, GL_FALSE, m_ui32VertexStride, (void*) (3 * sizeof(GLfloat)));

	// Draws a non-indexed triangle array
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

/*!****************************************************************************
 @Function		NewDemo
 @Return		PVRShell*		The demo supplied by the user
 @Description	This function must be implemented by the user of the shell.
				The user should return its PVRShell object defining the
				behaviour of the application.
******************************************************************************/
PVRShell* NewDemo()
{
	return new OGLES3Texturing();
}

/******************************************************************************
 End of file (OGLES3Texturing.cpp)
******************************************************************************/

