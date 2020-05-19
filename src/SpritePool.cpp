#include "SpritePool.h"

#include <ncine/Application.h>
#include <ncine/Sprite.h>
#include <ncine/Texture.h>

SpritePool::SpritePool(unsigned int size, nc::Texture *texture)
    : size_(size), nextFree_(0),
      width_(texture->width()), height_(texture->height())
{
	ASSERT(size_ > 0);

	sprites_.setCapacity(size);
	for (unsigned int i = 0; i < size; i++)
		sprites_.pushBack(nctl::makeUnique<nc::Sprite>(&nc::theApplication().rootNode(), texture));
}

nc::Sprite *SpritePool::acquire()
{
	nc::Sprite *sprite = nullptr;

	if (nextFree_ < size_)
	{
		sprite = sprites_[nextFree_].get();
		nextFree_++;
	}

	return sprite;
}

void SpritePool::release(unsigned int index)
{
	ASSERT(index < nextFree_);

	swap(sprites_[index], sprites_[nextFree_ - 1]);
	nextFree_--;
}

void SpritePool::reserve(unsigned int index)
{
	ASSERT(index <= size_);
	nextFree_ = index;
}

void SpritePool::acquireAll()
{
	nextFree_ = size_;
}

void SpritePool::releaseAll()
{
	nextFree_ = 0;
}
