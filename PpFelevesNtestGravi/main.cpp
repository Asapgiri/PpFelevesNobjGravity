#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <GL/glu.h>
//#include <GL/glut.h>

#include <corecrt_math_defines.h>
#include <chrono>
#include <iostream>
#include <time.h>
#include <ctime>
#include "nobjp.h"


#define RENDER_MS   20000
#define OBJ_CNT     4096


#define millis() std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

void RenderVectorsToFrame(Nobj* nobj);

int main()
{
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // glfw: initialize and configure
    // ------------------------------
    //glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "N-bject problem", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    //{
    //    std::cout << "Failed to initialize GLAD" << std::endl;
    //    return -1;
    //}

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment


    // Project work N object gravity ...
    // ---------------------------------
    Nobj nobj(SCREEN_WIDTH, SCREEN_HEIGHT, OBJ_CNT);
    std::cout << "Created NOBJ with " << nobj.N() << " elements." << std::endl;
    std::cout << "Starting details of elements:" << std::endl;
    MyObj* obj = nobj.GetObjects();
    for (int i = 0; i < nobj.N(); i++) {
        printf("P(x:%7.2f y:%7.2f z:%7.2f)   O|m:%9.2f s:%6.2f|   "
               "v(x:%7.2f y:%7.2f z:%7.2f)\n", 
               obj[i].pos.x, obj[i].pos.y, obj[i].pos.z, obj[i].m,   obj[i].s,
               obj[i].v.x,   obj[i].v.y,   obj[i].v.z);
    }

    double dt, fps;
    long t, old_t, new_ts;
    long s_time;
    long cnt_dt;
    double sum_dt, avg_dt;

    t      = millis();
    old_t  = t;
    new_ts = t + 100;
    s_time = t + RENDER_MS;
    sum_dt = 0;
    cnt_dt = 0;

    std::cout << std::endl;

#pragma region defines
#define CALC_TIME()                                         \
    t = millis();                                           \
    dt = (t - old_t) / 1000.0;                              \
    sum_dt += dt;                                           \
    cnt_dt++;                                               \
    if (t > new_ts) {                                       \
        fps = 1 / dt;                                       \
        printf("\rdt = %fs    (~%.2f fps)    ", dt, fps);   \
        new_ts = t + 100;                                   \
    }                                                       \
    old_t = t


    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);               
#define RENDER_FRAME()                                  \
    glClear(GL_COLOR_BUFFER_BIT);                       \
    RenderVectorsToFrame(&nobj);                        \
    glfwSwapBuffers(window);                            \
    glfwPollEvents()
    
    int s = 0;
#define STATISTICS(threads)             \
    avg_dt = sum_dt / cnt_dt;           \
    printf("\r <%d> => Render average dt = %fs    avg(~%.2f fps)   frame count: %d   (%d threads)\n", ++s, avg_dt, 1.0 / avg_dt, cnt_dt, threads);  \
    t = millis();                       \
    old_t = t -10;                      \
    new_ts = t + 100;                   \
    s_time = t + RENDER_MS;             \
    sum_dt = 0;                         \
    cnt_dt = 0
#pragma endregion defines


    // render loop 1    -- classic 1 sequential
    // -----------
    while (!glfwWindowShouldClose(window) && t < s_time)
    {
        CALC_TIME();
    
        // Process NOBJ data
        nobj.ProgressAll(dt);
    
        RENDER_FRAME();
    }
    nobj.CreateThreads(2);
    STATISTICS(1);


    // render loop 2    -- render on N threads
    // -----------
    while (!glfwWindowShouldClose(window) && t < s_time)
    {
        CALC_TIME();

        // Process NOBJ data
        nobj.ProgressOnThreads(dt);

        RENDER_FRAME();
    }
    nobj.StopThreads();
    nobj.CreateThreads(4);
    STATISTICS(2);


    // render loop 3    -- render on N threads
    // -----------
    while (!glfwWindowShouldClose(window) && t < s_time)
    {
        CALC_TIME();

        // Process NOBJ data
        nobj.ProgressOnThreads(dt);

        RENDER_FRAME();
    }
    nobj.StopThreads();
    nobj.CreateThreads(8);
    STATISTICS(4);


    // render loop 4    -- render on N threads
    // -----------
    while (!glfwWindowShouldClose(window) && t < s_time)
    {
        CALC_TIME();

        // Process NOBJ data
        nobj.ProgressOnThreads(dt);

        RENDER_FRAME();
    }
    nobj.StopThreads();
    nobj.CreateThreads(16);
    STATISTICS(8);


    // render loop 5    -- render on N threads
    // -----------
    while (!glfwWindowShouldClose(window) && t < s_time)
    {
        CALC_TIME();

        // Process NOBJ data
        nobj.ProgressOnThreads(dt);

        RENDER_FRAME();
    }
    nobj.StopThreads();
    nobj.CreateThreads(32);
    STATISTICS(16);


    // render loop 6    -- render on N threads
    // -----------
    while (!glfwWindowShouldClose(window) && t < s_time)
    {
        CALC_TIME();

        // Process NOBJ data
        nobj.ProgressOnThreads(dt);

        RENDER_FRAME();
    }
    nobj.StopThreads();
    STATISTICS(32);








    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
    int numberOfVertices = numberOfSides + 2;

    GLfloat twicePi = 2.0f * M_PI;

    GLfloat *circleVerticesX = new GLfloat[numberOfVertices];
    GLfloat *circleVerticesY = new GLfloat[numberOfVertices];
    GLfloat *circleVerticesZ = new GLfloat[numberOfVertices];

    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = z;

    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
        circleVerticesZ[i] = z;
    }

    GLfloat *allCircleVertices = new GLfloat[(numberOfVertices) * 3];

    for (int i = 0; i < numberOfVertices; i++)
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState(GL_VERTEX_ARRAY);

    delete[] circleVerticesX;
    delete[] circleVerticesY;
    delete[] circleVerticesZ;
    delete[] allCircleVertices;
}
void RenderVectorsToFrame(Nobj* nobj) {
    MyObj* obj = nobj->GetObjects();

    for (int i = 0; i < nobj->N(); i++) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, obj[i].allCircleVertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTICES);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}