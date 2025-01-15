#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>

// Screen dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Ripple parameters
float timeStep = 0.5f;  // Increased time step for faster ripple propagation
float damping = 0.98f;   // Reduced damping to let ripples last longer
std::vector<std::vector<float>> heightMap(WIDTH, std::vector<float>(HEIGHT, 0.0f));
std::vector<std::vector<float>> velocity(WIDTH, std::vector<float>(HEIGHT, 0.0f));

void applyRipple(int x, int y, float strength) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    heightMap[x][y] += strength;
}

void updateRipple() {
    std::vector<std::vector<float>> newHeightMap = heightMap;
    for (int x = 1; x < WIDTH - 1; ++x) {
        for (int y = 1; y < HEIGHT - 1; ++y) {
            // Calculate the Laplacian (neighboring differences)
            float laplacian = heightMap[x - 1][y] + heightMap[x + 1][y] +
                              heightMap[x][y - 1] + heightMap[x][y + 1] - 4 * heightMap[x][y];
            velocity[x][y] += laplacian * timeStep;
            velocity[x][y] *= damping; // Apply damping to slow down the ripple gradually
            newHeightMap[x][y] += velocity[x][y];
        }
    }
    heightMap = newHeightMap;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            float h = heightMap[x][y];
            glColor3f(0.0f, 0.0f, 0.5f + h); // Blue intensity based on height
            glVertex2f((float)x / WIDTH, (float)y / HEIGHT); // Proper scaling for OpenGL coordinates
        }
    }
    glEnd();
    glutSwapBuffers();
}

void idle() {
    updateRipple();
    glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Invert y-coordinate to match OpenGL's coordinate system
        applyRipple(x, HEIGHT - y - 1, 4.0f); // Increased ripple strength
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Water Ripple Effect");
    glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f); // Ortho projection
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouseFunc);
    glutMainLoop();
    return 0;
}
