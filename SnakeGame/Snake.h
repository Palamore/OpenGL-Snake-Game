#pragma once

#ifndef SNAKE_H
#define SNAKE_H

enum class SnakeDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

typedef struct Pos
{
	int x;
	int y;
};

class Snake
{
public:
	Snake();
	~Snake() = default;
	Snake(const Snake& rhs);
	Snake& operator=(const Snake& rhs);

	inline void IncreaseBodyLength()
	{
		mBodyLength++;
	}

	inline unsigned int GetBodyLength() const
	{
		return mBodyLength;
	}

	inline void SetDirection(SnakeDirection sd)
	{
		mDirection = sd;
	}

	inline SnakeDirection GetDirection() const
	{
		return mDirection;
	}

private:
	unsigned int mBodyLength;
	SnakeDirection mDirection;

};



#endif