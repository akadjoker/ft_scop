#pragma once
#include <math.h>

constexpr float PI = 3.14159265359f;
inline float toRadians(float degrees) 
{
    return degrees * (PI / 180.0f);
}


struct Color 
{
    static const Color WHITE;
    static const Color GRAY;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color MAGENTA;
    static const Color CYAN;
    static const Color TRANSPARENT;
    static  const Color ORANGE;
    static  const Color PURPLE;
    static  const Color PINK;
    static  const Color BROWN;
    static  const Color CORNFLOWER;
    static  const Color BABYBLUE;
    static  const Color LIME;
    static  const Color PEACH;
    static  const Color LAVENDER;
    static  const Color TURQUOISE;
    static  const Color GOLD;
    static  const Color SILVER;
    static  const Color BRONZE;
    static  const Color SKYBLUE;
    static const  Color ShadesOfGray ;


    Color()
    {
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
        a = 1.0f;
    }

    Color(float _r, float _g, float _b, float _a=1.0f)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    void set(float _r, float _g, float _b, float _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    float r, g, b, a;
};

struct Vector2
{
    Vector2()
    {
        x = 0;
        y = 0;
    }
    Vector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    float x, y;
};


struct Vector3
{
    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void Normalize()
    {
        float length = sqrt(x*x + y*y + z*z);
        if (length > 0.0f)
        {
            float invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
    }

    float Length()
    {
        return sqrt(x*x + y*y + z*z);
    }


    Vector3 Cross(const Vector3 &vec)
    {
        return Vector3(y*vec.z - z*vec.y,
                       z*vec.x - x*vec.z,
                       x*vec.y - y*vec.x);
    }

    static Vector3 Normalize(const Vector3 &vec)
    {
        float length = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
        if (length > 0.0f)
        {
            float invLength = 1.0f / length;
            return Vector3(vec.x * invLength, vec.y * invLength, vec.z * invLength);
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    static float Dot(const Vector3 &vec1, const Vector3 &vec2)
    {
        return (vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);
    }

    static Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2)
    {
        return Vector3(vec1.y*vec2.z - vec1.z*vec2.y,
                       vec1.z*vec2.x - vec1.x*vec2.z,
                       vec1.x*vec2.y - vec1.y*vec2.x);
    }

    static Vector3 Min(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(a.x < b.x ? a.x : b.x,
                       a.y < b.y ? a.y : b.y,
                       a.z < b.z ? a.z : b.z);
    }

    static Vector3 Max(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(a.x > b.x ? a.x : b.x,
                       a.y > b.y ? a.y : b.y,
                       a.z > b.z ? a.z : b.z);
    }

    Vector3 operator+(const Vector3 &vec) const
    {
        return Vector3(x + vec.x, y + vec.y, z + vec.z);
    }
    Vector3 operator-(const Vector3 &vec) const
    {
        return Vector3(x - vec.x, y - vec.y, z - vec.z);
    }
    Vector3 operator*(float scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 operator/(float scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }
    Vector3& operator+=(const Vector3 &vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }
    Vector3& operator-=(const Vector3 &vec)
    {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }
    Vector3& operator*=(const Vector3 &vec)
    {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;
        return *this;
    }

    Vector3& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vector3& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    bool operator==(const Vector3 &vec) const
    {
        return (x == vec.x && y == vec.y && z == vec.z);
    }
    bool operator!=(const Vector3 &vec) const
    {
        return (x != vec.x || y != vec.y || z != vec.z);
    }

    
    float x, y, z;
};

struct Plane3D
{
    Plane3D()
    {
        normal.x = 0;
        normal.y = 0;
        normal.z = 0;
        d = 0;
    }
    Plane3D(const Vector3 &_normal, float _d)
    {
        normal = _normal;
        d = _d;
    }

    Plane3D(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
    {
        Vector3 e1 = v2 - v1;
        Vector3 e2 = v3 - v1;
        normal = Vector3::Cross(e1, e2);
        normal.Normalize();
        d = -Vector3::Dot(normal, v1);
    }

    Vector3 normal;
    float d;
};


struct Rectangle
{
    Rectangle()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }
    Rectangle(float _x, float _y, float _width, float _height)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
    void set(float _x, float _y, float _width, float _height)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
    void Clip(const Rectangle &rect)
    {
        float x1 = x;
        float y1 = y;
        float x2 = x + width;
        float y2 = y + height;
        float rx1 = rect.x;
        float ry1 = rect.y;
        float rx2 = rect.x + rect.width;
        float ry2 = rect.y + rect.height;
        if (x1 < rx1) x1 = rx1;
        if (y1 < ry1) y1 = ry1;
        if (x2 > rx2) x2 = rx2;
        if (y2 > ry2) y2 = ry2;
        x = x1;
        y = y1;
        width = x2 - x1;
        height = y2 - y1;
    }
    bool Contains(int px, int py)
    {
        return (px >= x && py >= y && px < x + width && py < y + height);
    }
    bool Contains(float px, float py)
    {
        return (px >= x && py >= y && px < x + width && py < y + height);
    }

    bool Contains(const Vector2 &point)
    {
        return (point.x >= x && point.y >= y && point.x < x + width && point.y < y + height);
    }


    float x, y, width, height;
};

struct Quaternion
{
    float x, y, z, w;

    Quaternion()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }
    Quaternion(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    void set(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    void Identity()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }
    void FromAxisAngle(const Vector3 &axis, float angle)
    {
        float sinAngle;
        angle *= 0.5f;
        Vector3 vn = Vector3::Normalize(axis);
        sinAngle = sin(angle);
        x = (vn.x * sinAngle);
        y = (vn.y * sinAngle);
        z = (vn.z * sinAngle);
        w = cos(angle);
        Normalize();
    }
    void FromEuler(float pitch, float yaw, float roll)
    {

     float x0 = cosf(pitch*0.5f);
    float x1 = sinf(pitch*0.5f);
    float y0 = cosf(yaw*0.5f);
    float y1 = sinf(yaw*0.5f);
    float z0 = cosf(roll*0.5f);
    float z1 = sinf(roll*0.5f);

    x = x1*y0*z0 - x0*y1*z1;
    y = x0*y1*z0 + x1*y0*z1;
    z = x0*y0*z1 - x1*y1*z0;
    w = x0*y0*z0 + x1*y1*z1;


        // float p = toRadians(pitch) * 0.5f;
        // float y = toRadians(yaw) * 0.5f;
        // float r = toRadians(roll) * 0.5f;

        // float sinp = sin(p);
        // float siny = sin(y);
        // float sinr = sin(r);
        // float cosp = cos(p);
        // float cosy = cos(y);
        // float cosr = cos(r);

        // x = sinr * cosp * cosy - cosr * sinp * siny;
        // y = cosr * sinp * cosy + sinr * cosp * siny;
        // z = cosr * cosp * siny - sinr * sinp * cosy;
        // w = cosr * cosp * cosy + sinr * sinp * siny;

        // Normalize();
    }
    void Normalize()
    {
        float length = sqrt(x*x + y*y + z*z + w*w);
        if (length > 0.0f)
        {
            float invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }
};

struct Matrix 
{


    Matrix()
    {
                 set( 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f );

    }
    Matrix( float m0, float m4, float m8, float m12,
            float m1, float m5, float m9, float m13,
            float m2, float m6, float m10, float m14,
            float m3, float m7, float m11, float m15 )
    {
        set( m0, m4, m8, m12,
             m1, m5, m9, m13,
             m2, m6, m10, m14,
             m3, m7, m11, m15 );
    }
    Matrix(const Matrix &mat)
    {
        set( mat.m0, mat.m4, mat.m8, mat.m12,
             mat.m1, mat.m5, mat.m9, mat.m13,
             mat.m2, mat.m6, mat.m10, mat.m14,
             mat.m3, mat.m7, mat.m11, mat.m15 );
    }

    Vector3 Transform(const Vector3 &vec) const
    {
        return Vector3(m0*vec.x + m4*vec.y + m8*vec.z + m12,
                       m1*vec.x + m5*vec.y + m9*vec.z + m13,
                       m2*vec.x + m6*vec.y + m10*vec.z + m14);
    }
    Vector3 TransformNormal(const Vector3 &vec) const
    {
        return Vector3(m0*vec.x + m4*vec.y + m8*vec.z,
                       m1*vec.x + m5*vec.y + m9*vec.z,
                       m2*vec.x + m6*vec.y + m10*vec.z);
    }


    Matrix& operator=(const Matrix &mat)
    {
        set( mat.m0, mat.m4, mat.m8, mat.m12,
             mat.m1, mat.m5, mat.m9, mat.m13,
             mat.m2, mat.m6, mat.m10, mat.m14,
             mat.m3, mat.m7, mat.m11, mat.m15 );
        return *this;
    }

    Matrix operator*(const Matrix &mat) const
    {
        Matrix result;
        result.m0 = m0*mat.m0 + m4*mat.m1 + m8*mat.m2 + m12*mat.m3;
        result.m1 = m1*mat.m0 + m5*mat.m1 + m9*mat.m2 + m13*mat.m3;
        result.m2 = m2*mat.m0 + m6*mat.m1 + m10*mat.m2 + m14*mat.m3;
        result.m3 = m3*mat.m0 + m7*mat.m1 + m11*mat.m2 + m15*mat.m3;
        result.m4 = m0*mat.m4 + m4*mat.m5 + m8*mat.m6 + m12*mat.m7;
        result.m5 = m1*mat.m4 + m5*mat.m5 + m9*mat.m6 + m13*mat.m7;
        result.m6 = m2*mat.m4 + m6*mat.m5 + m10*mat.m6 + m14*mat.m7;
        result.m7 = m3*mat.m4 + m7*mat.m5 + m11*mat.m6 + m15*mat.m7;
        result.m8 = m0*mat.m8 + m4*mat.m9 + m8*mat.m10 + m12*mat.m11;
        result.m9 = m1*mat.m8 + m5*mat.m9 + m9*mat.m10 + m13*mat.m11;
        result.m10 = m2*mat.m8 + m6*mat.m9 + m10*mat.m10 + m14*mat.m11;
        result.m11 = m3*mat.m8 + m7*mat.m9 + m11*mat.m10 + m15*mat.m11;
        result.m12 = m0*mat.m12 + m4*mat.m13 + m8*mat.m14 + m12*mat.m15;
        result.m13 = m1*mat.m12 + m5*mat.m13 + m9*mat.m14 + m13*mat.m15;
        result.m14 = m2*mat.m12 + m6*mat.m13 + m10*mat.m14 + m14*mat.m15;
        result.m15 = m3*mat.m12 + m7*mat.m13 + m11*mat.m14 + m15*mat.m15;
        return result;
    }
    void Identity()
    {
        set( 1.0f, 0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 1.0f, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f );
    }
  
     void set(float _m0, float _m4, float _m8, float _m12,
           float _m1, float _m5, float _m9, float _m13,
           float _m2, float _m6, float _m10, float _m14,
           float _m3, float _m7, float _m11, float _m15)
    {
        m0 = _m0; m4 = _m4; m8 = _m8; m12 = _m12;
        m1 = _m1; m5 = _m5; m9 = _m9; m13 = _m13;
        m2 = _m2; m6 = _m6; m10 = _m10; m14 = _m14;
        m3 = _m3; m7 = _m7; m11 = _m11; m15 = _m15;
    }

    void Ortho(double left, double right, double bottom, double top, double nearPlane, double farPlane)
    {
        float rl = (float)(right - left);
        float tb = (float)(top - bottom);
        float fn = (float)(farPlane - nearPlane);

        m0 = 2.0f/rl;
        m1 = 0.0f;
        m2 = 0.0f;
        m3 = 0.0f;
        m4 = 0.0f;
        m5 = 2.0f/tb;
        m6 = 0.0f;
        m7 = 0.0f;
        m8 = 0.0f;
        m9 = 0.0f;
        m10 = -2.0f/fn;
        m11 = 0.0f;
        m12 = -((float)left + (float)right)/rl;
        m13 = -((float)top + (float)bottom)/tb;
        m14 = -((float)farPlane + (float)nearPlane)/fn;
        m15 = 1.0f;
    }

    void Perspective(double fovY, double aspect, double nearPlane, double farPlane)
    {
        float f = (float)(1.0 / tan(fovY * 0.5));
        float fn = (float)(1.0 / (nearPlane - farPlane));

        m0 = f / (float)aspect;
        m1 = 0.0f;
        m2 = 0.0f;
        m3 = 0.0f;
        m4 = 0.0f;
        m5 = f;
        m6 = 0.0f;
        m7 = 0.0f;
        m8 = 0.0f;
        m9 = 0.0f;
        m10 = ((float)nearPlane + (float)farPlane) * fn;
        m11 = -1.0f;
        m12 = 0.0f;
        m13 = 0.0f;
        m14 = (2.0f * (float)nearPlane * (float)farPlane) * fn;
        m15 = 0.0f;
    }

    void Translate(float x, float y, float z)
    {

        set( 1.0f, 0.0f, 0.0f, x,
                      0.0f, 1.0f, 0.0f, y,
                      0.0f, 0.0f, 1.0f, z,
                      0.0f, 0.0f, 0.0f, 1.0f );
    }
    void Translate(const Vector3 &vec)
    {
        set( 1.0f, 0.0f, 0.0f, vec.x,
                      0.0f, 1.0f, 0.0f, vec.y,
                      0.0f, 0.0f, 1.0f, vec.z,
                      0.0f, 0.0f, 0.0f, 1.0f );
    }

    void Scale(float x, float y, float z)
    {
                      set( x, 0.0f, 0.0f, 0.0f,
                      0.0f, y, 0.0f, 0.0f,
                      0.0f, 0.0f, z, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f );
    }

    void Rotate(float angle, float x, float y, float z)
    {
        float c = cos(angle);
        float s = sin(angle);
        float ic = 1.0f - c;

        float icxy = ic*x*y;
        float icxz = ic*x*z;
        float icyz = ic*y*z;

        float sx = s*x;
        float sy = s*y;
        float sz = s*z;

        set( x*x*ic + c, icxy - sz, icxz + sy, 0.0f,
             icxy + sz, y*y*ic + c, icyz - sx, 0.0f,
             icxz - sy, icyz + sx, z*z*ic + c, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f );
    }

    void Rotate(const Quaternion &q) 
    {
        Identity();

        float a2 = q.x*q.x;
        float b2 = q.y*q.y;
        float c2 = q.z*q.z;
        float ac = q.x*q.z;
        float ab = q.x*q.y;
        float bc = q.y*q.z;
        float ad = q.w*q.x;
        float bd = q.w*q.y;
        float cd = q.w*q.z;

        m0 = 1 - 2*(b2 + c2);
        m1 = 2*(ab + cd);
        m2 = 2*(ac - bd);

        m4 = 2*(ab - cd);
        m5 = 1 - 2*(a2 + c2);
        m6 = 2*(bc + ad);

        m8 = 2*(ac + bd);
        m9 = 2*(bc - ad);
        m10 = 1 - 2*(a2 + b2);


    }

    void LookAt(Vector3 eye, Vector3 target, Vector3 up)
    {
        Vector3 zaxis = Vector3::Normalize(eye - target);
        Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
        Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

        m0 = xaxis.x;
        m1 = yaxis.x;
        m2 = zaxis.x;
        m3 = 0.0f;
        m4 = xaxis.y;
        m5 = yaxis.y;
        m6 = zaxis.y;
        m7 = 0.0f;
        m8 = xaxis.z;
        m9 = yaxis.z;
        m10 = zaxis.z;
        m11 = 0.0f;
        m12 = -Vector3::Dot(xaxis, eye);
        m13 = -Vector3::Dot(yaxis, eye);
        m14 = -Vector3::Dot(zaxis, eye);
        m15 = 1.0f;
    }

        void LookAtRH(Vector3 eye, Vector3 target, Vector3 up)
        {
            Vector3 zaxis = Vector3::Normalize(target - eye);  
            Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
            Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

            m0 = xaxis.x;
            m1 = yaxis.x;
            m2 = -zaxis.x;  
            m3 = 0.0f;
            m4 = xaxis.y;
            m5 = yaxis.y;
            m6 = -zaxis.y;  
            m7 = 0.0f;
            m8 = xaxis.z;
            m9 = yaxis.z;
            m10 = -zaxis.z; 
            m11 = 0.0f;
            m12 = -Vector3::Dot(xaxis, eye);
            m13 = -Vector3::Dot(yaxis, eye);
            m14 = Vector3::Dot(zaxis, eye);  
            m15 = 1.0f;
        }




  
    float m0, m4, m8, m12;      
    float m1, m5, m9, m13;      
    float m2, m6, m10, m14;     
    float m3, m7, m11, m15;     
} ;
