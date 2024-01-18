#include <iostream>
#include <vector>
#include <iomanip>
#include "Character.h"

namespace Character {

#define BaseP 0.006
#define SoftPT 74
#define W_max 90

	std::vector<double> sucessRate_VECTOR;
	std::vector<std::vector<double>>Q_k_Matrix;
	std::vector<double>P_5050_Vector;




	int Factorial(int x) {
		int result = 1;
		while (x > 1) {
			result *= x;
			x--;
		}
		return result;
	}

	int BinomialCoefficient(int N, int n) {
		return Factorial(N) / (Factorial(N - n) * Factorial(n));
	}

	int Chars_Min(int C) {
		return C + 1;
	}

	int Chars_Max(int C) {
		return 2 * (C + 1);
	}

	int N(int n, int C) {
		return Chars_Max(C) - n;
	}

	int Total(int C) {
		return std::pow(2, Chars_Min(C));
	}

	double P_5050(int n, int C) {
		return (double)BinomialCoefficient(Chars_Min(C), n) / (double)Total(C);
	}

	double P_5050(int n, int C, bool pity) {
		if (pity) {
			if (n == 0) { return 0; }
			return P_5050(n - 1, C - 1);
		}
		return P_5050(n, C);
	}

	void get5050(int C, bool pity) {
		P_5050_Vector = std::vector<double>(Chars_Min(C) + 1);
		for (int n = 0; n <= Chars_Min(C); n++) {
			P_5050_Vector[n] = P_5050(n, C, pity);
		}
	}

	double PT(int w) {
		double result = (1 - BaseP) / (W_max - (SoftPT - 1));
		result *= w - (SoftPT - 1);
		result += BaseP;
		return result;
	}

	double f(int w, int wsp = 0) {
		if (w <= wsp) { return 0; }
		if (w < SoftPT) { return BaseP; }
		if (w <= W_max) { return PT(w); }
		return 0;
	}

	double lost(int w, int wsp = 0) {
		return 1 - f(w, wsp);
	}

	double LostAcc(int w, int wsp = 0) {
		double product = 1;
		for (int i = 0; i <= w; i++) {
			product *= lost(i, wsp);
		}
		return product;
	}

	double succesRate(int w, int wsp = 0) {
		if (w <= 0) { return f(w); }
		return f(w, wsp) * LostAcc(w - 1, wsp);
	}

	void getSuccesRate() {
		sucessRate_VECTOR = std::vector<double>(W_max + 1, 0);
		for (int i = 0; i <= W_max; i++) {
			sucessRate_VECTOR[i] = succesRate(i);
		}
	}

	int W_MAX(int w) {
		if (w <= 0) { return 0; }
		if (w >= W_max) { return W_max; }
		return w;
	}

	double Q_k(int k, int w) {
		if (k == 1) {
			if (w > W_max) { return 0; }
			return sucessRate_VECTOR[w];
		}
		double sum = 0;
		for (int i = std::max(w - W_max * (k - 1), 1); i <= W_MAX(w - (k - 1)); i++) {
			sum += (sucessRate_VECTOR[i] * Q_k_Matrix[k - 1][w - i]);
		}
		return sum;
	}

	void getQ_k(int C) {
		Q_k_Matrix = std::vector<std::vector<double>>(Chars_Max(C) + 1, std::vector<double>(Chars_Max(C) * (W_max)+1));

		for (int k = 1; k <= Chars_Max(C); k++) {
			for (int w = 0; w <= W_max * (k); w++) {
				Q_k_Matrix[k][w] = Q_k(k, w);
			}
		}
	}

	double Q_sp(int k, int w, int wsp) {
		if (k == 1) {
			if (w > W_max) { return 0; }
			return succesRate(w, wsp);
		}
		double sum = 0;
		for (int i = std::max(w - W_max * (k - 1), 1); i <= W_MAX(w - (k - 1)); i++) {
			sum += (succesRate(i, wsp) * Q_k_Matrix[k - 1][w - i]);
		}
		return sum;
	}

	double Q_Total(int w, int C, int wsp, bool pity) {
		w = std::min(w, W_max * Chars_Max(C));
		double sum = 0;
		for (int i = 0; i <= Chars_Min(C); i++) {
			sum += P_5050(i, C, pity) * Q_sp(N(i, C), w + wsp, wsp);
		}
		return sum;
	}

	double Q_Total(int w, int C, int wsp) {
		w = std::min(w, W_max * Chars_Max(C));
		double sum = 0;
		for (int i = 0; i <= Chars_Min(C); i++) {
			sum += P_5050_Vector[i] * Q_sp(N(i, C), w + wsp, wsp);
		}
		return sum;
	}

	double Q_cum(int w, int C) {
		double sum = 0;
		for (int i = 0; i <= w; i++) {
			sum += Q_Total(i, C);
		}

		return sum;
	}

	double Q_cum(int w, int C, int wsp) {
		double sum = 0;
		for (int i = 0; i <= w; i++) {
			sum += Q_Total(i, C, wsp);
		}
		return sum;
	}


	double TotalChanceChar(int w, int C, int wsp) {
		w = std::min(w, Chars_Max(C) * W_max);
		return Q_cum(w, C, wsp);
	}

	void CharacterGetValues(int C, bool pity) {
		getSuccesRate();
		getQ_k(C);
		get5050(C, pity);
	}
}

void main2() {
	int C = 6;
	Character::CharacterGetValues(C, false);
	std::cout << 100 * Character::TotalChanceChar(500, C, 0) << "%" << std::endl;
}