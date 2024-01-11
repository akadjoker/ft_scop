#pragma once
#include "pch.h"
#include "Math.h"



constexpr float DEFAULT_TEXTURE_MAPPING = 1.0f;


class Shader;




struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 texcoord;
    Color   color;
    Vertex()
    {
        position.x = 0.0f;
        position.y = 0.0f;
        position.z = 0.0f;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        texcoord.x = 0.0f;
        texcoord.y = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
    }
};

struct Material
{
    Color diffuse;
    Color specular;
    float shininess;
    Material()
    {
        diffuse.r = 1.0f;
        diffuse.g = 1.0f;
        diffuse.b = 1.0f;
        diffuse.a = 1.0f;
        specular.r = 1.0f;
        specular.g = 1.0f;
        specular.b = 1.0f;
        specular.a = 1.0f;
        shininess = 1.0f;
    }
};

struct BoundingBox
{
    Vector3 min;
    Vector3 max;
    BoundingBox()
    {
        min.x = 99990.0f;
        min.y = 99990.0f;
        min.z = 99990.0f;
        max.x = -999990.0f;
        max.y = -999990.0f;
        max.z = -999990.0f;
    }

    Vector3 GetCenter()
    {
        return (min + max) * 0.5f;
    }

    Vector3 GetSize()
    {
        return max - min;
    }

    void Transform(const Matrix& m)
    {
        Vector3 corners[8];
        corners[0] = Vector3(min.x, min.y, min.z);
        corners[1] = Vector3(min.x, min.y, max.z);
        corners[2] = Vector3(min.x, max.y, min.z);
        corners[3] = Vector3(min.x, max.y, max.z);
        corners[4] = Vector3(max.x, min.y, min.z);
        corners[5] = Vector3(max.x, min.y, max.z);
        corners[6] = Vector3(max.x, max.y, min.z);
        corners[7] = Vector3(max.x, max.y, max.z);

        min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
        max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (int i = 0; i < 8; i++)
        {
            corners[i] = m.Transform(corners[i]);
            min = Vector3::Min(min, corners[i]);
            max = Vector3::Max(max, corners[i]);
        }
    }
     void reset(float x, float y, float z)
        {
            min.x = x;
            min.y = y;
            min.z = z;
            max.x = x;
            max.y = y;
            max.z = z;
        }
        void reset(const Vector3 &point)
        {
            min = point;
            max = point;
        }
        void addInternalBox(const BoundingBox &box)
        {
            addInternalPoint(box.min);
            addInternalPoint(box.max);
        }
        void addInternalPoint(float x, float y, float z)
        {
            if (x < min.x)
                min.x = x;
            if (y < min.y)
                min.y = y;
            if (z < min.z)
                min.z = z;
            if (x > max.x)
                max.x = x;
            if (y > max.y)
                max.y = y;
            if (z > max.z)
                max.z = z;
        }
        void addInternalPoint(const Vector3 &point)
        {
            if (point.x < min.x)
                min.x = point.x;
            if (point.y < min.y)
                min.y = point.y;
            if (point.z < min.z)
                min.z = point.z;
            if (point.x > max.x)
                max.x = point.x;
            if (point.y > max.y)
                max.y = point.y;
            if (point.z > max.z)
                max.z = point.z;
        }
        
};

class Surface
{
    friend class Mesh;
    public:
        Surface();
        ~Surface();

        void Build();
        void Draw();

        int AddVertex(const Vertex& vertex);
        int AddVertex(float x, float y, float z, float u, float v, float r, float g, float b, float a);
        int AddVertex(float x, float y, float z, float u, float v);

        int AddIndex(Uint32 index);
        int AddFace(Uint32 a, Uint32 b, Uint32 c);

        void Center();
        void MakePlanarMapping(float resolution = 1.0f);

        void CalculateNormals();

    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        BoundingBox         m_boundingBox;
        int m_material;
        bool m_containsNormals;
        bool m_containsTexcoords;
        bool m_containsColors;
        struct Buffer
        {
            GLuint vao;
            GLuint vbo;
            GLuint ebo;
        } ;
        Buffer m_buffer ;


};


class Mesh
{
public:
    Mesh();
    ~Mesh();

    void MakePlanarMapping(float value);
    void Center();
    void ShadesOfGray();
    




    void Build();
    void Build(bool generateNormals, bool generateTexcoords);
    bool Load(const std::string& filename);
    void Render();

    void Clear();

    void SetMatrix(const Matrix &m);

    void SetColor(bool useColor);

    void Setlerp(float lerp);

    static Mesh *CreateCube(const Vector3 &size);

    Material* addMaterial();
    Surface*  addSurface(int materialIndex);

private:
    
    void    add_triangle(Surface *surf,const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& texposs, const std::string& a, const std::string& b, const std::string& c) ;
     void   add_polygon(Surface *surf, const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& texposs, const std::vector<std::string>& polygonVertices) ;
  
    bool Read(const char* buffer);


  
    std::vector<Surface*> m_surfaces;
    std::vector<Material*> m_materials;

    Uint32 defaultTextureId;  
    Uint32 defaultShaderId;

    Uint32 mpvId;
    Uint32 textId;
    Uint32 lerpId; 
    Matrix matrix;

    float interpolate;
    bool useColor;
//for model
    std::string Mtl;
	std::string Group;
};

