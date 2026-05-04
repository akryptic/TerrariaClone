#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>
#include <worldGenerator.h>
#include <imgui.h>

#include <cstdint>
#include <string>

struct GameData
{
    GameMap gameMap;
    Camera2D camera;
    std::uint16_t selectedBlock = Block::dirt;
} gameData;

AssetManager assetManager;

bool initGame()
{
    assetManager.loadAll();

    generateWorld(gameData.gameMap);

    gameData.camera.target = {20, 120}; // world-space center of view, we still use this as the camera position
    gameData.camera.rotation = 0.0f;
    gameData.camera.zoom = 100.0f;

    return true;
}

bool updateGame()
{
    float deltaTime = GetFrameTime();
    if (deltaTime > 1.f / 5)
    {
        deltaTime = 1.f / 5;
    }

    gameData.camera.offset = {GetScreenWidth() / 2.f, GetScreenHeight() / 2.f}; // screen-space center of view

    ClearBackground({75, 75, 150, 255});

#pragma region camera movement
    static float CAMERA_SPEED = 10;
    if (IsKeyDown(KEY_A))
    {
        gameData.camera.target.x -= CAMERA_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_D))
    {
        gameData.camera.target.x += CAMERA_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_W))
    {
        gameData.camera.target.y -= CAMERA_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_S))
    {
        gameData.camera.target.y += CAMERA_SPEED * deltaTime;
    }
#pragma endregion

    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
    int blockX = (int)floor(worldPos.x);
    int blockY = (int)floor(worldPos.y);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
        if (b)
        {
            *b = {};
        }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
        if (b)
        {
            b->type = gameData.selectedBlock;
        }
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        gameData.selectedBlock++;
        if (gameData.selectedBlock >= Block::BLOCKS_COUNT)
        {
            gameData.selectedBlock = 0;
        }
    }

    if (IsKeyPressed(KEY_R))
    {
        if (gameData.selectedBlock <= 0)
        {
            gameData.selectedBlock = Block::BLOCKS_COUNT - 1;
        }
        gameData.selectedBlock--;
    }

    std::string text = std::string("Selected Block: ") + blockNames[static_cast<int>(gameData.selectedBlock)];

    BeginMode2D(gameData.camera);

    Vector2 topLeftView = GetScreenToWorld2D({0, 0}, gameData.camera);
    Vector2 bottomRightView = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, gameData.camera);

    int startXView = (int)floorf(topLeftView.x - 1);
    int endXView = (int)ceilf(bottomRightView.x + 1);
    int startYView = (int)floorf(topLeftView.y - 1);
    int endYView = (int)ceilf(bottomRightView.y + 1);

    startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
    endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

    startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
    endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);

    for (int y = startYView; y < endYView; y++)
    {
        for (int x = startXView; x < endXView; x++)
        {
            auto &b = gameData.gameMap.getBlockUnsafe(x, y);

            if (b.type != Block::air)
            {
                DrawTexturePro(
                    assetManager.textures,
                    getTextureAtlas(b.type, 0, 32, 32), // source
                    {(float)x, (float)y, 1, 1},         // dest
                    {0, 0},                             // origin (top-left corner)
                    0.0f,                               // rotation
                    WHITE                               // tint
                );
            }
        }
    }

    // Draw Selected block
    DrawTexturePro(
        assetManager.frame,
        {0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height}, // source
        {(float)blockX, (float)blockY, 1, 1},                                      // dest
        {0, 0},                                                                    // origin (top-left corner)
        0.0f,                                                                      // rotation
        WHITE                                                                      // tint
    );

    EndMode2D();
    DrawText(text.c_str(), 10, 10, 20, WHITE);
    DrawFPS(10, 40);

    ImGui::Begin("Game Controlls");

    ImGui::SliderFloat("Camera zoom: ", &gameData.camera.zoom, 5, 150);
    ImGui::SliderFloat("Camera speed: ", &CAMERA_SPEED, 5, 100);

    ImGui::NewLine();
    static int targetFPS = 240;
    const int MAX_FPS = 1000;
    const char *label = (targetFPS == MAX_FPS) ? "Unlimited" : "%d FPS";
    if (ImGui::SliderInt("Target FPS: ", &targetFPS, 10, 1000, label))
    {
        (targetFPS == MAX_FPS) ? SetTargetFPS(0) : SetTargetFPS(targetFPS);
    }

    ImGui::End();

    return true;
}

void closeGame()
{
}