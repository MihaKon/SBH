#include <string>
#include <vector>
#include "./LevenshteinDistanceCalc.hpp"

int getLevenshteinDistance(std::string sequenceOne, std::string sequenceTwo)
{
    int size1 = sequenceOne.size();
    int size2 = sequenceTwo.size();
    std::vector<std::vector<int>> verif(size1 + 1, std::vector<int>(size2+1,0));

    if (size1 == 0)
        return size2;
    if (size2 == 0)
        return size1;

    for (int i = 0; i <= size1; i++)
        verif[i][0] = i;
    for (int j = 0; j <= size2; j++)
        verif[0][j] = j;

    for (int i = 1; i <= size1; i++)
    {
        for (int j = 1; j <= size2; j++)
        {
            int cost = (sequenceTwo[j - 1] == sequenceOne[i - 1]) ? 0 : 1;

            verif[i][j] = std::min(
                std::min(verif[i - 1][j] + 1, verif[i][j - 1] + 1),
                verif[i - 1][j - 1] + cost);
        }
    }

    return verif[size1][size2];
}
