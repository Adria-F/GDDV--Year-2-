#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	// OnKeys WASD keys -----------------------------------

	// TODO 3: Make the camera go up/down when pressing R (up) F(down)
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		Position += vec3(0, CAMERA_SPEED, 0);
		Reference += vec3(0, CAMERA_SPEED, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		Position -= vec3(0, CAMERA_SPEED, 0);
		Reference -= vec3(0, CAMERA_SPEED, 0);;
	}

	// TODO 4: Make the camera go forward (w) and backward with (s)
	// Note that the vectors X/Y/Z contain the current axis of the camera
	// you can read them to modify Position
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		Reference -= CAMERA_SPEED*Z;
		Position -= CAMERA_SPEED*Z;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		Reference += CAMERA_SPEED*Z;
		Position += CAMERA_SPEED*Z;
	}

	// TODO 5: Make the camera go left (a) and right with (d)
	// Note that the vectors X/Y/Z contain the current axis of the camera
	// you can read them to modify Position
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		Reference -= CAMERA_SPEED*X;
		Position -= CAMERA_SPEED*X;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		Reference += CAMERA_SPEED*X;
		Position += CAMERA_SPEED*X;
	}
	
	// Mouse motion ----------------
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		// TODO (Homework): Rotate the camera with the mouse
		// Rotation over reference
		float distance = sqrt(pow(Position.x - Reference.x, 2.0) + pow(Position.y - Reference.y, 2.0) + pow(Position.z - Reference.z, 2.0));
		float angleV = atan((Position.y - Reference.y) / (Position.z - Reference.z));
		float angleH = atan((Position.x - Reference.x) / (Position.z - Reference.z));

		angleV -= dy/10;
		angleH -= dx/10;

		if (angleV > 90)
			angleV = 90;
		if (angleV < -90)
			angleV = -90;

		if (angleH > 90)
			angleH = 90;
		if (angleH < -90)
			angleH = -90;
		
		vec3 newPos;
		newPos.x = sin(angleH);
		newPos.y = sin(angleV);
		newPos.z = cos(angleV);
		newPos *= distance;

		Position = newPos;
		LookAt(Reference);

		// Rotation over camera
		/*float Dx = dx *0.1f;
		float Dy = dy* 0.1f;

		if (Dx != 0 || Dy != 0)
			LookAt({ Reference.x - Dx, Reference.y + Dy, 0 });*/
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
