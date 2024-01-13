

#include "pch.h"
#include "scop.h"
#include "Camera.h"

 int SCREEN_WIDTH = 1024;
 int SCREEN_HEIGHT = 720;
constexpr float ZOOM_SPEED = 1.0f;
constexpr float YAW_SPEED = 0.5f;
constexpr float PITCH_SPEED = 0.6f;
constexpr float ROTATE_SPEED = 5.0f;
constexpr float MOVE_SPEED = 5.0f;

constexpr float LOAD_ANIM_SPEED = 5.0f;

std::string meshToLoad;
Mesh *model = nullptr;
int  GameState =0;

std::mutex s_mutex;  
bool LoadMesh(const char *filename)
{
    bool state = false;

    Log(1,"LoadMesh %s",filename);
    std::lock_guard<std::mutex> guard(s_mutex);

    try 
    {
        if (model)
        {
          
            Log(1,"Create mesh");

            state = model->Load(filename);
        }
    }
    catch (const std::exception& e) 
    {
        Log(2,"LoadMesh error %s",e.what());
        state = false;
    }
    catch (...) 
    {
        Log(2,"LoadMesh error");
        state = false;
    }
    Log(1,"LoadMesh done");

    return state;
}


int main()
{




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
        Log(2, "Error creating window%s " , SDL_GetError() );
        SDL_Quit();
        return -1;
    }


    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) 
    {
        Log(2,  "Erro creating  OpenGL  context %s ",SDL_GetError() );
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

    model = new Mesh();
    model->Load("resources/plane.obj");
    model->Build();


    Font font;

    GUI widgets;

    Window *window1 = widgets.CreateWindow("Mesh Transform", 10, SCREEN_HEIGHT- 360, 300, 350);

    window1->CreateLabel("yaw",   10, 30);
    window1->CreateLabel("pitch", 10, 74);
    window1->CreateLabel("roll",  10, 114);

    Slider *sliderYaw   = window1->CreateSlider(false, 10, 50, 200, 20, 0, 360, 0);
    Slider *sliderPitch = window1->CreateSlider(false, 10, 90, 200, 20, 0, 360, 0);
    Slider *sliderRoll  = window1->CreateSlider(false, 10, 130, 200, 20, 0, 360, 0);

    Slider *colorLerp = window1->CreateSlider(false, 10, 300, 160, 20, 0, 1, 0.5);
    Button *buttonLerp = window1->CreateButton("Swicth", 200, 300, 80, 20);
    buttonLerp->SetkeyMap(true,SDLK_t);

    Button *moveLeft = window1->CreateButton("move - x", 10, 170, 100, 20);
    moveLeft->SetkeyMap(true,SDLK_LEFT);
    Button *moveRight = window1->CreateButton("move + x", 120, 170, 100, 20);
    moveRight->SetkeyMap(true,SDLK_RIGHT);
    
    Button *moveUp = window1->CreateButton("move - y", 10, 200, 100, 20);
    moveUp->SetkeyMap(true,SDLK_UP);
    Button *moveDown = window1->CreateButton("move + y", 120, 200, 100, 20);
    moveDown->SetkeyMap(true,SDLK_DOWN);

    Button *moveFront = window1->CreateButton("move - z", 10, 230, 100, 20);
    moveFront->SetkeyMap(true,SDLK_PAGEUP);
    Button *moveBack = window1->CreateButton("move + z", 120, 230, 100, 20);
    moveBack->SetkeyMap(true,SDLK_PAGEDOWN);

    Button *reset = window1->CreateButton("reset", 50, 260, 100, 20);
    
    Window *window2 = widgets.CreateWindow("Mesh Tool", SCREEN_WIDTH-160, 24, 150, 190);

    Button *buttonUv= window2->CreateButton("Uv", 10, 50, 100, 20);
    Slider *sliderUv = window2->CreateSlider(false, 10, 20, 100, 20, 0.01f, 5.0f, 1.0f);

    Button *buttonCenter= window2->CreateButton("Center Mesh", 10, 100, 100, 20);
    Button *buttonShades= window2->CreateButton("Shades/Gray", 10, 130, 100, 20);
    CheckBox *backFaces = window2->CreateCheckBox("Back Faces", true, 10, 160, 20, 20);


    
    
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
    float uvForce = 1.0f;
    bool isKeyShift = false;
    int BlendState=2;
    
    float blndAnimation = 0.0f;

    //load animation
    float   loadSpeed = 0;
    int     loadFrame = 0;

    std::future<bool> isLoadMesh;
    // mutex = SDL_CreateMutex();

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
                        
                        meshToLoad = dropped_filedir;
                        model->Clear();
                        isLoadMesh = std::async(std::launch::async, LoadMesh,  meshToLoad.c_str()); 
                        GameState=1;

                       
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
                        SCREEN_WIDTH = w;
                        SCREEN_HEIGHT = h;
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
                    widgets.OnKeyUp(event.key.keysym.sym);    
                    break;
                }
            
                case SDL_KEYDOWN:
                {
                    widgets.OnKeyDown(event.key.keysym.sym);
                    if (event.key.keysym.sym == SDLK_ESCAPE)      quit = true;
                    
                    break;  
                }

            }

        }

        
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     
        if (GameState==0)
        {

            colorLerp->OnValueChanged = [&](float value)
            {
                
                model->Setlerp(value);   
            };
            buttonLerp->OnClick = [&]()
            {
                Log(0,"Switch");
                if (blndAnimation<=0.0f)
                {
                    BlendState=0;
                    blndAnimation = 0.0f;
                } else 
                {
                    BlendState=1;
                    blndAnimation = 1.0f;
                }
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

            //mesh tool
            buttonUv->OnClick = [&]()
            {
                model->MakePlanarMapping(uvForce);
            };
            sliderUv->OnValueChanged = [&](float value)
            {
                uvForce = value;
            };
            buttonCenter->OnClick = [&]()
            {
                model->Center();
            };

            buttonShades->OnClick = [&]()
            {
                model->ShadesOfGray();
            };

                switch (BlendState)
                {
                    case 0:
                    {
                        
                        blndAnimation += 0.2f * deltaTime;
                        if (blndAnimation > 1.0f) 
                        {
                            blndAnimation = 1.0f;
                            
                            BlendState=2;
                        }
                    //  model->Setlerp(blndAnimation);
                        colorLerp->SetValue(blndAnimation);
                    //  Log(0,"blndAnimation %f",blndAnimation);
                        break;
                    }
                    case 1:
                    {
                        
                        blndAnimation -= 0.2f * deltaTime;
                        
                        if (blndAnimation < 0.0f) 
                        {
                            
                            blndAnimation = 0.0f;
                            BlendState=2;
                        }
                    //   model->Setlerp(blndAnimation);
                        colorLerp->SetValue(blndAnimation);
                    //   Log(0,"blndAnimation %f",blndAnimation);
                        break;
                    }
                    case 2:
                    {
                        
                        //do nothing
                    
                        break;
                    }
                }

            

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
                
                glEnable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);
                if (backFaces->GetChecked())
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);



                }


                model->Render();


                glDisable(GL_CULL_FACE);


                batch3D.DrawGrid(10,10);
                batch3D.SetMatrix(mvp);
                batch3D.Set3D();
                batch3D.Render();

               
                




            batch2D.DrawRectangle(20,18,210,20,Color(0.0,0.0,0.0,1),true);
            batch2D.DrawRectangle(20,18,210,20,Color(1,1,1,1),false);
            //  batch2D.DrawRectangle(20,180,210,20,Color(1.0,1.0,1.0,1),true);
            

            font.DrawText(&batch2D, "Luis Santos", Vector2(200, 400),  Color(1, 1, 1, 1));
            font.DrawText(&batch2D, 20,20, Color(1, 1, 1, 1), "Fps:%f DeltaTime:%f",fps,deltaTime);


                widgets.Update(deltaTime);
                widgets.Render(&batch2D);
            
                batch2D.Set2D();
                batch2D.Render();
               
        } else 
        if (GameState==1)///prepare to load
        {
              
            glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            batch2D.DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,Color(0.1,0.1,0.1,1),true);
            batch2D.DrawRectangle(1,1,SCREEN_WIDTH-1,SCREEN_HEIGHT-2,Color(1,1,1,1),false);
            font.DrawText(&batch2D, "Please Wait!", Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),  Color(1, 1, 1, 1));
            batch2D.Set2D();
            batch2D.Render();
            SDL_GL_SwapWindow(window);


              
             GameState = 2;
        } else  
        if (GameState==2)///loading
        {


            const char* text[] = 
            {
                "Loading...",
                "lOading..",
                "loAding.",
                "loaDing",
                "loadIng.",
                "loadiNg..",
                "loadinG...",
                "loading",
            };

            int count = sizeof(text) / sizeof(text[0]);
            loadSpeed += LOAD_ANIM_SPEED * deltaTime;
            if (loadSpeed > 1.0f)
            {
                loadSpeed = 0.0f;
                loadFrame++;
                if (loadFrame >= count) loadFrame = 0;
            }
            




                batch2D.DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,Color(0.1,0.1,0.1,1),true);
                batch2D.DrawRectangle(1,1,SCREEN_WIDTH-1,SCREEN_HEIGHT-2,Color(1,1,1,1),false);
                font.DrawText(&batch2D, text[loadFrame], Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),  Color(1, 1, 1, 1));
                batch2D.Set2D();
                batch2D.Render();
      
                if (isLoadMesh.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                {
                        bool state =false;
                        try
                        {
                            state=isLoadMesh.get(); 

                            if (!state)
                                model->BuildCube(Vector3(1,1,1));
                            else
                                model->Build();
                        }
                        catch (const std::exception& e)
                        {
                            Log(2, "Error get  future state : %s", e.what());
                        }
                  
                    GameState = 0;    
                }
            
        } 

        
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
   //   SDL_DestroyMutex(mutex);
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

