#include <iostream>
#include <iomanip>
#include <locale>

#define maxTrys 90
#define softPity 74
#define baseProbability 0.6f
#define softPityProbability 6

#define weaponMaxTrys 80
#define weaponSoftPity 63
#define weaponBaseProbability 0.7f
#define weaponSoftPityProbability 5.52f
#define weaponPityMaxTrys 3

double probabilityOf[maxTrys + 1];
double chanceOfNeeding[maxTrys + 1];

double weaponProbabilityOf[weaponMaxTrys + 1];
double weaponChanceOfNeeding[weaponMaxTrys + 1];
double weaponProbabilityOf_Pity[weaponPityMaxTrys + 1];
double weaponChanceOfNeeding_Pity[weaponPityMaxTrys + 1];

void preCalculationsWeapon() {
	weaponProbabilityOf[0] = 0;
	for (int i = 1; i < weaponSoftPity; i++) {
		weaponProbabilityOf[i] = weaponBaseProbability;
	}
	for (int i = weaponSoftPity; i <= weaponMaxTrys; i++) {
		weaponProbabilityOf[i] = weaponBaseProbability + weaponSoftPityProbability * (i - weaponSoftPity + 1);
		std::cout << i << ": " << weaponProbabilityOf[i] << std::endl;
		if (weaponProbabilityOf[i] > 100) { weaponProbabilityOf[i] = 100; }
	}
	for (int i = 0; i <= weaponMaxTrys; i++) {
		static double peopleLeftW = 1000;
		double peopleWon = peopleLeftW * weaponProbabilityOf[i] / 100;
		peopleLeftW -= peopleWon;
		weaponChanceOfNeeding[i] = peopleWon / 1000;
	}

	weaponProbabilityOf_Pity[0] = 0;
	weaponProbabilityOf_Pity[1] = 0.75 * 0.5;
	weaponProbabilityOf_Pity[2] = 0.25 * 0.5 + (0.75 * 0.5) * (0.75 * 0.5);
	weaponProbabilityOf_Pity[3] = 0.25 * 0.5 * 1 + (0.75 * 0.5) * 0.25 + (0.75 * 0.5) * (0.75 * 0.5);
	double x = 0;
	for (int i = 0; i <= 3; i++) {
		//std::cout << weaponProbabilityOfPity[i] << std::endl;
		x += weaponProbabilityOf_Pity[i];
	}
	//std::cout << std::endl << x << std::endl;
}

void preCalculationsCharacter() {
	probabilityOf[0] = 0;
	for (int i = 1; i < softPity; i++) {
		probabilityOf[i] = baseProbability;
	}
	for (int i = softPity; i <= maxTrys; i++) {
		probabilityOf[i] = baseProbability + softPityProbability * (i - softPity + 1);
		if (probabilityOf[i] > 100) { probabilityOf[i] = 100; }
	}

	for (int i = 0; i <= maxTrys; i++) {
		static double peopleLeft = 1000;
		double peopleWon = peopleLeft * probabilityOf[i] / 100;
		peopleLeft -= peopleWon;
		chanceOfNeeding[i] = peopleWon / 1000;
	}

}

double* chancesToPullCharacter(int recursions) {
	if (recursions < 1) { return { 0 }; }
	if (recursions == 1) { return chanceOfNeeding; }

	double* sumOfPosibilitys = new double[maxTrys * recursions + 1]{ 0 };

	if (recursions % 2 == 0) {
		double* Iteration = chancesToPullCharacter(recursions / 2);

		for (int i = 0; i <= maxTrys * recursions / 2; i++) {
			for (int j = 0; j <= maxTrys * recursions / 2; j++) {
				sumOfPosibilitys[i + j] += Iteration[i] * Iteration[j];
			}
		}
		//this is the same as:
		/*

		double* Iteration1 = new double[maxTrys * (recursions / 2) + 1];
		Iteration1 = chancesToPull(recursions / 2);
		double* Iteration2 = new double[maxTrys * (recursions / 2) + 1];
		Iteration2 = chancesToPull(recursions / 2);

		for (int i = 0; i <= maxTrys * recursions / 2; i++) {
			for (int j = 0; j <= maxTrys * recursions / 2; j++) {
				sumOfPosibilitys[i + j] += Iteration1[i] * Iteration2[j];
			}
		}

		*/

	}
	else {
		double* Iteration1 = chancesToPullCharacter(1);
		double* Iteration2 = chancesToPullCharacter(recursions - 1);

		for (int i = 0; i <= maxTrys; i++) {
			for (int j = 0; j <= maxTrys * (recursions - 1); j++) {
				sumOfPosibilitys[i + j] += Iteration1[i] * Iteration2[j];
			}
		}

	}
	return sumOfPosibilitys;
}

