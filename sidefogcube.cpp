/*******************************************************************
 * SideFogCube:  A class to display a cloud of randomly
 * spaced, randomly spinning cubes with or without fog.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * January 2020 San Diego, California USA
 * ****************************************************************/
#include "sidefogcube.h"


SideFogCube::SideFogCube()
{
    cout << "\n\n\tCreating SideFogCube\n\n";
    quit = false;
    add = false;
    firstMouse = true;
    xpos = ypos = lastX = lastY = 0;
    genMatrices();
    /**  The cloud of cubes goes from -25 to 25 on
     * all three axis.  There is a light at each corner.
     */
    lighting[0].lightPos = vec3(25.0, 25.0, 25.0);
    lighting[1].lightPos = vec3(-25.0, 25.0, 25.0);
    lighting[2].lightPos = vec3(25.0, 25.0, -25.0);
    lighting[3].lightPos = vec3(-25.0, 25.0, -25.0);
    lighting[4].lightPos = vec3(25.0, -25.0, 25.0);
    lighting[5].lightPos = vec3(-25.0, -25.0, 25.0);
    lighting[6].lightPos = vec3(25.0, -25.0, -25.0);
    lighting[7].lightPos = vec3(-25.0, -25.0, -25.0);
    for (int x = 0; x < NUM_LIGHTS; x++)
    {
        lighting[x].lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

SideFogCube::~SideFogCube()
{
    cout << "\n\n\tDestroying SideFogCube\n\n";
    delete image;
    delete shader;
    delete camera;
}

void SideFogCube::debug()
{
    cout << "\n\n\t\tVertices\n\n\n\t";
    for (int x = 0; x < 108; x++)
    {
        if (((x % 3) == 0) && (x > 0))
        {
            cout << "\n\t";
            if (((x % 9) == 0) && (x > 0))
            {
                cout << "\n\t";
            }
        }
        cout << calcCube[x] << ", "; 
    }
    cout << "\n\n\t\tNormals\n\n\n\t";
    for (int x = 0; x < 108; x++)
    {
        if (((x % 3) == 0) && (x > 0))
        {
            cout << "\n\t";
            if (((x % 9) == 0) && (x > 0))
            {
                cout << "\n\t";
            }
        }
        cout << calcNorm[x] << ", "; 
    }
    cout << "\n\n\tTextures\n\n\n\t";
    for (int x = 0; x < 72; x++)
    {
        if (((x % 2) == 0) && (x > 0))
        {
            cout << "\n\t";
            if (((x % 6) == 0) && (x > 0))
            {
                cout << "\n\t";
            }
        }
        cout << calcTex[x] << ", ";
    }
    cout << "\n\n";
}

int SideFogCube::main(int argc, char **argv)
{
    //!ClanLib uses a class internal main, nice for c++.
    
    //! Create a console window for text-output if not available
    CL_ConsoleWindow console("Console");
    console.redirect_stdio();
    
    quit = false;
    try
    {
        //! Initialize ClanLib base components
        setup_core = new CL_SetupCore();
        setup_core->init();
        //! Initialize the ClanLib display component
        setup_display = new CL_SetupDisplay();
        setup_display->init();
        //! Initilize the OpenGL drivers
        setup_gl = new CL_SetupGL();
        setup_gl->init();
        glState = new CL_OpenGLState();
        glState->set_active();
        //! Removing the true on this function will
        //! stop the program from using fullscreen.
        window = new CL_OpenGLWindow(string("ClanLib Learn OpenGL Example"), SCR_WIDTH, SCR_HEIGHT, true);
        if (window == NULL)
        {
            std::cout << "Failed to create ClanLib window" << std::endl;
            setup_gl->deinit();
            setup_display->deinit();
            setup_core->deinit();
            return 1;
        }
        
        //! Connect the Window close event
        slot_quit = window->sig_window_close().connect(this, &SideFogCube::windowClose);

        //! Connect the various signals to their slots.
        progIC = window->get_ic();
        keyboardID = &progIC->get_keyboard();
        mouseID = &progIC->get_mouse();
        slot_input_up = keyboardID->sig_key_up().connect(this, &SideFogCube::processInput);
        slot_input_down = keyboardID->sig_key_down().connect(this, &SideFogCube::keyDown);
        slot_mouse = mouseID->sig_pointer_move().connect(this, &SideFogCube::mouseMove);
        gl_state = new CL_OpenGLState(window->get_gc()); 
        gl_state->set_active();
        if (gl_state->is_active() > 0)
        {
            value = "True";
        } 
        else
        {
            value = "False";
        }
        //! Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
        glewExperimental = GL_TRUE;
        cout << "\n\n\tThe GL state is active:  " << value << "\n";
        GLenum err = glewInit();
        framebufferSize(SCR_WIDTH, SCR_HEIGHT);
        if (GLEW_OK != err)
        {
            cout << "\n\n\tFailed to initialize GLEW.\n";
            setup_gl->deinit();
            setup_display->deinit();
            setup_core->deinit();
            return 1;
        }
        cout << "\n\n\tUsing GLEW Version: " << glewGetString(GLEW_VERSION) << "\n\n";
        glEnable(GL_DEPTH_TEST);  
        bool enabled = glIsEnabled(GL_DEPTH_TEST);
        if (enabled)
        {
            cout << "\n\n\tDepth Test Enabled\n\n";
        }
        else
        {
            cout << "\n\n\tDepth Test Not Enabled\n\n";
        }
        glDepthFunc(GL_LESS);
        //!glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        enabled = glIsEnabled(GL_CULL_FACE);
        if (enabled)
        {
            cout << "\n\n\tCull Face Enabled\n\n";
        }
        else
        {
            cout << "\n\n\tCull Face Not Enabled\n\n";
        }
        glCullFace(GL_BACK);
        glDepthRange(0.1f, 1000.0f);
    }
    catch(exception exc)
    {
        cout << "\n\n\tProgram Initialization Error:  " << exc.what() << "\n\n";
    }
    camera = new Camera(1000, 900, initPos);
    //! Define and compile the shaders.
    shader = new Shader();
    shader->initShader(string("fogvec.glsl"),
    string("fogfrag.glsl"), string("objshader.bin"));
    //! Set the background image.
    image = new CreateImage();
    image->setImage("container.png");
    texture1 = image->textureObject();
    //! Set the foreground images.
    image->create2DTexArray(texImages, imageNames);
    //! Define the locations and image indices.
    permLoc();
    //! Generate the object.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(4, VBO);
    //! 1. Bind the Vertex Array Object.
    glBindVertexArray(VAO);
    //! 2. copy our vertices arrays into the buffers for OpenGL to use.
    //! Position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(calcCube), calcCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //! Normal attribute.
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(calcNorm), calcNorm, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    //! Texture attribute.
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(calcTex), calcTex, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    //! Uniform buffer to feed the uniform.
    dataIndex = glGetUniformBlockIndex(shader->Program, "itemData");   
    glUniformBlockBinding(shader->Program, dataIndex, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, VBO[3]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(itemData), (void*)&itemData,
    GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindVertexArray(0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, dataIndex);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, VBO[3], 0, sizeof(itemData));
    //! Variables for the event loop.
    int degrees = 0;
    degrees = (int)
    //! Initialize the random number generator.
    chrono::system_clock::now().time_since_epoch().count();
    degrees = abs(degrees);
    srand(degrees);
    //! Grab a time to count degrees by the clock.
    start = chrono::system_clock::now();
    //! render loop
    //! ----------
    float radius = 10.0f;
    int count = 0;
    while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
    {
        //! Grab a time to adjust camera speed.
        intbegin  = chrono::system_clock::now();
        //! Reset the model.
        model = mat4(1.0f);
        //! Find the camera.
        projection = camera->GetPerspective();
        view = camera->GetViewMatrix(); //! render
        viewPos = camera->GetPosition();

        float camX = sin((double)degrees * onedegree) * radius * 1.5f;
        float camZ = cos((double)degrees * onedegree) * radius * 1.5f;
        //! ------ Count degrees rotated.
        end = chrono::system_clock::now();
        degrees = (int) chrono::duration_cast<chrono::milliseconds>(end - start).count();
        degrees /= 10;
        degrees %= 360;
        //! Use the shaders.
        vec4 color = vec4(0.3f, 0.3f, 0.3f, 0.5f); 
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->Use();
        //! We have a fog toggle on the space key.
        shader->setBool("foggy", foggy);
        //! Start feeding in the buffer data.
        glBindVertexArray(VAO);
        //! Initialize the lighting system.
        string uniname = "";
        string index = "";
        for(unsigned int i = 0; i < NUM_LIGHTS; i++)
        {
            stringstream ss;
            ss << i; 
            index = ss.str(); 
            uniname = "lighting[" + index + "].lightPos";
            shader->setVec3(uniname.c_str(), lighting[i].lightPos);
            uniname = "lighting[" + index + "].lightColor";
            shader->setVec4(uniname.c_str(), lighting[i].lightColor);
        }  

        viewPos = camera->GetPosition();
        //! Sort the locations based on the current camera
        //! position.
        sortDists(degrees);
        //! Set the crate background.
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture1);
        shader->setInt("cratetex", 0);
        //! Set the foreground images.
        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D_ARRAY, texImages);
        shader->setInt("tex", 1);
        //! Pass in the necessary uniforms.
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setVec3("viewPos", viewPos);
        shader->setVec4("fogColor", vec4(0.3f, 0.3f, 0.3f, 1.0f));
        //! Fog is variable based on the right arrow 
        //! and left arrow keys.
        shader->setFloat("fogMinDist", minfog);
        shader->setFloat("fogMaxDist", maxfog);
        glBindBuffer(GL_UNIFORM_BUFFER, VBO[3]);
        //! Pass the image indices and cube distances.
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(itemData), (void*) &itemData); 
        glBindBuffer(GL_UNIFORM_BUFFER, 0);    
        //! Use the instancing feature to create 
        //! multiple copies of each set of images.
        for (int x = 0; x < NUM_IMAGES; x++)
        {
            shader->setInt("repetition", x);
            int beginvert = 0;
            int endvert = 6;
            int sides = 6;
            //! Call for each image one side at a time.
            for (int side = 0; side < 6; side++)
            {
                shader->setInt("side", side);
                glDrawArraysInstanced(GL_TRIANGLES, beginvert, endvert, NUM_INSTANCES);
                beginvert += sides;
                endvert += sides;
            
            }
        }
        //! Uncomment this to get a listing of the 
        //! uniforms recognized by the shaders.
        //!UniformPrinter printer2(shader->Program);
        glBindVertexArray(0); 
        intend = chrono::system_clock::now();
        //! Swap buffers
        CL_Display::flip();
        CL_System::keep_alive();
    }

    //! ------------------------------------------------------------------
    setup_core->deinit();
    setup_display->deinit();
    setup_gl->deinit();
    return 0;
}

