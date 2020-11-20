

// Mohammad Mohajerani ID:G20656222 
//The object of this game is to guide each of the family of three frogs, 
//in turn, across the lanes of traffic to the grass verge at the other side of the road.
//The scene of the game that you must develop appears in the picture overleaf.

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

#include <sstream>
using namespace std;

#include <iostream>
using namespace std;

 // State Defenition

#define STATE_Game_Playing         0
#define STATE_Game_Paused          1
#define STATE_Game_Over            3

#define STATE_Model_MOVING         0
#define STATE_Model_TELEPORTING    1

#define STATE_Model_MOVING_Right   0
#define STATE_Model_MOVING_Left    1

#define STATE_Frog_Waiting         0
#define STATE_Frog_Crossing        1
#define STATE_Frog_Safe            2
#define STATE_Frog_Dead            3

#define It_is_FREE                 0
#define It_is_HAPPEND              1

#define STATE_Frog_JUMP_Waiting    0
#define STATE_Frog_JUMP_Forward    1
#define STATE_Frog_JUMP_Backward   2
#define STATE_Frog_JUMP_RightSide  3
#define STATE_Frog_JUMP_LeftSide   4
#define STATE_Frog_LANDED          5

#define maxTime                   21


void stateCreator(int aState[], int nModels, int state)
{

	for (int i = 0; i < nModels; i++)
	{
		aState[i] = state;
	}
}

void arrayCreator(float aState[], int nModels, int value)
{

	for (int i = 0; i < nModels; i++)
	{
		aState[i] = value;
	}
}

void modelCreator(int nModel, IModel* aModel[], IMesh* mModel, int rotate, float bX, float x, int y, float bZ, int z)
{
	for (int i = 0; i < nModel; i++)
	{
		aModel[i] = mModel->CreateModel(bX + x*i,y, bZ + z*i);
		aModel[i]->RotateLocalY(rotate);
	}
}

void processVehicleState(int nModel, IModel* model[], int state[], float ds, float xPositon, float time,
	float delay[], int rotate)
{
	for (int i = 0; i < nModel; i++)
	{
		switch (state[i])
		{
		case STATE_Model_MOVING:
			//(a)Update:
			model[i]->MoveLocalZ(ds);

			//(b) Transitions:
			if (xPositon > 0) //line 1, 2
			{

				if (model[i]->GetX() > 50) 
				{
					model[i]->RotateZ(0.1f);
				}
				else 
				{
					if (model[i]->GetX() >= -xPositon)
					{
						model[i]->ResetOrientation();
						model[i]->RotateY(rotate);
						model[i]->SetY(0);
					}
				}
				

				if (model[i]->GetX() <= -xPositon)
				{
					delay[i] += time;
					model[i]->RotateZ(0.1f);
				}
				if (delay[i] > 2)
				{
					state[i] = STATE_Model_TELEPORTING;
					delay[i] = 0;
				}
			}

			if (xPositon < 0) // line 2,3
			{
				if (model[i]->GetX() <- 50) {
					model[i]->RotateZ(-0.1f);
				}
				else
				{
					if (model[i]->GetX() <= -xPositon)
					{
						model[i]->ResetOrientation();
						model[i]->RotateY(rotate);
						model[i]->SetY(0);
					}
				}
				if (model[i]->GetX() >= -xPositon)
				{
					delay[i] += time;
					model[i]->RotateZ(-0.1f);
				}
				if (delay[i] > 2)
				{
					state[i] = STATE_Model_TELEPORTING;
					delay[i] = 0;
				}
			}

			break;

		case STATE_Model_TELEPORTING:
			//(a)Update:
			
						model[i]->ResetOrientation();
						model[i]->RotateY(rotate);
						
						if (xPositon < 0)//line 3,4
						{
							model[i]->SetX(-65);
							model[i]->RotateZ(40);
							model[i]->SetY(-5);
						}
						if (xPositon > 0)//line 1,2
						{
							model[i]->SetX(55);
							model[i]->RotateZ(-60);
							model[i]->SetY(-3);
						}
						
			
						//(b) Transitions:
						state[i] = STATE_Model_MOVING;
			break;
		}
	}
}

