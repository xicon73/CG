//
// Created by Afonso on 16/05/17.
//

#ifndef PROJECT_CAMERA_H
#define PROJECT_CAMERA_H

#include <math.h>

struct camera {
    double x, y, z;
    double alpha, beta, radius;
    double la_x, la_y, la_z;
};

struct camera camera_new(double alpha, double beta, double radius);
struct camera camera_update(struct camera camera);
struct camera camera_rotate(struct camera camera, double angle);
struct camera camera_incline(struct camera camera, double angle);
struct camera camera_zoom(struct camera camera, double level);

#endif //PROJECT_CAMERA_H
