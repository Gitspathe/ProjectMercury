#include <iostream>
#include <functional>
#include <chrono>
#include <SDL2/SDL.h>

#include "Common/Color.h"
#include "Render/OpenGLRenderer.h"
#include "Render/Screen.h"
#include "World/GameWorld.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::function<void()> loop;
bool running = true;
Render::OpenGLRenderer* renderer;
Render::Screen* screen;
bool isInit = false;
float px, py, pdx, pdy, pa;

#define mapX  8      //map width
#define mapY  8      //map height
#define mapS 20      //map cube size
#define DR 0.0174533
int map[]=           //the map array. Edit to change level but keep the outer walls
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void main_loop()
{
    loop();
}

float degToRad(int a)
{
    return a * M_PI / 180.0;
}

void drawPlayer()
{
    screen->drawPoint(8, px, py, Common::Color::getWhite(), true);
}

void flipMap() {
    int flippedMap[mapY * mapX];
    for (int y = 0; y < mapY; y++) {
        for (int x = 0; x < mapX; x++) {
            flippedMap[(mapY - 1 - y) * mapX + x] = map[y * mapX + x];
        }
    }
    std::copy_n(flippedMap, (mapY * mapX), map);
}

void drawMap2D() {
    int x, y, xo, yo;
    for(y = 0; y < mapY; y++){
        for(x = 0; x < mapX; x++){
            Common::Color color;
            if(map[y * mapX + x] == 1) {
                color = Common::Color::getWhite();
            } else {
                color = Common::Color::getBlack();
            }
            xo = x * mapS;
            yo = y * mapS;
            screen->drawPoint(mapS - 2, 20 + xo, 20 + yo, color);
        }
    }
}

void drawRays3D()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;

    for(r = 0; r < 60; r++) {
        // Normalize the player's angle (pa) to be within [0, 2 * PI]
        ra = -(pa * (M_PI / 180.0)) - (30 * DR); // Convert degrees to radians
        ra += r * DR;
        if (ra < 0) ra += 2 * M_PI;
        if (ra > 2 * M_PI) ra -= 2 * M_PI;

        // Cast one ray for now
        dof = 0;

        // Variables to store the intersection points and distances
        float hDist = 1000000, vDist = 1000000; // Initialize with a large value
        float hx = px, hy = py; // Horizontal intersection point
        float vx = px, vy = py; // Vertical intersection point

        // --- Horizontal raycasting (Y-axis) ---
        float aTan = -1 / tan(ra);

        // Determine initial ray direction based on the angle
        if (ra > M_PI) {
            // Looking up
            ry = ((static_cast<int>(py) / mapS) * mapS) - 0.001f;  // Step to the top grid line
            rx = (py - ry) * aTan + px; // Calculate initial x intersection
            yo = -mapS; // Moving up in Y direction
            xo = -yo * aTan; // X step
        } else if (ra < M_PI) {
            // Looking down
            ry = ((static_cast<int>(py) / mapS) * mapS) + mapS;  // Step to the bottom grid line
            rx = (py - ry) * aTan + px; // Calculate initial x intersection
            yo = mapS; // Moving down in Y direction
            xo = -yo * aTan; // X step
        } else {
            // Perfectly horizontal ray (no Y intersection)
            rx = px;
            ry = py;
            dof = 8; // Skip raycasting
        }

        while (dof < 8) {
            mx = (int)(rx) / mapS;  // Convert world coordinates to map grid index
            my = (int)(ry) / mapS;

            // Check if the ray is outside the map bounds
            if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
                mp = my * mapX + mx;
                if (map[mp] == 1) {
                    // Hit a wall
                    hx = rx;
                    hy = ry;
                    hDist = sqrt((hx - px) * (hx - px) + (hy - py) * (hy - py)); // Distance to wall
                    dof = 8; // Stop raycasting
                } else {
                    rx += xo; // Move to the next intersection
                    ry += yo;
                    dof++;
                }
            } else {
                // Exit if ray goes out of map bounds
                dof = 8;
            }
        }

        // --- Vertical raycasting (X-axis) ---
        float nTan = -tan(ra);

        // Determine initial ray direction based on the angle
        if (ra > M_PI / 2 && ra < 3 * M_PI / 2) {
            // Looking left
            rx = ((static_cast<int>(px) / mapS) * mapS) - 0.001f;  // Step to the left grid line
            ry = (px - rx) * nTan + py; // Calculate initial y intersection
            xo = -mapS; // Step left
            yo = -xo * nTan; // Y step
        } else if (ra < M_PI / 2 || ra > 3 * M_PI / 2) {
            // Looking right
            rx = ((static_cast<int>(px) / mapS) * mapS) + mapS;  // Step to the right grid line
            ry = (px - rx) * nTan + py; // Calculate initial y intersection
            xo = mapS; // Step right
            yo = -xo * nTan; // Y step
        } else {
            // Perfectly vertical ray (no X intersection)
            rx = px;
            ry = py;
            dof = 8; // Skip raycasting
        }

        dof = 0;
        while (dof < 8) {
            mx = (int)(rx) / mapS;
            my = (int)(ry) / mapS;

            // Check if the ray is outside the map bounds
            if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
                mp = my * mapX + mx;
                if (map[mp] == 1) {
                    // Hit a wall
                    vx = rx; vy = ry;
                    vDist = sqrt((vx - px) * (vx - px) + (vy - py) * (vy - py)); // Distance to wall
                    dof = 8; // Stop raycasting
                } else {
                    rx += xo; // Move to the next intersection
                    ry += yo;
                    dof++;
                }
            } else {
                // Exit if ray goes out of map bounds
                dof = 8;
            }
        }

        // Choose the closer intersection (horizontal or vertical)
        if (vDist < hDist) {
            rx = vx;
            ry = vy;
        } else {
            rx = hx;
            ry = hy;
        }

        // Draw the ray from player to the intersection point
        Common::Color color = Common::Color::getRed();
        screen->drawLine(px, py, rx, ry, color);
        ra += DR;
        if(ra < 0) {ra += 2*M_PI;}
        if(ra > 2*M_PI) {ra -= 2*M_PI;}
    }
}