void processTyerState(int nModel, IModel* aMTyer[][3], int state[], float ds)
{

	for (int i = 0; i < nModel; i++)
	{
		switch (state[i])
		{
		case STATE_Model_MOVING_Right:
			//(a)Update:
			for (int j = 0; j < 3; j++)
			{
				aMTyer[i][j]->MoveX(ds);
				aMTyer[i][j]->SetY((0.2 *sin(aMTyer[i][j]->GetX()))-3);
			}

			//(b) Transitions:
			if (aMTyer[i][2]->GetX() >= 50)
				state[i] = STATE_Model_MOVING_Left;
			break;

		case STATE_Model_MOVING_Left:
			//(a)Update:
			for (int j = 0; j < 3; j++)
			{
				aMTyer[i][j]->MoveX(-ds);
				aMTyer[i][j]->SetY((0.2 *sin(aMTyer[i][j]->GetX())) - 3);
			}

			//(b) Transitions:
			if (aMTyer[i][0]->GetX() <= -50)
				state[i] = STATE_Model_MOVING_Right;
			break;
		}
	}
}

//Circle-to-rectangle collision detection vehicles
void vehiclesCollision(int nModel, IModel* aModel[], IModel* frog, int aStateF[],
	int &nCf, float & xC, float & zC, int & dead)
{
	for (int l = 0; l < nModel; l++)
	{
		float modelMinX = aModel[l]->GetX() - 8, modelMaxX = aModel[l]->GetX() + 8,
			modelMinZ = aModel[l]->GetZ() - 6, modelMaxZ = aModel[l]->GetZ() + 6;

		float frogX = frog->GetX(), frogZ = frog->GetZ();
		if ((frogX > modelMinX) && (frogX < modelMaxX) &&
			(frogZ > modelMinZ) && (frogZ < modelMaxZ))
		{
			xC = 0; zC = -15;
			dead += 1;
			aStateF[nCf] = STATE_Frog_Dead;			
		}
	}
}

////Detect sphere-to-sphere collisions between the tyrrs and the frogs(Which is suport matrix an array of model,
void tyerscollision(int nTyers, int nGTyers, IModel* aTyer[][3], IModel* aFrog, IModel* tempModel,
	bool & TyerColliedsToFrog)
{
	TyerColliedsToFrog = It_is_FREE;
	for (int row = 0; row < nTyers; row++)
	{
		for (int col = 0; col < nGTyers; col++)
		{
			float Dx, Dz;
			Dx = aTyer[row][col]->GetX() - aFrog->GetX();
			Dz = aTyer[row][col]->GetZ() - aFrog->GetZ();
			float collisionDist = sqrt(Dx*Dx + Dz*Dz);

			if (collisionDist < 5)
			{
				TyerColliedsToFrog = It_is_HAPPEND;
				tempModel->AttachToParent(aTyer[row][col]);
			}
		}
	}
}

//Detect sphere-to-sphere collisions between the plants and the frogs
void treesCollision(int nModel, IModel* aModel[], IModel* aFrog, bool & collision)
{
	for (int i = 0; i < nModel; i++)
	{
		float Dx, Dy, Dz;
		Dx = aModel[i]->GetX() - aFrog->GetX();
		Dz = aModel[i]->GetZ() - aFrog->GetZ();
		float collisionDist = sqrt(Dx*Dx + Dz*Dz);

		if (collisionDist < 5)	// Collision occurred…
		{
			//If a collision occurs, set the bullet’s Y position to 25.
			collision = It_is_HAPPEND;
		}
	}
}

// Detect sphere - to - sphere collisions between the a frog to the other frogs
void FrogCollision(int nModel, IModel* aModel[], IModel* aFrog,int aStateF[], bool & collision, int exception)
{
	for (int i = 0; i < nModel; i++)
	{
		if ((i != exception)&&(aStateF[i]!=STATE_Frog_Dead))
		{
			float Dx, Dy, Dz;
			Dx = aModel[i]->GetX() - aFrog->GetX();
			Dz = aModel[i]->GetZ() - aFrog->GetZ();
			float collisionDist = sqrt(Dx*Dx + Dz*Dz);

			if (collisionDist < 5)	// Collision occurred…
			{
				//If a collision occurs, set the bullet’s Y position to 25.
				collision = It_is_HAPPEND;
			}
		}
	}
}

