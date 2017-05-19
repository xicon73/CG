#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

const char *HELP = "";

int main(int argc, char *argv[]) {
    struct model *m = NULL;

    // Parse command line arguments
    if (argc >= 5 && strcmp("square", argv[1]) == 0) {
        double width = atof(argv[2]);
        int divisions = atoi(argv[3]);

        m = model_new_square(width, divisions);
    } else if (argc >= 7 && strcmp("box", argv[1]) == 0) {
        double width = atof(argv[2]);
        double height = atof(argv[3]);
        double depth = atof(argv[4]);
        int divisions = atoi(argv[5]);

        m = model_new_box(width, height, depth, divisions);
    } else if (argc >= 6 && strcmp("sphere", argv[1]) == 0) {
        double radius = atof(argv[2]);
        int stacks = atoi(argv[3]);
        int slices = atoi(argv[4]);

        m = model_new_sphere(radius, stacks, slices);
    } else if (argc > 10 && strcmp("cone", argv[1]) == 0) {

    } else if (argc > 10 && strcmp("cylinder", argv[1]) == 0) {

    } else if (argc >= 6 && strcmp("torus", argv[1]) == 0) {
        double inner_radius = atof(argv[2]);
        double out_radius = atof(argv[3]);
        int stacks = atoi(argv[4]);

        //m = model_torus(inner_radius, out_radius, stacks);
    } else if (argc >= 8 && strcmp("ellipsoid", argv[1]) == 0) {
        double r1 = atof(argv[2]);
        double r2 = atof(argv[3]);
        double r3 = atof(argv[4]);
        int stacks = atoi(argv[5]);
        int slices = atoi(argv[6]);

        m = model_new_ellipsoid(r1, r2, r3, stacks, slices);
    } else if (argc >= 10 && strcmp("super_ellipsoid", argv[1]) == 0) {
        double width = atof(argv[2]);
        double height = atof(argv[3]);
        double depth = atof(argv[4]);
        double r = atof(argv[5]);
        double t = atof(argv[6]);
        int stacks = atoi(argv[7]);
        int slices = atoi(argv[8]);

        //m = model_super_ellipsoid(width, height, depth, r, t, stacks, slices);
    } else if (strcmp("bezier", argv[1]) == 0) {
        int tessellation = atoi(argv[2]);
        FILE *fp = fopen(argv[3], "r");

        if (fp) {
            int npatches = 0;
            struct vertex **patches = lerPatches(fp, &npatches);
            m = bezier(tessellation, patches, npatches);
            fclose(fp);
        }
    } else {
        fprintf(stderr, "%s", HELP);
        exit(-1);
    }

    if (!m) {
        fprintf(stderr, "The requested model could not be generated.\n");
        exit(-1);
    }

    // Write the model data to the file
    const char *filename = argv[argc - 1];
    FILE *fp = fopen(filename, "w");

    if (fp) {
        int r = model_write(m, fp);

        // Release the acquired resources
        //free(m);
        //fclose(fp);

        if (r < 0) {
            fprintf(stderr, "Failed to write the model data\n");
            exit(-1);
        }
    } else {
        fprintf(stderr, "The requested file '%s' could not be opened\n",
                filename);
        free(m);
        exit(-1);
    }

    return 0;
}