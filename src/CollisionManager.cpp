#include "CollisionManager.h"
#include "Util.h"
#include <algorithm>



int CollisionManager::squaredDistance(glm::vec2 P1, glm::vec2 P2)
{
	int diffOfXs = P2.x - P1.x;
	int diffOfYs = P2.y - P1.y;
	int result = diffOfXs * diffOfXs + diffOfYs * diffOfYs;

	return result;
}

bool CollisionManager::squaredRadiusCheck(GameObject * object1, GameObject * object2)
{
	glm::vec2 P1 = object1->getPosition();
	glm::vec2 P2 = object2->getPosition();
	int halfHeights = (object1->getHeight() + object2->getHeight()) * 0.5f;

	//if (glm::distance(P1, P2) < halfHeights) {

	if (CollisionManager::squaredDistance(P1, P2) < (halfHeights * halfHeights)) {
		if (!object2->getIsColliding()) {
			
			object2->setIsColliding(true);

			switch (object2->getType()) {
			case PLANET:
				std::cout << "Collision with Planet!" << std::endl;
				TheSoundManager::Instance()->playSound("yay", 0);
				break;
			case MINE:
				std::cout << "Collision with Mine!" << std::endl;
				TheSoundManager::Instance()->playSound("thunder", 0);
				break;
			default:
				//std::cout << "Collision with unknown type!" << std::endl;
					break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->setIsColliding(false);
		return false;
	}
}

bool CollisionManager::AABBCheck(GameObject * object1, GameObject * object2)
{
	// prepare relevant variables
	glm::vec2 P1 = object1->getPosition();
	glm::vec2 P2 = object2->getPosition();
	float P1width = object1->getWidth();
	float P1height = object1->getHeight();
	float P2width = object2->getWidth();
	float P2height = object2->getHeight();

	if(
		P1.x < P2.x + P2width &&
		P1.x + P1width > P2.x &&
		P1.y < P2.y + P2height &&
		P1.y + P1height > P2.y
		)
	{
		if (!object2->getIsColliding()) {

			object2->setIsColliding(true);

			switch (object2->getType()) {
			case PLANET:
				std::cout << "Collision with Planet!" << std::endl;
				TheSoundManager::Instance()->playSound("yay", 0);
				break;
			case MINE:
				std::cout << "Collision with Mine!" << std::endl;
				TheSoundManager::Instance()->playSound("thunder", 0);
				break;
			default:
				//std::cout << "Collision with unknown type!" << std::endl;
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->setIsColliding(false);
		return false;
	}
	
	return false;
}

bool CollisionManager::lineLineCheck(glm::vec2 line1Start, glm::vec2 line1End, glm::vec2 line2Start, glm::vec2 line2End)
{
	float x1 = line1Start.x;
	float x2 = line1End.x;
	float x3 = line2Start.x;
	float x4 = line2End.x;
	float y1 = line1Start.y;
	float y2 = line1End.y;
	float y3 = line2Start.y;
	float y4 = line2End.y;

	// calculate the distance to intersection point
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) 
	{
		return true;
	}
	
	return false;
}

bool CollisionManager::lineRectCheck(glm::vec2 line1Start, glm::vec2 line1End, glm::vec2 recStart, float recWidth, float recHeight)
{
	float x1 = line1Start.x;
	float x2 = line1End.x;
	float y1 = line1Start.y;
	float y2 = line1End.y;
	float rx = recStart.x;
	float ry = recStart.y;
	float rw = recWidth;
	float rh = recHeight;

	//TODO FIX THIS

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	bool left = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx, ry + rh));
	bool right = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx + rw, ry), glm::vec2(rx + rw, ry + rh));
	bool top = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx + rw, ry));
	bool bottom = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry + rh), glm::vec2(rx + rw, ry + rh));

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}

	return false;
}

int CollisionManager::circleAABBsquaredDistance(glm::vec2 circleCentre, int circleRadius, glm::vec2 boxStart, int boxWidth, int boxHeight)
{
	float dx = std::max(boxStart.x - circleCentre.x, 0.0f);
	dx = std::max(dx, circleCentre.x - (boxStart.x + boxWidth));
	float dy = std::max(boxStart.y - circleCentre.y, 0.0f);
	dy = std::max(dy, circleCentre.y - (boxStart.y + boxHeight));

	return (dx * dx) + (dy * dy);
}

bool CollisionManager::circleAABBCheck(GameObject * object1, GameObject * object2)
{
	// circle
	glm::vec2 circleCentre = object1->getPosition();
	int circleRadius = std::max(object1->getWidth() * 0.5f, object1->getHeight() * 0.5f);
	// aabb
	int boxWidth = object2->getWidth();
	int boxHeight = object2->getHeight();
	glm::vec2 boxStart = object2->getPosition() - glm::vec2(boxWidth * 0.5f, boxHeight * 0.5f);

	if(circleAABBsquaredDistance(circleCentre, circleRadius, boxStart, boxWidth, boxHeight) <= (circleRadius * circleRadius))
	{
		if (!object2->getIsColliding()) {

			object2->setIsColliding(true);

			switch (object2->getType()) {
			case PLANET:
				std::cout << "Collision with Planet!" << std::endl;
				TheSoundManager::Instance()->playSound("yay", 0);
				break;
			case MINE:
				std::cout << "Collision with Mine!" << std::endl;
				TheSoundManager::Instance()->playSound("thunder", 0);
				break;
			case BRICK:
				std::cout << "Collision with Brick!" << std::endl;
				TheSoundManager::Instance()->playSound("bounce", 0);
				break;
			default:
				//std::cout << "Collision with unknown type!" << std::endl;
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->setIsColliding(false);
		return false;
	}
	
	return false;
}

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}
