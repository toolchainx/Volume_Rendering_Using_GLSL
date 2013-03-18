// 首先绘制一个正方体
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtx/transform2.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

using namespace std;
using glm::mat4;
using glm::vec3;
int g_vao;
int g_programHandle;
int g_winWidth = 400;
int g_winHeight = 400;
int g_angle = 0;



int checkForOpenGLError(const char* file, int line)
{
    // return 1 if an OpenGL error occured, 0 otherwise.
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
	cout << "glError in file " << file
	     << "@line " << line << gluErrorString(glErr) << endl;
	retCode = 1;
    }
    return retCode;
}
void keyboard(unsigned char key, int x, int y);
void display(void);
// 初始化顶点缓冲区对象
void initVBO()
{
    GLfloat vertices[24] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
    };
// 绘制正方体的六个面
// 正面用逆时针绘制
// front: 1 5 7 3
// back: 0 2 6 4
// left：0 1 3 2
// right:7 5 4 6    
// up: 2 3 7 6
// down: 1 0 4 5
    GLuint indices[36] = {
	1,5,7,
	7,3,1,
	0,2,6,
        6,4,0,
	0,1,3,
	3,2,0,
	7,5,4,
	4,6,7,
	2,3,7,
	7,6,2,
	1,0,4,
	4,5,1
    };
    GLuint gbo[2];
    
    glGenBuffers(2, gbo);
    GLuint vertexdat = gbo[0];
    GLuint veridxdat = gbo[1];
    glBindBuffer(GL_ARRAY_BUFFER, vertexdat);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // used in glDrawElement()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(GLuint), indices, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    // vao like a closure binding 3 buffer object: verlocdat vercoldat and veridxdat
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0); // for vertexloc
    glEnableVertexAttribArray(1); // for vertexcol

    // the vertex location is the same as the vertex color
    glBindBuffer(GL_ARRAY_BUFFER, vertexdat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat *)NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLfloat *)NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);
    // glBindVertexArray(0);
    g_vao = vao;
}
void drawBox()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindVertexArray(g_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
    glDisable(GL_CULL_FACE);
}
// check the compilation result
GLboolean compileCheck(GLuint shader)
{
    GLint err;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (GL_FALSE == err)
    {
	GLint logLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0)
	{
	    char* log = (char *)malloc(logLen);
	    GLsizei written;
	    glGetShaderInfoLog(shader, logLen, &written, log);
	    cerr << "Shader log: " << log << endl;
	    free(log);		
	}
    }
    return err;
}
// init shader object
GLuint initShaderObj(const GLchar* srcfile, GLenum shaderType)
{
    ifstream inFile(srcfile, ifstream::in);
    // use assert?
    if (!inFile)
    {
	cerr << "Error openning file: " << srcfile << endl;
	exit(EXIT_FAILURE);
    }
    
    const int MAX_CNT = 1000;
    GLchar *shaderCode = (GLchar *) calloc(MAX_CNT, sizeof(GLchar));
    inFile.read(shaderCode, MAX_CNT);
    if (inFile.eof())
    {
	size_t bytecnt = inFile.gcount();
	*(shaderCode + bytecnt) = '\0';
    }
    else if(inFile.fail())
    {
	cout << srcfile << "read failed " << endl;
    }
    else
    {
	cout << srcfile << "is too large" << endl;
    }
    // create the shader Object
    GLuint shader = glCreateShader(shaderType);
    if (0 == shader)
    {
	cerr << "Error creating vertex shader." << endl;
    }
    // cout << shaderCode << endl;
    // cout << endl;
    const GLchar* codeArray[] = {shaderCode};
    glShaderSource(shader, 1, codeArray, NULL);
    free(shaderCode);

    // compile the shader
    glCompileShader(shader);
    if (GL_FALSE == compileCheck(shader))
    {
	cerr << "shader compilation failed" << endl;
    }
    return shader;
}
GLboolean checkShaderLinkStatus(GLuint pgmHandle)
{
    GLint status;
    glGetProgramiv(pgmHandle, GL_LINK_STATUS, &status);
    if (GL_FALSE == status)
    {
	GLint logLen;
	glGetProgramiv(pgmHandle, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0)
	{
	    GLchar * log = (GLchar *)malloc(logLen);
	    GLsizei written;
	    glGetProgramInfoLog(pgmHandle, logLen, &written, log);
	    cerr << "Program log: " << log << endl;
	}
    }
    return status;
}
// link shader program
GLuint linkShaderObj(GLuint vertShader, GLuint fragShader)
{
    // Create the shader program
    GLuint programHandle = glCreateProgram();
    if (0 == programHandle)
    {
	cerr << "Error create shader program" << endl;
	exit(EXIT_FAILURE);
    }

    // Bind index 0 to the shader input variable "VerPos"
    glBindAttribLocation(programHandle, 0, "VerPos");
    // Bind index 1 to the shader input variable "VerClr"
    glBindAttribLocation(programHandle, 0, "VerClr");

    // Attach the shaders to the shader program
    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);

    // Link the program
    glLinkProgram(programHandle);
    if (GL_FALSE == checkShaderLinkStatus(programHandle))
    {
	cerr << "Failed to link shader program!" << endl;
	exit(EXIT_FAILURE);
    }
    return programHandle;
}
// 初始化着色器程序

