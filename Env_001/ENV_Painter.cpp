#include "ENV_Painter.h"



env_01::env_01() :
    weights(1), scores(0){
    PreviousScore = 0;
    shared_data = Shared(static_cast<void*>(this), &InputGrid[0][0], &Pixels[0][0], &UsedSpaceCount, (double*)&scores);
}

int32_t env_01::Reset() {
    scores = 0;
    PreviousScore = 0;
    UsedSpaceCount = 0;
    step_count = 0;
    for (int i = 0; i < ROOM_COUNT; i++) RoomPixelsCount[i] = 0;
    for (int i = 0; i <= MAX_X; i++)
        for (int j = 0; j <= MAX_Y; j++) {
            Pixels[i][j] = -1;
        }
    return 0;
}

int32_t env_01::Step(){ //(int x1, int y1, int x2, int y2, int color) {
    // code is 1-based, but the input is 0 based, so... +1,
    // ++ to keep 1-index
    int x1 = shared_data.x1+1, x2 = shared_data.x2+1, y1 = shared_data.y1+1, y2 = shared_data.y2+1, color = shared_data.color;

    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);

    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++) {
            if (Pixels[i][j] != color) {
                if (Pixels[i][j] != -1)
                    RoomPixelsCount[Pixels[i][j]]--;
                else {
                    // color is -1: new space unlocked!
                    UsedSpaceCount++;
                }
                RoomPixelsCount[color]++;
                Pixels[i][j] = color;
            }
        }
    UpdateScores();
    double NewScore = Objectives::Average(scores, weights);
    double delta = NewScore - PreviousScore;
    PreviousScore = NewScore;
    //return delta;
    step_count ++;
    shared_data.Score = delta;
    shared_data.Terminated = (step_count >= MAX_STEPS);
    shared_data.Truncated = false;
    return 0;
}

// gymnasium.Env.step(self, action: ActType) --> tuple[ObsType, SupportsFloat, bool, bool, dict[str, Any]]
// Observation, Reward, Terminated, Turncated, info = Env.step(action)

int32_t env_01::Render() {
    for (int j = 1; j <= MAX_Y; j++) {
        for (int i = 1; i <= MAX_X; i++) {
            if (Pixels[i][j] == -1)
                std::cout << "- ";
            else
                std::cout << Pixels[i][j] << ' ';
        }
        std::cout << '\n';
    }
    //std::cout << "used space count:" << UsedSpaceCount<<'\n';
    std::cout << "Scores: \n";
    scores.Print();
    //std::cout << "Weights: \n";
    //weights.Print();
    return 0;
}


int32_t env_01::Close() {
    return 0;
}

void env_01::UpdateScores() {
    //scr = Objectives(-1);
    if (checkBlankSpaces() || checkRooms() || checkConnectivity() || checkCorrectSize()) {

    }
}

bool env_01::checkCorrectSize() {
    for (int i = 0; i < ROOM_COUNT; i++) {
        scores.RoomsHaveDesiredArea[i] = MappedScore(0, RoomPixelsGoal[i], MAX_X * MAX_Y, RoomPixelsCount[i]) + 1;
    }
    return false;
}

bool env_01::checkRooms() {
    bool terminate = false;
    for (int i = 0; i < ROOM_COUNT; i++) {
        scores.WeHaveRoom[i] = ((RoomPixelsCount[i] == 0) ? 0 : 1) + 1;
        if (RoomPixelsCount[i] == 0) {
            terminate = true;
        }
    }
    return terminate;
}

bool env_01::checkBlankSpaces() {
    scores.AllBlankSpacesAreUsed = (double)(UsedSpaceCount) / (double)(MAX_X * MAX_Y) + 1;
    return false;
}

bool env_01::checkConnectivity() {
    DisjointSet connections((MAX_X + 2) * (MAX_Y + 2));
    std::vector<int> spaces[ROOM_COUNT];

    for (int i = 1; i <= MAX_X; i++) {
        for (int j = 1; j <= MAX_Y; j++) {
            if (Pixels[i][j] != -1)
                spaces[Pixels[i][j]].push_back(j * MAX_X + i);
        }
    }

    for (int i = 1; i <= MAX_X; i++) {
        for (int j = 1; j <= MAX_Y; j++) {
            if (Pixels[i][j] != -1) {
                if (Pixels[i][j] == Pixels[i][j + 1]) connections.Join(j * MAX_X + i, (j + 1) * MAX_X + i);
                if (Pixels[i][j] == Pixels[i + 1][j]) connections.Join(j * MAX_X + i, j * MAX_X + i + 1);
            }
        }
    }

    //RoomsAreConnected
    for (int i = 0; i < ROOM_COUNT; i++) {
        scores.RoomsAreConnected[i] = connections.GetDisjointnessOfElements(spaces[i]) + 1;
    }
    return false;

}