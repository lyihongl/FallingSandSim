#include <cstdio>
#include <cstdlib>
#include <windows.h>

#define GLM_FORCE_RADIANS 1
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <random>
// #include <stb_image.h>

#include "inc/cell.hpp"
#include "inc/world.hpp"
#include "inc/shaders.hpp"
#include "inc/game_constants.hpp"

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;
static const float VP_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
static SDL_Window *window = NULL;
static SDL_GLContext maincontext;


uint32_t frame_count = 0;
RNG *rng_gen;

// std::uniform_int_distribution<int> *dist;

static void sdl_die(const char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}

void init_screen(const char *caption) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) sdl_die("Couldn't initialize SDL");
    atexit(SDL_Quit);
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create the window
    if (SCREEN_FULLSCREEN) {
        window = SDL_CreateWindow(
            caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        window = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    }
    if (window == NULL) sdl_die("Couldn't set video mode");

    maincontext = SDL_GL_CreateContext(window);
    if (maincontext == NULL) sdl_die("Failed to create OpenGL context");

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    // Disable depth test and face culling.
    // glShadeModel(GL_FLAT)
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
}

int main(int argc, char **argv) {
    std::random_device rd;
    std::mt19937 mt(rd());
    rng_gen = new RNG(mt, 1, 10);
    // dist = new std::uniform_int_distribution(0,10);
    init_screen("OpenGL 4.5");
    SDL_Event event;
    bool quit = false;
    World w(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
    // std::cout <<"world: "<< __PIXEL_WIDTH <<" "<<__PIXEL_HEIGHT<<std::endl;
    Shader s("./shaders/test.vert", "./shaders/test.frag");
    Uint32 time_step_ms = 1000 / 60;
    Uint32 next_game_step = SDL_GetTicks(); // initial value
    // std::string x;
    while (!quit) {
        Uint32 now = SDL_GetTicks();

        SDL_GL_SwapWindow(window);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                quit = true;
                break;
            }
            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                w.updateDim(event.window.data1, event.window.data2);
                break;
            }
            }
        }

        if (next_game_step <= now) {
            frame_count = (frame_count + 1) % UINT32_MAX;
            while (next_game_step <= now) {
                next_game_step += time_step_ms; // count 1 game tick done
            }
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            w.Render(s);
            w.update();
            // std::cin >> x;

            // RenderGame();
        }
    }
}