void initShader()
{
// 初始化顶点着色器
    GLuint vertHandle = initShaderObj("shader/raycasting.vert", GL_VERTEX_SHADER);
// 初始化片段着色器
    GLuint fragHandle = initShaderObj("shader/raycasting.frag", GL_FRAGMENT_SHADER);
// 初始化着色器程序, use it in an appropriate time
    g_programHandle = linkShaderObj(vertHandle, fragHandle);
// 获得由着色器编译器分配的索引(可选)
}
void display()
{
    glEnable(GL_DEPTH_TEST);
    // test the gl_error
    checkForOpenGLError(__FILE__, __LINE__);
    glViewport(0, 0, g_winWidth, g_winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glUseProgram(g_programHandle);
    //  transform the box
    glm::mat4 projection = glm::perspective(60.0f, (GLfloat)g_winWidth/g_winHeight, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
    				 glm::vec3(0.0f, 0.0f, 0.0f), 
    				 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = mat4(1.0f);
    model *= glm::rotate((float)g_angle, glm::vec3(0.0f, 1.0f, 0.0f));
    model *= glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f)); 
    // notice the multiplication order
    glm::mat4 mvp = projection * view * model;
    GLuint mvpIdx = glGetUniformLocation(g_programHandle, "MVP");
    if (mvpIdx >= 0)
    {
    	glUniformMatrix4fv(mvpIdx, 1, GL_FALSE, &mvp[0][0]);
    }
    else
    {
    	cerr << "can't get the MVP" << endl;
    }
    // draw the back face of the box 
    // draw the front face of the box
    checkForOpenGLError(__FILE__, __LINE__);
    drawBox();
    checkForOpenGLError(__FILE__, __LINE__);
    // glUseProgram(0);
    // glutWireTeapot(0.5);
    glutSwapBuffers();
}
void rotateDisplay()
{
    g_angle = (g_angle + 1) % 360;
    glutPostRedisplay();
}
void reshape(int w, int h)
{
    g_winWidth = w;
    g_winHeight = h;
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("GLUT Test");
    glutInitWindowSize(400, 400);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
	/* Problem: glewInit failed, something is seriously wrong. */
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&display);
    glutReshapeFunc(&reshape);
    glutIdleFunc(&rotateDisplay);
    initShader();
    initVBO();
    glutMainLoop();
    return EXIT_SUCCESS;
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '\x1B':
	exit(EXIT_SUCCESS);
	break;
    }
}
