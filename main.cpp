#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <limits>
#include <chrono>

using namespace std;
typedef chrono::high_resolution_clock Clock;

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
            this->numberOfTypes = numberOfTypes;
        }
        
        void printCoinSolution(int denominationValues[]) {
            cout << value << " cents =";
            for(int i = 0; i < numberOfTypes; i++) {
                if(coinTypes[i] > 0) {
                    cout << denominationValues[i] << ":" << coinTypes[i] << " ";
                }
            }
            cout << endl;
        }
        
        int numberOfTypes;
        int* coinTypes;
	    int totalCoins;
        int value;
};

CoinSolution CalculateBottomUp(int, int*, int);
int* copyArrayValues(int* from, int length);
CoinSolution calculateMemo(int problem, CoinSolution * solved, int * coinDenominations,
	int coinDenominationsLength);
CoinSolution calculateRecursion(int problem, int * coinDenominations,
	int coinDenominationsLength);
int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "Ya done messed up" << endl;
        return 0;
    }
    string filename(argv[1]);
    ifstream infile(filename);
    if(infile.is_open()) {
        string number; 
        getline(infile, number);
        int numberOfCoinDenominations = stoi(number, nullptr);
        int *denominations = new int[numberOfCoinDenominations];
        for(int i = 0; i < numberOfCoinDenominations; i++) {
            getline(infile, number);
            denominations[i] = stoi(number);
        }   
        getline(infile, number);
        int numberOfProblems = stoi(number);
        int *problems= new int[numberOfProblems];
        for(int i = 0; i < numberOfProblems; i++) {
            getline(infile, number);
            problems[i] = stoi(number);
        }
		int maxCoinProblem = *max_element(problems, problems +
			numberOfProblems - 1);

		CoinSolution *solvedProblems = new CoinSolution[maxCoinProblem+1];
		for (int i = 0; i < numberOfProblems; i++) {
			auto startTime = Clock::now();
			CoinSolution result = CalculateBottomUp(problems[i], denominations,
				numberOfCoinDenominations);
			auto endTime = Clock::now();
			unsigned long int duration = (unsigned long int)chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();
			result.printCoinSolution(denominations);
		}
		solvedProblems[0].InitCoinTypes(numberOfCoinDenominations);
		for (int i = 0; i < numberOfProblems; i++) {
			CoinSolution result = calculateMemo(problems[i], solvedProblems, denominations, numberOfCoinDenominations);
			result.printCoinSolution(denominations);
		}
		for (int i = 0; i < numberOfProblems; i++) {
			if (problems[i] <= 40) {
				CoinSolution result = calculateRecursion(problems[i], denominations, numberOfCoinDenominations);
				result.printCoinSolution(denominations);
			}
		}
    }
	return 0; 

}

CoinSolution CalculateBottomUp(int problem, int* coinDenominations, 
	int coinDenominationsLength) {
 

    CoinSolution *solvedProblems = new CoinSolution[problem + 1];
    for (int i = 0; i <= problem; i++) {
        solvedProblems[i].InitCoinTypes(coinDenominationsLength);
    }
    solvedProblems[0].totalCoins = 0;
    // zero
    for(int i = 1; i <= problem; i++) {
        int optimalLast = numeric_limits<int>::max();
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
		solvedProblems[i].coinTypes = copyArrayValues(
			solvedProblems[i - coinDenominations[coinType]].coinTypes,
			coinDenominationsLength);

        solvedProblems[i].coinTypes[coinType]++;
		solvedProblems[i].value = i;
    }
	return solvedProblems[problem];
}

CoinSolution calculateMemo(int problem, CoinSolution * solved, int * coinDenominations,
	int coinDenominationsLength) {

	CoinSolution optimal(coinDenominationsLength);
	CoinSolution temp(coinDenominationsLength);
	int coin;
	optimal.totalCoins = numeric_limits<int>::max();
	for (int i = 0; i < coinDenominationsLength; i++) {
		if (solved[problem].value != problem && problem - coinDenominations[i] >= 0
			&& problem != 0) {
			temp = calculateMemo(problem - coinDenominations[i], solved, coinDenominations,
				coinDenominationsLength);
		}
		else if (solved[problem].value == problem) {
			return solved[problem];
		}
		if (temp.totalCoins < optimal.totalCoins) {
			optimal.coinTypes = copyArrayValues(temp.coinTypes, coinDenominationsLength);
			optimal.numberOfTypes = temp.numberOfTypes;
			optimal.totalCoins = temp.totalCoins;
			coin = i;
		}
	}
	optimal.value = problem;
	optimal.totalCoins++;
	optimal.coinTypes[coin]++;
	solved[problem] = optimal;
	return optimal;
}

CoinSolution calculateRecursion(int problem, int * coinDenominations,
	int coinDenominationsLength) {
	CoinSolution optimal(coinDenominationsLength);
	optimal.numberOfTypes = coinDenominationsLength;
	CoinSolution temp(coinDenominationsLength);
	temp.numberOfTypes = coinDenominationsLength;
	int coin;
	optimal.totalCoins = numeric_limits<int>::max();
	for (int i = 0; i < coinDenominationsLength; i++) {
		if (problem - coinDenominations[i] >= 0) {
			temp = calculateRecursion(problem - coinDenominations[i], coinDenominations,
				coinDenominationsLength);
		}
		if (temp.totalCoins < optimal.totalCoins) {
			optimal.coinTypes = copyArrayValues(temp.coinTypes, coinDenominationsLength);
			optimal.numberOfTypes = temp.numberOfTypes;
			optimal.totalCoins = temp.totalCoins;
			coin = i;
		}
	}
	if (problem != 0) {
		optimal.value = problem;
		optimal.totalCoins++;
		optimal.coinTypes[coin]++;
	}
	return optimal;
}

int* copyArrayValues(int* from, int length) {
   int* to = new int[length];
    for(int i = 0; i < length; i++) {
        to[i] = from[i];
    }
	return to;
}

void outputToCSV(int coinValue, long time, string algorithmType) {
    ofstream outputFile;
    outputFile.open("data.csv", ofstream::app);
    outputFile << algorithmType << "," << time << "," << coinValue << endl;
    outputFile.close();
}
