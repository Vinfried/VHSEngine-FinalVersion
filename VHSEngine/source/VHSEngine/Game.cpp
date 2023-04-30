#include "VHSEngine/Game.h"
#include "VHSEngine/Graphics/GraphicsEngine.h"
#include "VHSEngine/Graphics/Model.h"
#include "VHSEngine/Input.h"
#include "VHSEngine/Graphics/Camera.h"
#include "VHSEngine/Graphics/Material.h"
#include "VHSEngine/Collisions/Collision.h"

Game& Game::GetGameInstance()
{
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	static Game* GameInstance = &GetGameInstance();
	delete GameInstance;
}

void Game::Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	Graphics = make_shared<GraphicsEngine>();

	if (Graphics->InitGE(WTitle, bFullscreen, WWidth, WHeight)) {
		bIsGameOver = false;
	}

	Run();
}

TexturePtr Game::GetDefaultEngineTexture()
{
	return Graphics->DefaultEngineTexture;
}

MaterialPtr Game::GetDefaultEngineMaterial()
{
	return Graphics->DefaultEngineMaterial;
}

void Game::RemoveModelFromGame(ModelPtr& ModelToRemove)
{
	//remove from the graphics engine
	Graphics->RemoveModel(ModelToRemove);

	// change the reference to nullptr and remove from the game
	ModelToRemove = nullptr;
}

Game::Game()
{
	cout << "Game Initialised" << endl;

	Graphics = nullptr;
	bIsGameOver = false;
	
}

Game::~Game()
{
	Graphics = nullptr;
	cout << "Game Over..." << endl;
}

