/*******************************************************************
 * SideFogCube:  A class to display a cloud of randomly
 * spaced, randomly spinning cubes with or without fog.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * January 2020 San Diego, California USA
 * ****************************************************************/
#ifndef SIDEFOGCUBE_H
#define SIDEFOGCUBE_H

#include "commonheader.h"
#include "shader.h"
#include "createimage.h"
#include "camera.h"
#include "uniformprinter.h"

/** \class SideFogCube 
 * The class that creates a cloud of 
 * randomly spinning cubes in a fog or not.
 */
class SideFogCube : CL_ClanApplication
{
public:
    SideFogCube();
    ~SideFogCube();
    
    /** \brief main
     * The ClanLib internal main.
     */
    virtual int main(int argc, char **argv);
private:

    /** ClanLib variables for managing the window
     * and providing an environment for OpenGL.
     */
    CL_OpenGLWindow *window;
    CL_InputContext *progIC;
    CL_InputDevice *keyboardID;
    CL_InputDevice *mouseID;
    CL_OpenGLState *glState;
    
    //! The Shader class to manage the shaders.
    Shader *shader, *skyBoxShader;
    
    //! The CreateImage class to create textures.
    CreateImage *image;
    
    //! The Camera class to manage frustum and camera
    //! movement.
    Camera *camera;
    
    /** \brief debug
     * Allows for examination of the generated
     * cube data.
     */
    void debug();
    
    /** \brief permLoc 
     * Creates the location, orientation, spin and 
     * image index information.
     */
    void permLoc();
    
    /** \brief genMatrices 
     * Generates the cube object.
     */
    void genMatrices();
    
    /** \brief genTexture 
     * Generates the texture coodinates.
     */
    vec2 genTexture(vec3 normal, vec3 triangle);
    
    /** \brief frameBufferSize 
     * Calls the OpenGl function to size the framebuffer.
     */
    void framebufferSize(int width, int height);
    
    /** \brief processInput
     * Closes the program.
     */
    void processInput(const CL_InputEvent &key);
    
    /** \brief keyDown
     * Handles key presses.
     */
    void keyDown(const CL_InputEvent &key);
    
    /** \brief mouseMove
     * Handles mouse motion.
     */
	void mouseMove(const CL_InputEvent &key);
    
    /** \brief windowClose
     * Handles when you click the little box one the corner
     * of the window.  But this program is set to be fullscreen.
     */
    void windowClose();
    
    /** \brief calcRand
     * Calculates a random variable to be part of a 
     * location coordinate for a cube.
     */
    float calcRand(int x);
    
    /** \brief randAxis
     * Calculates a random axis for the cube to spin on.
     */
    float randAxis();
    
    /** \brief sortDists
     * Sorts the distances from the camera so the furthest
     * are drawn first and the nearest are drawn last.
     * Also packs the data arrays going to the shaders.
     */
    void sortDists(int degrees);
    
    /** \brief cmp
     * A function to define what is greater than and 
     * what is less than for our data structure (sorted 
     * on distance).
     */
    static bool cmp(const PosOrient &a, const PosOrient &b);
    //! settings
    //! Change this to suite your monitor.
    const unsigned int SCR_WIDTH = 1280;
    const unsigned int SCR_HEIGHT = 1024;
    //! We have a light at each corner of the cloud of cubes.
    static const unsigned int NUM_LIGHTS = 8;
    //! Various booleans.
    bool quit, add, firstMouse, foggy = true;
    string value;
    vec3 viewPos;
    const vec3 initPos = vec3(0.0f, 0.0f, 20.0f);
    mat4 model, view, projection;
    float xpos, ypos, lastX, lastY;
    //! For  conversion from degrees to radians.
    const float onedegree = (float) acos(-1) / 180.0f;
    unsigned int VBO[4], VAO, minfog = 0.1f, maxfog = 25.0f; 
    unsigned int texture1, texture2, dataIndex;
    CL_Slot slot_quit, slot_input_up, slot_input_down, 
    slot_mouse, slot_roll;
    //! Initialize ClanLib base components
    CL_SetupCore *setup_core;

    //! Initialize the ClanLib display component
    CL_SetupDisplay *setup_display;

    //! Initialize the OpenGL drivers
    CL_SetupGL *setup_gl;
    CL_OpenGLState *gl_state;
    //! Define the lights for the shaders.
    Lights lighting[NUM_LIGHTS];
    //! A vector to hold the array of location data.
    vector<PosOrient>distVals;
    /** The individual arrays that go into distVals.
     * The items must all be moved together for
     * each cube, which is why they are placed in 
     * distVals.
     */
    vec3 loc[NUM_INSTANCES * NUM_IMAGES];
    vec3 xaxis[NUM_INSTANCES * NUM_IMAGES];
    vec3 yaxis[NUM_INSTANCES * NUM_IMAGES];
    float angles[NUM_INSTANCES * NUM_IMAGES];
    //! The data for the shader uniform. For
    //! it's definition see commonheader.h.
    InstData itemData;
    //! The calculated information for the cube.
    float calcTex[72];
    float calcNorm[108];
    float calcCube[108];
    //! The pointer for the texture2DArray.
    unsigned int texImages;
    //! The images used in the images directory.
    string imageNames[NUM_IMAGES] =
    {
        "awesomeface.png", "eucharist.png", 
        "palette.png", "panda.png",
        "seahorse.png", "sparkle.png", 
        "star.png", "sunflowers.png",
        "superman.png", "paris.png",
        "grapes.png", "lemon.png",
        "sun.png", "mexican.png",
        "abstract.png", "suites.png"
    };
    //! Timing for the animation and the camera.
    chrono::_V2::system_clock::time_point end, start;
    chrono::_V2::system_clock::time_point intbegin, intend;
    
    //!---------------------------------------------------------
    //! set up vertex data (and buffer(s)) and configure 
    //! vertex attributes
    //!---------------------------------------------------------
            // Vertex        //Color
    float cube[56] = {
         0.5f,  0.5f,  0.5f, //0
        -0.5f,  0.5f,  0.5f, //1
        -0.5f, -0.5f,  0.5f, //2
         0.5f, -0.5f,  0.5f, //3
         0.5f, -0.5f, -0.5f, //4
         0.5f,  0.5f, -0.5f, //5
        -0.5f,  0.5f, -0.5f, //6
        -0.5f, -0.5f, -0.5f  //7
    };
    vec3 normals[6] {
         vec3( 1.0f,   0.0f,  0.0f),
         vec3( 0.0f,   1.0f,  0.0f),
         vec3( 0.0f,   0.0f,  1.0f),
         vec3(-1.0f,   0.0f,  0.0f),
         vec3( 0.0f,  -1.0f,  0.0f),
         vec3( 0.0f,   0.0f, -1.0f)
    };
    
    /** This array shows the order of processing
     * for each vertex in the cube array.  The cube 
     * array defines each corner of the cube.  The
     * indices array allows the genMatrices function 
     * to generate a cube from the eight vertices. 
     * Each face of the cube has two triangles
     * or six vertices and there are six faces.
     */
    unsigned int indices[NUM_VERTICES] = {
        0, 1, 2, 0, 2, 3,
        0, 3, 4, 0, 4, 5,
        0, 5, 6, 0, 6, 1,
        7, 1, 6, 7, 2, 1,
        7, 5, 4, 7, 6, 5,
        7, 3, 2, 7, 4, 3
    };
};

#endif // SIDEFOGCUBE_H
