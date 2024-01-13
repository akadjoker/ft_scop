#include "Mesh.h"
#include "Texture.h"
#include "Utils.h"
#include "Shader.h"
#include "File.h"


//***************************************************************************************
//***************************************************************************************
//SURFACE
//***************************************************************************************
//***************************************************************************************


Surface::Surface(): m_material(-1)
{
    m_buffer.vao = 0;
    m_buffer.vbo = 0;
    m_buffer.ebo = 0;
    m_isCentered=false;


}

Surface::~Surface()
{
    Log(1,"Surface release");
    glBindVertexArray(0);
    glDeleteBuffers(1, &m_buffer.vbo);
    glDeleteBuffers(1, &m_buffer.ebo);
    glDeleteVertexArrays(1, &m_buffer.vao);
}

void Surface::Build()
{
  
    if (m_vertices.size() == 0 || m_indices.size() == 0) return;
    Log(1,"Surface Update %d %d ",m_vertices.size(),m_indices.size() );
    if (m_buffer.vao == 0 || m_buffer.vbo == 0 || m_buffer.ebo == 0) 
    {
        
        glGenVertexArrays(1, &m_buffer.vao) ;
        glBindVertexArray(m_buffer.vao);

        glGenBuffers(1, &m_buffer.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));


        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));


        glGenBuffers(1, &m_buffer.ebo);

        glBindVertexArray(0);
    }
    


    glBindBuffer(GL_ARRAY_BUFFER, m_buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);



}

