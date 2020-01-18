/*******************************************************************
 * Camera:  A class to contain the functions of an OpenGL camera.
 * Adapted from a class proposed on www.learnopengl.com.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * December 2019 San Diego, California USA
 * ****************************************************************/

#include "camera.h"

    //! Constructor with vectors
Camera::Camera(int width, int height, vec3 position, vec3 up, float yaw, float pitch) 
{
    cout << "\n\n\tCreating Camera.\n\n";
    this->width = width;
    this->height = height;
    this->position = position;
    this->upOrig = up;
    Front = vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(int width, int height, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    cout << "\n\n\tCreating Camera.\n\n";
    this->width = width;
    this->height = height;
    Front = vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    Position = vec3(posX, posY, posZ);
    this->position = Position;
    WorldUp = vec3(upX, upY, upZ);
    this->upOrig = WorldUp;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::~Camera()
{
    cout << "\n\n\tDestroying Camera.\n\n";
}

vec3 Camera::GetPosition()
{
    return Position;
}

void Camera::SetPosition(vec3 position)
{
    Position = position;
}

mat4 Camera::GetViewMatrix()
{
    return lookAt(Position, Position + Front, Up);
}

mat4 Camera::GetPerspective()
{
    return perspective(Zoom, (float)width / (float)height, 0.1f, 1000.0f);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    //! Move the camera around the scene.
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    else if (direction == BACKWARD)
        Position -= Front * velocity;
    else if (direction == LEFT)
        Position -= Right * velocity;
    else if (direction == RIGHT)
        Position += Right * velocity;
    else if (direction == UP)
        Position += WorldUp * velocity;
    else if (direction == DOWN)
        Position -= WorldUp * velocity;
    
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    //! Point the camera according to the mouse motion.
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    //! Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    //! Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(Camera_Movement inout)
{
    //! Handles zoom in and zoom out.
    if (inout == AWAY)
    {
        Zoom += onedegree;
    }
    if (inout == CLOSER)
    {
        Zoom -= onedegree;
    }
    if(Zoom <= onedegree)
    {
        Zoom = onedegree;
    }
    if(Zoom >= onedegree * 45.0f)
    {
        Zoom = onedegree * 45.0f;
    }
}

void Camera::updateCameraVectors()
{
    //! Calculate the new Front vector
    vec3 front;
    front.x = cos(Yaw * onedegree) * cos(Pitch * onedegree);
    front.y = sin(Pitch * onedegree);
    front.z = sin(Yaw * onedegree) * cos(Pitch * onedegree);
    Front = normalize(front);
    //! Also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp));  //! Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = normalize(cross(Right, Front));
}

void Camera::resetCamera()
{
    cout << "\n\n\tCreating Camera.\n\n";
    this->width = width;
    this->height = height;
    this->position = position;
    Front = vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    Position = position;
    WorldUp = upOrig;
    Yaw = YAW;
    Pitch = PITCH;
    updateCameraVectors();
}    
