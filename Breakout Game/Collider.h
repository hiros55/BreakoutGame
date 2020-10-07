class Collider {
public:
	Collider() {};
	~Collider() {};
	float X, Y;//‹…“–‚½‚è”»’è—p
	int BarCollision(int XMin, int XMax, int YMin, int YMax, float radius);
	int StaticCollision(int XMin, int XMax, int YMin, int YMax, float radius);
};
