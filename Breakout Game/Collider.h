class Collider {
public:
	Collider() {};
	~Collider() {};
	float X, Y;//�������蔻��p
	int BarCollision(int XMin, int XMax, int YMin, int YMax, float radius);
	int StaticCollision(int XMin, int XMax, int YMin, int YMax, float radius);
};