// Frog jumping State Transitions
void frogJump(IModel* dummy, IModel* frog, int & frogJumpState, int & degree, float ds, float xC,
	float zC, bool treesColliedsToFrog, bool frogColliedsToFrogs)
{
	// Usfull tmprory variable to simUilate the jumping
	const float KPi = 3.1415926f;
	const float radians =KPi/ 180.0f * degree;
	float x = frog->GetX();
	float y = frog->GetY();
	float z = dummy->GetZ();
	float jumpY =5* sin(radians);
	if (jumpY <= 0) jumpY = -jumpY;	
		
	switch (frogJumpState)
	{
	case STATE_Frog_JUMP_Waiting:
		break;

	case STATE_Frog_JUMP_Forward:
		degree += 1;
		if (z - zC <= 10) // Move 10 unit forward
		{			
			if ((treesColliedsToFrog)||(frogColliedsToFrogs)) // Colision ( Jump back)
			{
				dummy->SetZ(zC);
				frogJumpState = STATE_Frog_LANDED;
			}
			else
				frog->SetY(jumpY);
				dummy->MoveZ(ds);
			
		}
		else
			frogJumpState = STATE_Frog_LANDED;

		break;

	case STATE_Frog_JUMP_Backward:
		degree += 1;
		if (z - zC >= -10) //Move 10 unit backward
		{
			frog->SetY(jumpY);
			if ((treesColliedsToFrog) || (frogColliedsToFrogs)) // Colision ( Jump back)
			{
				dummy->SetZ(zC);
				frogJumpState = STATE_Frog_LANDED;
			}
			else			
				dummy->MoveZ(-ds);			
		}
		else
			frogJumpState = STATE_Frog_LANDED;

		break;

	case STATE_Frog_JUMP_RightSide:
		degree += 1;
		if (x - xC <= 10) // Move 10 unit Right Side
		{
			frog->SetY(jumpY);
			if ((treesColliedsToFrog) || (frogColliedsToFrogs)) // Colision ( Jump back)
			{
				frog->SetX(xC);
				frogJumpState = STATE_Frog_LANDED;
			}
			else
				frog->MoveX(ds);		
		}
		else
		{
			frogJumpState = STATE_Frog_LANDED;
		}
		break;

	case STATE_Frog_JUMP_LeftSide:
		degree += 1;
		if (x - xC >= -10) // Move 10 unit Left Side
		{
			frog->SetY(jumpY);
			if ((treesColliedsToFrog) || (frogColliedsToFrogs)) // Colision ( Jump back)
			{
				frog->SetX(xC);
				frogJumpState = STATE_Frog_LANDED;
			}
			else
				frog->MoveX(-ds);			
		}
		else
		{
			frogJumpState = STATE_Frog_LANDED;
		}
		break;

	case STATE_Frog_LANDED:
		frog->SetY(0);
		degree = 0;
		treesColliedsToFrog = It_is_FREE;
		frogJumpState = STATE_Frog_JUMP_Waiting;
		break;
	}
}

// Get ready for next frog
void getReadyForNextFrog(int i, int nFrogs, int aStateF[], IModel* aFrog[], IModel* dummy,float & CountDown,
	int & gameState,int & nCF)
{
	if ((i < nFrogs - 1) && (aStateF[i + 1] == STATE_Frog_Waiting))
	{
		dummy->SetPosition(aFrog[i]->GetX(), aFrog[i]->GetY(), aFrog[i]->GetZ());
		aFrog[i]->DetachFromParent();
		aFrog[i]->SetPosition(dummy->GetX(), dummy->GetY(), dummy->GetZ());
		dummy->SetPosition(0, 0, 0);
		CountDown = maxTime;
		aStateF[i + 1] = STATE_Frog_Crossing;
		nCF = i+1;
	}

}

