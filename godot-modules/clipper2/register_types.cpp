/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "clipper2.h"

void initialize_clipper2_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<Clipper2>();
}

void uninitialize_clipper2_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	// Nothing to do here in this example.
}