void init()
{
    px = 100;
    py = 100;
    flipMap();
    screen = new Render::Screen(384, 192);
    renderer = new Render::OpenGLRenderer();
    renderer->setRenderScale(3.0f);
    renderer->init(new World::GameWorld(), screen);
    isInit = true;
}

int FixAng(int a)
{
    if(a > 359) { a -= 360; }
    if(a < 0) { a += 360; }
    return a;
}

void buttons(unsigned char key)
{
    if(key=='a'){ pa+=5; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
    if(key=='d'){ pa-=5; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
    if(key=='w'){ px+=pdx*5; py+=pdy*5;}
    if(key=='s'){ px-=pdx*5; py-=pdy*5;}

    // Keep player within map bounds
    px = std::max(0.0f, std::min(px, (float)mapX * mapS - 1));
    py = std::max(0.0f, std::min(py, (float)mapY * mapS - 1));
}

void run()
{
    auto started = std::chrono::high_resolution_clock::now();
    screen->clear(Common::Color::getBlack());
    drawMap2D();
    drawRays3D();
    drawPlayer();
    renderer->update();
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << std::endl;
}

int main()
{
    loop = [&] {
        if(!isInit) {
            init();
        }

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type){
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int x, y; SDL_GetMouseState(&x, &y);
                    break;
                }
                case SDL_KEYDOWN: {
                    if(e.key.keysym.sym >= 33 && e.key.keysym.sym <= 126) {
                        buttons(static_cast<char>(e.key.keysym.sym));
                    }
                }
                default:
                    break;
            }
        }
        run();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#endif

    init();

#ifndef __EMSCRIPTEN__
    while(running) main_loop();
#endif

    SDL_Quit();
    return 0;
}
