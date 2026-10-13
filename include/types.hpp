namespace types {
	
	typedef struct {
		union {
			int x;
			int w;
		};
		union {
			int y;
			int h;
		};
	} Vector2d;

	typedef struct {
		union {
			int x;
			int r;
		};
		union {
			int y;
			int g;
		};
		union {
			int z;
			int b;
		};
	} Vector3d;

}