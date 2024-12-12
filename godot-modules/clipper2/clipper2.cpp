/* clipper2.cpp */

#include "clipper2.h"

#include "core/variant/typed_array.h"
#include "Clipper2Lib/src/clipper2/clipper.h"


// Converts PackedVector2Array to PathD
Clipper2Lib::PathD convert_to_pathd(const PackedVector2Array &points) {
	Clipper2Lib::PathD path;
	for (int i = 0; i < points.size(); i++) {
		const Vector2 &vec = points[i];
		path.push_back({static_cast<double>(vec.x), static_cast<double>(vec.y)});
	}
	return path;
}

//Clipper2Lib::PathsD convert_to_pathsd(const PackedVector2Array &points) {
//	Clipper2Lib::PathD path = convert_to_pathd(points);
//	Clipper2Lib::PathsD paths = { path };
//	return paths;
//}

Clipper2Lib::PathsD convert_to_pathsd(const PackedVector2Array &points) {
	return { convert_to_pathd(points) };
}

// Converts PathsD to Godot Array of PackedVector2Arrays
Array convert_to_array(const Clipper2Lib::PathsD &paths) {
	Array result;
	for (const auto &subpath : paths) {
		PackedVector2Array packed_array;
		for (const auto &point : subpath) {
			packed_array.push_back(Vector2(static_cast<float>(point.x), static_cast<float>(point.y)));
		}
		result.push_back(packed_array);
	}
	return result;
}

// Wrapper around InflatePathD
Array Clipper2::inflate_paths(const PackedVector2Array &shape, double delta, int jt, int et,
							  double miter_limit, int precision, double arc_tolerance) {
	// Convert int to Clipper2Lib enums
	Clipper2Lib::JoinType join_type = static_cast<Clipper2Lib::JoinType>(jt);
	Clipper2Lib::EndType end_type = static_cast<Clipper2Lib::EndType>(et);

	// Convert Godot PackedVector2Array to PathD
	Clipper2Lib::PathD path = convert_to_pathd(shape);

	// Create a PathsD from the PathD
	Clipper2Lib::PathsD paths = { path };

	// Call the Clipper2 InflatePaths function
	Clipper2Lib::PathsD result = Clipper2Lib::InflatePaths(paths, delta, join_type, end_type, miter_limit, precision, arc_tolerance);

	// Convert the result back to Godot Array of PackedVector2Arrays
	return convert_to_array(result);
}

Array Clipper2::deflate_recursive(const PackedVector2Array &shape, double delta, int join_type, int end_type) {
	Clipper2Lib::PathsD p = convert_to_pathsd(shape);
	Array result;

	while (p.size())
	{
		result.push_back(convert_to_array(p));
		p = InflatePaths(p, delta, static_cast<Clipper2Lib::JoinType>(join_type), static_cast<Clipper2Lib::EndType>(end_type));
		p = SimplifyPaths(p, 0.025);
	}
	return result;
}

void Clipper2::add(int p_value) {
	count += p_value;
}

void Clipper2::reset() {
	count = 0;
}

int Clipper2::get_total() const {
	return count;
}

void Clipper2::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add", "value"), &Clipper2::add);
	ClassDB::bind_method(D_METHOD("reset"), &Clipper2::reset);
	ClassDB::bind_method(D_METHOD("get_total"), &Clipper2::get_total);
	ClassDB::bind_method(D_METHOD("inflate_paths", "shape", "delta", "jt", "et", "miter_limit", "precision", "arc_tolerance"), &Clipper2::inflate_paths);
	ClassDB::bind_method(D_METHOD("deflate_recursive", "shape", "delta", "join_type", "end_type"), &Clipper2::deflate_recursive);
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::JoinType::Square));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::JoinType::Bevel));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::JoinType::Round));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::JoinType::Miter));
// 
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::EndType::Polygon));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::EndType::Joined));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::EndType::Butt));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::EndType::Square));
	// BIND_ENUM_CONSTANT(static_cast<int>(Clipper2Lib::EndType::Round));
}

Clipper2::Clipper2() {
	count = 0;
}
