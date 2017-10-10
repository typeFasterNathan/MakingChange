#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

class CoinSolution{
    public:
        CoinSolution() {
            totalCoins = 0;    
            value = 0;
            numberOfTypes = 0;
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
        
        printCoinSolution(int [] denominationValues) {
            cout << value << " cents =";
            for(int i = 0; i < numberOfTypes; i++) {
                if(coinTypes[i] < 0) {
                    cout << denominationValues[i] << ":" << coinTypes[i] << " ";
                }
            }
            cout << endl;
        }
        
        int numberOfTypes;
        int* coinTypes;
	    int totalCoins;
        int value;
}

int main(int argc, char** argv) {
    if(agc < 2) {
        cout << "Ya done messed up" << endl;
    }
    string filename = new string(argv[1]);
    ifstream infile(filename);
    if(infile.is_open()) {
        string number; 
        getline(infile, number);
        int numberOfCoinDenominations = stoi(number);
        int denominations[numberOfCoinDenominations];
        for(int i = 0; i < numberOfCoinDenomintations; i++) {
            getline(infile, number);
            denominationsp[i] = stoi(number);
        }   
        getline(infile, number);
        int numberOfProblems = stoi(number);
        int problems[numberOfProblems];
        for(int i = 0; i < numberOfProblems; i++) {
            getline(infile, number);
            problems[i] = stoi(number);
        }
        
        CalculateBottomUp(problems, numberOfProblems, denominations, 
            numberOfCoinDenominations);
    }
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
        int optimalLast = 1000000;
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


