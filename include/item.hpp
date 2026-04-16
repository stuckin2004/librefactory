/*
 * The item is a basic struct with just a few parameters.
 * Most of the per-item data is handled by Lua
 */

#ifndef ITEM_HPP
#define ITEM_HPP

#include <stdint.h>

struct Item
{
	uint32_t id;
	const char *assetPath;
	const char *itemName;
	const char *itemDesc;
};

#endif /* ITEM_HPP */