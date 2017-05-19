//
// Created by Afonso on 16/05/17.
//

#include "camera.h"

struct camera camera_new(double alpha, double beta, double radius) {
    return camera_update((struct camera) { 0, 0, 0, alpha, beta, radius });
}

struct camera camera_update(struct camera camera) {
    return (struct camera) {
            camera.radius * sin(camera.alpha) * cos(camera.beta),
            camera.radius * sin(camera.beta),
            camera.radius * cos(camera.alpha) * cos(camera.beta),
            camera.alpha, camera.beta, camera.radius,
            camera.la_x, camera.la_y, camera.la_z
    };
}

struct camera camera_rotate(struct camera camera, double angle) {
    return camera_update((struct camera) { camera.x, camera.y, camera.z,
                                           camera.alpha + angle,
                                           camera.beta, camera.radius,
                                           camera.la_x, camera.la_y, camera.la_z });
}

struct camera camera_incline(struct camera camera, double angle) {
    double beta = camera.beta + angle > 1.5 ? 1.5 :
                  camera.beta + angle < -1.5 ? -1.5 : camera.beta + angle;
    return camera_update((struct camera) { camera.x, camera.y, camera.z,
                                           camera.alpha, beta, camera.radius,
                                           camera.la_x, camera.la_y, camera.la_z });
}

struct camera camera_zoom(struct camera camera, double level) {
    double radius = camera.radius + level < 0.1 ? 0.1 : camera.radius + level;
    return camera_update((struct camera) { camera.x, camera.y, camera.z,
                                           camera.alpha, camera.beta, radius,
                                           camera.la_x, camera.la_y, camera.la_z});
}