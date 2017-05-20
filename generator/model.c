#include "model.h"

struct model *model_new(size_t nvertices, struct vertex *vertices) {
    size_t vertices_s = nvertices * sizeof(struct vertex);
    struct model *model = malloc(sizeof(struct model) + vertices_s);

    if (model) {
        model->nvertices = nvertices;

        if (vertices) {
            memcpy(model->vertices, vertices, vertices_s);
        }
    }

    return model;
}

struct model *model_new_square(double width, int divisions) {
    assert(divisions >= 0);

    struct model *model = model_new(2, NULL);

    return model;
}

struct model *model_new_box(double width, double height, double depth,
                            int divisions) {
    return NULL;
}

struct vertex ellipsoid_param(double r1, double r2, double r3,
                              double theta, double phi) {
    double px = r1 * cos(theta) * sin(phi);
    double py = r2 * sin(theta);
    double pz = r3 * cos(theta) * cos(phi);

    double nx = cos(theta) * sin(phi);
    double ny = sin(theta);
    double nz = cos(theta) * cos(phi);

    double s = (-atan2(-px, pz) + M_PI) / (2 * M_PI);
    double t = ((-py / r2) + 1)/2;

    return (struct vertex) { px, py, pz, nx, ny, nz, s, t };
}

struct model *model_new_ellipsoid(double r1, double r2, double r3,
                                  int stacks, int slices) {
    struct model *m = model_new(2 * 3 * stacks * slices, NULL);

    if (!m) {
        return NULL;
    }

    size_t i = 0;
    double theta_step = M_PI / stacks, phi_step = 2 * M_PI / slices;

    for (double theta = -M_PI_2; theta < M_PI_2; theta += theta_step) {
        for (double phi = -M_PI; phi < M_PI; phi += phi_step) {
            m->vertices[i++] = ellipsoid_param(r1, r2, r3, theta, phi);
            m->vertices[i++] = ellipsoid_param(r1, r2, r3, theta, phi + phi_step);
            m->vertices[i++] = ellipsoid_param(r1, r2, r3, theta + theta_step, phi + phi_step);

            m->vertices[i++] = ellipsoid_param(r1, r2, r3, theta, phi);
            m->vertices[i++] = ellipsoid_param(r1, r2, r3, theta + theta_step, phi + phi_step);
            m->vertices[i++] = ellipsoid_param(r1, r2, r3, theta + theta_step, phi);
        }
    }

    return m;
}

struct model *model_new_sphere(double radius, int stacks, int slices) {
    return model_new_ellipsoid(radius, radius, radius, stacks, slices);
}

/* Xico*/

struct vertex **lerPatches(FILE *fp, int *numero) {
    int npatches;
    fscanf(fp, "%d\n", &npatches);
    *numero = npatches;
    int indices[npatches][16];

    for (int i = 0; i < npatches; i++) {
        fscanf(fp, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
               &indices[i][0], &indices[i][1], &indices[i][2], &indices[i][3],
               &indices[i][4], &indices[i][5], &indices[i][6], &indices[i][7],
               &indices[i][8], &indices[i][9], &indices[i][10], &indices[i][11],
               &indices[i][12], &indices[i][13], &indices[i][14], &indices[i][15]);
    }

    int npoints;
    fscanf(fp, "%d\n", &npoints);

    struct vertex *vertices = malloc(sizeof(struct vertex) * npoints);

    for (int i = 0; i < npoints; i++) {
        fscanf(fp, " %lf, %lf, %lf\n", &vertices[i].position_x, &vertices[i].position_y, &vertices[i].position_z);
    }


    struct vertex **patches = malloc(sizeof(struct vertex *) * npatches);

    for (int i = 0; i < npatches; i++) {
        patches[i] = malloc(sizeof(struct vertex) * 16);
        for (int j = 0; j < 16; j++) {
            patches[i][j] = vertices[indices[i][j]];
        }
    }

    return patches;
}

void multUmQuatro(double A[4], double B[16], double C[4]) {
    C[0] = B[0] * A[0] + B[4] * A[1] + B[8] * A[2] + B[12] * A[3];
    C[1] = B[1] * A[0] + B[5] * A[1] + B[9] * A[2] + B[13] * A[3];
    C[2] = B[2] * A[0] + B[6] * A[1] + B[10] * A[2] + B[14] * A[3];
    C[3] = B[3] * A[0] + B[7] * A[1] + B[11] * A[2] + B[15] * A[3];
}

