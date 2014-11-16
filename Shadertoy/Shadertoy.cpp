//
//  Shadertoy.cpp
//  Shadertoy
//
//  Created by Gabriele on 04/10/14.
//  Copyright (c) 2014 Ga2 & co. All rights reserved.
//

#include "Shadertoy.h"

bool recompile = false;

Shadertoy::Shadertoy(): g_width(480), g_heigth(320) {};

Shadertoy::~Shadertoy() {};

void Shadertoy::play() {
    if (init()) {
        loop();
    }
}

bool Shadertoy::init() {
    
    glfwSetErrorCallback([](int error, const char* description) {
        cerr << description << endl;
    });
    
    if (!glfwInit()) {
        cout << "Error init" << endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    win = glfwCreateWindow(g_width, g_heigth, "Shadertoy", nullptr, nullptr);

    if (!win) {
        cout << "Error creatin window" << endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(win);
    glewExperimental = GL_TRUE;
    glewInit();
    
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);
    
    printf("Renderer: %s\n", renderer);
    printf("Version: %s\n", version);

    return true;
}

bool Shadertoy::compileShaders(const string& vertex, const string& fragment) {
    const char *vertexsource = vertex.c_str();
    const char *fragmentsource = fragment.c_str();

    auto vertexS = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexS, 1, &vertexsource, nullptr);
    glShaderSource(fragmentS, 1, &fragmentsource, nullptr);
    glCompileShader(vertexS);
    glCompileShader(fragmentS);
    
    program = glCreateProgram();
    glAttachShader(program, vertexS);
    glAttachShader(program, fragmentS);
    glLinkProgram(program);
    
    auto params = -1;
    glGetProgramiv (program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
        cout << "ERROR: could not link shader program" << endl;
        
        auto error_len = 0;
        glGetShaderiv(vertexS, GL_INFO_LOG_LENGTH, &error_len);
        
        if (error_len > 0) {
            auto max_length = error_len;
            auto actual_length = 0;
            char log[max_length];

            glGetShaderInfoLog (vertexS, max_length, &actual_length, log);
            cout << "=== VERTEX SHADER ===\n" << log << "\n==========\n" << endl;
        }
        
        error_len = 0;
        glGetShaderiv(fragmentS, GL_INFO_LOG_LENGTH, &error_len);
        
        if (error_len > 0) {
            auto max_length = error_len;
            auto actual_length = 0;
            char log[max_length];
            
            glGetShaderInfoLog (fragmentS, max_length, &actual_length, log);
            cout << "=== FRAGMENT SHADER ===\n" << log << "\n==========\n" << endl;
        }

        return false;
	}
    
    glDeleteShader (vertexS);
	glDeleteShader (fragmentS);
    
    return true;
}

void Shadertoy::loop() {
    GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    if (!loadFromFile()) {
        compileShaders(vertexSource, fragmentSource);
    }
    
    auto pos = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray (pos);
    
    auto time = glGetUniformLocation(program, "global_time");
    auto resolution = glGetUniformLocation(program, "resolution");
    
    glUseProgram(program);
    
    while (!glfwWindowShouldClose(win)) {
        glfwGetWindowSize(win, &g_width, &g_heigth);
        updateFpsCounter();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_width, g_heigth);
        
        auto current_time = glfwGetTime();
        glUniform1f(time, current_time);
        glUniform2f(resolution, g_width, g_heigth);
        
        if(recompile) {
            loadFromFile();
            glUseProgram(program);
            recompile = false;
        }
            
        glBindVertexArray (vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        
        glfwPollEvents();
        glfwSwapBuffers(win);

    }
}

string Shadertoy::loadFile(const string& filename) {
    ifstream t(filename.c_str());
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void Shadertoy::updateFpsCounter() {
    static double previousSeconds = glfwGetTime ();
	static int frameCcount;
	double currentSseconds = glfwGetTime ();
	double elapsedSeconds = currentSseconds - previousSeconds;
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentSseconds;
		double fps = (double)frameCcount / elapsedSeconds;
        stringstream tmp;
        tmp << "opengl @ fps: " << fps << endl;
        glfwSetWindowTitle (win, tmp.str().c_str());
        frameCcount = 0;
	}
	frameCcount++;
}

bool Shadertoy::loadFromFile() {
    auto vSource = loadFile("shaders/s.vert");
    auto fSource = loadFile("shaders/s.frag");
    
    return compileShaders(vSource, fSource);
}

