#pragma once

namespace Weapon {
	double TotalChanceWeapon(int w, int R, int wsp=0, int F=0, bool pity=false);
	double Q_Total(int w, int R, int wsp=0, int F=0, bool pity=false);
	void WeaponGetValues(int R);
}