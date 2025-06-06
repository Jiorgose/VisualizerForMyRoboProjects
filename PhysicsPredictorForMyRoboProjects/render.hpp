#pragma once

#include "common.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

void render(GLuint shader, GLuint textureId, GLuint RBO, GLuint FBO, GLuint VAO, GLFWwindow* window);

void create_triangle(GLuint& VAO, GLuint& VBO);

void add_shader(GLuint program, const char* shader_code, GLenum type);
void create_shaders(GLuint& shader, const char* vertex_shader_code, const char* fragment_shader_code);

void create_framebuffer(int WIDTH, int HEIGHT, GLuint& texture_id, GLuint& FBO, GLuint& RBO, AppState* state);
void bind_framebuffer(GLuint FBO);
void unbind_framebuffer();
void rescale_framebuffer(int width, int height, GLuint texture_id, GLuint RBO, GLuint FBO);