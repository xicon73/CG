#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "scene.h"

struct scene SCENE = {};
float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

float alpha = 0, beta = 35, r = 10;


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

    gluLookAt(camX, camY, camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    render_light(SCENE.lights);
    render_group(SCENE.root);

    // Reset texture loading
    glBindTexture(GL_TEXTURE_2D, 0);

    // Update FPS
    static int frame = 0;
    static double time = 0, timebase = 0;
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);

    if (time - timebase > 1000) {
        char s[64];
        sprintf(s,"FPS:%4.2f", frame * 1000.0 / (time - timebase));
        timebase = time;
        frame = 0;
        glutSetWindowTitle(s);
    }

    glutSwapBuffers();
}

// Mouse and keyborad functions
void process_keys(unsigned char key, int x, int y) {
    glutPostRedisplay();
}

void process_special_keys(int key, int x, int y) {
    glutPostRedisplay();
}

void process_mouse_buttons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else { // Middle button
            tracking = 0;
            /*picked = picking(xx,yy);

            if (picked)
                printf("Picked Snowman number %d\n", picked);
            else
                printf("Nothing selected\n");*/
            glutPostRedisplay();
        }
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}


void process_mouse_motion(int xx, int yy) {
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {
        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * 							     sin(betaAux * 3.14 / 180.0);

    glutPostRedisplay();
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

    camX = r * sin(alpha * 3.14 / 180.0) * cos(beta * 3.14 / 180.0);
    camZ = r * cos(alpha * 3.14 / 180.0) * cos(beta * 3.14 / 180.0);
    camY = r * 							   sin(beta * 3.14 / 180.0);

    glClearColor(0, 0, 0, 0);

    glEnable(GL_LIGHTING);
  //  glEnable(GL_LIGHT0);
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

    glutMainLoop();

    return 0;
}