#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

static const double pi = 2 * acos(0.0);

void render(double t, int width, int height, GLuint shader, GLuint textureId, GLuint RBO, GLuint FBO, GLuint VAO);

void create_triangle(GLuint& VAO, GLuint& VBO);

void add_shader(GLuint program, const char* shader_code, GLenum type);
void create_shaders(GLuint& shader, const char* vertex_shader_code, const char* fragment_shader_code);

void create_framebuffer(int WIDTH, int HEIGHT, GLuint& texture_id, GLuint& FBO, GLuint& RBO);
void bind_framebuffer(GLuint FBO);
void unbind_framebuffer();
void rescale_framebuffer(int width, int height, GLuint texture_id, GLuint RBO);