void Game::Run()
{
	if (!bIsGameOver) {

		//create an input class to detect input
		GameInput = new Input();

		ShaderPtr TextureShader = Graphics->CreateShader({
			L"Game/Shaders/TextureShader/TextureShader.svert",
			L"Game/Shaders/TextureShader/TextureShader.sfrag"
			});

		//import custom meshes 
		Model2 = Graphics->ImportModel("Game/Models/Primitives/Sphere.fbx", TextureShader);

		Floor = Graphics->ImportModel("Game/Models/Primitives/Cube.fbx", TextureShader);

		Ceiling = Graphics->ImportModel("Game/Models/Primitives/Cube.fbx", TextureShader);
		
		Wall = Graphics->ImportModel("Game/Models/damaged-wall/source/SM_Wall_Damaged.obj", TextureShader);
		Wall2 = Graphics->ImportModel("Game/Models/damaged-wall/source/SM_Wall_Damaged.obj", TextureShader);
		Wall3 = Graphics->ImportModel("Game/Models/damaged-wall/source/SM_Wall_Damaged.obj", TextureShader);
		Wall4 = Graphics->ImportModel("Game/Models/damaged-wall/source/SM_Wall_Damaged.obj", TextureShader);

		Door = Graphics->ImportModel("Game/Models/Door/Model/10057_wooden_door_v3_iterations-2.obj", TextureShader);

		CeilingLight = Graphics->ImportModel("Game/Models/068_Volica_LED_System/AM152_068_Volica_LED_System.obj", TextureShader);
		CeilingLight2 = Graphics->ImportModel("Game/Models/068_Volica_LED_System/AM152_068_Volica_LED_System.obj", TextureShader);
		CeilingLight3 = Graphics->ImportModel("Game/Models/068_Volica_LED_System/AM152_068_Volica_LED_System.obj", TextureShader);
		CeilingLight4 = Graphics->ImportModel("Game/Models/068_Volica_LED_System/AM152_068_Volica_LED_System.obj", TextureShader);

		ExitSign = Graphics->ImportModel("Game/Models/ExitSign/Model/Exit.obj", TextureShader);

		Soldier = Graphics->ImportModel("Game/Models/Soldier/Model/Soldier.obj", TextureShader);

		//transform the models and meshes (Scale, Location and Rotation)
		
		Model2->Transform.Location = Vector3(3.0f, 0.0f, -3.0f);
		
		Floor->Transform.Location = Vector3(0.0f, -2.0f, 0.0f);
		Floor->Transform.Scale = Vector3(60.0f, 1.0f, 30.0f);

		Ceiling->Transform.Scale = Vector3(60.0f, 1.0f, 30.0f);
		Ceiling->Transform.Location = Vector3(0.0f, 6.0f, 0.0f);

		CeilingLight->Transform.Scale = Vector3(0.04f);
		CeilingLight->Transform.Location = Vector3(-15.0f, 5.0f, -8.0f);

		CeilingLight2->Transform.Scale = Vector3(0.04f);
		CeilingLight2->Transform.Location = Vector3(-15.0f, 5.0f, 8.0f);

		CeilingLight3->Transform.Scale = Vector3(0.04f);
		CeilingLight3->Transform.Location = Vector3(15.0f, 5.0f, -8.0f);

		CeilingLight4->Transform.Scale = Vector3(0.04f);
		CeilingLight4->Transform.Location = Vector3(15.0f, 5.0f, 8.0f);

		Wall->Transform.Scale = Vector3(0.3f, 0.1f, 0.05f);
		Wall->Transform.Rotation.y = 0.0f;
		Wall->Transform.Location = Vector3(0.0f, -1.5f, 15.0f);

		Wall2->Transform.Scale = Vector3(0.3f, 0.1f, 0.05f);
		Wall2->Transform.Rotation.y = 180.0f;
		Wall2->Transform.Location = Vector3(0.0f, -1.5f, -15.0f);

		Wall3->Transform.Scale = Vector3(0.15f, 0.1f, 0.05f);
		Wall3->Transform.Rotation.y = 90.0f;
		Wall3->Transform.Location = Vector3(30.0f, -1.5f, 0.0f);

		Wall4->Transform.Scale = Vector3(0.15f, 0.1f, 0.05f);
		Wall4->Transform.Rotation.y = 270.0f;
		Wall4->Transform.Location = Vector3(-30.0f, -1.5f, 0.0f);

		Door->Transform.Scale = Vector3(0.02f);
		Door->Transform.Rotation = Vector3(90.0f, 0.0f, 180.0f);
		Door->Transform.Location = Vector3(25.0f,2.7f, -14.3f);

		ExitSign->Transform.Location = Vector3(23.8f, 1.0f, -14.3f);

		Soldier->Transform.Scale = Vector3(0.025f);
		Soldier->Transform.Location = Vector3(0.0f, -1.5f, 0.0f);

		//add collisions
		Floor->AddCollisionToModel(Vector3(60.0f, 1.0f, 30.0f));

		Ceiling->AddCollisionToModel(Vector3(60.0f, 1.0f, 30.0f));
		
		Model2->AddCollisionToModel(Vector3(4.0f));
		
		Wall->AddCollisionToModel(Vector3(60.0f, 8.0f, 1.3f), Vector3(0.0f, 4.0f, 0.0f));
		Wall2->AddCollisionToModel(Vector3(60.0f, 8.0f, 1.3f), Vector3(0.0f, 4.0f, 0.0f));
		Wall3->AddCollisionToModel(Vector3(1.3f, 8.0f, 30.0f), Vector3(0.0f, 4.0f, 0.0f));
		Wall4->AddCollisionToModel(Vector3(1.3f, 8.0f, 30.0f), Vector3(0.0f, 4.0f, 0.0f));

		Door->AddCollisionToModel(Vector3(3.0f, 4.0f, 2.0f), Vector3(0.0f, -2.0f, 1.0f));

		Soldier->AddCollisionToModel(Vector3(4.0f, 4.5f, 3.0f), Vector3(0.0f, 2.0f, 0.0f));

		//create the texture
		TexturePtr TConcrete = Graphics->CreateTexture("Game/Textures/GreySquare_S.jpg");
		TexturePtr TTech = Graphics->CreateTexture("Game/Textures/seamless-tech-texture.jpg");
		TexturePtr TWall = Graphics->CreateTexture("Game/Models/damaged-wall/textures/T_Wall_Damaged_BC.png");
		TexturePtr TFloor = Graphics->CreateTexture("Game/Textures/StoneBricksBeige015_Flat.jpg");
		TexturePtr TDoor = Graphics->CreateTexture("Game/Models/Door/Texture/10057_wooden_door_v1_diffuse.jpg");
		TexturePtr TSoldier = Graphics->CreateTexture("Game/Models/Soldier/Texture/Soldier_Texture.png");
		TexturePtr TExit = Graphics->CreateTexture("Game/Models/ExitSign/Texture/Exit.png");

		//create a material
		MaterialPtr MConcrete = make_shared<Material>();
		MaterialPtr MTech = make_shared<Material>();
		MaterialPtr MWall = make_shared<Material>();
		MaterialPtr MFloor = make_shared<Material>();
		MaterialPtr MDoor = make_shared<Material>();
		MaterialPtr MSoldier = make_shared<Material>();
		MaterialPtr MExit = make_shared<Material>();


		//assign the base colour of the materials using the textures
		MConcrete->BaseColour.TextureV3 = TConcrete;
		MTech->BaseColour.TextureV3 = TTech;
		MWall->BaseColour.TextureV3 = TWall;
		MFloor->BaseColour.TextureV3 = TFloor;
		MDoor->BaseColour.TextureV3 = TDoor;
		MSoldier->BaseColour.TextureV3 = TSoldier;
		MExit->BaseColour.TextureV3 = TExit;

		//apply the material to models and meshesww
		Model2->SetMaterialBySlot(0, MTech);
		Wall->SetMaterialBySlot(1, MWall);
		Wall2->SetMaterialBySlot(1, MWall);
		Wall3->SetMaterialBySlot(1, MWall);
		Wall4->SetMaterialBySlot(1, MWall);
		Ceiling->SetMaterialBySlot(0, MWall);
		Floor->SetMaterialBySlot(0, MWall);
		Door->SetMaterialBySlot(1, MDoor);
		Soldier->SetMaterialBySlot(1, MSoldier);
		ExitSign->SetMaterialBySlot(1, MExit);


		//apply lighting
		Model2->GetMaterialBySlot(0)->EmissiveColour.MultiplierV3 = Vector3(0.0f, 0.0f, 255.0f);

	}

	while (!bIsGameOver) {
		//Make sure twe process what the user has done
		ProcessInput();
		
		//Apply the logic base on the inputs and the AI Logic
		Update();

		//Render the screen based on the 2 functions above
		Draw();
	}

	//Clean the game after it ends
	CloseGame();
}

