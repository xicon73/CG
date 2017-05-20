//
// Created by Afonso on 16/05/17.
//

#ifndef PROJECT_RENDER_H
#define PROJECT_RENDER_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <IL/il.h>
#include <math.h>
#include "scene.h"

extern double TIME;

void render_group(struct group *group);

#endif //PROJECT_RENDER_H
