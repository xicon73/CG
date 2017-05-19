//
// Created by Afonso on 16/05/17.
//
#include "render.h"

void render_transformation(struct transformation *transformation) {
    if (!transformation) return;

    if (transformation->type == 'r') {              // Rotation
        glRotated(transformation->rotation.angle, transformation->rotation.x,
                  transformation->rotation.y, transformation->rotation.z);
    } else if (transformation->type == 's') {        // Scale
        glScaled(transformation->scale.x, transformation->scale.y,
                 transformation->scale.z);
    } else if (transformation->type == 't') {        // Translation
        glTranslated(transformation->translation.x,
                     transformation->translation.y,
                     transformation->translation.z);
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