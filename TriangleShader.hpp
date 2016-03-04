#pragma once
#include <assert.h>//message
/*
// class ShaderId
// {
// public:
// 	ShaderId()//构造函数，用于初始化变量
// 	{
// 		_shaderid  =  -1;
// 	}
// 	int _shaderid;
// };
// 
// class ProgramId
// {
// public:
// 	int _programid;
// 
// 	ProgramId()//构造函数，用于初始化变量
// 	{
// 		_programid  =  -1;
// 	}
// 	
// 	bool creatProgram(const char* vertex,const char* fragment)
// 	{
// 
// 	}
// protected:
// private:
// };
*/

class CreatShaderProgram
{
public:
	GLuint ProgramObject;//creat program obbject

	//! create a shader object ,load the shader source and compile the shader 
	GLuint LoadShader(GLenum type,const char *shaderSrc)
	{
		GLuint shader;
		GLint  compiled;
		bool error = false;
		//create the shader object
		shader = glCreateShader(type);
		if (shader == 0 )
		{
			return 0;
		}
		//load the shader source
		glShaderSource(shader,1,&shaderSrc,NULL);

		//compile the shader 
		glCompileShader(shader);

		//check the compole status
		glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);

			GLint infoLen;
			glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLen);
			error  =  infoLen==GL_FALSE;

			if (error)
			{
				GLchar messages[256];
				//char* infoLog = malloc(sizeof(char) * infoLen);
				glGetShaderInfoLog(shader,sizeof(messages),NULL,messages);
				assert( messages && 0 != 0);//断言，错误时弹窗
				
			}
			glDeleteShader(shader);
			return 0;
		}

};

class ProgramShader : public CreatShaderProgram
{
public:
	GLuint programObject;

	ProgramShader()
	{

	}
	~ProgramShader()
	{

	}
	virtual bool initialize()
	{
		//const char* vShaderStr[]=;
		GLbyte vShaderStr[] =  
			"attribute vec4 vPosition;    \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = vPosition;  \n"
			"}                            \n";

		GLbyte fShaderStr[] =  
			"precision mediump float;                     \n"
			"void main()                                  \n"
			"{                                            \n"
			"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
			"}                                            \n";
		
		GLuint vertexShader;
		GLuint fragmentShader;
		
		GLint linked;

		vertexShader    =  LoadShader(GL_VERTEX_SHADER,&vShaderStr);
		fragmentShader  =  LoadShader(GL_FRAGMENT_SHADER,&fShaderStr);
		//creat program obbject
		ProgramObject = glCreateProgram();
		if (ProgramObject == 0)		
			return 0;

			glAttachShader(programObject,vertexShader);
			glAttachShader(programObject,fragmentShader);

			//link the program 
			glLinkProgram(programObject);

			//check the link status 
			glGetProgramiv(programObject,GL_LINK_STATUS,&linked);
			if (!linked)
			{
				GLint infoLen = 0;
				glGetProgramiv(programObject,GL_INFO_LOG_LENGTH,&infoLen);
				if ( infoLen > 1 )
				{
					char* infoLog = malloc (sizeof(char) * infoLen );

					glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
					//esLogMessage ( "Error linking program:\n%s\n", infoLog );            

					free ( infoLog );
				}

				glDeleteProgram ( programObject );
				return FALSE;
			}
			glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
			return TRUE;	
	
	}

	
};