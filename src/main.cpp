#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <unistd.h>
#include <algorithm>
#include <set>
#include <string>

#include "../include/UI.h"
#include "../include/raycaster.h"
#include "../include/entity.h"
#include "../include/rectangle.h"
#include "../include/eventChecks.h"
#include "../include/wall.h"
#include "../include/movement.h"
#include "../include/world_utils.h"

int main()
{
    chdir(SDL_GetBasePath());
    srand((unsigned int)time(NULL));
    // 1080x1920
    const int WORLD_W = 800, WORLD_H = 800;
    const int SCREEN_W = 1600, SCREEN_H = 900;

    const int WALL_SIZE = 50;

    // UI ui{ WORLD_W, WORLD_H , 0 };
    UI ui{ SCREEN_W, SCREEN_H , 1800 };

    const int FPS = 60;                // set FPS
    const int frameDelay = 1000 / FPS; // delay according to FPS
    Uint32 frameStart;                 // keeps track of time (?)
    int frameTime;

    // RayCaster player = RayCaster(WORLD_W / 2, WORLD_H / 2);
    RayCaster player = RayCaster(0, 0);

    // for one wall in direction of wall in square (u = up, d = down, l = left, r = right)
    // for corners: F = ┌, T = ┐, L = └, J = ┘
    // for corridors: H = ║, B = ═
    int a = 10, b = 11, c = 12, d = 13, e = 14, f = 15; // wtf am i doing
    std::array<std::array<std::string, 16>, 16> worldMap2;
    worldMap2[0] = { "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[1] = { "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[2] = { "O0", "O0", "O0", "d1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[3] = { "O0", "O0", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[4] = { "O0", "F1", "B2", "O0", "B1", "T1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[5] = { "O0", "H1", "O0", "H1", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[6] = { "O0", "H2", "O0", "u1", "O0", "H2", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[7] = { "O0", "O0", "O0", "O0", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[8] = { "O0", "O0", "O0", "O0", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[9] = { "O0", "O0", "O0", "O0", "O0", "H2", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[a] = { "O0", "O0", "O0", "O0", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[b] = { "O0", "O0", "O0", "O0", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[c] = { "O0", "O0", "O0", "O0", "O0", "H1", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[d] = { "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[e] = { "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };
    worldMap2[f] = { "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0", "O0" };

    std::vector<Wall*> walls;
    // walls = createWorld(worldMap, WORLD_W, WORLD_H, WALL_SIZE, ui.getRenderer());
    walls = createWorld2(worldMap2, WORLD_W, WORLD_H, WALL_SIZE, ui.getRenderer());

    Uint32 mouseState;

    bool pause = false;
    bool quit = false;

    // SDL_Event event;
    EventChecks eventChecks;

    while (!eventChecks.getQuit())
    {
        frameStart = SDL_GetTicks(); // limit framerate (see end of while loop)
        eventChecks.checkEvents(player, ui, frameStart);
        // pause the game
        ui.clear();

        if (eventChecks.getPause())
        {
            SDL_SetRelativeMouseMode(SDL_FALSE); // getting mouse cursor again
            ui.drawBackground();
            ui.drawTexturedWalls(player.getDistances(), player.getPositionOnWall(), player.getTextureInfo());
            ui.drawMiniMap(walls, player.getRays());
            ui.printText("Game paused", 0, 0, { 255, 255, 255, 150 });

        }
        else
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);

            // SDL_PumpEvents();
            // mouseState = SDL_GetMouseState(&x, &y);
            // rayCaster.setPosition(ui, x, y);

            // check if the camera should rotate
            int direction = eventChecks.getRotationDirection();
            player.rotate(direction); // rotate camera
            // check which movements should be executed
            MovementFlag movement = eventChecks.getMovement();
            // move the camera accordingly



            // player.move(movement);
            // player.move2(eventChecks);

            player.emitLight(walls);

            player.checkCollision(walls);

            player.updatePreviousPosition();

            //============= 2D ===========================
            // ui.clear(); // render WORLD black
            // ui.drawWalls(walls);
            // ui.drawRays(player.getRays());
            // ui.present(); // render WORLD
            //================= 3D =====================

            ui.drawBackground();
            ui.drawTexturedWalls(player.getDistances(), player.getPositionOnWall(), player.getTextureInfo());
            ui.drawMiniMap(walls, player.getRays());

            ui.drawCrossHair();
        }// end else (pause game)
        ui.present();

        // frame time to limit FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        // std::cout << frameStart << std::endl;
    }

    for (int i = 0; i < walls.size(); i++)
    {
        delete walls[i];
        // std::cout << "clean up!" << std::endl;
    }

    return 0;
}