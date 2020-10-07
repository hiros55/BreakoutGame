#include "Collider.h"

int Collider::BarCollision(int XMin, int XMax, int YMin, int YMax, float radius) {
	if (XMin - radius < X && XMax + radius >  X && YMin <  Y && YMax >  Y) {
		//printf("1");
		return 1;//‰¡
	}

	if (XMin <  X && XMax >  X && YMax + radius > Y && YMin - radius < Y) {
		//printf("2");
		return 2; //ã
	}

	if ((XMax - X) * (XMax - X) + (YMax - Y) * (YMax - Y) < radius * radius) {
		//printf("3");
		return 2;//¶ã
	}

	if ((XMin - X) * (XMin - X) + (YMax - Y) * (YMax - Y) < radius * radius) {
		//printf("4");
		return 2;//‰Eã
	}
}

int Collider::StaticCollision(int XMin, int XMax, int YMin, int YMax, float radius) {
	if (XMin - radius <  X && XMax + radius >  X && YMin <  Y && YMax >  Y) {
		//printf("1");
		return 1;
	}
	if (XMin <  X && XMax >  X && YMin - radius <  Y && YMax + radius >  Y) {
		//printf("2");
		return 2;
	}
	if ((XMin - X) * (XMin - X) + (YMin - Y) * (YMin - Y) < radius * radius) {
		//printf("3");
		return 2;
	}
	if ((XMax - X) * (XMax - X) + (YMin - Y) * (YMin - Y) < radius * radius) {
		//printf("4");
		return 2;
	}
	if ((XMin - X) * (XMin - X) + (YMax - Y) * (YMax - Y) < radius * radius) {
		//printf("5");
		return 2;
	}
	if ((XMax - X) * (XMax - X) + (YMax - Y) * (YMax - Y) < radius * radius) {
		//printf("6");
		return 2;
	}
	return -1;
}