void calculateWeaponPity(int* indices, int currentIndex, int remainingLoops, double* weaponChanceOfGuaranted, int amountWeapons) {
	if (remainingLoops == 0) {
		int sum = 0;
		for (int i = 0; i < currentIndex; i++) {
			sum += indices[i];
		}
		double probability = 1;
		for (int i = 0; i < currentIndex; i++) {
			probability *= weaponProbabilityOf_Pity[indices[i]];
		}
		weaponChanceOfGuaranted[sum] += probability;
		return;
	}
	for (int i = 0; i <= 3; i++) {
		indices[currentIndex] = i;
		calculateWeaponPity(indices, currentIndex + 1, remainingLoops - 1, weaponChanceOfGuaranted, amountWeapons);
	}
}

void weaponPity(double* weaponChanceOfGuaranted, int amountWeapons) {
	int* indices = new int[amountWeapons];
	calculateWeaponPity(indices, 0, amountWeapons, weaponChanceOfGuaranted, amountWeapons);

	double* acumulatedChance = new double[amountWeapons * 3 + 1]{ 0 };
	for (int i = 1; i <= amountWeapons * 3; i++) {
		acumulatedChance[i] = weaponChanceOfGuaranted[i] + acumulatedChance[i - 1];
	}
	for (int i = 0; i <= amountWeapons * 3; i++) {
		//std::cout << i << ": " << weaponChanceOfGuaranted[i] * 100 << " :: " << acumulatedChance[i] * 100 << std::endl;
	}
	delete[] indices;
	delete[] acumulatedChance;
}

double* temp(int rec) {
	if (rec < 0) { return{ 0 }; }
	if (rec == 0 || rec == 1) { return weaponChanceOfNeeding; }

	double* sum = new double[rec * weaponMaxTrys + 1]{ 0 };
	double* iteration = temp(rec - 1);
	for (int i = 0; i <= (rec - 1) * weaponMaxTrys; i++) {
		for (int j = 0; j <= weaponMaxTrys; j++) {
			sum[i + j] += iteration[i] * weaponChanceOfNeeding[j];
		}
	}
	return sum;
}

void probabilityOfRefinement(double* sumOfProbability, int amountWeapons) {

	double* weaponChanceOfPity = new double[amountWeapons * weaponPityMaxTrys + 1]{ 0 };
	weaponPity(weaponChanceOfPity, amountWeapons);

	for (int i = 0; i <= weaponPityMaxTrys * amountWeapons; i++) {
		double* tempvar = temp(i);
		for (int j = 0; j <= i * weaponMaxTrys; j++) {
			sumOfProbability[j] += weaponChanceOfPity[i] * tempvar[j];
		}
	}
	delete[] weaponChanceOfPity;
}

void weaponCount() {
	double allProbabilitys[5][5 * weaponMaxTrys * weaponPityMaxTrys + 1] = { 0 };
	double allCumulative[5][5 * weaponMaxTrys * weaponPityMaxTrys + 1] = { 0 };
	for (int amountWeapons = 5; amountWeapons > 0; amountWeapons--) {


		int maxPulls = amountWeapons * weaponPityMaxTrys;
		double* sumOfProbability = new double[maxPulls * weaponMaxTrys + 1]{ 0 };
		probabilityOfRefinement(sumOfProbability, amountWeapons);


		for (int i = 0; i <= maxPulls * weaponMaxTrys; i++) {
			allProbabilitys[amountWeapons - 1][i] = sumOfProbability[i];
		}

		for (int i = 1; i <= maxPulls * weaponMaxTrys; i++) {
			allCumulative[amountWeapons - 1][i] = sumOfProbability[i] + allCumulative[amountWeapons - 1][i - 1];
		}
		delete[] sumOfProbability;
	}

	/*
	for (int i = 0; i <= 5 * weaponMaxTrys * weaponPityMaxTrys; i++) {
		std::cout << i << ";";
		for (int j = 4; j >= 0; j--) {
			std::cout << std::scientific << std::setprecision(20)<< allProbabilitys[j][i] << ";" << allCumulative[j][i] << ";";
		}
		std::cout << std::endl;
	}
	*/
}

void characterCount() {
	int amountCharacters;
	std::cout << "Target Refinement?" << std::endl;
	std::cin >> amountCharacters;
	if (amountCharacters < 1) { return; }

	double* sumOfPosibilitys = new double[maxTrys * amountCharacters + 1]{ 0 };
	sumOfPosibilitys = chancesToPullCharacter(amountCharacters);

	double* acumulatedChance = new double[maxTrys * amountCharacters + 1]{ 0 };

	acumulatedChance[0] = sumOfPosibilitys[0];
	for (int i = 1; i <= maxTrys * amountCharacters; i++) {
		acumulatedChance[i] = sumOfPosibilitys[i] + acumulatedChance[i - 1];
	}
	for (int i = 0; i <= maxTrys * amountCharacters; i++) {
		std::cout << i << ": " << sumOfPosibilitys[i] * 100 << " :: " << acumulatedChance[i] * 100 << std::endl;
	}
}