void SideFogCube::permLoc()
{
    //! Calculate the location and indices.
    PosOrient *locItem;
    //! Random angle for each instance, compute the MVP later
    for (int x = 0; x < NUM_IMAGES * NUM_INSTANCES; x++)
    {
        loc[x] = vec3(calcRand(x), calcRand(x), calcRand(x) - 15.0f);
    
        for (int y = 0; y < x; y++)
        {
            //! Check for collision and correct if necessary.
            if (distance(loc[x], loc[y]) < 1.50)
            {
                loc[x] = vec3(calcRand(x), calcRand(x), calcRand(x) - 15.0f);
                //! Reset the loop so the system can check  
                //! if it is an acceptable location.
                y = 0;
            }
        }
        //! Find the spin axis.
        xaxis[x] = vec3(calcRand(1), calcRand(1), calcRand(1));
        normalize(xaxis[x]);
        yaxis[x] = vec3(calcRand(1), calcRand(1), calcRand(1));
        normalize(yaxis[x]);
        angles[x] = randAxis();
        //! Create a new data item.
        locItem = new PosOrient();
        locItem->locon = loc[x];
        locItem->angles = angles[x];
        locItem->xaxis = xaxis[x];
        locItem->yaxis = yaxis[x];
        locItem->dist = 0;
        //! Calculate six image indices.
        for (int y = 0; y < 6; y++)
        {
            locItem->index[y] = rand() % NUM_IMAGES;
        }
        //! Add the item to the collection.
        distVals.push_back(*locItem);
    }
}

