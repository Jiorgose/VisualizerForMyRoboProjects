#include "render.hpp"

void render(double t, static const double pi)
{
  const float red = (float)(0.5 + 0.5 * sin(t));
  const float green = (float)(0.5 + 0.5 * sin(t + pi * 2 / 3));
  const float blue = (float)(0.5 + 0.5 * sin(t + pi * 4 / 3));

  glClearColor(red, green, blue, 1.0);

  glClear(GL_COLOR_BUFFER_BIT);
}