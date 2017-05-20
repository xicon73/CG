//
// Created by Afonso on 15/05/17.
//

#include "scene.h"

double randf(double min, double max) {
    double r = (double) rand() / RAND_MAX;
    return (max - min) * r + min;
}

double number_load(ezxml_t node, const char *value, const char *min, const char *max, double def) {
    if (ezxml_attr(node, value)) {
        return atof(ezxml_attr(node, value));
    }

    if (ezxml_attr(node, min) && ezxml_attr(node, max)) {
        return randf(atof(ezxml_attr(node, min)), atof(ezxml_attr(node, max)));
    }

    return def;
}

struct rotation rotation_load(ezxml_t node) {
    double angle = number_load(node, "angle", "angleMin", "angleMax", 0);
    double x = number_load(node, "x", "xMin", "xMax", 0);
    double y = number_load(node, "y", "yMin", "yMax", 0);
    double z = number_load(node, "z", "zMin", "zMax", 0);
    double time = number_load(node, "time", "timeMin", "timeMax", 0);

    return (struct rotation) { angle, x, y, z, time };
}

struct scale scale_load(ezxml_t node) {
    double x = number_load(node, "x", "xMin", "xMax", 1);
    double y = number_load(node, "y", "yMin", "yMax", 1);
    double z = number_load(node, "z", "zMin", "zMax", 1);

    return (struct scale) { x, y, z };
}

struct point *point_load(ezxml_t node, int *npoints) {
    if (!node) return NULL;

    struct point *p = calloc(1, sizeof(struct point));
    (*npoints)++;

    p->x = number_load(node, "x", "xMin", "xMax", 0);
    p->y = number_load(node, "y", "yMin", "yMax", 0);
    p->z = number_load(node, "z", "zMin", "zMax", 0);
    p->next = point_load(node->next, npoints);

    return p;
}

struct translation translation_load(ezxml_t node) {
    double time = number_load(node, "time", "timeMin", "timeMax", 0);
    double x = number_load(node, "x", "xMin", "xMax", 0);
    double y = number_load(node, "y", "yMin", "yMax", 0);
    double z = number_load(node, "z", "zMin", "zMax", 0);
    int npoints = 0;
    struct point *points = point_load(ezxml_child(node, "point"), &npoints);

    return (struct translation) { time, x, y, z, npoints, points };
}

struct transformation *transformations_load(ezxml_t node) {
    if (!node) return NULL;

    struct transformation *transformation = NULL;

    if (strcmp("rotate", ezxml_name(node)) == 0) {
        transformation = calloc(1, sizeof(struct transformation));
        transformation->type = 'r';
        transformation->rotation = rotation_load(node);
        transformation->next = transformations_load(node->ordered);
    } else if (strcmp("scale", ezxml_name(node)) == 0) {
        transformation = calloc(1, sizeof(struct transformation));
        transformation->type = 's';
        transformation->scale = scale_load(node);
        transformation->next = transformations_load(node->ordered);
    } else if (strcmp("translate", ezxml_name(node)) == 0) {
        transformation = calloc(1, sizeof(struct transformation));
        transformation->type = 't';
        transformation->translation = translation_load(node);
        transformation->next = transformations_load(node->ordered);
    } else {
        transformation = transformations_load(node->ordered);
    }

    return transformation;
}

GLuint texture_load(const char *path) {
    unsigned int t, tw, th;
    unsigned char *texData;
    unsigned int texID;

    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage(path);
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}

struct model *model_load(ezxml_t node) {
    if (!node) return NULL;

    // Read the file containing the vertices description list
    FILE *fp = fopen(ezxml_attr(node, "file"), "r");
    int nvertices = 0;
    fscanf(fp, "%d\n", &nvertices);
    double vertices[3 * nvertices], normals[3 * nvertices], textures[2 * nvertices];

    for (int i = 0, j = 0, k = 0; i < nvertices; i++, j+=3, k+=2) {
        fscanf(fp, "%lf %lf %lf, %lf %lf %lf, %lf %lf\n",
               vertices + j, vertices + j + 1, vertices + j + 2,
               normals + j, normals + j + 1, normals + j + 2,
               textures + k, textures + k + 1);
    }

    fclose(fp);

    // Create and assign the buffers
    struct model *model = calloc(1, sizeof(struct model));
    model->nvertices = nvertices;

    if (ezxml_attr(node, "texture")) {
        model->texture_id = texture_load(ezxml_attr(node, "texture"));
    }

    // Vertices buffer
    glGenBuffers(1, &(model->vertices));
    glBindBuffer(GL_ARRAY_BUFFER, model->vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Normals buffer
    glGenBuffers(1, &(model->normals));
    glBindBuffer(GL_ARRAY_BUFFER, model->normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    // Texture buffer
    glGenBuffers(1, &(model->texture));
    glBindBuffer(GL_ARRAY_BUFFER, model->texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);

    // Load next model
    model->next = model_load(node->next);

    return model;
}

struct group *group_load(ezxml_t node) {
    if (!node) return NULL;

    struct group *group = calloc(1, sizeof(struct group));

    // Load identifier
    strcpy(group->id, ezxml_attr(node, "id") ?: "NO_ID");

    // Load the models
    group->models = model_load(ezxml_get(node, "models", 0, "model", -1));

    // Load transformations
    group->transformations = transformations_load(node->child);

    // Load child
    group->child = group_load(ezxml_child(node ,"group"));

    // Load repetitions and next siblings
    if (ezxml_attr(node, "repeat") && atoi(ezxml_attr(node, "repeat")) > 1) {
        int repeat = atoi(ezxml_attr(node, "repeat"));
        char buf[64];
        snprintf(buf, 64, "%d", repeat - 1);
        ezxml_set_attr(node, "repeat", buf);

        group->sibling = group_load(node);
    } else {
        group->sibling = group_load(node->next);
    }

    return group;
}

struct scene scene_load(FILE *fp) {
    struct scene scene = {};
    ezxml_t tree = ezxml_parse_fp(fp);

    scene.root = group_load(ezxml_child(tree, "group"));

    ezxml_free(tree);

    return scene;
}