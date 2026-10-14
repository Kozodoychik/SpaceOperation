#ifndef TYPES_HPP
#define TYPES_HPP

namespace types {
	
	typedef struct {
		union {
			double x;
			double w;
		};
		union {
			double y;
			double h;
		};
	} Vector2d;

	typedef struct {
		union {
			double x;
			double r;
		};
		union {
			double y;
			double g;
		};
		union {
			double z;
			double b;
		};
	} Vector3d;

}

#endif