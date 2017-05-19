#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "render.h"
#include "scene.h"

struct scene SCENE = {};
struct camera CAMERA = {};

void change_size(int w, int h) {
    float ratio;

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    ratio = 1.0f * w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the clipping volume
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void render_scene() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(CAMERA.x, CAMERA.y, CAMERA.z,
              CAMERA.la_x, CAMERA.la_y, CAMERA.la_z,
              0.0, 1.0, 0.0);

    // Light
    GLfloat amb[4] =  { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diff[4] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat posl[4] = { 0.0, 0.0 ,1.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, posl);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

    // Material
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (float[]) { 1, 1, 1, 1 });

    render_group(SCENE.root);

    // Reset texture loading
    glBindTexture(GL_TEXTURE_2D, 0);
    glutSwapBuffers();
}

// Mouse and keyborad functions
void process_keys(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            CAMERA = camera_zoom(CAMERA, -0.1);
            break;
        case 's':
            CAMERA = camera_zoom(CAMERA, +0.1);
            break;
        case 'a':
            CAMERA = camera_rotate(CAMERA, -0.1);
            break;
        case 'd':
            CAMERA = camera_rotate(CAMERA, +0.1);
            break;
        case '+':
            CAMERA = camera_zoom(CAMERA, -0.5);
            break;
        case '-':
            CAMERA = camera_zoom(CAMERA, +0.5);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void process_special_keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            CAMERA = camera_rotate(CAMERA, -0.1);
            break;
        case GLUT_KEY_RIGHT:
            CAMERA = camera_rotate(CAMERA, 0.1);
            break;
        case GLUT_KEY_UP:
            CAMERA = camera_incline(CAMERA, 0.1);
            break;
        case GLUT_KEY_DOWN:
            CAMERA = camera_incline(CAMERA, -0.1);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void process_mouse_buttons(int button, int state, int x, int y) {

}

void process_mouse_motion(int x, int y) {
}

int main(int argc, char *argv[]) {
    // Start the OpenGL engine
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(512,512);
    glutCreateWindow(argv[1]);

    glutDisplayFunc(render_scene);
    glutReshapeFunc(change_size);

    #ifndef __APPLE__
	glewInit();
    #endif

    // InitGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glClearColor(0, 0, 0, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    // Init DEVIL
    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    // Callback functions
    glutKeyboardFunc(process_keys);
    glutSpecialFunc(process_special_keys);
    glutMouseFunc(process_mouse_buttons);
    glutMotionFunc(process_mouse_motion);

    // Load the SCENE from XML
    if (argc < 2) {
        fprintf(stderr, "You must specify a SCENE to read.\n");
        exit(-1);
    }

    FILE *fp = fopen(argv[1], "r");

    if (!fp) {
        fprintf(stderr, "Could not open the file: '%s'\n", argv[1]);
        exit(-1);
    }

    SCENE = scene_load(fp);
    CAMERA = camera_new(0, 35, 10);

    glutMainLoop();

    return 0;
}