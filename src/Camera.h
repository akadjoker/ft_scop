#include "Math.h"


constexpr float LIMIT_Y = 85.0f;




struct Camera 
{

    Vector3 direction;

    Matrix matrix;
    float pitch;  
    float yaw;    
    float  distance;

    Camera()
    {

        direction.x = 0;
        direction.y = 0;
        direction.z = 0;
        pitch = 0;
        yaw = 0;
        distance = 10;
    }
    void Update()
    {
        
        float x = distance * cos(toRadians(yaw)) * cos(toRadians(pitch));
        float y = distance * sin(toRadians(pitch));
        float z = distance * sin(toRadians(yaw)) * cos(toRadians(pitch));


        matrix.LookAtRH(Vector3(x,y,z),  direction, Vector3(0, 1, 0));
      
    }
    void Rotate(float x, float y)
    {
        yaw += x;
        pitch += y;
        if (pitch > LIMIT_Y) pitch = LIMIT_Y;
        if (pitch < -LIMIT_Y) pitch = -LIMIT_Y;
    }
};