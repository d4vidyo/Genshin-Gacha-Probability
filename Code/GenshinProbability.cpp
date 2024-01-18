#include <iostream>
#include <iomanip>
#include <vector>
#include "Character.h"
#include "Weapon.h"

std::vector<double>Char_Q_Total;
std::vector<double>Wpn_Q_Total;
std::vector<double>Q_Total_Vec;

double Q_Total(int w, int C, int wsp_char, int R, int wsp_wpn, int F, bool wpnPity) {
	double sum = 0;
	for (int i = 0; i <= w; i++) {
		if (Char_Q_Total[i] == -1) { Char_Q_Total[i] = Character::Q_Total(i, C, wsp_char); }
		if (Wpn_Q_Total[w - i] == -1) { Wpn_Q_Total[w - i] = Weapon::Q_Total(w - i, R, wsp_wpn); }
		sum += Char_Q_Total[i] * Wpn_Q_Total[w - i];
	}
	return sum;
}


double Q_cum(int w, int C, int wsp_char, int R, int wsp_wpn, int F, bool wpnPity) {
	double sum = 0;
	for (int i = 0; i <= w; i++) {
		if (Q_Total_Vec[i] == -1) { Q_Total_Vec[i] = Q_Total(i, C, wsp_char, R, wsp_wpn, F, wpnPity); }
		sum += Q_Total_Vec[i];
	}
	return sum;
}

void main() {

	int C = 6;
	bool charPity = false;
	int wsp_char = 0;

	int R = 5;
	int wsp_wpn = 0;
	int F = 0;
	bool wpnPity = false;

	int w = (C + 1) * 2 * 90 + R * 3 * 80;
	Character::CharacterGetValues(C, charPity);
	Weapon::WeaponGetValues(R);
	Q_Total_Vec = std::vector<double>(w + 1, -1);
	Char_Q_Total = std::vector<double>(w + 1, -1);
	Wpn_Q_Total = std::vector<double>(w + 1, -1);


	for (int i = 0; i <= w; i++) {
		//std::cout << std::setprecision(20) << i << ";" << Character::TotalChanceChar(i, C, wsp_char) << std::endl;
		//std::cout << std::setprecision(20) << i << ";" << Weapon::TotalChanceWeapon(i, R, wsp_wpn, F, wpnPity ) << std::endl;
		td::cout << std::setprecision(20) << i << ";" << Q_cum(i, C, wsp_char, R, wsp_wpn, F, wpnPity) << std::endl;
	}

}
