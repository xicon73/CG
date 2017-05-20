//
// Created by Afonso on 15/05/17.
//

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <IL/il.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ezxml.h"

#define STRING_MAX_SIZE 128

struct point {
    double x, y, z;
    struct point *next;
};

struct transformation {
    char type;  // 'r' = rotate, 's' = scale, 't' = translate

    struct rotation {
        double angle, x, y, z;
        int time;
    } rotation;

    struct scale {
        double x, y, z;
    } scale;

    struct translation {
        double time, x, y, z;
        int npoints;
        struct point *points;
    } translation;

    struct transformation *next;
};

struct model {
    int nvertices;
    GLuint vertices, normals, texture;
    GLuint texture_id;
    struct model *next;
};

struct group {
    char id[STRING_MAX_SIZE];

    struct model *models;
    struct transformation *transformations;
    struct group *child, *sibling;
};

struct scene {
    struct group *root;
};

struct scene scene_load(FILE *fp);

#endif //ENGINE_SCENE_H
