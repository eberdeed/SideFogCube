/*******************************************************************
 * Camera:  A class to contain the functions of an OpenGL camera.
 *  Adapted from a class proposed on www.learnopengl.com.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * December 2019 San Diego, California USA
 * ****************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include "commonheader.h"

/** \class Camera
 * An abstract camera class that processes input and 
 * calculates the corresponding Euler Angles, Vectors 
 * and Matrices for moving the camera about a 3-dimensional
 * landscape. For use in OpenGL.
 */
class Camera
{
public:

    /** \brief Camera Movement
     * Defines several possible options for camera movement. 
     * Used as an abstraction to stay away from window-system
     * specific input methods.  To access this from the 
     * calling class use "Camera::Camera_Movement::FORWARD", etc.
     */
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        CLOSER,
        AWAY,
        UP,
        DOWN
    };
    //! Default camera values
    constexpr static float YAW         = -90.0f;
    constexpr static float PITCH       =  0.0f;
    constexpr static float SPEED       =  0.5f;
    constexpr static float SENSITIVITY =  0.3f;
    constexpr static float ZOOM        =  45.0f;
    constexpr static float onedegree = (float) acos(-1) / 180.0f;

    //! Camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    //! Theses two variables are to capture data for
    //! the resetCamera function.
    vec3 position;
    vec3 upOrig;
    //! Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    int width, height;
    mat4 projection;

    /** \brief Camera 
     * Constructor with vectors
     */
    Camera(int width, int height, vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), 
    float yaw = YAW, float pitch = PITCH); 

    /** \brief Camera
     * Constructor with variables.
     */
    Camera(int width, int height, float posX, float posY, float posZ, float upX, float upY, 
           float upZ, float yaw, float pitch);
    ~Camera();

    /** \brief GetViewMatrix
     * Returns the view matrix calculated using Euler Angles 
     * and the LookAt matrix.
     */
    mat4 GetViewMatrix();

    /** \brief GetPerspective
     * Returns the lookAt matrix.
     */
    mat4 GetPerspective();

    /** \brief resetCamera
     * Resets the camera to the original position and zoom.
     */
    void resetCamera();

    /** \brief GetPosition
     * Returns the camera position.
     */
    vec3 GetPosition();

    /** \brief SetPosition
     * Sets the camera position.
     */
    void SetPosition(vec3 position);

    /** \brief ProcessKeyboard
     * Processes input received from any keyboard-like input
     * system. Accepts input parameter in the form of camera
     * defined ENUM (to abstract it from windowing systems)
     */
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    
    /** \brief ProcessMouseMovement
     * Processes input received from a mouse input system.
     * Expects the offset value in both the x and y direction.
     */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    /** \brief ProcessMouseScroll
     * Processes input received from a mouse scroll-wheel 
     * event. Only requires input on the vertical wheel-axis
     */
    void ProcessMouseScroll(Camera_Movement inout);

private:
    
    /** \brief updateCameraVectors
     * Calculates the front vector from the Camera's (updated)
     * Euler angles.
     */
    void updateCameraVectors();
    
};

#endif // CAMERA_H
