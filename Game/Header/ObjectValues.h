#pragma once
// definition of halth values for each GameObjects
#define BUILDERHEALTH 850
#define MELEEHEALTH 1250
#define RANGEDHEALTH 1065
#define HOUSEHEALTH 2250
#define BARRACKHEALTH 2500

// definition of cost values for each GameObjects
#define BUILDERCOST 50
#define MELEECOST 75
#define RANGEDCOST 100
#define HOUSECOST 100
#define BARRACKCOST 200

// definition of construction time (in seconds) values for each GameObjects
#define BUILDERCONSTRUCTIONTIME 10.0f
#define MELEECONSTRUCTIONTIME 10.0f
#define RANGEDCONSTRUCTIONTIME 10.0f
#define HOUSECONSTRUCTIONTIME 5.0f
#define BARRACKCONSTRUCTIONTIME 10.0f

// definition of speed values for each Unit classes
#define BUILDERSPEED 1.0f
#define MELEESPEED  1.65f
#define RANGEDSPEED 1.65f

// definiton of attack value for each CombatUnit classes
#define MELEEATTACK 20
#define RANGEDATTACK 20


//definiton the shortest distance to attack for the CombatUnit
#define RANGEDDISTANCE 64.0f
#define MELEEDISTANCE 15.6f


//Maximum population value at the start of the game
#define MAXPOPULATIONSTART 10


//number of maximum population increase when a house is built
#define INCREASEMAXPOPULATION 5


//number of builder units and houses at the start of the game for each player
#define STARTBUILDERUNITS 2
#define STARTHOUSES 1