int Fakultät(int n) {
	int Fak = 1;
	while (n > 0) {
		Fak *= n;
		n--;
	}
	return Fak;
}

double BinDistributionPercent(int pity, int fivtyfivtys) {
	int M = pow(2, fivtyfivtys);
	int binom = Fakultät(fivtyfivtys) / (Fakultät(pity - fivtyfivtys) * Fakultät(fivtyfivtys - (pity - fivtyfivtys)));
	//std::cout << "M: " << M << " bin: " << binom << std::endl;
	return (double)binom / (double)M;
}

void probabilityOfConstalation(double* sumOfPosibilitys, int targetConstalation) {
	int minPulls = targetConstalation + 1;
	int maxPulls = (targetConstalation + 1) * 2;

	for (int i = minPulls; i <= maxPulls; i++) {
		double* sum = new double[maxTrys * i + 1]{ 0 };
		sum = chancesToPullCharacter(i);
		double p = BinDistributionPercent(i, minPulls);
		for (int j = 0; j <= maxTrys * i; j++) {
			sumOfPosibilitys[j] += sum[j] * p;
		}
	}
}

void characterIncludingPity() {
	int targetConstalation = 6;
	std::cout << "Target Constalation?" << std::endl;
	std::cin >> targetConstalation;
	if (targetConstalation < 0) { return; }

	int minPulls = targetConstalation + 1;
	int maxPulls = (targetConstalation + 1) * 2;

	double* sumOfPosibilitys = new double[maxTrys * maxPulls + 1]{ 0 };
	double* acumulatedChance = new double[maxTrys * maxPulls + 1]{ 0 };

	probabilityOfConstalation(sumOfPosibilitys, targetConstalation);

	acumulatedChance[0] = sumOfPosibilitys[0];
	for (int i = 1; i <= maxTrys * maxPulls; i++) {
		acumulatedChance[i] = acumulatedChance[i - 1] + sumOfPosibilitys[i];
	}
	double x = 0;
	for (int i = 0; i <= maxTrys * maxPulls; i++) {
		std::cout /* << i << "; " */ << sumOfPosibilitys[i] << "; " << std::scientific << std::setprecision(20) << acumulatedChance[i];
		if (acumulatedChance[i] == acumulatedChance[i - 1]) {
			//std::cout << " no difference detected";
		}
		std::cout << std::endl;
	}
}

void charAndWeapon() {
	//Char:
	int targetConstalation = 6;
	std::cout << "Charackter Constalation: ";
	std::cin >> targetConstalation;
	if (targetConstalation < 0) { return; }

	int minCharPulls = targetConstalation + 1;
	int maxCharPulls = (targetConstalation + 1) * 2;

	int constalationSize = maxTrys * maxCharPulls;
	double* constalationProbability = new double[constalationSize + 1]{ 0 };
	probabilityOfConstalation(constalationProbability, targetConstalation);

	//Weapon:
	int amountWeapons = 5;
	std::cout << std::endl << "Weapon Refinement: ";
	std::cin >> amountWeapons;
	if (amountWeapons < 1) { return; }

	int minWeaponPulls = amountWeapons;
	int maxWeaponPulls = amountWeapons * weaponPityMaxTrys;

	int refinementSize = maxWeaponPulls * weaponMaxTrys;
	double* refinementProbability = new double[refinementSize + 1]{ 0 };
	probabilityOfRefinement(refinementProbability, amountWeapons);

	//Combined:
	double* bothProbability = new double[constalationSize + refinementSize + 1]{ 0 };
	double* bothAcumulation = new double[constalationSize + refinementSize + 1]{ 0 };
	for (int i = 0; i <= constalationSize; i++) {
		for (int j = 0; j <= refinementSize; j++) {
			bothProbability[i + j] += constalationProbability[i] * refinementProbability[j];
		}
	}
	for (int i = 1; i <= constalationSize + refinementSize; i++) {
		bothAcumulation[i] = bothProbability[i] + bothAcumulation[i - 1];
	}
	std::cout << "Step size: ";
	int stepSize;
	std::cin >> stepSize;
	std::cout << std::endl << std::endl;
	std::cout << "Wish : Probability of this Wish :: Probability with this amount of Wishes" << std::endl;
	for (int i = 0; i <= constalationSize + refinementSize; i += stepSize) {
		std::cout << i << " : " << std::setprecision(5) << bothProbability[i] * 100 << "% :: " << bothAcumulation[i] * 100 << "%" << std::endl;
	}


	//End:
	delete[] constalationProbability;
	delete[] refinementProbability;
	delete[] bothProbability;
	delete[] bothAcumulation;
}

int main()
{
	std::locale mylocale("");
	std::cout.imbue(mylocale);


	preCalculationsCharacter();
	//characterCount();
	//characterIncludingPity();

	preCalculationsWeapon();
	//weaponCount();

	//charAndWeapon();

	return 0;
}
