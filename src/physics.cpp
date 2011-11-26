#include "physics.h"

static b2World world(b2Vec2(0.0f, -9.8f));
static const float32 timeStep = 1.0f / 60.0f;
static const int32 velocityIterations = 6; 
static const int32 positionIterations = 2;

b2Body* ground = 0;
b2Body* body = 0;

bool setupPhysics() {
    world.SetAllowSleeping(true);
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    
    b2Body* ground = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    ground->CreateFixture(&groundBox, 0.0f);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; bodyDef.position.Set(0.0f, 4.0f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox; dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef; 
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f; 
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    
    return true;
}


void simulate() {
    for (int32 i = 0; i < 60; ++i) {
        world.Step(timeStep, velocityIterations, positionIterations); 
        b2Vec2 position = body->GetPosition();
        float32 angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
}