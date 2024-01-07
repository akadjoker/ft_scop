

#include "pch.h"
#include "scop.h"
#include "Camera.h"

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 720;
constexpr float ZOOM_SPEED = 5.0f;
constexpr float YAW_SPEED = 0.5f;
constexpr float PITCH_SPEED = 0.6f;
constexpr float ROTATE_SPEED = 5.0f;
constexpr float MOVE_SPEED = 5.0f;


struct mtl_t 
{
			Vector3 Ka;
			Vector3 Kd;
			Vector3 Ks;
			float Km;
			float Ni;
			float Ns;
			float d;

			std::string Name;
			std::string TexturePath;
};



struct Importer
{
    bool Load(const std::string &fname)
    {

            File file(fname, "rb");
            if (!file.IsOpen()) 
            {
                Log(2,"Error reading OBJ data");    
                return false;
            }
            int size = file.Size();
             std::vector<char> data;
			 data.resize(size);
             file.Read(&data[0], size);
			 const char* fileDataConst = (const char*)&data[0];
             file.Close();


            // std::ifstream infile;
			// infile.open(fname, std::ifstream::binary);

			// if (!infile.is_open()) 
            // {
			// 	std::cout << "Obj::FromFile() failed! Could not open file named: " << fname << std::endl;
			// 	infile.close();
			// 	return false;
			// }

			// infile.seekg(0, std::ios::end);
			// size_t file_size_in_byte = (size_t)infile.tellg();

			// std::vector<char> data;
			// data.resize(file_size_in_byte);
			// infile.seekg(0, std::ios::beg);

			// infile.read(&data[0], file_size_in_byte);

			// const char* fileDataConst = (const char*)&data[0];

			return Read(fileDataConst);

			//infile.close();
            

    }

    bool Read(const char* buffer)
    {
        std::cout<<"Read"<<std::endl;
       	std::string curline;
			int i = -1;
			int texid = 0;
			int normalid = 0;

			std::vector<Vector3> vertices;
			std::vector<Vector3> normals;
			std::vector<Vector2> texposs;

			while (buffer[++i] != 0) 
            {
				char l = buffer[i];

				if ((l == '\n' || l == '\r') && curline.size() > 0) 
                {
					auto parts = Split(curline, ' ');

					if (parts[0] == "v") 
                    {
						vertices.push_back(Vector3((float)atof(parts[1].c_str()), (float)atof(parts[2].c_str()), (float)atof(parts[3].c_str())));
					} else if (parts[0] == "vt") 
                    {
						texposs.push_back(Vector2((float)atof(parts[1].c_str()), (float)atof(parts[2].c_str())));
					} else if (parts[0] == "vn") 
                    {
						normals.push_back(Vector3((float)atof(parts[1].c_str()), (float)atof(parts[2].c_str()), (float)atof(parts[3].c_str())));
					} else if (parts[0] == "f") 
                    {
						if (parts.size() == 4) 
                        {
                             // triangle
                             Log(0,"Triangle");
					 		add_triangle( vertices, normals, texposs, parts[1], parts[2], parts[3]);

						} else if (parts.size() == 5) 
                        { 
                            Log(0,"Quad");
                            // quad
					   		add_triangle( vertices, normals, texposs, parts[1], parts[2], parts[3]);
							add_triangle( vertices, normals, texposs, parts[1], parts[3], parts[4]);
						} else 
                        {
							
							Log(2," Invalid face count while loading model");
                            return false;
						}
					} else if (parts[0] == "usemtl") 
                    {
						Mtl = parts[1];
                        Log(0,"usemtl %s",Mtl.c_str());
					} else if (parts[0] == "g") 
                    {
						if (Group != "") 
                        {
                            Log(0,"New Group");
							//this->Meshes.Add(m);
							//m = ObjMesh();
						}

						Group = parts[1];
					}

					curline = "";
				}

				if (l != '\n' && l != '\r') curline += l;
			}

			//if (m.Vertices.Count > 0) this->Meshes.Add(m);
        
        return true;
    }
    void   add_triangle(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<Vector2>& texposs, const std::string& a, const std::string& b, const std::string& c) 
    {
			auto data_a = Split(a, '/');
			auto data_b = Split(b, '/');
			auto data_c = Split(c, '/');

			Vertex v_a;
			Vertex v_b;
			Vertex v_c;

			// vertice ID is always filled in
			// -1 due not zero-indexed array but one-indexed
			int vertID_a = atoi(data_a[0].c_str()) - 1;
			int vertID_b = atoi(data_b[0].c_str()) - 1;
			int vertID_c = atoi(data_c[0].c_str()) - 1;

			// fix negative offset
			if (vertID_a < 0) vertID_a += vertices.size() + 1;
			if (vertID_b < 0) vertID_b += vertices.size() + 1;
			if (vertID_c < 0) vertID_c += vertices.size() + 1;

			v_a.position = vertices[vertID_a];
			v_b.position = vertices[vertID_b];
			v_c.position = vertices[vertID_c];

			// UV
			if (data_a.size() > 1 && data_a[1].size() > 0)
             {
				int texID_a = atoi(data_a[1].c_str()) - 1;
				int texID_b = atoi(data_b[1].c_str()) - 1;
				int texID_c = atoi(data_c[1].c_str()) - 1;

				if (texID_a < 0) texID_a += texposs.size() + 1;
				if (texID_b < 0) texID_b += texposs.size() + 1;
				if (texID_c < 0) texID_c += texposs.size() + 1;

				v_a.texcoord = texposs[texID_a];
				v_b.texcoord = texposs[texID_b];
				v_c.texcoord = texposs[texID_c];
			}

			// normals
			if (data_a.size() > 2 && data_a[2].size() > 0) 
            {
				int normID_a = atoi(data_a[2].c_str()) - 1;
				int normID_b = atoi(data_b[2].c_str()) - 1;
				int normID_c = atoi(data_c[2].c_str()) - 1;

				if (normID_a < 0) normID_a += normals.size() + 1;
				if (normID_b < 0) normID_b += normals.size() + 1;
				if (normID_c < 0) normID_c += normals.size() + 1;

			}


			Vertices.push_back(v_a);
			Indices.push_back(Vertices.size() - 1);

			Vertices.push_back(v_b);
			Indices.push_back(Vertices.size() - 1);

			Vertices.push_back(v_c);
			Indices.push_back(Vertices.size() - 1);
		}

