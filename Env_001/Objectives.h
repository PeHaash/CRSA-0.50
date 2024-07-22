
/// NOT YET!

/*
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
*/