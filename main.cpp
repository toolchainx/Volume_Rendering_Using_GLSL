// 首先绘制一个正方体
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


int g_vao;
int g_vertexIndices;
void keyboard(unsigned char key, int x, int y);
void display(void);
// 初始化顶点缓冲区对象
void initVBO()
{
    float vertices[8] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
    }
    float colors[8] = {
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0
    }
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
	5,7,3,
	0,2,6,
	2,6,4,
	0,1,3,
	1,3,2,
	7,5,4,
	5,4,6,
	2,3,7,
	3,7,6,
	1,0,4,
	0,4,5
    }
    GLuint gbo[3];
    glGenBuffers(3, gbo);
    GLuint verlocdat = gbo[0];
    GLuint vercoldat = gbo[1];
    GLuint veridxdat = gbo[2];
    glBindBuffer(GL_ARRAY_BUFFER, verlocdat);
    glBufferData(GL_ARRAY_BUFFER, 8, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vercoldat);
    glBufferData(GL_ARRAY_BUFFER, 8, colors, GL_STATIC_DRAW);
    // used in glDrawElement()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veridxdat);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // for vertexloc
    glEnableVertexAttribArray(1); // for vertexcol
    GLuint vao;
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, verlocdat);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, (GLvoid *)NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vercoldat);
    glVertexAttribPointer(1, 3, GL_FLOAT, 0, (GLvoid *)NULL);
    glBindVertexArray(0);
    g_vao = vao;
    g_vertexIndices = veridxdat;
}
void drawBox()
{
    glBindVertexArray(g_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_vertexIndices);
    glDrawElements(GL_TRANGLES, 36, GL_UNSIGNED_INT, (GLvoid *)NULL);
}
// 初始化着色器程序
void initShader()
{
// 初始化顶点着色器

// 初始化片段着色器

// 初始化着色器程序

// 指定属性与索引的绑定

// 链接着色器对象

// 获得由着色器编译器分配的索引(可选)
    
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
}
int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);
    glutCreateWindow("GLUT Test");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
	/* Problem: glewInit failed, something is seriously wrong. */
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glutKeyboardFunc(&keyboard);
    glutDisplayFunc(&display);
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



