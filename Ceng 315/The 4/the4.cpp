#include "the4.h"

unsigned int alice_cutting_cake(const unsigned short cake_width, const unsigned short cake_height, bool** perfect_cuts) {
    std::vector<std::vector<unsigned int>> map(cake_width + 1, std::vector<unsigned int>(cake_height + 1, 0));


    for (unsigned short w = 1; w < cake_width + 1; w++) {
        for (unsigned short h = 1; h < cake_height + 1; h++) {
            if (perfect_cuts[w][h]) {
                map[w][h] = 0;
            }
            else map[w][h] = w * h;
                for (unsigned short i = 1; i < w; i++) {
                    map[w][h] = std::min(map[w][h], map[i][h] + map[w - i][h]);
                }

                for (unsigned short j = 1; j < h; j++) {
                    map[w][h] = std::min(map[w][h], map[w][j] + map[w][h - j]);
                }
            }
        }
    return map[cake_width][cake_height];
}