double B(double u, double v, double P[16]) {
    double M[16] = {
            -1, 3, -3, 1,
            3, -6, 3, 0,
            -3, 3, 0, 0,
            1, 0, 0, 0
    };
    double U[4] = {u * u * u, u * u, u, 1};
    double V[4] = {v * v * v, v * v, v, 1};
    double UM[4];
    multUmQuatro(U, M, UM);
    double UMP[4];
    multUmQuatro(UM, P, UMP);
    double UMPM[4];
    multUmQuatro(UMP, M, UMPM);
    double final = (UMPM[0] * v * v * v) + (UMPM[1] * v * v) + (UMPM[2] * v) + UMPM[3];

    return final;
}

struct model *bezier(int tessellation, struct vertex **patches, int npatches) {
    struct model *m = model_new(2 * 3 * tessellation * tessellation * npatches, NULL);
    double step = 1 / (double) tessellation;

    for (int i = 0, j = 0; i < npatches; i++) {
        double Px[16] = {
                patches[i][0].position_x, patches[i][1].position_x, patches[i][2].position_x, patches[i][3].position_x,
                patches[i][4].position_x, patches[i][5].position_x, patches[i][6].position_x, patches[i][7].position_x,
                patches[i][8].position_x, patches[i][9].position_x, patches[i][10].position_x,
                patches[i][11].position_x,
                patches[i][12].position_x, patches[i][13].position_x, patches[i][14].position_x,
                patches[i][15].position_x
        };
        double Py[16] = {
                patches[i][0].position_y, patches[i][1].position_y, patches[i][2].position_y, patches[i][3].position_y,
                patches[i][4].position_y, patches[i][5].position_y, patches[i][6].position_y, patches[i][7].position_y,
                patches[i][8].position_y, patches[i][9].position_y, patches[i][10].position_y,
                patches[i][11].position_y,
                patches[i][12].position_y, patches[i][13].position_y, patches[i][14].position_y,
                patches[i][15].position_y
        };
        double Pz[16] = {
                patches[i][0].position_z, patches[i][1].position_z, patches[i][2].position_z, patches[i][3].position_z,
                patches[i][4].position_z, patches[i][5].position_z, patches[i][6].position_z, patches[i][7].position_z,
                patches[i][8].position_z, patches[i][9].position_z, patches[i][10].position_z,
                patches[i][11].position_z,
                patches[i][12].position_z, patches[i][13].position_z, patches[i][14].position_z,
                patches[i][15].position_z
        };


        for (double u = 0; u < 1; u += step) {
            for (double v = 0; v < 1; v += step) {
                struct vertex v1 = {
                        B(u, v, Px),
                        B(u, v, Pz),
                        B(u, v, Py),
                        0, 0, 0, 0, 0
                };
                struct vertex v2 = {
                        B(u + step, v, Px),
                        B(u + step, v, Pz),
                        B(u + step, v, Py),
                        0, 0, 0, 0, 0
                };
                struct vertex v3 = {
                        B(u + step, v + step, Px),
                        B(u + step, v + step, Pz),
                        B(u + step, v + step, Py),
                        0, 0, 0, 0, 0
                };
                struct vertex v4 = {
                        B(u, v + step, Px),
                        B(u, v + step, Pz),
                        B(u, v + step, Py),
                        0, 0, 0, 0, 0
                };

                m->vertices[j++] = v1;
                m->vertices[j++] = v2;
                m->vertices[j++] = v3;
                m->vertices[j++] = v1;
                m->vertices[j++] = v3;
                m->vertices[j++] = v4;
            }
        }
    }
    return m;
}


void model_free(struct model *model) {
    free(model);
}

int model_write(struct model *model, FILE *fp) {
    assert(model != NULL);
    assert(fp != NULL);

    // Generate and write the vertices list
    int r = fprintf(fp, "%zu\n", model->nvertices);

    for (size_t i = 0; i < model->nvertices && r; i++) {
        r = fprintf(fp, "%lf %lf %lf, %lf %lf %lf, %lf %lf\n",
                    model->vertices[i].position_x, model->vertices[i].position_y, model->vertices[i].position_z,
                    model->vertices[i].normal_x, model->vertices[i].normal_y, model->vertices[i].normal_z,
                    model->vertices[i].texture_s, model->vertices[i].texture_t);
    }

    return r;
}