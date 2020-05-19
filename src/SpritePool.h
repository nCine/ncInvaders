#ifndef SPRITE_POOL_H
#define SPRITE_POOL_H

#include <ncine/common_macros.h>
#include <nctl/Array.h>

namespace ncine {
class Sprite;
class Texture;
}

namespace nc = ncine;

/// The class that handles the pools of game sprites, such as rockets, bombs and enemies
/*! All entities are contained in a contiguous array, released ones are always at the back. */
class SpritePool
{
  public:
	SpritePool(unsigned int size, nc::Texture *texture);

	/// Acquires a single sprite from the pool
	nc::Sprite *acquire();
	/// Releases a single sprite so it can return to the pool
	/*! \param index The index inside the pool array of the sprite to be released */
	/*! \warning A cycle over pool entities that might call `release()` should always start from the last acquired one and go backwards. */
	void release(unsigned int index);
	/// Sets the number of entities to consider as acquired
	void reserve(unsigned int index);
	/// Acquires all entities in one go
	void acquireAll();
	/// Releases all entities in one go
	void releaseAll();

	/// Subscript operator to make the pool act as an ordinary array
	inline nc::Sprite &operator[](unsigned int i) { return *sprites_[i]; }

	/// Returns the total size of the pool
	inline unsigned int totalSize() const { return size_; }
	/// Returns the number of acquired entities
	inline unsigned int acquiredSize() const { return nextFree_; }

	inline int spriteWidth() const { return width_; }
	inline int spriteHeight() const { return height_; }

  private:
	unsigned int size_;
	/// The index of the first free sprite, the next one to be acquired
	unsigned int nextFree_;

	nctl::Array<nctl::UniquePtr<nc::Sprite>> sprites_;
	int width_;
	int height_;

	SpritePool(const SpritePool &) = delete;
	SpritePool &operator=(const SpritePool &) = delete;
};

#endif
