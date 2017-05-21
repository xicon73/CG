#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vertex {
    double position_x, position_y, position_z;
    double normal_x, normal_y, normal_z;
    double texture_s, texture_t;
};

struct model {
    size_t nvertices;
    struct vertex vertices[];
};


struct model *model_new(size_t nvertices, struct vertex *vertices);

struct model *model_new_square(double width, int divisions);

struct model *model_new_box(double width, double height, double depth,
                            int divisions);

struct model *model_new_ellipsoid(double r1, double r2, double r3,
                                  int stacks, int slices);

struct model *model_new_sphere(double radius, int stacks, int slices);

struct model *model_new_torus(double radius_1, double radius_2, int stacks, int slices);

struct model *model_new_bezier(int tesselation, FILE *patches);

void model_free(struct model *model);

int model_write(struct model *model, FILE *fp);

struct vertex **lerPatches(FILE *fp, int *numero);

struct model *bezier(int tessellation, struct vertex **patches, int npatches);