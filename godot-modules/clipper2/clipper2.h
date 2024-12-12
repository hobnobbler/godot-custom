/* clipper2.h */

#ifndef CLIPPER2_H
#define CLIPPER2_H

#include "core/object/ref_counted.h"
#include "Clipper2Lib/src/clipper2/clipper.h"

class Clipper2 : public RefCounted {
	GDCLASS(Clipper2, RefCounted);

	int count;

protected:
	static void _bind_methods();

public:
	Array inflate_paths(const PackedVector2Array &shape, double delta, int jt, int et,
						double miter_limit = 2.0, int precision = 2, double arc_tolerance = 0.0);
	Array deflate_recursive(const PackedVector2Array &shape, double delta, int join_type, int end_type);
	void add(int p_value);
	void reset();
	int get_total() const;

	Clipper2();
};

#endif // CLIPPER2_H