//! ---------------------------------------------------------------------------------------------------------
void SideFogCube::keyDown(const CL_InputEvent &key)
{
    int delta = (int)
    //! Use timing to create a cameraSpeed variable.
    chrono::duration_cast<chrono::nanoseconds>(intend -
    intbegin).count();
    float deltaTime = (float)delta / 10000000.0f;
    float cameraSpeed = 0.5f * deltaTime;
    //! Toggle the fog.
    if (key.id == CL_KEY_SPACE)
    {
        foggy = !foggy;
        if (foggy)
        {
            shader->setInt("foggy", foggy);
        }
        else
        {
            shader->setInt("foggy", foggy);
        }
    }
    //! Motion keys.
    if (key.id == CL_KEY_W)
    {
        camera->ProcessKeyboard(Camera::Camera_Movement::FORWARD, cameraSpeed);
    }
    if (key.id == CL_KEY_S)
    {
        camera->ProcessKeyboard(Camera::Camera_Movement::BACKWARD, cameraSpeed);
    }
    if (key.id == CL_KEY_A)
    {
        camera->ProcessKeyboard(Camera::Camera_Movement::LEFT, cameraSpeed);
    }
    if (key.id == CL_KEY_D)
    {
        camera->ProcessKeyboard(Camera::Camera_Movement::RIGHT, cameraSpeed);
    }
    if (key.id == CL_KEY_R)
    {
        camera->ProcessKeyboard(Camera::Camera_Movement::UP, cameraSpeed);
    }
    if (key.id == CL_KEY_F)
    {
        camera->ProcessKeyboard(Camera::Camera_Movement::DOWN, cameraSpeed);
    }
    //! Reset the camera.
    if (key.id == CL_KEY_Z)
    {
        camera->resetCamera();
        
    }
    //! Zoom keys.
    if (key.id == CL_KEY_UP)
    {
        camera->ProcessMouseScroll(Camera::Camera_Movement::CLOSER);
    }
    if (key.id == CL_KEY_DOWN)
    {
        camera->ProcessMouseScroll(Camera::Camera_Movement::AWAY);
    }
    //! Fog distance keys.
    if (key.id == CL_KEY_RIGHT)
    {
        minfog += 1.0f;
        maxfog += 1.0f;
        if (maxfog > 60.0f)
        {
            maxfog = 60.0f;
            minfog = 35.0f;
        }
    }
    if (key.id == CL_KEY_LEFT)
    {
        minfog -= 1.0f;
        maxfog -= 1.0f;
        if (minfog < 0.1f)
        {
            minfog = 0.1f;
            maxfog = 25.0f;
        }
    }
}    