// Prosess the frog State
void procesFrogState(int nFrogs, IModel* aFrog[], int &nCF, int aStateF[], float ds, IModel* dummy,float & CountDown, int &gameState,
	float &xC, float &zC, IModel* tempModel, bool & TyerColliedsToFrog, int jumpStat, int & dead,int & safe,IModel* aFrogIcone[])
{
	for (int i = 0; i < nFrogs; i++)
	{
		switch (aStateF[i])
		{
		case STATE_Frog_Waiting:
			aFrog[i]->SetSkin("frog.bmp");
			aFrogIcone[i]->SetSkin("frog.bmp");
			if (aStateF[0] == STATE_Frog_Waiting) //Make first Frog Crossing to beable to move
				aStateF[0] = STATE_Frog_Crossing;
			break;

		case STATE_Frog_Crossing:
			if (aFrog[i]->GetZ() >= 115) // in Trees area
			{
				aFrog[i]->SetY(0); xC = 0; zC = -15;
				safe += 1;
				aStateF[i] = STATE_Frog_Safe;

			}
			if ((aFrog[i]->GetZ() >= 70) && (aFrog[i]->GetZ() <= 110)) // In Poision Sea area
			{
				if (aFrog[i]->GetY()<1)
					if (TyerColliedsToFrog)
					{
						aFrog[i]->SetX(tempModel->GetX());
						TyerColliedsToFrog == 0;
					}
					else
					{
						xC = 0; zC = -15;
						dead += 1;
						aStateF[i] = STATE_Frog_Dead;
					}

			}
			if (CountDown <= 0) // Lose the time
			{
				xC = 0; zC = -15;
				dead += 1;
				aStateF[i] = STATE_Frog_Dead;
			}
			break;

		case STATE_Frog_Safe:
			aFrog[i]->SetSkin("frog_blue.bmp");
			aFrogIcone[i]->SetSkin("frog_blue.bmp");
			//Get ready for next frog
			getReadyForNextFrog(i, nFrogs, aStateF, aFrog, dummy, CountDown, gameState, nCF); // Reset teh temprory setting

			break;

		case STATE_Frog_Dead:
			aFrog[i]->SetSkin("frog_red.bmp");
			aFrogIcone[i]->SetSkin("frog_red.bmp");
			if ((aFrog[i]->GetZ() >= 70) && (aFrog[i]->GetZ() <= 110)) // if is in Posion Sea area
			{
					aFrog[i]->MoveY(-ds); // Move under the Sea
					getReadyForNextFrog(i, nFrogs, aStateF, aFrog, dummy, CountDown, gameState, nCF); // Reset teh temprory setting
			}
			else // Dead With Cars Or Time Losing
			{
				aFrog[i]->SetY(0);
				aFrog[i]->ScaleY(0.2);
				getReadyForNextFrog(i, nFrogs, aStateF, aFrog, dummy, CountDown, gameState, nCF); // Reset teh temprory setting
			}
			break;

		}
	}
}

