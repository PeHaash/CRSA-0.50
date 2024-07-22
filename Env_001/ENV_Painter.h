#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

#include "DisjointSet.h"
#include "SharedLib.h"

#define __CUST_ENV__ env_01

#include "Defines.h"
#if !defined(MAX_X) || !defined(MAX_Y) || !defined(ROOM_COUNT)
#error "you should define some constants in compile time"
#endif

const int OBJECTIVE_COUNT = ROOM_COUNT * 3 + 1;



struct Shared {
    void* CustEnv;
    // Reset Options: 
    int* InputGrid;
    // Action:
    int x1, x2, y1, y2, color;
    // Observation:
    int* Pixels;
    int* UsedSpacesCount;

    // Score, Terminations:
    double Score;
    bool Terminated, Truncated;
    double* SubScores;
    Shared(void* cust_env, int* input_grid, int* pixels, int* used_space, double* Objs):
        CustEnv(cust_env), InputGrid(input_grid), Pixels(pixels), UsedSpacesCount(used_space){
            x1 = y1 = x2 = y2 = color = 0;
            Score = 0;
            Terminated = Truncated = false;
            SubScores = Objs;
        }
    Shared() = default;
};

/*
struct ResetInput {
    int* GridShape;
};

struct StepInput {
    int x1,y1,x2,y2,color;
};

struct Environment {
    int* Pixels;
    int* UnusedSpaces;
};

struct StepOutput {
    // --> tuple[ObsType, SupportsFloat, bool, bool, dict[str, Any]]
    Environment Observation;
    double Score;
    bool Terminated;
    bool Truncated;
    // a dict also of info!
};

struct RenderOutput {
    int Len;
    char* Chars;
};
*/
// Felan negaheshoon dar, ba'dan jodash mikonam

struct Objectives {
    double WeHaveRoom[ROOM_COUNT]; // 0 or 1
    double AllBlankSpacesAreUsed; // 0 to 1
    double RoomsAreConnected[ROOM_COUNT]; // 0 to 1
    double RoomsHaveDesiredArea[ROOM_COUNT];

    Objectives(double t) {
        AllBlankSpacesAreUsed = t;
        for (int i = 0; i < ROOM_COUNT; i++) {
            WeHaveRoom[i] = RoomsAreConnected[i] = RoomsHaveDesiredArea[i] = t;
        }
    }
    static double Average(Objectives s, Objectives w) {
        double sum = 0, sumOfws = 0;
        double* ss = reinterpret_cast<double*>(&s);
        double* ww = reinterpret_cast<double*>(&w);
        for (int i = 0; i < OBJECTIVE_COUNT; i++) {
            sum += ss[i] * ww[i];
            sumOfws += ww[i];
        }
        return sum / sumOfws;
    }
    void Print() const {
        std::cout << "----------\n";

        std::cout << "We have the rooms: ";
        for (int i = 0; i < ROOM_COUNT; i++)
            std::cout << i << ": <" << WeHaveRoom[i] << ">, ";
        std::cout << '\n';

        std::cout << "All blanks are used?: " << AllBlankSpacesAreUsed << '\n';

        std::cout << "Rooms connected: ";
        for (int i = 0; i < ROOM_COUNT; i++)
            std::cout << i << ": <" << RoomsAreConnected[i] << ">, ";
        std::cout << '\n';

        // about the correct size
        std::cout << "The correct size?: ";
        for (int i = 0; i < ROOM_COUNT; i++)
            std::cout << i << ": <" << RoomsHaveDesiredArea[i] << ">, ";
        std::cout << '\n';

        std::cout << "----------\n";
    }
};


class env_01 {
    // ma inja miaim ye kar mikonim: ye ghalam midim daste agent ke hey subspace bekeshe!

public:
    Shared shared_data; // data shared between the front & back
private:
    double PreviousScore = 0; // we only send Delta!
    int RoomPixelsCount[ROOM_COUNT] = {0};
    int RoomPixelsGoal[ROOM_COUNT] = GOAL_AREAS; // MOVE IT IN THE RESET!!! (with other stuff)
    int Pixels[MAX_X + 1][MAX_Y + 1];
    int InputGrid[MAX_X + 1][MAX_Y + 1]={2};
    int UsedSpaceCount;
    int step_count = 0;
    Objectives weights, scores;

private:
    void UpdateScores();
    bool checkCorrectSize();
    bool checkRooms();
    bool checkBlankSpaces();
    bool checkConnectivity();

public:
    env_01();
    int32_t Step(); //(int x1, int y1, int x2, int y2, int color);
        // gymnasium.Env.step(self, action: ActType) --> tuple[ObsType, SupportsFloat, bool, bool, dict[str, Any]]
        // Observation, Reward, Terminated, Turncated, info = Env.step(action)
    int32_t Reset();
    int32_t Render();
    int32_t Close();
};

