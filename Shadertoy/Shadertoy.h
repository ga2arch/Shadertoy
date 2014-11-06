//
//  Shadertoy.h
//  Shadertoy
//
//  Created by Gabriele on 04/10/14.
//  Copyright (c) 2014 Ga2 & co. All rights reserved.
//

#include "gl/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <CoreServices/CoreServices.h>

using namespace std;

extern bool recompile;

class Shadertoy {
    
public:
    Shadertoy();
    ~Shadertoy();
    
    void play();
    
    int g_width;
    int g_heigth;
    
    GLFWwindow* win;
    GLuint program;
    
    const string vertexSource =
        "#version 110\n"
        "attribute vec3 position;"
        "uniform float global_time;"
        "uniform vec2  resolution;"
        "varying float iGlobalTime;"
        "varying vec2  iResolution;"
        "void main() {"
        " iGlobalTime = global_time;"
        " iResolution = resolution;"
        " gl_Position = vec4(position, 1.0);"
        "}";
    
    const string fragmentSource =
        "#version 110\n"
        "varying float iGlobalTime;"
        "varying vec2  iResolution;"
        "void main(void) {"
        " gl_FragColor = vec4(vec3(1.0), 1.0);"
        "}";
    
    const GLfloat points[18] = {
		-1.0f, -1.0f,  0.0f,
        1.0f, -1.0f,  0.0f,
        1.0f,  1.0f,  0.0f,
        1.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f
	};


private:
    bool init();
    void loop();
    bool compileShaders(const string& vertex, const string& fragment);
    bool loadFromFile();
    
    void updateFpsCounter();
    string loadFile (const string& filename);
    
};