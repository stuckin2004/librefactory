/*
 * The entity is a basic struct with just a few parameters.
 * The first class-based approach didn't reall fit with
 * how Lua does tables and all that, this should be fine.
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <stdint.h>

#define MAX_ENTITIES 512

struct Entity
{
	uint32_t id;
	const char *assetPath;
	bool alive;
};

#endif /* ENTITY_HPP */