void SideFogCube::processInput(const CL_InputEvent &key)
{
    cout << "\n\n\tIn processInput.  " << key.str << "\n\n";
    if(key.id == CL_KEY_ESCAPE)
    {
        quit = true;
    }
}

void SideFogCube::mouseMove(const CL_InputEvent &key)
{
    //! Orient the camera.
    CL_Point loc = key.mouse_pos;
    xpos = loc.x;
    ypos = loc.y;
    int repeat = key.repeat_count;
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    camera->ProcessMouseMovement(xoffset, yoffset);
    
}    

//! ---------------------------------------------------------------------------------------------
void SideFogCube::framebufferSize(int width, int height)
{
    //! make sure the viewport matches the new window dimensions; note that width and 
    //! height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    //! Set a videomode - 640x480
}

void SideFogCube::windowClose()
{
    cout << "\n\n\tIn on_window_close.\n\n";
    quit = true;
}

void SideFogCube::genMatrices()
{
    //! We generate three matrices:  the vertices of the cube,
    //! the normals and the texture coordinates.
    vec3 triangle[3];
    vec3 normal[3];
    vec2 texture[3];
    int countTex = 0, countNorm = 0;
    for (int x = 0; x < 36; x+= 3)
    {
        for (int y = 0; y < 3; y++)
        {
            triangle[y] = vec3(0.0f);
            normal[y] = vec3(0.0f);
            texture[y] = vec2(0.0f);
        }
        for (int y = 0; y < 3; y++)
        {
            triangle[0][y] = cube[(indices[x] * 3) + y];
            triangle[1][y] = cube[((indices[x + 1] * 3) + y)];
            triangle[2][y] = cube[((indices[x + 2]* 3) + y)];
        }

        if (((triangle[0].x == triangle[1].x)
        && (triangle[0].x == triangle[2].x)
        && (triangle[1].x == triangle[2].x)) 
        && (triangle[0].x > 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[0];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].x == triangle[1].x)
        && (triangle[0].x == triangle[2].x)
        && (triangle[1].x == triangle[2].x)) 
        && (triangle[0].x < 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[3];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].y == triangle[1].y)
        && (triangle[0].y == triangle[2].y)
        && (triangle[1].y == triangle[2].y)) 
        && (triangle[0].y > 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[1];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].y == triangle[1].y)
        && (triangle[0].y == triangle[2].y)
        && (triangle[1].y == triangle[2].y)) 
        && (triangle[0].y < 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[4];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].z == triangle[1].z)
        && (triangle[0].z == triangle[2].z)
        && (triangle[1].z == triangle[2].z)) 
        && (triangle[0].z > 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[2];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].z == triangle[1].z)
        && (triangle[0].z == triangle[2].z)
        && (triangle[1].z == triangle[2].z)) 
        && (triangle[0].z < 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[5];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
/*
        cout << "\n\n\t\tTriangle Vertices\n\n\t";
        for (int z = 0; z < 3; z++)
        {
            cout << "\n\tTriangle " << z << " : " << triangle[z].x
            << ", " << triangle[z].y << ", " << triangle[z].z;
        }
*/
        int count = 0;
        for (int z = 0; z < 3; z++)
        {
            //!cout << "\n\tTriangle x,y,z ";
            //!cout << "\n\tNormal x,y,z  + Echo x,y,z ";
            for (int y = 0; y < 3; y++)
            {
                calcNorm[countNorm] = normal[z][y];
                calcCube[countNorm] = triangle[z][y];
                //!cout << triangle[z][y] << ", ";
                //!cout << normal[z][y] << ", " << calcNorm[countNorm] 
                //!<< " : " << countNorm << " : ";
                countNorm++;
            }
            for (int y = 0; y < 2; y++)
            {
                calcTex[countTex] = texture[z][y];
                countTex++;
            }
        }
        //!cout << "\n\n";
    }
}
vec2 SideFogCube::genTexture(vec3 normal, vec3 triangle)
{
    //!Texture coordinates are generated seperately.
    vec2 texture;
    if (normal.x != 0.0f)
    {
        if (triangle.y > 0.0f)
        {   
            texture.x = 1.0f;
        }
        else
        {
            texture.x = 0.0f;
        }
        if (triangle.z > 0.0f)
        {   
            texture.y = 1.0f;
        }
        else
        {
            texture.y = 0.0f;
        }
    }
    else if (normal.y != 0.0f)
    {
    
        if (triangle.x > 0.0f)
        {   
            texture.x = 1.0f;
        }
        else
        {
            texture.x = 0.0f;
        }
        if (triangle.z > 0.0f)
        {   
            texture.y = 1.0f;
        }
        else
        {
            texture.y = 0.0f;
        }
    }
    else if (normal.z != 0.0f)
    {
        if (triangle.x > 0.0f)
        {   
            texture.x = 1.0f;
        }
        else
        {
            texture.x = 0.0f;
        }
        if (triangle.y > 0.0f)
        {   
            texture.y = 1.0f;
        }
        else
        {
            texture.y = 0.0f;
        }
    }
    return texture;
}