    std::vector<Vertex> Vertices;
	std::vector<Uint16> Indices;
    std::string Mtl;
	std::string Group;
}  ;



int main()
{

//     Importer importer;
//     importer.Load("resources/42.obj");

// //     // Font f;
// //     // f.Load("resources/font1.fnt");

// //     Texture t;
// //     t.Load("resources/GameTutorials_24.tga");

//  return 0;




    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        Log(2, "Erro ao inicializar a SDL %s ", SDL_GetError() );
        return -1;
    }
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    

    SDL_Window* window = SDL_CreateWindow("scop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) 
    {
        Log(2, "Erro ao criar a janela %s " , SDL_GetError() );
        SDL_Quit();
        return -1;
    }


    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) 
    {
        Log(2,  "Erro ao criar o contexto OpenGL %s ",SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Log(0,"Vendor  :  %s",glGetString(GL_VENDOR));
    Log(0,"Renderer:  %s",glGetString(GL_RENDERER));
    Log(0,"Version :  %s",glGetString(GL_VERSION));

    RenderBatch batch2D;
    RenderBatch batch3D;
    Camera camera;
    camera.distance = 30;
    camera.pitch = 45;
    camera.yaw = 45;

    batch2D.Init(12,2840*8);
    batch3D.Init(8,2840*4);

    Matrix projection2D;
    projection2D.Ortho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1000);
    batch2D.SetMatrix(projection2D);

    Matrix projection3D;
    projection3D.Perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

    //Mesh *model = Mesh::CreateCube(Vector3(1,1,1));
    //model = new Mesh();

    Mesh *model = new Mesh();
    model->Load("resources/42.obj");

    Font font;

    GUI widgets;

    Window *window1 = widgets.CreateWindow("Options", 100, 100, 300, 350);

    window1->CreateLabel("yaw",   10, 30);
    window1->CreateLabel("pitch", 10, 74);
    window1->CreateLabel("roll",  10, 114);

    Slider *sliderYaw   = window1->CreateSlider(false, 10, 50, 200, 20, 0, 360, 0);
    Slider *sliderPitch = window1->CreateSlider(false, 10, 90, 200, 20, 0, 360, 0);
    Slider *sliderRoll  = window1->CreateSlider(false, 10, 130, 200, 20, 0, 360, 0);

    Slider *colorLerp = window1->CreateSlider(false, 10, 300, 200, 20, 0, 1, 0.5);

    Button *moveLeft = window1->CreateButton("move - x", 10, 170, 100, 20);
    Button *moveRight = window1->CreateButton("move + x", 120, 170, 100, 20);
    
    Button *moveUp = window1->CreateButton("move - y", 10, 200, 100, 20);
    Button *moveDown = window1->CreateButton("move + y", 120, 200, 100, 20);

    Button *moveFront = window1->CreateButton("move - z", 10, 230, 100, 20);
    Button *moveBack = window1->CreateButton("move + z", 120, 230, 100, 20);

    Button *reset = window1->CreateButton("reset", 50, 260, 100, 20);
    


    
    
    Texture *texture = new Texture();
    texture->Load("resources/brick.tga");
    
    bool mouseUse=false;

    double lastTime = GetTime();
    int frameCount = 0;
    double fps = 0.0;
    Uint32 lastFrameTime = GetTicks(); 

    float rotYaw = 0;
    float rotPitch = 0;
    float rotRoll = 0;
    Vector3 position = Vector3(0,1,0);
    bool isKeyShift = false;

    bool quit = false;
    while (!quit) 
    {
        Uint32 currentFrameTime = GetTicks(); 
        Uint32 deltaTimeUint32 = currentFrameTime - lastFrameTime; 
        float deltaTime = deltaTimeUint32 / 1000.0f; 

         const Uint8 *state = SDL_GetKeyboardState(NULL);

         if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) 
             isKeyShift = true;
          else 
             isKeyShift = false;
        


        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {

                case SDL_QUIT:
                {
                     quit = true;
                    break;    
                }
                case SDL_DROPFILE:          
                {
                    char *dropped_filedir = event.drop.file;
                    Log(0,"Dropped file: %s",dropped_filedir);

                    std::string extension = ToLower(GetFileExtension(dropped_filedir));
                    std::string filename = GetFileNameNoExtension(dropped_filedir);
                    std::string path = GetFilePath(dropped_filedir);

                    Log(0,"File extension: %s",extension.c_str());
                    Log(0,"File name: %s",filename.c_str());
                    Log(0,"File path: %s",path.c_str());
                    if (extension == "obj")
                    {
                        model->Clear();
                        model->Load(dropped_filedir);
                    }
                    else if (extension == "tga")
                    {
                        if (texture!=nullptr)
                        {
                            Log(1,"Delete texture");
                            delete texture;
                            texture = new Texture();
                            texture->Load(dropped_filedir);
                        }
                    }
                    else
                    {
                        Log(1,"File not supported");
                    }


                    
                    SDL_free(dropped_filedir);
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        int w = event.window.data1;
                        int h = event.window.data2;
                        glViewport(0, 0, w, h);
                        projection2D.Ortho(0, w, h, 0, 0, 1000);
                        batch2D.SetMatrix(projection2D);

                        projection3D.Perspective(45.0f, (float)w / (float)h, 0.1f, 1000.0f);
                        batch3D.SetMatrix(projection3D);

                    }
                    break;  
                }    
                case SDL_MOUSEMOTION:
                {
                    if (mouseUse && isKeyShift)
                        camera.Rotate(event.motion.xrel * YAW_SPEED, event.motion.yrel * PITCH_SPEED);
                    
                    widgets.OnMouseMove(event.motion.x,event.motion.y);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseUse = true;
                     
                    }
                    widgets.OnMouseDown(event.button.x,event.button.y,event.button.button);
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseUse = false;
                    }
                    widgets.OnMouseUp(event.button.x,event.button.y,event.button.button);
                    
                    break;
                }
                case SDL_MOUSEWHEEL:
                {   
                    camera.distance -= event.wheel.y * ZOOM_SPEED;
                    if (camera.distance < 1) camera.distance = 1;
                    
                    break;
                }

                
                case SDL_KEYUP:
                {
                    
                    break;
                }
            
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)      quit = true;
                    
                    break;  
                }

            }

        }

        
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

    colorLerp->OnValueChanged = [&](float value)
    {
        
        model->Setlerp(value);   
    };


    sliderYaw->OnValueChanged = [&](float value) 
    {
        rotYaw =  toRadians(value);
    };
    sliderPitch->OnValueChanged = [&](float value)
    {
        rotPitch =  toRadians(value);
    };
    sliderRoll->OnValueChanged = [&](float value)
    {
        rotRoll =  toRadians(value);
    };
    moveLeft->OnDown = [&]()
    {
        position.x -= MOVE_SPEED * deltaTime;
    };
    moveRight->OnDown = [&]()
    {
        position.x += MOVE_SPEED * deltaTime;
    };

    moveUp->OnDown = [&]()
    {
        position.y -= MOVE_SPEED * deltaTime;
    };
    moveDown->OnDown = [&]()
    {
        position.y += MOVE_SPEED * deltaTime;
    };

    moveFront->OnDown = [&]()
    {
        position.z -= MOVE_SPEED * deltaTime;
    };
    moveBack->OnDown = [&]()
    {
        position.z += MOVE_SPEED * deltaTime;
    };

    reset->OnClick = [&]()
    {
        rotYaw = 0;
        rotPitch = 0;
        rotRoll = 0;
        position = Vector3(0,1,0);
        sliderYaw->SetValue(0);
        sliderPitch->SetValue(0);
        sliderRoll->SetValue(0);
    };

        if (state[SDL_SCANCODE_Q])      
            rotYaw  += ROTATE_SPEED *  deltaTime;
        else if (state[SDL_SCANCODE_E])
            rotYaw -= ROTATE_SPEED *  deltaTime;

        if (state[SDL_SCANCODE_W])
            rotPitch += ROTATE_SPEED *  deltaTime;
        else if (state[SDL_SCANCODE_S])
            rotPitch -= ROTATE_SPEED *  deltaTime;

        if (state[SDL_SCANCODE_A])
            rotRoll += ROTATE_SPEED *  deltaTime;
        else if (state[SDL_SCANCODE_D])
            rotRoll -= ROTATE_SPEED *  deltaTime;



        //3d stuff
 
        camera.Update();
        
        Matrix modelTraslate;
        modelTraslate.Translate(position);

        Matrix modelScale;
        modelScale.Scale(1,1,1);
 
        Quaternion rotate;
        rotate.FromEuler(rotPitch,rotYaw,rotRoll);

        

        Matrix modelRotate;
        modelRotate.Rotate(rotate);

        //order is important
        Matrix modelTrasform = modelTraslate * modelScale * modelRotate;
        Matrix trasform = projection3D * camera.matrix * modelTrasform;
        model->SetMatrix(trasform);


        Matrix mvp = projection3D * camera.matrix;
        batch3D.SetMatrix(mvp);
        texture->Bind();
        model->Render();


        batch3D.DrawGrid(10,10);
        batch3D.SetMatrix(mvp);
        batch3D.Render();

        //2d stuff
        glDisable(GL_DEPTH_TEST);    
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // batch2D.DrawLine(5,5,100,100,Color(0,1,0,1));
        




       batch2D.DrawRectangle(20,18,210,20,Color(0.0,0.0,0.0,1),true);
       batch2D.DrawRectangle(20,18,210,20,Color(1,1,1,1),false);
     //  batch2D.DrawRectangle(20,180,210,20,Color(1.0,1.0,1.0,1),true);
       

       font.DrawText(&batch2D, "Luis Santos", Vector2(200, 400),  Color(1, 1, 1, 1));
       font.DrawText(&batch2D, 20,20, Color(1, 1, 1, 1), "Fps:%f DeltaTime:%f",fps,deltaTime);


        widgets.Update(deltaTime);
        widgets.Render(&batch2D);
     
   
        batch2D.Render();
        glDisable(GL_BLEND);

        
        SDL_GL_SwapWindow(window);

        double currentTime = GetTime();
        frameCount++;
        if (currentTime - lastTime >= 1.0)
         {
            fps = frameCount / (currentTime - lastTime);
            std::string title = "scop by lrosa-do  [" + std::to_string(static_cast<int>(fps)) + ", "+std::to_string(deltaTime)+"ms]" ;
         
            SDL_SetWindowTitle(window, title.c_str()    );
            frameCount = 0;
            lastTime = currentTime;
        }
        lastFrameTime = currentFrameTime;


    }
    widgets.Clear();
    delete model;
    delete texture;
    batch2D.Release();
    batch3D.Release();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Log(0,"By! By!  ");

    return 0;
}

