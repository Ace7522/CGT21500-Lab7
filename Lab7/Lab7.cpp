#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

int main()
{
	// Create our window and world with gravity 0,1
	RenderWindow window(VideoMode(800, 600), "Bounce");
	World world(Vector2f(0, 1));

	// Create the ball
	PhysicsCircle ball;
	ball.setCenter(Vector2f(400, 50));
	ball.setRadius(20);
	world.AddPhysicsBody(ball);
	ball.applyImpulse(Vector2f(0.25, 0));

	// Init Counts
	int thudCount(0);
	int bangCount(0);

	// Create Walls (Top, Bottom, Left, Right)
	PhysicsRectangle tWall;
	PhysicsRectangle bWall;
	PhysicsRectangle lWall;
	PhysicsRectangle rWall;

	// Group Walls
	PhysicsRectangle* walls[4] = { &tWall, &bWall, &lWall, &rWall };

	// Set Wall Sizes
	tWall.setSize(Vector2f(800, 20));
	bWall.setSize(Vector2f(800, 20));
	lWall.setSize(Vector2f(20, 800));
	rWall.setSize(Vector2f(20, 800));

	// Set Wall Centers
	tWall.setCenter(Vector2f(400, 0));
	bWall.setCenter(Vector2f(400, 600));
	lWall.setCenter(Vector2f(0, 300));
	rWall.setCenter(Vector2f(800, 300));

	// Set Walls to be Static, add them to the world, and give them a collision
	for (PhysicsRectangle* wall : walls)
	{
		wall->setStatic(true);
		world.AddPhysicsBody(*wall);
		wall->onCollision = [&thudCount](PhysicsBodyCollisionResult result)
		{
			cout << "thud " << thudCount << endl;
			thudCount++;
		};
	}

	// Center block
	PhysicsRectangle centerBlock;
	centerBlock.setSize(Vector2f(100, 100));
	centerBlock.setCenter(Vector2f(400, 300));
	centerBlock.setStatic(true);
	world.AddPhysicsBody(centerBlock);
	centerBlock.onCollision = [&bangCount](PhysicsBodyCollisionResult result)
	{
		cout << "bang " << bangCount << endl;
		bangCount++;

		if (bangCount >= 2) exit(0);
	};

	Clock clock;
	Time lastTime(clock.getElapsedTime());
	while (true) {
		// calculate MS since last frame
		Time currentTime(clock.getElapsedTime());
		Time deltaTime(currentTime - lastTime);

		int deltaTimeMS(deltaTime.asMilliseconds());
		if (deltaTimeMS > 0)
		{
			world.UpdatePhysics(deltaTimeMS);
			lastTime = currentTime;
		}

		// Clear Window
		window.clear(Color(0, 0, 0));

		// Draw Ball
		window.draw(ball);
		
		// Draw Walls and Center Block
		for (PhysicsRectangle* wall : walls)
		{
			window.draw(*wall);
		}
		window.draw(centerBlock);

		// Display window
		window.display();
	}
}