void Surface::Draw()
{
    if (m_vertices.size() == 0 || m_indices.size() == 0)
        return;

    glBindVertexArray(m_buffer.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer.ebo);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

int Surface::AddVertex(const Vertex &vertex)
{
    m_vertices.push_back(vertex);
    m_boundingBox.addInternalPoint(vertex.position);
    m_isCentered=false;
    return (int)m_vertices.size() - 1;
}

int Surface::AddVertex(float x, float y, float z, float u, float v, float r, float g, float b, float a)
{
    Vertex vertex;
    vertex.position.x = x;
    vertex.position.y = y;
    vertex.position.z = z;
    vertex.texcoord.x = u;
    vertex.texcoord.y = v;
    vertex.color.r = r;
    vertex.color.g = g;
    vertex.color.b = b;
    vertex.color.a = a;
    vertex.normal.x = 0.0f;
    vertex.normal.y = 0.0f;
    vertex.normal.z = 0.0f;
    m_boundingBox.addInternalPoint(vertex.position);
    m_isCentered=false;
    return AddVertex(vertex);
}

int Surface::AddVertex(float x, float y, float z, float u, float v)
{
    Vertex vertex;
    vertex.position.x = x;
    vertex.position.y = y;
    vertex.position.z = z;
    vertex.texcoord.x = u;
    vertex.texcoord.y = v;
    vertex.color.r = 1.0f;
    vertex.color.g = 1.0f;
    vertex.color.b = 1.0f;
    vertex.color.a = 1.0f;
    vertex.normal.x = 0.0f;
    vertex.normal.y = 0.0f;
    vertex.normal.z = 0.0f;
    m_boundingBox.addInternalPoint(vertex.position);
    m_isCentered=false;
    return AddVertex(vertex);
}

int Surface::AddIndex(Uint32 index)
{
    m_isCentered=false;
    m_indices.push_back(index);
    return (int)m_indices.size() - 1;

}

int Surface::AddFace(Uint32 a, Uint32 b, Uint32 c)
{
    m_isCentered=false;
    AddIndex(a);
    AddIndex(b);
    AddIndex(c);
    return (int)m_indices.size() - 3;
}

bool  Surface::Center()
{
    if (m_isCentered)     return false;
    Vector3 center = m_boundingBox.GetCenter();
    for (Uint32 i = 0; i < (Uint32)m_vertices.size(); i++)
    {
        Vertex& vertex = m_vertices[i];
        vertex.position.x -= center.x;
        vertex.position.y -= center.y;
        vertex.position.z -= center.z;
        m_vertices[i] = vertex;
    }
    m_isCentered=true;
    return true;
}

bool Surface::MakePlanarMapping(float resolution)
{
    if (m_vertices.size() == 0)        return false;
    if (m_indices.size() == 0)        return false;


    for (Uint32 i = 0; i < (Uint32)m_indices.size(); i+=3)
    {
        Uint32 a = m_indices[i];
        Uint32 b = m_indices[i + 1];
        Uint32 c = m_indices[i + 2];
        Vertex& vertexA = m_vertices[a];
        Vertex& vertexB = m_vertices[b];
        Vertex& vertexC = m_vertices[c];

        Plane3D plane(vertexA.position, vertexB.position, vertexC.position);

        plane.normal.x = fabsf(plane.normal.x);
        plane.normal.y = fabsf(plane.normal.y);
        plane.normal.z = fabsf(plane.normal.z);

        if (plane.normal.x > plane.normal.y && plane.normal.x > plane.normal.z)
        {
            vertexA.texcoord.x = vertexA.position.y * resolution;
            vertexA.texcoord.y = vertexA.position.z * resolution;
            vertexB.texcoord.x = vertexB.position.y * resolution;
            vertexB.texcoord.y = vertexB.position.z * resolution;
            vertexC.texcoord.x = vertexC.position.y * resolution;
            vertexC.texcoord.y = vertexC.position.z * resolution;
        }
        else if (plane.normal.y > plane.normal.x && plane.normal.y > plane.normal.z)
        {
            vertexA.texcoord.x = vertexA.position.x * resolution;
            vertexA.texcoord.y = vertexA.position.z * resolution;
            vertexB.texcoord.x = vertexB.position.x * resolution;
            vertexB.texcoord.y = vertexB.position.z * resolution;
            vertexC.texcoord.x = vertexC.position.x * resolution;
            vertexC.texcoord.y = vertexC.position.z * resolution;
        }
        else
        {
            vertexA.texcoord.x = vertexA.position.x * resolution;
            vertexA.texcoord.y = vertexA.position.y * resolution;
            vertexB.texcoord.x = vertexB.position.x * resolution;
            vertexB.texcoord.y = vertexB.position.y * resolution;
            vertexC.texcoord.x = vertexC.position.x * resolution;
            vertexC.texcoord.y = vertexC.position.y * resolution;
        }


    }

    return true;


}

bool Surface::CalculateNormals()
{
    if (m_vertices.size() == 0)
        return false;
    if (m_indices.size() == 0)
        return false;
    for (Uint32 i = 0; i < (Uint32)m_vertices.size(); i++)
    {
        Vertex& vertex = m_vertices[i];
        vertex.normal.x = 0.0f;
        vertex.normal.y = 0.0f;
        vertex.normal.z = 0.0f;
        m_vertices[i] = vertex;
    }
    for (Uint32 i = 0; i < (Uint32)m_indices.size(); i += 3)
    {
        Uint32 a = m_indices[i];
        int b = m_indices[i + 1];
        int c = m_indices[i + 2];
        Vertex& vertexA = m_vertices[a];
        Vertex& vertexB = m_vertices[b];
        Vertex& vertexC = m_vertices[c];
        float x1 = vertexB.position.x - vertexA.position.x;
        float y1 = vertexB.position.y - vertexA.position.y;
        float z1 = vertexB.position.z - vertexA.position.z;
        float x2 = vertexC.position.x - vertexA.position.x;
        float y2 = vertexC.position.y - vertexA.position.y;
        float z2 = vertexC.position.z - vertexA.position.z;
        float nx = y1 * z2 - z1 * y2;
        float ny = z1 * x2 - x1 * z2;
        float nz = x1 * y2 - y1 * x2;
        vertexA.normal.x += nx;
        vertexA.normal.y += ny;
        vertexA.normal.z += nz;
        vertexB.normal.x += nx;
        vertexB.normal.y += ny;
        vertexB.normal.z += nz;
        vertexC.normal.x += nx;
        vertexC.normal.y += ny;
        vertexC.normal.z += nz;
        m_vertices[a] = vertexA;
        m_vertices[b] = vertexB;
        m_vertices[c] = vertexC;
    }
    for (Uint32 i = 0; i < (Uint32)m_vertices.size(); i++)
    {
        Vertex& vertex = m_vertices[i];
        float length = sqrtf(vertex.normal.x * vertex.normal.x + vertex.normal.y * vertex.normal.y + vertex.normal.z * vertex.normal.z);
        if (length > 0.0f)
        {
            vertex.normal.x /= length;
            vertex.normal.y /= length;
            vertex.normal.z /= length;
        }
        m_vertices[i] = vertex;
    }
    return true;
}

//***************************************************************************************
//***************************************************************************************
//MESH
//***************************************************************************************
//***************************************************************************************

Mesh::Mesh()
{
    const char *defaultVShaderCode =
    "#version 320   es                    \n"
     "precision mediump float;           \n"
    "in vec3 vertexPosition;            \n"
    "in vec2 vertexTexCoord;            \n"
    "in vec4 vertexColor;               \n"
    "out vec2 fragTexCoord;             \n"
    "out vec4 fragColor;                \n"
    "uniform mat4 mvp;                  \n"
    "void main()                        \n"
    "{                                  \n"
    "    fragTexCoord = vertexTexCoord; \n"
    "    fragColor = vertexColor;       \n"
    "    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
    "}                                  \n";


    const char *defaultFShaderCode =
    "#version 320  es    \n"
    "precision mediump float;           \n"
    "in vec2 fragTexCoord;              \n"
    "in vec4 fragColor;                 \n"
    "out vec4 finalColor;               \n"
    "uniform float interpolationFactor; \n"
    "uniform sampler2D texture0;        \n"
    "void main()                        \n"
    "{                                  \n"
    "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
    "    finalColor = mix(texelColor, fragColor, interpolationFactor);          \n"
    "}                                  \n";




    unsigned int vShaderId = 0;
    unsigned int fShaderId = 0;

    vShaderId = CompileShader(defaultVShaderCode, GL_VERTEX_SHADER);
    fShaderId = CompileShader(defaultFShaderCode, GL_FRAGMENT_SHADER);
    if (vShaderId != 0 && fShaderId != 0)
    {
        defaultShaderId = LoadShaderProgram(vShaderId, fShaderId);
        if (defaultShaderId != 0)
        {
            Log(0, "SHADER: [ID %i] Default shader loaded successfully", defaultShaderId);

             glDeleteShader(vShaderId);
             glDeleteShader(fShaderId);
        }
    } else
    {
        Log(2,"SHADER: Failed to load default shader");
        return ;
    }

    mpvId = glGetUniformLocation(defaultShaderId, "mvp");
    textId = glGetUniformLocation(defaultShaderId, "texture0");
    lerpId = glGetUniformLocation(defaultShaderId, "interpolationFactor");

    glUniform1i(textId, 0);

    unsigned char pixels[4] = { 255, 255, 255, 255 };
    defaultTextureId = CreateTexture(pixels, 1, 1, PixelFormat::R8G8B8A8);

    if (defaultTextureId != 0) Log(0, "TEXTURE: [ID %i] Default texture loaded successfully", defaultTextureId);
}

Mesh::~Mesh()
{
    Log(1, "Mesh release");

    UnloadTexture(defaultTextureId);
    glDeleteProgram(defaultShaderId);
    Clear();
    for (auto material : m_materials)
        delete material;
}

bool Mesh::Load(const std::string &filename)
{

        File file(filename, "rb");
        if (!file.IsOpen())
        {
            Log(2,"Error reading OBJ(%s) data",filename.c_str());
            return false;
        }
        int size = file.Size();
        if (size <= 1)
        {
            Log(2,"Error reading OBJ(%s) data",filename.c_str());
            file.Close();
            return false;
        }
        std::vector<char> data;
        data.resize(size);
        file.Read(&data[0], size);
        const char* fileDataConst = (const char*)&data[0];
        file.Close();

        Group = "";
        Mtl = "";

   
   try
   {

        bool status = Read(fileDataConst);
        if (!status)
        {
            Log(2,"Error reading OBJ(%s) data",filename.c_str());
            return false;
        }
        return status;
   }
    catch (const std::exception& e)
    {
        Log(2,"Error reading OBJ(%s) : Error: %s",filename.c_str(),e.what());
        return false;
    }

       
        return false;


}




//this code contain protection because off some stupid students that 
//like to inventing crazy bugs on  the avaliation, the cost is a slow loading time
    bool Mesh::Read(const char* buffer)
    {

        
       	    std::string curline;
			int i = -1;
			
			std::vector<Vector3> vertices;
			std::vector<Vector3> normals;
			std::vector<Vector2> texposs;
            std::vector<Uint16> vfaces;
            std::vector<Uint16> uvfaces;
            std::vector<Uint16> nfaces;

 

            Surface *surface = addSurface(0);

            

			while (buffer[++i] != 0)
            {
				char l = buffer[i];

				if ((l == '\n' || l == '\r') && curline.size() > 0)
                {
                    
					auto parts = Split(curline, ' ');
  

					if (parts[0] == "v")
                    { 
                        
                       
                       //   std::cout<<"x:"<<parts[1]<<" y:"<<parts[2].c_str()<<" z:"<<parts[3].c_str()<<std::endl;

					vertices.push_back(Vector3((float)std::stof(parts[1]), (float)std::stof(parts[2]), (float)std::stof(parts[3])));
					} else if (parts[0] == "vt")
                    {
						texposs.push_back(Vector2((float)std::stof(parts[1]), (float)std::stof(parts[2])));
                        
					} else if (parts[0] == "vn")
                    {
                        
						normals.push_back(Vector3((float)std::stof(parts[1]), (float)std::stof(parts[2]), (float)std::stof(parts[3])));
					} else if (parts[0] == "f")
                    {
						if (parts.size() == 4)
                        {

                            //std::cout<<"parts[1]:"<<parts[1]<<" parts[2]:"<<parts[2]<<" parts[3]:"<<parts[3]<<std::endl;
           
                          // Log(0,"Triangle");
                          
                            add_triangle(surface, vertices, normals, texposs, parts[1], parts[2], parts[3]);
                          
              

						} else if (parts.size() == 5)
                        {
                            //  std::cout<<"parts[1]:"<<parts[1]<<" parts[2]:"<<parts[2]<<" parts[3]:"<<parts[3]<<" parts[4]:"<<parts[4]<<std::endl;
                           

                          //  Log(0,"Quad");
                            // quad
					   		add_triangle(surface, vertices, normals, texposs, parts[1], parts[2], parts[3]);
							add_triangle(surface, vertices, normals, texposs, parts[1], parts[3], parts[4]);
						} else
                        {

				
                            std::vector<std::string> polygonVertices;
                            for (size_t i = 1; i < parts.size(); ++i) 
                            {
                                std::cout<<"parts["<<i<<"]:"<<parts[i]<<std::endl;
                                polygonVertices.push_back(parts[i]);
                            }
                            if (polygonVertices.size() >= 3)
                            {
                                Log(1," Adding polygon  %d", parts.size());
                                add_polygon(surface, vertices, normals, texposs, polygonVertices);
                            }
                            
        				}
					} else if (parts[0] == "usemtl")
                    {
						Mtl = parts[1];
                        //Log(0,"usemtl %s",Mtl.c_str());
					} else if (parts[0] == "g")
                    {
						if (Group != "")
                        {
                           // Log(0,"New Group");

                            surface = addSurface(0);
						}

						Group = parts[1];
					}

					curline = "";
				}

				if (l != '\n' && l != '\r') curline += l;
			}

        if (surface->m_vertices.size() == 0 || surface->m_indices.size() == 0)
        {
            Log(2,"No vertices found");
            return false;
        }


        return true;
        
        
      
    }

//this code contain protection because off some stupid students that like to inventing crazy bugs on  the avaliation, the cost is a slow loading time
    void Mesh::add_polygon(Surface *surf, const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& texposs, const std::vector<std::string>& polygonVertices)
    {
    std::vector<Vector3> polyVertices;
    std::vector<Vector2> polyTexCoords;
    std::vector<Vector3> polyNormals;

    bool containUV = false;
    bool containNormals = false;



    for (auto& vertex : polygonVertices)
    {
        auto data = Split(vertex, '/');

        if (data.size() == 3)
        {
            containNormals = true;
            containUV = true;
        } else if (data.size() == 2)
        {
            containNormals = false;
            containUV = false;
        } else if (data.size() == 1)
        {
            containNormals = false;
            containUV = false;
        } else
        {
            Log(1," malformed vertex");
            return;
        }
  
       




      
                if (containUV)
                {
                    int texID = std::stoi(data[1]) - 1; 
                    if (texID < 0)
                    {
                        texID += texposs.size() + 1;
                    }
                    if(texID < (int)texposs.size()) 
                        polyTexCoords.push_back(texposs[texID]);
                }

                if (containNormals)
                {
                    int normID = std::stoi(data[2]) - 1;
                    if (normID < 0)
                    {
                        normID += normals.size() + 1;
                    }
                    if(normID < (int)normals.size())
                        polyNormals.push_back(normals[normID]);
                }
   

            int vertID = std::stoi(data[0]) - 1;
            if (vertID < 0)
            {
                vertID += vertices.size() + 1;
            }

            if (vertID >= (int)vertices.size())
            {
                Log(1," malformed vertex");
                return;
            }
            polyVertices.push_back(vertices[vertID]);

    }



    // Triangulate the polygon
    for (size_t i = 1; i < polyVertices.size() - 1; ++i)
    {
        Vertex v_a, v_b, v_c;

        v_a.position = polyVertices[0];
        v_b.position = polyVertices[i];
        v_c.position = polyVertices[i + 1];

        if (containUV)
        {

         v_a.texcoord = polyTexCoords[0];
         v_b.texcoord = polyTexCoords[i];
         v_c.texcoord = polyTexCoords[i + 1];
        }

        if (containNormals)
        {
         v_a.normal = polyNormals[0];
         v_b.normal = polyNormals[i];
         v_c.normal = polyNormals[i + 1];
        }


        Uint32 fa = surf->AddVertex(v_a);
        Uint32 fb = surf->AddVertex(v_b);
        Uint32 fc = surf->AddVertex(v_c);

        surf->AddFace(fc, fb, fa);
    }
   
}

//this code contain protection because off some stupid students that like to ivinventingent crazy bugs on  the avaliation, the cost is a slow loading time
    void   Mesh::add_triangle(Surface *surf,  const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& texposs, const std::string& a, const std::string& b, const std::string& c)
    {
      

			auto data_a = Split(a, '/');
			auto data_b = Split(b, '/');
			auto data_c = Split(c, '/');

            bool containUVa = data_a.size() == 3;
            bool containUVb = data_b.size() == 3;
            bool containUVc = data_c.size() == 3;

            bool containNormalsa = containUVa;
            bool containNormalsb = containUVb;
            bool containNormalsc = containUVc;



			Vertex v_a;
			Vertex v_b;
			Vertex v_c;


			int vertID_a = std::stoi(data_a[0]) - 1;
			int vertID_b = std::stoi(data_b[0]) - 1;
			int vertID_c = std::stoi(data_c[0]) - 1;

          

		
			if (vertID_a < 0) vertID_a += vertices.size() + 1;
			if (vertID_b < 0) vertID_b += vertices.size() + 1;
			if (vertID_c < 0) vertID_c += vertices.size() + 1;

         
            if (vertID_a >=(int) vertices.size() || vertID_b >=(int) vertices.size() || vertID_c >=(int) vertices.size())
            {
                Log(1," malformed vertex");
                return;
            }

			v_a.position = vertices[vertID_a];
			v_b.position = vertices[vertID_b];
			v_c.position = vertices[vertID_c];



			// // UV
            if (containUVa && containUVb && containUVc)
            {
			if (data_a.size() > 1 && data_a[1].size() > 0 && 
                data_b.size() > 1 && data_b[1].size() > 0 && 
                data_c.size() > 1 && data_c[1].size() > 0)
            {
            //      Log(1,"uv a %d %s",data_a[1].size(),data_a[1].c_str());
            //      Log(1,"uv b %d %s",data_b[1].size(),data_b[1].c_str());
            //      Log(1,"uv c %d %s",data_c[1].size(),data_c[1].c_str());

                 bool ignore =false;
              

                    int texID_a = std::stoi(data_a[1]) - 1;
                    int texID_b = std::stoi(data_b[1]) - 1;
                    int texID_c = std::stoi(data_c[1]) - 1;

                    if (texID_a < 0) texID_a += texposs.size() + 1;
                    if (texID_b < 0) texID_b += texposs.size() + 1;
                    if (texID_c < 0) texID_c += texposs.size() + 1;

                    if (texID_a < 0 || texID_b < 0 || texID_c < 0)
                    {
                        Log(1," malformed UV");
                        ignore = true;
                    }
                    if (texID_a >=(int) texposs.size() || texID_b >=(int) texposs.size() || texID_c >=(int) texposs.size())
                    {
                        Log(1,"malformed UV");
                        ignore = true;    
                    }
                    if (!ignore)
                    {
                        v_a.texcoord = texposs[texID_a];
                        v_b.texcoord = texposs[texID_b];
                        v_c.texcoord = texposs[texID_c];
                    }
                 
             }
			}

			 // normals
             if (containNormalsa && containNormalsb && containNormalsc)
             {
			 if (data_a.size() > 2 && data_a[2].size() >  0 && 
                 data_b.size() > 2 && data_b[2].size() >  0 && 
                 data_c.size() > 2 && data_c[2].size() >  0)
             {
              
                      bool ignore =false;

              
                    
                // Log(1,"normal a %d %s",data_a[2].size(),data_a[2].c_str());
                // Log(1,"normal b %d %s",data_b[2].size(),data_b[2].c_str());
                // Log(1,"normal c %d %s",data_c[2].size(),data_c[2].c_str());


				int normID_a = std::stoi(data_a[2]) - 1;
				int normID_b = std::stoi(data_b[2]) - 1;
				int normID_c = std::stoi(data_c[2]) - 1;

				if (normID_a < 0) normID_a += normals.size() + 1;
				if (normID_b < 0) normID_b += normals.size() + 1;
				if (normID_c < 0) normID_c += normals.size() + 1;

                if (normID_a < 0 || normID_b < 0 || normID_c < 0)
                {
                    Log(1," malformed normal negative ");
                    ignore=true;
                }
                if (normID_a >=(int) normals.size() || normID_b >=(int) normals.size() || normID_c >=(int) normals.size())
                {
                    Log(1," malformed normal");
                     ignore=true;
                }

                if (!ignore)
                {
                v_a.normal = normals[normID_a];
                v_b.normal = normals[normID_b];
                v_c.normal = normals[normID_c];
                }
                
              }
			 }

            Uint32 fa = surf->AddVertex(v_a);
            Uint32 fb = surf->AddVertex(v_b);
            Uint32 fc = surf->AddVertex(v_c);

            surf->AddFace(fc, fb, fa);
        
       

}




void Mesh::Render()
{
    if (m_surfaces.size() == 0)         return;

         glUseProgram(defaultShaderId);
        GLfloat mat[16]=
        {
                matrix.m0, matrix.m1, matrix.m2, matrix.m3,
                matrix.m4, matrix.m5, matrix.m6, matrix.m7,
                matrix.m8, matrix.m9, matrix.m10, matrix.m11,
                matrix.m12, matrix.m13, matrix.m14, matrix.m15
        };
    glUniformMatrix4fv(mpvId, 1, false, mat);
    glUniform1i(textId, 0);


    glUniform1f(lerpId, interpolate);
    glActiveTexture(GL_TEXTURE0);
    for (auto surface : m_surfaces)
    {

        surface->Draw();
    }
}

void Mesh::Clear()
{
    if (m_surfaces.size() == 0)
        return;
    for (auto surface : m_surfaces)
    {
        delete surface;
    }
    m_surfaces.clear();

}

void Mesh::SetMatrix(const Matrix &m)
{
    matrix = m;
}

void Mesh::SetColor(bool useColor)
{
    this->useColor = useColor;
    this->interpolate = 0.0f;
}

void Mesh::Setlerp(float lerp)
{
    this->interpolate = lerp;
}

void Mesh::Build()
{
    for (auto surface : m_surfaces)
    {
        surface->CalculateNormals();
        surface->Build();
    }
}


void Mesh::MakePlanarMapping(float value)
{
    for (auto surface : m_surfaces)
    {
        if (surface->MakePlanarMapping(value))
            surface->Build();
    }
}

void Mesh::Center()
{
    for (auto surface : m_surfaces)
    {
        if (surface->Center())
            surface->Build();
    }
}

void Mesh::ShadesOfGray()
{
    for (auto surface : m_surfaces)
    {
    
        for (auto& vertex : surface->m_vertices)
        {
            Vector3 normal = vertex.normal;
            

           
            if ( (normal.z == 1.0f || normal.z==-1.0f) && normal.y <= 0.5f && normal.x <= 0.5f )
            {
                 vertex.color=Color::ShadesOfGray;
            } 
            else
            {


                float variation = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 1.0f; 

                vertex.color.r = variation;
                vertex.color.g = variation;
                vertex.color.b = variation;

            }

        }
        surface->Build();
    }
}



void Mesh::Build(bool generateNormals, bool generateTexcoords)
{
     for (auto surface : m_surfaces)
    {
        surface->Center();

        if (generateNormals)
            surface->CalculateNormals();
       
        if (generateTexcoords)
            surface->MakePlanarMapping(DEFAULT_TEXTURE_MAPPING);
        surface->Build();
    }
}

Material *Mesh::addMaterial()
{
    Material* material = new Material();
    m_materials.push_back(material);
    return material;
}

Surface *Mesh::addSurface(int materialIndex)
{
    Surface* surface = new Surface();
    surface->m_material = materialIndex;
    m_surfaces.push_back(surface);
    return surface;
}


Mesh *Mesh::CreateCube(const Vector3 &size)
{
    Mesh *mesh = new Mesh();

    Surface *surface  = mesh->addSurface(0);



    surface->m_indices=
    {
        0,2,1,   0,3,2,
        1,5,4,   1,2,5,
        4,6,7,   4,5,6,
        7,3,0,   7,6,3,
        9,5,2,   9,8,5,
        0,11,10,   0,10,7
    };



    surface->AddVertex(0, 0, 0, 0.0f, 1.0f);
    surface->AddVertex(1, 0, 0, 1.0f, 1.0f);

    surface->AddVertex(1, 1, 0, 1.0f, 0.0f);
    surface->AddVertex(0, 1, 0, 0.0f, 0.0f);

    surface->AddVertex(1, 0, 1, 0.0f, 1.0f);
    surface->AddVertex(1, 1, 1, 0.0f, 0.0f);

    surface->AddVertex(0, 1, 1, 1.0f, 0.0f);
    surface->AddVertex(0, 0, 1, 1.0f, 1.0f);

    surface->AddVertex(0, 1, 1, 0.0f, 1.0f);
    surface->AddVertex(0, 1, 0, 1.0f, 1.0f);

    surface->AddVertex(1, 0, 1, 1.0f, 0.0f);
    surface->AddVertex(1, 0, 0, 0.0f, 0.0f);



	for (int i=0; i<12; ++i)
	{
        Vector3 &v = surface->m_vertices[i].position;
        v -= Vector3(0.5f, 0.5f, 0.5f);
        v *= size;
	}

    mesh->Build();



    return mesh;
}

void Mesh::BuildCube(const Vector3 &size)
{
    Clear();
    Surface *surface = addSurface(0);

    surface->m_indices=
    {
        0,2,1,   0,3,2,
        1,5,4,   1,2,5,
        4,6,7,   4,5,6,
        7,3,0,   7,6,3,
        9,5,2,   9,8,5,
        0,11,10,   0,10,7
    };



    surface->AddVertex(0, 0, 0, 0.0f, 1.0f);
    surface->AddVertex(1, 0, 0, 1.0f, 1.0f);

    surface->AddVertex(1, 1, 0, 1.0f, 0.0f);
    surface->AddVertex(0, 1, 0, 0.0f, 0.0f);

    surface->AddVertex(1, 0, 1, 0.0f, 1.0f);
    surface->AddVertex(1, 1, 1, 0.0f, 0.0f);

    surface->AddVertex(0, 1, 1, 1.0f, 0.0f);
    surface->AddVertex(0, 0, 1, 1.0f, 1.0f);

    surface->AddVertex(0, 1, 1, 0.0f, 1.0f);
    surface->AddVertex(0, 1, 0, 1.0f, 1.0f);

    surface->AddVertex(1, 0, 1, 1.0f, 0.0f);
    surface->AddVertex(1, 0, 0, 0.0f, 0.0f);



	for (int i=0; i<12; ++i)
	{
        Vector3 &v = surface->m_vertices[i].position;
        v -= Vector3(0.5f, 0.5f, 0.5f);
        v *= size;
	}

    Build();
}
