//
// Created by Afonso on 16/05/17.
//
#include "render.h"

void buildRotMatrix(double *x, double *y, double *z, double m[16]){
    m[0]  = x[0] ; m[1]  = x[1] ; m[2]  = x[2] ; m[3]  = 0;
    m[4]  = y[0] ; m[5]  = y[1] ; m[6]  = y[2] ; m[7]  = 0;
    m[8]  = z[0] ; m[9]  = z[1] ; m[10] = z[2] ; m[11] = 0;
    m[12] = 0    ; m[13] = 0    ; m[14] = 0    ; m[15] = 1;
}

void cross(double *a, double *b, double *res){
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double *a){
    double l = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

double length(double *v){
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void multMatrixVector(double *m, double *v, double *res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void getCatmullRomPoint(double t, double *p0, double *p1, double *p2, double *p3, double *pos, double *deriv) {
    // catmull-rom matrix
    double m[4][4] = {{-0.5f, 1.5f,  -1.5f, 0.5f},
                      {1.0f,  -2.5f, 2.0f,  -0.5f},
                      {-0.5f, 0.0f,  0.5f,  0.0f},
                      {0.0f,  1.0f,  0.0f,  0.0f}};

    double px[4] = {p0[0], p1[0], p2[0], p3[0]};
    double py[4] = {p0[1], p1[1], p2[1], p3[1]};
    double pz[4] = {p0[2], p1[2], p2[2], p3[2]};
    double ax[4];
    double ay[4];
    double az[4];

    // Compute A = M * P
    multMatrixVector((double *) m, (double *) px, ax);
    multMatrixVector((double *) m, (double *) py, ay);
    multMatrixVector((double *) m, (double *) pz, az);

    // Compute pos = T * A
    double mT[4] = {pow(t, 3), pow(t, 2), t, 1};
    pos[0] = mT[0] * ax[0] + mT[1] * ax[1] + mT[2] * ax[2] + mT[3] * ax[3];
    pos[1] = mT[0] * ay[0] + mT[1] * ay[1] + mT[2] * ay[2] + mT[3] * ay[3];
    pos[2] = mT[0] * az[0] + mT[1] * az[1] + mT[2] * az[2] + mT[3] * az[3];

    // compute deriv = T' * A
    double mTl[4] = {3 * pow(t, 2), 2 * t, 1, 0};
    deriv[0] = mTl[0] * ax[0] + mTl[1] * ax[1] + mTl[2] * ax[2] + mTl[3] * ax[3];
    deriv[1] = mTl[0] * ay[0] + mTl[1] * ay[1] + mTl[2] * ay[2] + mTl[3] * ay[3];
    deriv[2] = mTl[0] * az[0] + mTl[1] * az[1] + mTl[2] * az[2] + mTl[3] * az[3];
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(double gt, double *pos, double *deriv, double p[][3], int POINT_COUNT) {

    double t = gt * POINT_COUNT; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
    indices[1] = (indices[0] + 1) % POINT_COUNT;
    indices[2] = (indices[1] + 1) % POINT_COUNT;
    indices[3] = (indices[2] + 1) % POINT_COUNT;

    getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}


void render_axis(double length) {
    glBegin(GL_LINES);

    // X Axis
    glColor3f(1.0, 0.0, 0.0);
    glVertex3d(length, 0.0, 0.0);
    glVertex3d(-length, 0.0, 0.0);

    // Y Axis
    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, -length, 0.0);
    glVertex3d(0.0, length, 0.0);

    // Z Axis
    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, -length);
    glVertex3d(0.0, 0.0, length);

    glColor3d(1.0, 1.0, 1.0);
    glEnd();
}

void render_catmull_curve(struct point *points, int npoints) {
    // desenhar a curva usando segmentos de reta - GL_LINE_LOOP
    double res[3];
    double deriv[3];
    double p[npoints][3];

    for (int i = 0; i < npoints; i++) {
        p[i][0] = points->x; p[i][1] = points->y; p[i][2] = points->z;
        points = points->next;
    }

    double gt = 0.0;
    glBegin(GL_LINE_LOOP);
    while (gt < 1.0) {
        getGlobalCatmullRomPoint(gt, res, deriv, p, npoints);
        glVertex3d(res[0], res[1],res[2]);
        gt+= 0.01;
    }
    glEnd();
}

double TIME = 0;

void render_catmull(struct point *points, int npoints) {
    if (!points) return;

    double t = TIME;
    double res[3];
    double deriv[3];
    double p[npoints][3];

    for (int i = 0; i < npoints; i++) {
        p[i][0] = points->x; p[i][1] = points->y; p[i][2] = points->z;
        points = points->next;
    }

    getGlobalCatmullRomPoint(t, res, deriv, p, npoints);

    glTranslated(res[0],res[1],res[2]);
    glutPostRedisplay();
}

void render_transformation(struct transformation *transformation) {
    if (!transformation) return;

    if (transformation->type == 'r') {              // Rotation
        glRotated(transformation->rotation.angle, transformation->rotation.x,
                  transformation->rotation.y, transformation->rotation.z);
        if (transformation->rotation.time > 0) {
            int time = glutGet(GLUT_ELAPSED_TIME) % transformation->rotation.time;
            double angle = (double) time / transformation->rotation.time * 360.0;
            glRotated(angle, transformation->rotation.x, transformation->rotation.y, transformation->rotation.z);
        }
    } else if (transformation->type == 's') {        // Scale
        glScaled(transformation->scale.x, transformation->scale.y,
                 transformation->scale.z);
    } else if (transformation->type == 't') {        // Translation
        glTranslated(transformation->translation.x,
                     transformation->translation.y,
                     transformation->translation.z);

        if (transformation->translation.npoints > 0) {
            render_catmull_curve(transformation->translation.points, transformation->translation.npoints);
            render_catmull(transformation->translation.points,
                           transformation->translation.npoints);
        }
    }

    render_transformation(transformation->next);
}

void render_model(struct model *model) {
    if (!model) return;

    // Draw the texture
    glBindTexture(GL_TEXTURE_2D, model->texture_id);

    // Draw the vertices
    glBindBuffer(GL_ARRAY_BUFFER, model->vertices);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    // Draw the normals
    glBindBuffer(GL_ARRAY_BUFFER, model->normals);
    glNormalPointer(GL_DOUBLE, 0, 0);

    // Draw the texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, model->texture);
    glTexCoordPointer(2, GL_DOUBLE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, model->nvertices);

    render_model(model->next);
}

void render_group(struct group *group) {
    if (!group) return;

    glPushMatrix();

    render_transformation(group->transformations);
    render_model(group->models);
    render_group(group->child);

    glPopMatrix();
    render_group(group->sibling);
}