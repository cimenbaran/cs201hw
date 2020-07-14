#include <string>
#include <vector>
#include "MiniFW_Modified.h"
#include "randgen.h"
#include "Robots_Modified.h"
#include "strutils.h"

using namespace std;

//bool IsCellEmpty(int xPos[],int yPos[], int size, int x, int y) { // Created but not used, instead GetCellCount is used
//	for (int i = 0; i< size; i++) {
//		if (xPos[i] == x && yPos[i] == y) {return false;}
//	}
//	return true;
//}
void ResurrectAll(Robot & a, Robot & b, Robot & c, Robot & d) {
	if (!a.IsAlive()) {a.Resurrect();}
	if (!b.IsAlive()) {b.Resurrect();}
	if (!c.IsAlive()) {c.Resurrect();}
	if (!d.IsAlive()) {d.Resurrect();}
}

string findHighestScore(Robot & a, Robot & b, Robot & c, Robot & d) {
	int highest = 0;
	string name;
	if (a.GetBagCount() > highest) {
		highest = a.GetBagCount();
		name = a.GetName();
	}
	if (b.GetBagCount() > highest) {
		highest = b.GetBagCount();
		name = b.GetName();
	}
	if (c.GetBagCount() > highest) {
		highest = c.GetBagCount();
		name = c.GetName();
	}
	if (d.GetBagCount() > highest) {
		highest = d.GetBagCount();
		name = d.GetName();
	}
	return name;
}

void playerDied(Robot & player, int & lifes) {
	lifes--;
	if (lifes != 0) {
		ShowMessage("You have " + itoa(lifes) + " lives left");
		player.Resurrect();
	}
}

void playerFlow(Robot & player, int & lifes) {
	bool willMove = false; // Checking if player pressed any buttons
	if (IsPressed(keyLeftArrow)) {
		player.TurnFace(west);
		willMove = true;
	}
	else if (IsPressed(keyUpArrow)) {
		player.TurnFace(north);
		willMove = true;
	}
	else if (IsPressed(keyRightArrow)) {
		player.TurnFace(east);
		willMove = true;
	}
	else if (IsPressed(keyDownArrow)) {
		player.TurnFace(south);
		willMove = true;
	}
	if (willMove) {
		if (!player.Blocked()) {
			player.Move();
			if (player.IsAlive()) {player.PickAllThings();}
		}
		else {
			player.Move();
			playerDied(player, lifes);
		}
	}
}

void npcFlow(Robot & npc, Robot & player, int & lifes, Direction & pastDirection, bool & isBlocked) {
	RandGen rnd;
	Direction direction;
	do {
		direction = Direction(rnd.RandInt(0,3));
	} while (isBlocked && pastDirection == direction);
	pastDirection = direction;
	npc.TurnFace(direction);
	if (!npc.Blocked()) {
		npc.Move();
		if (npc.IsAlive()) {npc.PickAllThings();}
	}
	else {
		isBlocked = true;
		npc.Move();
		if (!player.IsAlive()) {
			playerDied(player, lifes);
		}
	}
}

int main() {
	
	string playerName, robot1Name, robot2Name, robot3Name;


	GetInput("Please enter the name of the player robot", playerName);
    GetInput("Please enter the name of the monster robot 1", robot1Name);
	GetInput("Please enter the name of the monster robot 2", robot2Name);
    GetInput("Please enter the name of the monster robot 3", robot3Name);

	// Create and set Things
	RandGen rnd;

	int x,y,thing;

	for (int i=0; i<20; i++){
		do {
			thing = rnd.RandInt(1,10);
			x = rnd.RandInt(0,7);
			y = rnd.RandInt(0,7);
			if (GetCellCount(x, y) != 0	 &&thing + GetCellCount(x,y) <= 10) {
				PutThings(x, y, thing);
			}
		} while	(GetCellCount(x,y) + thing >10); // Checks is those coordinates is empty               [ and doesnt cross with any robot's starting position   ---->  || ((x==0 && y==0) || (x==0 && y==7) || (x==7 && y==7) || (x==5 && y==2) ) ]
		
		PutThings(x, y, thing);

	}
	
	int playerLife = 3;

	// Create Player & NPC's with coordinates
	Robot player(5,2);
	Robot npc1(0,0);
	Robot npc2(0,7);
	Robot npc3(7,7);
	
	// Set Names
	player.SetName(playerName);
	npc1.SetName(robot1Name);
	npc2.SetName(robot2Name);
	npc3.SetName(robot3Name);

	// Set Colors
	player.SetColor(green);
	npc1.SetColor(red);
	npc2.SetColor(red);
	npc3.SetColor(red);

	// Take the things at the starting positions
	player.PickAllThings();
	npc1.PickAllThings();
	npc2.PickAllThings();
	npc3.PickAllThings();

	int npcCount=0;

	// In order to improve the NPCs / AI, I created a memory for them so they can learn from their mistakes
	Direction npc1Direction = east;
	Direction npc2Direction = east;
	Direction npc3Direction = east;
	bool npc1Blocked = false;
	bool npc2Blocked = false;
	bool npc3Blocked = false;

	while (playerLife != 0 && GetThingCount(0,0,7,7) != 0) {
		if (npcCount == 0) {npcFlow(npc1, player, playerLife, npc1Direction, npc1Blocked);}
		else if (npcCount == 1) {npcFlow(npc2, player, playerLife, npc2Direction, npc2Blocked);}
		else if (npcCount == 2) {npcFlow(npc3, player, playerLife, npc3Direction, npc3Blocked);}

		if (playerLife !=0 && GetThingCount(0,0,7,7) != 0) {ResurrectAll(npc1,npc2,npc3,player);}; // NPC's may have crashed to another robot
		
		if (playerLife !=0 && GetThingCount(0,0,7,7) != 0) {playerFlow(player, playerLife);}

		if (playerLife !=0 && GetThingCount(0,0,7,7) != 0) {ResurrectAll(npc1,npc2,npc3,player);}; // Player may have crashed to another robot

		npcCount++;
		if (npcCount == 3) {npcCount=0;}
	}
	
	if (playerLife==0) {ShowMessage(player.GetName() + " is out of lives, you just lost the game.");}
	else if (GetThingCount(0,0,7,7) == 0) {
		string name = findHighestScore(player, npc1, npc2, npc3);
		if (name == player.GetName()) {ShowMessage("Congratulations. " + name + " has won the game.");}
		else {ShowMessage("You lost. " + name + " has won the game.");}
	}

	return 0;
}