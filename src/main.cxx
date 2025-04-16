extern "C"
{
    #include "raylib.h"
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

#include "btBulletDynamicsCommon.h"
#include <vector>
#include <memory>

// Physics settings
const float PHYSICS_TIME_STEP = 1.0f / 60.0f;
const int MAX_SUB_STEPS = 10;

// GameObject class that links Raylib and Bullet Physics
class GameObject {
private:
    // Raylib visual properties
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Color color;
    bool isDynamic;

    // Bullet physics properties
    btRigidBody* rigidBody;
    btCollisionShape* collisionShape;
    btDefaultMotionState* motionState;

public:
    GameObject(Vector3 pos, Vector3 rot, Vector3 scl, Color clr, 
              float mass, btCollisionShape* shape, btDiscreteDynamicsWorld* world) 
        : position(pos), rotation(rot), scale(scl), color(clr),
          isDynamic(mass > 0.0f), collisionShape(shape) {
        
        // Set up the Bullet physics object
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(btVector3(position.x, position.y, position.z));
        transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
        
        // Calculate inertia for dynamic objects
        btVector3 localInertia(0, 0, 0);
        if (isDynamic) {
            collisionShape->calculateLocalInertia(mass, localInertia);
        }
        
        // Create motion state and rigid body
        motionState = new btDefaultMotionState(transform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
        rigidBody = new btRigidBody(rbInfo);
        
        // Add the rigid body to the physics world
        world->addRigidBody(rigidBody);
    }
    
    ~GameObject() {
        delete rigidBody;
        delete motionState;
        // collisionShape is managed externally
    }
    
    // Synchronize the physics state to the visual state
    void updateFromPhysics() {
        if (isDynamic) {
            btTransform trans;
            rigidBody->getMotionState()->getWorldTransform(trans);
            
            // Update position
            btVector3 origin = trans.getOrigin();
            position.x = origin.getX();
            position.y = origin.getY();
            position.z = origin.getZ();
            
            // Update rotation (simplified for this example)
            btQuaternion rot = trans.getRotation();
            // In a real implementation, convert quaternion to Euler angles properly
            rotation.x = rot.getX();
            rotation.y = rot.getY();
            rotation.z = rot.getZ();
        }
    }
    
    // Draw the object using Raylib
    void draw() {
        // Simple example for cube
        DrawCube(position, scale.x, scale.y, scale.z, color);
        DrawCubeWires(position, scale.x, scale.y, scale.z, BLACK);
    }
    
    // Apply a force to the physics object
    void applyForce(Vector3 force) {
        if (isDynamic) {
            rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));
        }
    }
};

// Game world that manages both physics and rendering
class GameWorld {
private:
    // Bullet physics components
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    
    // Storage for all game objects
    std::vector<std::unique_ptr<GameObject>> objects;
    
    // Common collision shapes (reused for efficiency)
    btCollisionShape* groundShape;
    btCollisionShape* boxShape;
    btCollisionShape* sphereShape;
    
    // Time tracking for physics
    float accumulator = 0.0f;

public:
    GameWorld() {
        // Initialize Bullet Physics
        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
        
        // Set gravity
        dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
        
        // Create common collision shapes
        groundShape = new btBoxShape(btVector3(50.0f, 0.5f, 50.0f));
        boxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
        sphereShape = new btSphereShape(1.0f);
        
        // Create the ground
        createGround();
    }
    
    ~GameWorld() {
        // Clear all objects
        objects.clear();
        
        // Delete collision shapes
        delete groundShape;
        delete boxShape;
        delete sphereShape;
        
        // Delete Bullet components
        delete dynamicsWorld;
        delete solver;
        delete overlappingPairCache;
        delete dispatcher;
        delete collisionConfig;
    }
    
    void createGround() {
        // Create a static ground plane
        Vector3 groundPos = { 0.0f, -0.5f, 0.0f };
        Vector3 groundRot = { 0.0f, 0.0f, 0.0f };
        Vector3 groundScale = { 100.0f, 1.0f, 100.0f };
        Color groundColor = GRAY;
        
        objects.push_back(std::make_unique<GameObject>(
            groundPos, groundRot, groundScale, groundColor, 
            0.0f, groundShape, dynamicsWorld));  // Mass of 0 means static
    }
    
    void createBox(Vector3 position, Color color, float mass) {
        Vector3 boxRot = { 0.0f, 0.0f, 0.0f };
        Vector3 boxScale = { 2.0f, 2.0f, 2.0f };
        
        objects.push_back(std::make_unique<GameObject>(
            position, boxRot, boxScale, color, 
            mass, boxShape, dynamicsWorld));
    }
    
    void createSphere(Vector3 position, Color color, float mass) {
        Vector3 sphereRot = { 0.0f, 0.0f, 0.0f };
        Vector3 sphereScale = { 2.0f, 2.0f, 2.0f }; // Scale for visual representation
        
        objects.push_back(std::make_unique<GameObject>(
            position, sphereRot, sphereScale, color, 
            mass, sphereShape, dynamicsWorld));
    }
    
    void update(float deltaTime) {
        // Accumulate time for fixed physics timestep
        accumulator += deltaTime;
        
        // Update physics with fixed timestep for stability
        while (accumulator >= PHYSICS_TIME_STEP) {
            dynamicsWorld->stepSimulation(PHYSICS_TIME_STEP, 1, PHYSICS_TIME_STEP);
            accumulator -= PHYSICS_TIME_STEP;
        }
        
        // Update all game objects
        for (auto& obj : objects) {
            obj->updateFromPhysics();
        }
    }
    
    void draw() {
        for (auto& obj : objects) {
            obj->draw();
        }
    }
    
    void applyForceToObject(int index, Vector3 force) {
        if (index >= 0 && index < objects.size()) {
            objects[index]->applyForce(force);
        }
    }
};

// Main game loop
int main() {
    // Initialize Raylib
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib + Bullet Physics Integration");
    
    // Set up camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Create game world
    GameWorld world;
    
    // Create some dynamic objects
    world.createBox((Vector3){ 0.0f, 10.0f, 0.0f }, RED, 1.0f);
    world.createSphere((Vector3){ 3.0f, 15.0f, 0.0f }, BLUE, 1.0f);
    world.createBox((Vector3){ -3.0f, 20.0f, 0.0f }, GREEN, 1.0f);
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update camera manually
        if (IsKeyDown(KEY_W)) camera.position.z -= 0.2f;
        if (IsKeyDown(KEY_S)) camera.position.z += 0.2f;
        if (IsKeyDown(KEY_A)) camera.position.x -= 0.2f;
        if (IsKeyDown(KEY_D)) camera.position.x += 0.2f;
        if (IsKeyDown(KEY_Q)) camera.position.y -= 0.2f;
        if (IsKeyDown(KEY_E)) camera.position.y += 0.2f;
        
        // Calculate delta time
        float deltaTime = GetFrameTime();
        
        // Update physics and game objects
        world.update(deltaTime);
        
        // Apply force on key press
        if (IsKeyPressed(KEY_SPACE)) {
            world.applyForceToObject(1, (Vector3){ 0.0f, 20.0f, 0.0f });
        }
        
        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);
                world.draw();
                DrawGrid(100, 1.0f);
            EndMode3D();
            
            DrawFPS(10, 10);
            DrawText("Press SPACE to apply upward force to the blue sphere", 10, 40, 20, DARKGRAY);
        EndDrawing();
    }
    
    // Clean up Raylib
    CloseWindow();
    
    return 0;
}