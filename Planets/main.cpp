//
//  main.cpp
//  Planets
//
//  Created by Charlie Close on 05/04/2023.
//

#include<iostream>
#include"Mesh.h"
#include"Player.h"
#include"Shapes.h"
#include"Planet.h"
#include"Texture.h"
#include <thread>
#include"SimplexNoise.h"

const unsigned int width = 1200;
const unsigned int height = 800;

void CalculateMeshAsync(Planet* planet, glm::vec3 pos, bool* complete)
{
    planet->CalculateMesh(pos);
    *complete = true;
}

GLuint textures[3];

int main(int argc, const char * argv[]) {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
    GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    //Load GLAD so it configures OpenGL
    gladLoadGL();
    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, 2 * width, 2 * height);
    
    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    Shader planetProgram("Shaders/planet.vert", "Shaders/planet.frag");
    Shader waterProgram("Shaders/water.vert", "Shaders/water.frag");
    Shader atmosProgram("Shaders/atmos.vert", "Shaders/atmos.frag");
    Shader playerProgram("Shaders/player.vert", "Shaders/player.frag");

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 60.0f));
    Planet* planet = new Planet(glm::vec3(0, 0, 0), 40);
    bool* done = new bool(false);
    std::thread t (CalculateMeshAsync, planet, camera.Position, done);
    
    
    // water
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    
    // The texture we're going to render to
//    GLuint renderedTexture;
    glGenTextures(3, textures);
    
    glActiveTexture(GL_TEXTURE0);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width * 2, height * 2, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width * 2, height * 2);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textures[0], 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    
    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return 255;
    }
    
    Shader reflectMapProgram("Shaders/invert.vert", "Shaders/invert.frag");
    
    Texture grass("Textures/grass.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE, textures[1]);
    Texture player("Textures/astro.png", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_UNSIGNED_BYTE, textures[2]);
    
    shaderProgram.Activate();
    planetProgram.Activate();
    waterProgram.Activate();
    atmosProgram.Activate();
    playerProgram.Activate();
    
    grass.texUnit(planetProgram, "tex0", 1);
    player.texUnit(playerProgram, "Tex", 2);
    
    
    
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    
    t.join();
    *done = false;
    t = std::thread(CalculateMeshAsync, planet, camera.Position - planet->position, done);
    
    Player astro(glm::vec3(59.5), glm::vec3(0, 0, 1));
//    glm::vec3 playerPos(0, 0, 59.9);
    
    
    planet->SetMesh();
    
    while (!glfwWindowShouldClose(window))
    {
        
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 4 ){ // If last prinf() was more than 1 sec ago
            if (*done)
            {
                t.join();
                *done = false;
                t = std::thread(CalculateMeshAsync, planet, camera.Position - planet->position, done);
                planet->SetMesh();
            }
            // printf and reset timer
            printf("%f fps\n", double(nbFrames) / 4);
            nbFrames = 0;
            lastTime += 4;
        }

        // Handles camera inputs
        camera.Inputs(window, astro.pos);
        // Updates and exports the camera matrix to the Vertex Shader
//        camera.updateMatrix(45.0f, 1, 2000.0f);camera.updateMatrix(45.0f, 0.0005f, 0.05);
        camera.updateMatrix(45.0f, 0.5f, 500);
        
        // Clean the back buffer and depth buffer
        
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, 2 * width, 2 * height);
        planet->DrawR(camera, atmosProgram, planetProgram, grass);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//        planet->DrawR(camera, reflectMapProgram);
        planet->Draw(camera, atmosProgram, planetProgram, waterProgram, textures[0], grass);
//        astro.Draw(camera, playerProgram, player);
        
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    planetProgram.Delete();
    waterProgram.Delete();
    atmosProgram.Delete();
    reflectMapProgram.Delete();
    t.join();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
