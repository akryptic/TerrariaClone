#include "worldGenerator.h"
#include <randomStuff.h>
#include <FastNoiseSIMD.h>
#include <memory>
#include <iostream>

void generateWorld(GameMap &gameMap, int seed)
{
    const int w = 900;
    const int h = 500;

    gameMap.create(w, h);

    int stoneSize = 100;
    int dirtSize = 50;

    std::ranlux24_base rng(seed);

    std::unique_ptr<FastNoiseSIMD> dirtNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> stoneNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> steepnessNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

    dirtNoiseGenerator->SetSeed(seed++);
    stoneNoiseGenerator->SetSeed(seed++);
    steepnessNoiseGenerator->SetSeed(seed++);

    dirtNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    dirtNoiseGenerator->SetFractalOctaves(1);
    dirtNoiseGenerator->SetFrequency(0.02);

    stoneNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    stoneNoiseGenerator->SetFractalOctaves(4);
    stoneNoiseGenerator->SetFrequency(0.01);

    steepnessNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
    steepnessNoiseGenerator->SetFractalOctaves(3);
    steepnessNoiseGenerator->SetFrequency(0.0001);

    float *dirtNoise = dirtNoiseGenerator->GetEmptySet(w);
    float *stoneNoise = stoneNoiseGenerator->GetEmptySet(w);
    float *steepnessNoise = steepnessNoiseGenerator->GetEmptySet(w);

    dirtNoiseGenerator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
    stoneNoiseGenerator->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);
    steepnessNoiseGenerator->FillNoiseSet(steepnessNoise, 0, 0, 0, w, 1, 1);

    // Convert from [-1, 1] to [0, 1]
    for (int x = 0; x < w; x++)
    {
        dirtNoise[x] = (dirtNoise[x] + 1) / 2;
        stoneNoise[x] = (stoneNoise[x] + 1) / 2;
        steepnessNoise[x] = (steepnessNoise[x] + 1) / 2;

        stoneNoise[x] = std::pow(stoneNoise[x], steepnessNoise[x]); // steeper mountains
    }

    int dirtOffsetStart = -5;
    int dirtOffsetEnd = 35;

    int stoneHeightStart = 80;
    int stoneHeightEnd = 170;

    for (int x = 0; x < w; x++)
    {

        int stoneHeight = stoneHeightStart + (stoneHeightEnd - stoneHeightStart) * stoneNoise[x];
        int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
        dirtHeight = stoneHeight - dirtHeight;

        for (int y = 0; y < h; y++)
        {
            Block b;
            bool isSurface = (y == dirtHeight);

            if (y > stoneHeight)
            {
                b.type = Block::stone;
            }
            else if (y > dirtHeight)
            {
                b.type = Block::dirt;
            }
            else if (isSurface && getRandomChance(rng, 0.88))
            {
                b.type = Block::grassBlock;
            }
            else
            {
            }

            gameMap.getBlockUnsafe(x, y) = b;
        }
    }

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h - 1; y++)
        {
            bool growGrass = getRandomChance(rng, 0.25) && (gameMap.getBlockUnsafe(x, y + 1).type == Block::grassBlock);
            if (growGrass)
            {
                std::cout << "grass placed at x: " << x << " y: " << y << std::endl;
                gameMap.getBlockUnsafe(x, y).type = Block::grass;
            }
        }
    }

    FastNoiseSIMD::FreeNoiseSet(dirtNoise);
    FastNoiseSIMD::FreeNoiseSet(stoneNoise);
    FastNoiseSIMD::FreeNoiseSet(steepnessNoise);
}