//
// Created by lukas on 5/26/2016.
//
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

#ifndef AIRSHOW_SHADERLOADER_H
#define AIRSHOW_SHADERLOADER_H

using namespace std;

class ShaderLoader
{
public:
    static void loadProgram(GLuint* shaderProgram, string vsPath, string fsPath)
    {
        string sfs = readFragmentShader(fsPath);
        string svs = readVertexShader(vsPath);
        const char* fs = sfs.c_str();
        const char* vs = svs.c_str();

        //VERTEX SHADER

        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);


        glShaderSource(vertexShader, 1, &vs, NULL);
        glCompileShader(vertexShader);

        //checking for errors

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        //FRAGMENT SHADER

        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fs, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //SHADER PROGRAM


        *shaderProgram = glCreateProgram();

        glAttachShader(*shaderProgram, vertexShader);
        glAttachShader(*shaderProgram, fragmentShader);
        glLinkProgram(*shaderProgram);

        glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

private:
    static string readVertexShader(string vsPath)
    {
        string vs = "";
        string line;
        ifstream myfile (vsPath);
        if (myfile.is_open())
        {
            while ( !myfile.eof())
            {
                getline (myfile,line);
                vs += line +'\n';
            }
            myfile.close();
        }
        return vs;
    }

    static string readFragmentShader(string fsPath)
    {
        string fs = "";
        string line;
        ifstream myfile (fsPath);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                fs += line+'\n';
            }
            myfile.close();
        }
        return fs;
    }


};

#endif //AIRSHOW_SHADERLOADER_H
