#include "Snake.h"

Snake::Snake()
	:mBodyLength(1)
	,mDirection(SnakeDirection::UP)
{

}

Snake::Snake(const Snake& rhs)
	:mBodyLength(rhs.mBodyLength)
	,mDirection(rhs.mDirection)
{

}

Snake& Snake::operator=(const Snake& rhs)
{
	mBodyLength = rhs.mBodyLength;
	mDirection = rhs.mDirection;
	return *this;
}