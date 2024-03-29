#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

// Constants
const double g = 9.81; // Acceleration due to gravity (m/s^2)
const double theta = 0.785398; // Launch angle (45 degrees)
const double v0 = 22.5; // Initial velocity (m/s)

// Function to calculate y-coordinate given x-coordinate
double calculateY(double x) 
{
    return x * tan(theta) - (g * x * x) / (2 * (v0 * cos(theta)) * (v0 * cos(theta)));      //Trajectory Equation
}

// Function to draw the trajectory
void drawTrajectory() 
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3.0);
    glBegin(GL_LINES);
    // Draw X and Y axis
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(-60.0, 0.0);
    glVertex2f(60.0, 0.0);
    glVertex2f(0.0, 60.0);
    glVertex2f(0.0,-60.0);
    glEnd();

    //Draw Trajectory of Object
    glColor3f(0.2, 0.5, 0.9);
    glLineWidth(7.0);
    float prev_x =-1;
    float prev_y = 0;
    glBegin(GL_LINE_STRIP);

    for (double x = 0; x <= 52; x += 0.1) {
        glVertex2d(x, calculateY(x));
    }
    glEnd();

    glfwSwapBuffers(glfwGetCurrentContext());
}

// Function to handle window resizing
void handleResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspectRatio = static_cast<double>(width) / static_cast<double>(height);
    if (aspectRatio > 1.0) {
        glOrtho(0.0, 40.0 * aspectRatio, -5.0 * aspectRatio, 25.0 * aspectRatio, -1.0, 1.0);
    }
    else {
        glOrtho(0.0, 40.0, -5.0 / aspectRatio, 25.0 / aspectRatio, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Projectile Motion Trajectory", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the callback functions
    glfwSetFramebufferSizeCallback(window, handleResize);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        drawTrajectory();

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}