// Count the Score 
void scoreCounter(IModel* frog[],int nFrogs, int & score)
{
	score = 0;
	for (int i = 0; i < nFrogs; i++)
	{
		score += (int((frog[i]->GetZ() - 15) / 10)) * 10;
	}

}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder("Defult");


	/**** Set up your scene here ****/

	//-------------------------------------------------//
	//              Useful Variable                    //
	//-------------------------------------------------//
	int     nLevel                   = 1;                    // Number of Level
	int     tScore                   = 0;                    // Score of the Curent Level 
	int     finalScore               = 0;                    // Final Score
	int     degree                   = 0;                    // Degre of frog jumping
	int     nCF                      = 0;                    // Number of Curent turn frog for defult
	int     nfrogSafe                = 0;                    // how many frog are safe
	int     nFrogDead                = 0;                    // How many frog are dead
	int     jumpState                = STATE_Frog_LANDED;    // State transition diagrams for jamp and Movment of frog
	int     gameState                = STATE_Game_Playing;   // State transition diagrams for game
	bool    frogColliedsToFrogs      = It_is_FREE;           // Frog collieds to the other frogs
	bool    tyersColliedsToFrog      = It_is_FREE;           // Frogs coleads to the Tyers 
	bool    treesColliedsToFrog      = It_is_FREE;           // Frogs coleads to the trees
	float   xC                       = 0;                    // Curent X position of frog
    float   yC                       = 0;                    // Curent Y position of frog
	float   zC                       = 15;                   // Curent Z position of frog
	float   time                     = 8;                    // Starting cont down timer
	float   timer                    = 0;                    // Game count down timer
	float   countDown                = 20;                   // Frog count down  timer
	float   kGameSpeed               = 1;                    // Game speed
	
	// Constants
	const int nFrogs                 = 3;                    // Number of frog
	const int nCars                  = 3;                    // Number of Rover station Car  
	const int nVans                  = 3;                    // Number of transit Car 
	const int nRovers                = 1;                    // Number of Rang Rover Car 
	const int nPickUps               = 1;                    // Number of Pich Up Car
	const int nTyers                 = 4;                    // Number of Tyers rows
	const int nGTyers                = 3;                    // Number of Tyers colums
	const int nTrees                 = 6;                    // Number of Trees
	const float vLine2               = 100.0   /   8;        // Traffic Line 2 Velocity
	const float vLine1               = vLine2  /   2;        // Traffic Line 1 Velocity
	const float vLine3               = vLine2  * 1.5;        // Traffic Line 3 Velocity
	const float vLine4               = vLine2  *   2;        // Traffic Line 4 Velocity
	/***************************************************/

	//-------------------------------------------------//
	//  Camera, Meshes, Models, Fonts , State Set up   //
	//-------------------------------------------------//
	// Camera
	ICamera* cam     = myEngine->CreateCamera(kManual, 0, 40, -60);    // Camera Meash Creating
	// Fonts
	IFont* font      = myEngine->LoadFont("Adobe Garamond Pro", 50);   // On Screen Message Font loading
	IFont* message   = myEngine->LoadFont("Monotype Corsiva", 200);    // Game over Massage Font loading
    // Meshes
	IMesh* mSkybox   = myEngine->LoadMesh("skybox.x");                 // Sky Mesh loading loading
	IMesh* mSea      = myEngine->LoadMesh("surface.x");                // Sea Mesh loading
	IMesh* mIsland1  = myEngine->LoadMesh("island1.x");                // Island 1 Mesh loading
	IMesh* mFrog     = myEngine->LoadMesh("frog.x");                   // Frog Mesh loading
	IMesh* mVan      = myEngine->LoadMesh("transit.x");                // Van Mesh loading
	IMesh* mCar      = myEngine->LoadMesh("rover.x");                  // Rover Station Mesh loading
	IMesh* mDummy    = myEngine->LoadMesh("dummy.x");                  // Dummy mesh loading
	IMesh* mIsland2  = myEngine->LoadMesh("island2.x");                // Island 2 Mesh loading
	IMesh* mTyer     = myEngine->LoadMesh("tyre.x");                   // Tyers Mesh loading
	IMesh* mRover    = myEngine->LoadMesh("rangeRover.x");             // Rang Rover Mesh loading
	IMesh* mPickUp   = myEngine->LoadMesh("pickUp.x");                 // Pick Up mesh loading
	IMesh* mTrees    = myEngine->LoadMesh("plant.x");                  // Tree Mesh  loading
	// Models
	IModel* skybox   = mSea->CreateModel(0, -1000, 0);                 // Sky Model Creating         
	IModel* sea      = mSea->CreateModel(0, -2.5, 0);                  // Sea Model Creating
	IModel* island1  = mIsland1->CreateModel(0, -5, 40);               // Island 1  Model Creating
	IModel* dummy    = mDummy->CreateModel(0, 0, 0);                   // movment dummy Model Creating
	IModel* dummyCam = mDummy->CreateModel(0, 0, 0);                   // camera dumy Model Creating
	IModel* tempModel = mDummy->CreateModel(0, 0, 0);                  // Temprory Position  Model Creating
	IModel* island2  = mIsland2->CreateModel(0, -5, 115);              // Island 2  Model Creating
	//Arraye Models
	IModel* aVan[nVans];                                               // Van Array      
	IModel* aCar[nCars];                                               // Rover station Array
	IModel* aPickUp[nPickUps];                                         // Pick Up Array
	IModel* aRover[nRovers];                                           // Range Rover Array
	IModel* aFrog[nFrogs];                                             // Frog Array
	IModel* aTrees[nTrees];                                            // Tree  Array
	IModel* aTyer[nTyers] [nGTyers];                                   // Tyer Matrix Array
	IModel* aFrogIcone[nFrogs];                                        // Frog Icons Array

	// Array Model Creating
	// Place 3 frogs in their initial positions of(-10, 0, 15), (0, 0, 15) and (10, 0, 15).
	modelCreator(nFrogs, aFrog, mFrog, 0, -10, 10, 0, 15, 0);

	// Icon frog Setting
	modelCreator(nFrogs, aFrogIcone, mFrog, 0, -12,1.1, -6,15, 0);
	for (int i = 0; i < nFrogs; i++)
	{
		aFrogIcone[i]->Scale(0.1);
		aFrogIcone[i]->AttachToParent(cam);
		aFrogIcone[i]->RotateLocalX(-30);
	}

	// Ford Transit van
	modelCreator(nVans, aVan, mVan, -90, 50, -100 / nVans, 0, 35, 0);

	// Rover saloon car 
	modelCreator(nCars, aCar, mCar, 90, -50, 100 / nCars, 0, 45, 0);

	// vehicles, traffic lanes 1 and 4  
	modelCreator(nPickUps, aPickUp, mPickUp, -90, 50, -100 / nPickUps, 0, 25, 0); //line 1
	modelCreator(nRovers, aRover, mRover, 90, -50, 100 / nRovers, 0, 55, 0); //line4
	
	// Add some Trees.
	modelCreator(nTrees, aTrees, mTrees, 0, -50, 20, 0, 115, 0);

	// Add a line of four spare tyres form a bridge to the second island.
	for (int i = 0; i < nTyers; i++)
	{
		for (int l = 0; l < nGTyers; l++)
		{
			aTyer[i][l] = mTyer->CreateModel(-10+10*l,-3, 75 + 10 * i);
			aTyer[i][l]->Scale(10);
		}
	}

	


	//state Creating
	int aStateFrog[nFrogs];
	stateCreator(aStateFrog, nFrogs, STATE_Frog_Waiting);

	int aStateCar[nCars];
	stateCreator(aStateCar, nCars, STATE_Model_MOVING);

	int aStateVan[nVans];
	stateCreator(aStateVan, nVans, STATE_Model_MOVING);

	int aStatePickUp[nPickUps];
	stateCreator(aStatePickUp, nPickUps, STATE_Model_MOVING);

	int aStatRover[nRovers];
	stateCreator(aStatRover, nRovers, STATE_Model_MOVING);

	int aStateTyer[nTyers]{ STATE_Model_MOVING_Left,STATE_Model_MOVING_Right,STATE_Model_MOVING_Left,STATE_Model_MOVING_Right };

	// Arraye definations
	float aCarDelay[nCars];
	arrayCreator(aCarDelay, nCars, 0);

	float aVanDelay[nVans];
	arrayCreator(aVanDelay, nVans, 0);

	float aRoverDelay[nRovers];
	arrayCreator(aRoverDelay, nRovers, 0);

	float aPickUDelay[nPickUps];
	arrayCreator(aPickUDelay, nPickUps, 0);

	// Some Setting                   
	dummyCam->AttachToParent(dummy);
	cam->RotateLocalX(20);
	tempModel->Scale(0.1f);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		
		//-------------------------------------------------//
		//              usefue calculations                //
		//-------------------------------------------------//
		float dt       = myEngine->Timer();
		float dsCar    = kGameSpeed*  vLine3      *dt;
		float dsVan    = kGameSpeed*  vLine2      *dt;
		float dsPickUp = kGameSpeed*  vLine1      *dt;
		float dsRover  = kGameSpeed*  vLine4      *dt;
		float dsFrog   = kGameSpeed*  vLine2      *dt;
		float dsTyer   = kGameSpeed*  vLine2  /2  *dt;
		//**************************************************//
		
		
		//-------------------------------------------------//
		//                   Game State                    //
		//-------------------------------------------------//

		if (int(time) >=0)
			time -= dt;
			stringstream timeText;
			
			if (int(time) > 2)  	timeText << int(time) - 2;
			if (int(time) == 2)  	timeText << "Ready!?";
			if (int(time) == 1)  	timeText << "Go!";
		
		message->Draw(timeText.str(), 650, 250, kRed, kCentre, kTop);
		
			switch (gameState)				
			{
			case STATE_Game_Playing:

				if (int(time) <= 0)
				{
					countDown -= dt;

					//---------reset the collision valus---------
					treesColliedsToFrog = 0;
					frogColliedsToFrogs = 0;
					//--------------------------------------------

					//----------------------------------------------//
					//              Transportation                  //
					//----------------------------------------------//
					// Traffic line 1               
					processVehicleState(nPickUps, aPickUp, aStatePickUp, dsPickUp, 50, dt, aPickUDelay, -90);

					// Traffic line 2
					processVehicleState(nVans, aVan, aStateVan, dsVan, 50, dt, aVanDelay, -90);

					// Traffic line 3
					processVehicleState(nCars, aCar, aStateCar, dsCar, -50, dt, aCarDelay, 90);

					// Traffic line 4
					processVehicleState(nRovers, aRover, aStatRover, dsRover, -50, dt, aRoverDelay, 90);

					// Sea Traffic, whit the tyers
					processTyerState(nTyers, aTyer, aStateTyer, dsTyer);

					/************************************************/


					//----------------------------------------------//
					//               collision check                //
					//----------------------------------------------//

					// Frog collision to frog Check in Island 1 & 2        
					if ((aFrog[nCF]->GetZ() <= 25) || (105 <= aFrog[nCF]->GetZ()))
						FrogCollision(nFrogs, aFrog, aFrog[nCF], aStateFrog, frogColliedsToFrogs, nCF);

					// Vehicles Collision Check in Island 1     
					if (aFrog[nCF]->GetZ() <= 65)
					{
						// Cars collision
						vehiclesCollision(nCars, aCar, aFrog[nCF], aStateFrog, nCF, xC, zC, nFrogDead);

						// Vans collision
						vehiclesCollision(nVans, aVan, aFrog[nCF], aStateFrog, nCF, xC, zC, nFrogDead);

						// Rovers collision
						vehiclesCollision(nRovers, aRover, aFrog[nCF], aStateFrog, nCF, xC, zC, nFrogDead);

						// PicUps collision
						vehiclesCollision(nPickUps, aPickUp, aFrog[nCF], aStateFrog, nCF, xC, zC, nFrogDead);
					}

					// Tyers collision Check in Island 2
					if ((70 <= aFrog[nCF]->GetZ()) && (aFrog[nCF]->GetZ() <= 110))
						tyerscollision(nTyers, nGTyers, aTyer, aFrog[nCF], tempModel, tyersColliedsToFrog);

					// Trees collision Check in Island 2
					if (105 <= aFrog[nCF]->GetZ())
						treesCollision(nTrees, aTrees, aFrog[nCF], treesColliedsToFrog);
					/****************************************************/

					//-----------------------------------------------//
					//                  Frog                         //
					//-----------------------------------------------//

					// Frogs States
					procesFrogState(nFrogs, aFrog, nCF, aStateFrog, dsFrog, dummy, countDown, gameState, xC, zC, tempModel, tyersColliedsToFrog, jumpState, nFrogDead, nfrogSafe, aFrogIcone);

					// Frog jumping
					if (jumpState != STATE_Frog_JUMP_Waiting)
						frogJump(dummy, aFrog[nCF], jumpState, degree, dsFrog, xC, zC, treesColliedsToFrog, frogColliedsToFrogs);

					// If no frogs remain waiting the game transitions to the over state.
					if (nfrogSafe + nFrogDead == nFrogs)
						gameState = STATE_Game_Over;

					// Frogs movment handles keys  
					if (jumpState == STATE_Frog_JUMP_Waiting) //No frog movment key will be work until frog danded 
					{
						zC = dummy->GetZ();
						xC = aFrog[nCF]->GetX();

						if (myEngine->KeyHeld(Key_Z)/*Z button is held*/)
						{
							aFrog[nCF]->AttachToParent(dummy);
							cam->AttachToParent(dummyCam);
							aFrog[nCF]->ResetOrientation();
							aFrog[nCF]->RotateY(-90);
							if (-41 < aFrog[nCF]->GetX() + dsFrog)
								jumpState = STATE_Frog_JUMP_LeftSide;
						}

						if (myEngine->KeyHeld(Key_X)/* X button is held*/)
						{
							aFrog[nCF]->AttachToParent(dummy);
							cam->AttachToParent(dummyCam);
							aFrog[nCF]->ResetOrientation();
							aFrog[nCF]->RotateY(90);
							if (aFrog[nCF]->GetX() + dsFrog < 41)
								jumpState = STATE_Frog_JUMP_RightSide;
						}

						if (myEngine->KeyHeld(EKeyCode(192))/* ' button is held*/)
						{
							if (!((myEngine->KeyHeld(Key_Z)/*Z button is held*/) || (myEngine->KeyHeld(Key_X)/* X button is held*/))) //Privent of wrog movment
							{
								aFrog[nCF]->AttachToParent(dummy);
								cam->AttachToParent(dummyCam);
								aFrog[nCF]->ResetOrientation();
								jumpState = STATE_Frog_JUMP_Forward;
							}
						}

						if (myEngine->KeyHeld(EKeyCode(191))/* / button is held*/)
						{
							if (!((myEngine->KeyHeld(Key_Z)/*Z button is held*/) || (myEngine->KeyHeld(Key_X)/* X button is held*/))) //Privent of wrog movment
							{
								aFrog[nCF]->AttachToParent(dummy);
								cam->AttachToParent(dummyCam);
								aFrog[nCF]->ResetOrientation();
								aFrog[nCF]->RotateY(180);
								if (16 < (aFrog[nCF]->GetZ() + dsFrog))
									jumpState = STATE_Frog_JUMP_Backward;
							}
						}
					}
					// Pause the game
					if (myEngine->KeyHit(Key_P)/* P button is hit*/)
					{
						gameState = STATE_Game_Paused;
					}
					// Camera view roing up
					if (myEngine->KeyHeld(Key_Up)/* Up button is held*/)
					{
						cam->AttachToParent(dummyCam);
						if (cam->GetZ() <= dummyCam->GetZ() + 17)
							dummyCam->RotateLocalX(0.1);
					}
					// Camera view roing Down
					if (myEngine->KeyHeld(Key_Down)/* Up button is held*/)
					{
						cam->AttachToParent(dummyCam);
						if (cam->GetY() >= dummyCam->GetY() + 5)
							dummyCam->RotateLocalX(-0.1);
					}
					// Reset Camera Position
					if (myEngine->KeyHit(Key_C)/* Up button is held*/)
					{
						cam->DetachFromParent();
						dummyCam->ResetOrientation();
					}
				}
				/******************************************************/
				break;

			case STATE_Game_Paused:

				//Resume the game
				if (myEngine->KeyHit(Key_P)/* P button is hit*/)
				{
					gameState = STATE_Game_Playing;
				}
				break;

			case STATE_Game_Over:
				// show the massage
				font->Draw("Press Space key to reset the game", 660, 480, kWhite, kCentre, kTop);
				message->Draw("You Lost", 650, 320, kWhite, kCentre, kTop);

				// Reset the variables used in the game
				dummy->SetLocalPosition(0, 0, 0);
				frogColliedsToFrogs = 0; treesColliedsToFrog = 0; tyersColliedsToFrog = 0;
				xC = 0; zC = 15; nCF = 0; countDown = maxTime;
				jumpState == STATE_Frog_JUMP_Waiting;

				// Resetin the game
				if (nFrogDead == nFrogs)  // if every frog dead Reseat everything
				{
					font->Draw("Press Space key to reset the game", 660, 480, kWhite, kCentre, kTop);
					timer == 0; tScore = 0;
					for (int i = nFrogs - 1; i >= 0; i--)
					{
						mFrog->RemoveModel(aFrog[i]);
						aStateFrog[i] = STATE_Frog_Waiting;
					}
					//Place 3 frogs in their initial positions of(-10, 0, 15), (0, 0, 15) and (10, 0, 15).
					modelCreator(nFrogs, aFrog, mFrog, 0, -10, 10, 0, 15, 0);
					//state Creating
					stateCreator(aStateFrog, nFrogs, STATE_Frog_Waiting);
					if (myEngine->KeyHit(Key_Space))
					{
						time = 3;
						gameState = STATE_Game_Playing;
						finalScore = 0;
						nFrogDead = 0; nfrogSafe = 0;
					}
				}
				else    //if any frog is safe bring it back
				{
					finalScore += tScore;
					for (int i = 0; i < nFrogs; i++)
					{
						aFrog[i]->DetachFromParent();
						if (aStateFrog[i] == STATE_Frog_Safe)
						{
							aFrog[i]->SetSkin("frog.bmp");
							aFrogIcone[i]->SetSkin("frog.bmp");
							aStateFrog[i] = STATE_Frog_Waiting;
							aFrog[i]->SetLocalPosition(-10 + i * 10, 0, 15);
						}
						//if (i == nFrogs - 1)
						//	//{
						nfrogSafe = 0;
						time = 6;
						gameState = STATE_Game_Playing;
					}

					kGameSpeed += 0.1;
					nLevel += 1;
				}
								
				break;
			}
		//Exit	
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
			
		
		//Count-down timer in the corner of the screen, counting down in seconds from 20.
		stringstream counter;
		int colorT;
		counter << "Timer: " << int(countDown); 
		if (countDown >= 5)         colorT = kBlack;
		else                        colorT = kRed;
		font->Draw(counter.str(), 25, 25, colorT, kLeft, kTop);

		//Score in another corner of the screen. Every time a frog moves forwards 10 units, add 10 points to the score.
		scoreCounter(aFrog, nFrogs, tScore);
		stringstream Score;
		Score << "Score: " << finalScore+tScore;
		int colorS;
		if (finalScore + tScore <= 100)      	colorS = kRed;
		else if (finalScore + tScore >= 200)  	colorS = kGreen;
		else        			    colorS = kBlue;
			font->Draw(Score.str(), 1100, 25, colorS, kLeft, kTop);   
       
	    // Level in another corner of the scree.
		stringstream Level;
		Level << "Level: " << nLevel;
		font->Draw(Level.str(), 1100, 630, kWhite, kLeft, kTop);

		//******************************************************//
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