void Game::Update()
{
	//set delta time first always
	static double LastFrameTime = 0.0;
	//set the current time since the game has passed
	double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
	// find the time difference between the last and current frame
	double NewDeltaTime = CurrentFrameTime - LastFrameTime;
	//set delta time as seconds
	DeltaTime = NewDeltaTime / 1000.0;
	//update the last frame time for the next update
	LastFrameTime = CurrentFrameTime;


	Vector3 CameraInput = Vector3(0.0f);
	CDirections CamDirections = Graphics->EngineDefaultCam->GetDirections();
	CollisionPtr CamCol = Graphics->EngineDefaultCam->GetCameraCollision();
	Vector3 CamLocation = Graphics->EngineDefaultCam->GetTransform().Location;
	//check collision with certain models
	bool NotColliding = !CamCol->IsOverLapping(*Wall->GetCollision()) && !CamCol->IsOverLapping(*Wall2->GetCollision()) && !CamCol->IsOverLapping(*Wall3->GetCollision()) && !CamCol->IsOverLapping(*Wall4->GetCollision()) && !CamCol->IsOverLapping(*Floor->GetCollision()) && !CamCol->IsOverLapping(*Ceiling->GetCollision());

	//do movement

	if (Model2 != nullptr) {
		Model2->Transform.Rotation.x += -50.0f * GetFDeltaTime();
		Model2->Transform.Rotation.y += -50.0f * GetFDeltaTime();
		Model2->Transform.Rotation.z += -50.0f * GetFDeltaTime();
	}

	if (Soldier != nullptr) {
		Soldier->Transform.Rotation.y += 700.0f * GetFDeltaTime();
		//soldier chases you
		Soldier->Transform.Location.x += (CamLocation.x - Soldier->Transform.Location.x) * 0.001f;
		Soldier->Transform.Location.z += (CamLocation.z - Soldier->Transform.Location.z) * 0.001f;
			
	}

	//do collision stuff
	if (Model2 != nullptr && CamCol->IsOverLapping(*Model2->GetCollision())) {
		RemoveModelFromGame(Model2);
	}
	if (Door != nullptr && CamCol->IsOverLapping(*Door->GetCollision())){
		cout << "YOU WIN!!!" << endl;
		GetGameInstance().CloseApp();
	}
	if (CamCol->IsOverLapping(*Soldier->GetCollision())) {
		GetGameInstance().CloseApp();
		cout << "YOU LOSE... Spinning Johnny caught you" << endl;
	}
		
	
	//move camera forward
	if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {

		if (NotColliding)
		{
			CameraInput += CamDirections.Forward;
		}
		else
			Graphics->EngineDefaultCam->Translate(CamLocation - CamDirections.Forward);
	}

	//move camera backward
	if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
		if (NotColliding)
			CameraInput += -CamDirections.Forward;
		else
			Graphics->EngineDefaultCam->Translate(CamLocation + CamDirections.Forward);
	}

	//move camera left
	if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
		if (NotColliding)
			CameraInput += -CamDirections.Right;
		else
			Graphics->EngineDefaultCam->Translate(CamLocation + CamDirections.Right);

	}

	//move camera right
	if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
		if (NotColliding)
			CameraInput += CamDirections.Right;
		else
			Graphics->EngineDefaultCam->Translate(CamLocation - CamDirections.Right);
	}

	//move camera up
	if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
		if (!CamCol->IsOverLapping(*Floor->GetCollision()))
			CameraInput += -CamDirections.Up;
		else
			Graphics->EngineDefaultCam->Translate(CamLocation + CamDirections.Up);
	}

	//move camera down
	if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
		CameraInput += CamDirections.Up;
	}

	if (GameInput->IsKeyDown(SDL_SCANCODE_LSHIFT)) {
		Graphics->EngineDefaultCam->SetCameraSpeed(30.0f);
	}
	else
		Graphics->EngineDefaultCam->SetCameraSpeed(10.0f);

	
	Graphics->EngineDefaultCam->AddMovementInput(CameraInput);


	//move the camera according to input
	if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
		Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta);
		Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta);
		GameInput->ShowCursor(false);
	}
	else
		GameInput->ShowCursor(true);

	Graphics->EngineDefaultCam->Update();

}

void Game::ProcessInput()
{
	//Run the input detection for our game input
	GameInput->ProcessInput();
}

void Game::Draw()
{
	Graphics->ClearGraphics();

	Graphics->Draw();
	
	Graphics->PresentGraphics();
}

void Game::CloseGame()
{
	delete GameInput;
}
