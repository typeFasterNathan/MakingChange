#include <iostream>
#include <algorithm>

using namespace std;

class CoinSolution{
    public:
        CoinSolution() {
            totalCoins = 0;    
        }

	    CoinSolution(int numberOfTypes) {
			coinTypes = new int[numberOfTypes];
            fill_n(coinTypes, numberOfTypes, 0);
			totalCoins = 0; 
		}
        
        void InitCoinTypes(int numberOfTypes) {
            coinTypes = new int[numberOfTypes];
            fill_n(coinTypes, numberOfTypes, 0);
        }

        int* coinTypes;
	    int totalCoins;
}

int main() {

	return 0; 

}

int CalculateBottomUp(int[] coinProblems, int coinProblemsLength,
    int[] coinDenominations, int coinDenominationsLength) {
    
    int maxCoinProblem = *max_element(coinProblems, coinProblems + 
        coinProblemLength - 1);

    CoinSolution solvedProblems[coinProblems[maxCoinProblem]];
    solvedProblems[0].totalCoins = 0;
    // zero
    for(int i = 1; i < maxCoinProblem; i++) {
        int optimalLast = 100000;
        int coinType;
        for(int j = 0; j < coinDenominationsLength; j++) {
            if(i - coinDenominations[j] >= 0) {
                int temp = solvedProblems[i - coinDenominations[j]].totalCoins;
                if(temp + 1 < optimalLast) {
                    optimalLast = temp + 1;
                    coinType = j;
                }
            }
        }
        solvedProblems[i].totalCoins = optimalLast; 
        copy(solvedProblems[i - coinDenominations[j]], 
            solvedProblems[i - coinDenominations[j]] + coinDenominationsLength,
            solvedProblems[i];
        solvedProblems[i].coinTypes[coinType]++;
    }
}


