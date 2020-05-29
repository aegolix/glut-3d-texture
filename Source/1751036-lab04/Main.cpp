/*
* OGL02Animation.cpp: 3D Shapes with animation
*/
#include "Shape.h"

/* Global variables */
char title[] = "AnhThu's 3D Objects and Textures";
int refreshMills = 15; // refresh interval in milliseconds [NEW]
Shape* currentShape = NULL;
std::vector<Shape*> shapePool;
int shapeType = 0;
bool wireframeOn = false, clicked = false;
GLfloat* currentX = new GLfloat, * currentY = new GLfloat;
GLfloat* fX = new GLfloat, * fY = new GLfloat;

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f); // Set background depth to farthest
    glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL); // Set the type of depth-test
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_TEXTURE_2D);
}
/* Handler for window-repaint event. Called back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix
    
    if (!clicked) {
        currentShape->render(fX, fY);
    }
    else {
        currentShape->render(currentX, currentY);
    }
    
    glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
    // Update the rotational angle after each refresh [NEW]
}
/* Called back when timer expired [NEW] */
void timer(int value) {
    glutPostRedisplay(); // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}
/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) { // GLsizei for non-negative integer
 // Compute aspect ratio of the new window
    if (height == 0) height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity(); // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(60.0f, aspect, 0.1f, 100.0f);
}

void cleanUp() {

    for (int i = 0; i < shapePool.size(); ++i) {
        if (shapePool[i]) delete shapePool[i];
    }
}

void mainMenuHandler(int choice) {
    switch (choice) {
    case -1:
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        shapeType = choice;
        break;

    case 8:	// Exit
        std::cout << "Program is terminated, release memory" << std::endl;
        cleanUp();
        exit(0);
        break;
    }
}

void renderModeMenu(int choice) {
    switch (choice) {
    case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }
}

Shape* getShape() {
    Shape* r = NULL;
    switch (shapeType)
    {
    case -1:
        r = new Flat();
        break;
    case 0:
        r = new Cube();
        break;
    case 1:
        r = new Pyramid();
        break;
    case 2:
        r = new Sphere();
        break;
    case 3:
        r = new Cylinder();
        break;
    case 4:
        r = new Cone();
        break;
    case 5:
        r = new Torus();
        break;
    case 6:
        r = new Paraboloid();
        break;
    case 7:
        r = new Hyperboloid();
        break;
    default:
        break;
    }

    if (r)
        shapePool.push_back(r);

    return r;
}

void prepareMenu() {
    int rdm = glutCreateMenu(renderModeMenu);
    glutAddMenuEntry("Wireframe", 0);
    glutAddMenuEntry("Solid", 1);


    glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Circular PLane", -1);
    glutAddMenuEntry("Cube", 0);
    glutAddMenuEntry("Pyramid", 1);
    glutAddMenuEntry("Sphere", 2);
    glutAddMenuEntry("Cylinder", 3);
    glutAddMenuEntry("Cone", 4);
    glutAddMenuEntry("Torus", 5);
    glutAddMenuEntry("Paraboloid", 6);
    glutAddMenuEntry("Hyperboloid", 7);
    glutAddSubMenu("Render Mode", rdm);
    glutAddMenuEntry("Exit", 8);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void brushing(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        clicked = true;
        *currentX = ((GLfloat) x - WIDTH / 2) / WIDTH * 2 * 5;
        *currentY = ( - (GLfloat) y + HEIGHT/ 2) / HEIGHT * 2 * 5;
        currentShape = getShape();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 32: //ASCII for spacebar
        paused =! paused;
        break;

    case 'w':
    case 'W':
        wireframeOn = !wireframeOn;

        if (wireframeOn) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        break;

    case 'r':
    case 'R':
        resetPaused = !resetPaused;
        break;

    case 'n':
    case 'N':
        clicked = false;
        currentShape = getShape();
        currentShape->render(fX, fY);
        break;
    }
}

void programDescription() {
    std::cout << "WELCOME TO 3D OBJECT TRANSFORMATION AND TEXTURE MAPPING" << std::endl;
    std::cout << "~ Cube, Pyramid, Sphere, Cylinder, Cone, Torus, Paraboloid, Hyperboloid ~" << std::endl << std::endl;
    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%% INSTRUCTION %%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    std::cout << "    It draws Cube by default" << std::endl << std::endl;
    std::cout << "    Right-click menu to choose other shape, then:" << std::endl;
    std::cout << "         Left-click on the windows to generate shape at that (x, y) position, or" << std::endl;
    std::cout << "         Press key n or N to render such new shape at (0, 0, z)" << std::endl << std::endl;
    std::cout << "    To change between WIREFRAME mode and FILL mode:" << std::endl;
    std::cout << "         Right-click menu to choose, or" << std::endl;
    std::cout << "         Press key w or W to toggle" << std::endl << std::endl;
    std::cout << "    To reset objects to its initial rotation" << std::endl;
    std::cout << "         Press key r or R (reset rotation)" << std::endl << std::endl;
    std::cout << "    To pause objects' rotation" << std::endl;
    std::cout << "         Press key space bar to toggle" << std::endl << std::endl;
    std::cout << "    To add more textures" << std::endl;
    std::cout << "         Put .jpg files in Textures/ folder" << std::endl;
    std::cout << "         Update file total-number-of-textures.txt with new number of textures" << std::endl;
    std::cout << "         File name convention: tex<next-id>.jpg, e.g: tex3.jpg, tex22.jpg" << std::endl << std::endl;
    std::cout << "    NOTE:" << std::endl;
    std::cout << "    Shape's Depth, Rotation angle and Dimensions are RANDOMMED" << std::endl;
    std::cout << "    You just need to generate new shape by left-click or press n/N" << std::endl << std::endl << std::endl;

    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%% PROGRAM LOG %%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    programDescription();

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE); // Enable double buffered mode
    glutInitWindowSize(WIDTH, HEIGHT); // Set the window's initial width & height
    glutInitWindowPosition(600, 20); // Position the window's initial top-left corner
    glutCreateWindow(title); // Create window with the given title

    currentShape = new Cube();
    *fX = 0;
    *fY = 0;
    shapePool.push_back(currentShape);
    
    glutDisplayFunc(display); // Register callback handler for window re-paint event
    glutReshapeFunc(reshape); // Register callback handler for window re-size event
    glutMouseFunc(brushing);
    glutKeyboardFunc(keyboard);

    initGL(); // Our own OpenGL initialization
    prepareMenu();
    glutTimerFunc(0, timer, 0); // First timer call immediately [NEW]

    glutMainLoop(); // Enter the infinite event-processing loop

    cleanUp();
    return 0;
}