#include <iostream>
#include<vector>
#include<iomanip>
#include "Weapon.h"

namespace Weapon {

#define BaseP 0.007
#define SoftPT 65
#define W_max 80
#define rateUp 0.75
#define desiredP 0.5
	bool PityAktive = false;
	std::vector<double> succesRate_Vec;
	std::vector<std::vector<double>>Q_k_Matrix;
	std::vector<std::vector<double>>P_5050_Matrix;


	int Weapons_Max(int R) {
		return 3 * R;
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
		else { return 0; }
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
		if (w <= 0) { return f(w, wsp); }
		return f(w, wsp) * LostAcc(w - 1, wsp);
	}

	void getSuccesRate() {
		succesRate_Vec = std::vector<double>(W_max + 1, 0);
		for (int i = 0; i <= W_max; i++) {
			succesRate_Vec[i] = succesRate(i);
		}
	}

	double P_n(int n, int F = 0, bool pity = false) {
		if (F == 2) {
			if (n == 1) { return 1; }
			else { return 0; }
		}
		double P1;
		double P2;
		double P3;
		double r1;
		double r2;
		double r3;
		if (pity) {
			if (F == 1) {
				switch (n) {
				case 1:
					P1 = desiredP;
					return P1;
					break;
				case 2:
					P2 = 1 - desiredP;
					return P2;
					break;
				default:
					return 0;
				}
			}
			switch (n) {
			case 1:
				P1 = desiredP;
				return P1;
				break;
			case 2:
				r1 = 1 - desiredP;
				r2 = rateUp * desiredP;
				P2 = r1 * r2;
				return P2;
				break;
			case 3:
				r1 = 1 - desiredP;
				r2 = rateUp * (1 - desiredP);
				r3 = 1 - rateUp;
				P3 = r1 * (r2 + r3);
				return P3;
				break;
			default:
				return 0;
			}
		}
		if (F == 1) {
			switch (n) {
			case 1:
				P1 = rateUp * desiredP;
				return P1;
				break;
			case 2:
				r1 = 1 - rateUp;
				r2 = rateUp * desiredP;
				P2 = r1 + r2;
				return P2;
				break;
			default:
				return 0;
			}
		}

		switch (n) {
		case 1:
			P1 = rateUp * desiredP;
			return P1;
			break;
		case 2:
			r1 = (1 - rateUp) * desiredP;
			r2 = rateUp * (1 - desiredP) * rateUp * desiredP;
			r3 = rateUp * (1 - desiredP) * rateUp * (1 - desiredP);
			P2 = r1 + r2 + r3;
			return P2;
			break;
		case 3:
			P1 = rateUp * desiredP;
			r1 = (1 - rateUp) * desiredP;
			r2 = rateUp * (1 - desiredP) * rateUp * desiredP;
			r3 = rateUp * (1 - desiredP) * rateUp * (1 - desiredP);
			P2 = r1 + r2 + r3;
			return 1 - P1 - P2;
			break;
		default:
			return 0;
		}
	}

	double P_5050(int n, int R, int F = 0, bool pity = false) {
		if (R == 1) {
			return P_n(n, F, pity);
		}
		double sum = 0;
		for (int i = 1; i <= std::min(n, 3); i++) {
			sum += P_n(i, F, pity) * P_5050_Matrix[R - 1][n - i];
		}
		return sum;
	}

	void getP_5050(int R) {
		P_5050_Matrix = std::vector<std::vector<double>>(R + 1, std::vector<double>(Weapons_Max(R) + 1));
		for (int r = 1; r <= R; r++) {
			for (int n = 1; n <= Weapons_Max(R); n++) {
				P_5050_Matrix[r][n] = P_5050(n, r);
			}
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
			return succesRate_Vec[w];
		}
		double sum = 0;
		for (int i = std::max(w - W_max * (k - 1), 1); i <= W_MAX(w - (k - 1)); i++) {
			sum += (succesRate_Vec[i] * Q_k_Matrix[k - 1][w - i]);
		}
		return sum;
	}

	void getQ_k(int R) {
		Q_k_Matrix = std::vector<std::vector<double>>(Weapons_Max(R) + 1, std::vector<double>(W_max * (Weapons_Max(R)) + 1));
		for (int k = 1; k <= Weapons_Max(R); k++) {
			for (int w = 0; w <= W_max * Weapons_Max(R); w++) {
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

	double Q_Total(int w, int R, int wsp, int F, bool pity) {
		w = std::min(w, W_max * Weapons_Max(R));
		double sum = 0;
		for (int i = R; i <= Weapons_Max(R); i++) {
			sum += P_5050(i, R, F, pity) * Q_sp(i, w + wsp, wsp);
		}
		return sum;
	}

	double Q_cum(int w, int R, int F) {
		double sum = 0;
		for (int i = 0; i <= w; i++) {
			sum += Q_Total(i, R);
		}
		return sum;
	}

	double Q_cum(int w, int R, int wsp, int F, bool pity) {
		double sum = 0;
		for (int i = 0; i <= w; i++) {
			sum += Q_Total(i, R, wsp, F, pity);
		}
		return sum;
	}

	double TotalChanceWeapon(int w, int R, int wsp, int F, bool pity) {
		w = std::min(w, Weapons_Max(R) * W_max);
		return Q_cum(w, R, wsp, F, pity);
	}


	void WeaponGetValues(int R) {
		getSuccesRate();
		getQ_k(R);
		getP_5050(R);
	}
}

void UT_5050(int R) {
	for (int r = 1; r <= R; r++) {
		double sum = 0;
		for (int n = 0; n <= Weapon::Weapons_Max(R); n++) {
			std::cout << "r: " << r << ". n: " << n << ". 5050: " << Weapon::P_5050_Matrix[r][n] << std::endl;
			sum += Weapon::P_5050_Matrix[r][n];
		}
		std::cout << sum << std::endl;
	}
	//passed
}

void UT_succes() {
	for (int i = 0; i <= W_max; i++) {
		std::cout << Weapon::succesRate_Vec[i] << std::endl;
	}
	//passed
}

void main1() {
	int w = 500;
	int R = 5;
	int wsp = 60;
	int F = 0;
	bool pity = false;
	Weapon::WeaponGetValues(R);

	std::cout << 100 * Weapon::TotalChanceWeapon(w, R, wsp, F, pity) << "%" << std::endl;
}