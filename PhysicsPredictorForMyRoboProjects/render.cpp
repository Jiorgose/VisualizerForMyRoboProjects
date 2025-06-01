#include "render.hpp"

void render(double t, int width, int height, GLuint shader, GLuint textureId, GLuint RBO, GLuint FBO, GLuint VAO, variables& vars)
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  rescale_framebuffer(width, height, textureId, RBO);
  glViewport(0, 0, width, height);

  bind_framebuffer(FBO);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  GLint colorLocation = glGetUniformLocation(shader, "sColor");

  glUseProgram(shader);

  glUniform3f(colorLocation, vars.color[0], vars.color[1], vars.color[2]);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
  glUseProgram(0);

  unbind_framebuffer();
}

void create_triangle(GLuint& VAO, GLuint& VBO) {
  GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
       1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
       1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f,  0.0f, 1.0f
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void add_shader(GLuint program, const char* shader_code, GLenum type)
{
  GLuint current_shader = glCreateShader(type);

  const GLchar* code[1];
  code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = static_cast<GLint>(strlen(shader_code));

  glShaderSource(current_shader, 1, code, code_length);
  glCompileShader(current_shader);

  GLint result = 0;
  GLchar log[1024] = { 0 };

  glGetShaderiv(current_shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(current_shader, sizeof(log), NULL, log);
    std::cout << "Error compiling " << type << " shader: " << log << "\n";
    return;
  }

  glAttachShader(program, current_shader);
}

void create_shaders(GLuint& shader, const char* vertex_shader_code, const char* fragment_shader_code)
{
  shader = glCreateProgram();
  if (!shader) {
    std::cout << "Error creating shader program!\n";
    exit(1);
  }

  add_shader(shader, vertex_shader_code, GL_VERTEX_SHADER);
  add_shader(shader, fragment_shader_code, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar log[1024] = { 0 };

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(log), NULL, log);
    std::cout << "Error linking program:\n" << log << '\n';
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(log), NULL, log);
    std::cout << "Error validating program:\n" << log << '\n';
    return;
  }
}

void create_framebuffer(int WIDTH, int HEIGHT, GLuint& texture_id, GLuint& FBO, GLuint& RBO)
{
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void bind_framebuffer(GLuint FBO)
{
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void unbind_framebuffer()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rescale_framebuffer(int width, int height, GLuint texture_id, GLuint RBO)
{
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}