float SideFogCube::calcRand(int x)
{
    //! Random number for x,y,z values for cube location.
    const float MAXLOC = 25.0f;
    float item = 1000.0f;
    if (x == 0) 
    {
        x = 1;
    }
    while ((item < -MAXLOC) || (item > MAXLOC) || (item == 0))
    {
        float dividend = (float) (rand() % 100);
        float divisor = float(rand() % 100);
        while (divisor == 0.0f)
        {
            divisor = float(rand() % 100);
        }
        float scalar = dividend / (divisor + 1.0f);
        float sign = rand() % 10;
        if (sign < 5)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        item = sign * scalar * (float)(rand() % (int)MAXLOC);
        while (abs(item) > MAXLOC)
        {
            item /= 2.0f;
        }
    }
    return item;
}

float SideFogCube::randAxis()
{
    //! a random axis for cube rotation.
        float onedegree = 3.14159 / 180.0f;
        float result = (float)(rand() % 5);
        while (result == 0)
        {
            result = (float)(rand() % 5);
        }
        result *= onedegree;
        return result;
}

//! Less than operator for stable_sort.
bool SideFogCube::cmp(const PosOrient &a, const PosOrient &b)
{   
    return a.dist > b.dist;
}


void SideFogCube::sortDists(int degrees)
{
    for (int x = 0; x < NUM_INSTANCES * NUM_IMAGES; x++)
    {
        distVals[x].dist = distance(distVals[x].locon, viewPos);
        //!cout << "\n\n\tDistance: " << x << " : " << distVals[x].dist;
    }
    //! Sort uses the algorithm library.
    sort(distVals.begin(), distVals.end(), cmp);
    //! Create the data for the distVals data structure.
    for (int x = 0; x < NUM_IMAGES * NUM_INSTANCES; x++)
    {
        mat4 model = mat4(1.0f);
        model = translate(model, distVals[x].locon)
        * rotate(model, (float) degrees * distVals[x].angles * 2.0f, distVals[x].xaxis) 
        * rotate(model, (float) degrees * distVals[x].angles, distVals[x].yaxis);
        itemData.instModel[x] = model;
        //! Pass the uniform buffer data to the 
        //! itemData data structure.
        itemData.instIndex1[x] = vec4(distVals[x].index[0], distVals[x].index[1], distVals[x].index[2], distVals[x].index[3]);
        itemData.instIndex2[x] = vec2(distVals[x].index[4], distVals[x].index[5]);
        itemData.distance[x].x = distVals[x].dist;
    }
}

//! A peculiarity of ClanLib, the class creates itself.
SideFogCube my_app;
