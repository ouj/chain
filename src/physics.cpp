#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32

#include "physics.h"
#include "dims.h"
#include "render.h"
static b2World world(b2Vec2(0.0f, 0.0f));
static const float32 timeStep = 1.0f / 60.0f;
static const int32 velocityIterations = 6; 
static const int32 positionIterations = 2;

b2Body* ground = 0;
b2Body* worldBox = 0;
b2Body* body = 0;

DebugDraw debugDraw;

bool setupPhysics() {
    world.SetAllowSleeping(true);
    world.SetDebugDraw(&debugDraw);
    
    uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDraw.SetFlags(flags);
    
    // world box
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position.Set(0.0f, 0.0f);
    bd.angle = 0.0f;
    bd.linearVelocity.Set(0.0f, 0.0f);
    bd.angularVelocity = 0.0f;
    bd.linearDamping = 0.0f;
    bd.angularDamping = 0.0f;
    bd.allowSleep = true;
    bd.awake = true;
    bd.fixedRotation = false;
    bd.bullet = false;
    bd.active = true;
    bd.gravityScale = 1.0f;
    worldBox = world.CreateBody(&bd);
    
    {
        b2FixtureDef fd;
        fd.friction = 10.0f;
        fd.restitution = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = false;
        fd.filter.categoryBits = uint16(1);
        fd.filter.maskBits = uint16(65535);
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(WORLD_WIDTH, WORLD_HEIGHT);
        shape.m_vertex2.Set(WORLD_WIDTH, 0.0f);
        shape.m_vertex3.Set(0.0f, 0.0f);
        shape.m_hasVertex0 = false;
        shape.m_hasVertex3 = false;
        
        fd.shape = &shape;
        
        worldBox->CreateFixture(&fd);
    }
    {
        b2FixtureDef fd;
        fd.friction = 10.0f;
        fd.restitution = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = bool(0);
        fd.filter.categoryBits = uint16(1);
        fd.filter.maskBits = uint16(65535);
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(0.0f, WORLD_HEIGHT);
        shape.m_vertex2.Set(0.0f, 0.0f);
        shape.m_vertex3.Set(0.0f, 0.0f);
        shape.m_hasVertex0 = bool(0);
        shape.m_hasVertex3 = bool(0);
        
        fd.shape = &shape;
        
        worldBox->CreateFixture(&fd);
    }
    {
        b2FixtureDef fd;
        fd.friction = 10.0f;
        fd.restitution = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = bool(0);
        fd.filter.categoryBits = uint16(1);
        fd.filter.maskBits = uint16(65535);
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(0.0f, WORLD_HEIGHT);
        shape.m_vertex2.Set(WORLD_WIDTH, WORLD_HEIGHT);
        shape.m_vertex3.Set(0.0f, 0.0f);
        shape.m_hasVertex0 = bool(0);
        shape.m_hasVertex3 = bool(0);
        
        fd.shape = &shape;
        
        worldBox->CreateFixture(&fd);
    }
    {
        b2FixtureDef fd;
        fd.friction = 10.0f;
        fd.restitution = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = bool(0);
        fd.filter.categoryBits = uint16(1);
        fd.filter.maskBits = uint16(65535);
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(0.0f, 0.0f);
        shape.m_vertex2.Set(WORLD_WIDTH, 0.0f);
        shape.m_vertex3.Set(0.0f, 0.0f);
        shape.m_hasVertex0 = bool(0);
        shape.m_hasVertex3 = bool(0);
        
        fd.shape = &shape;
        
        worldBox->CreateFixture(&fd);
    }
    
//    b2BodyDef groundBodyDef;
//    groundBodyDef.position.Set(0.0f, -10.0f);
//    
//    b2Body* ground = world.CreateBody(&groundBodyDef);
//    b2PolygonShape groundBox;
//    groundBox.SetAsBox(50.0f, 10.0f);
//    ground->CreateFixture(&groundBox, 0.0f);
//    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; bodyDef.position.Set(2.0f, 4.0f);
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
    for (int32 i = 0; i < 1; ++i) {
        world.Step(timeStep, velocityIterations, positionIterations); 
        b2Vec2 position = body->GetPosition();
        float32 angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
}

void DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color)
{
	switch (fixture->GetType())
	{
        case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();
            
			b2Vec2 center = b2Mul(xf, circle->m_p);
			float32 radius = circle->m_radius;
			b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));
            
			debugDraw.DrawSolidCircle(center, radius, axis, color);
		}
            break;
            
        case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
			b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
			b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
			debugDraw.DrawSegment(v1, v2, color);
		}
            break;
            
        case b2Shape::e_chain:
		{
			b2ChainShape* chain = (b2ChainShape*)fixture->GetShape();
			int32 count = chain->m_count;
			const b2Vec2* vertices = chain->m_vertices;
            
			b2Vec2 v1 = b2Mul(xf, vertices[0]);
			for (int32 i = 1; i < count; ++i)
			{
				b2Vec2 v2 = b2Mul(xf, vertices[i]);
				debugDraw.DrawSegment(v1, v2, color);
				debugDraw.DrawCircle(v1, 0.05f, color);
				v1 = v2;
			}
		}
            break;
            
        case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			int32 vertexCount = poly->m_vertexCount;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];
            
			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}
            
			debugDraw.DrawSolidPolygon(vertices, vertexCount, color);
		}
            break;
            
        default:
            break;
	}
}

void drawWorld() {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-.5, WORLD_WIDTH+.5, -.5, WORLD_HEIGHT+.5, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    world.DrawDebugData();
    b2Body* b = worldBox;
    const b2Transform& xf = b->GetTransform();
    for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
    {
        if (b->IsActive() == false)
        {
            DrawShape(f, xf, b2Color(0.5f, 0.5f, 0.3f));
        }
        else if (b->GetType() == b2_staticBody)
        {
            DrawShape(f, xf, b2Color(0.5f, 0.9f, 0.5f));
        }
        else if (b->GetType() == b2_kinematicBody)
        {
            DrawShape(f, xf, b2Color(0.5f, 0.5f, 0.9f));
        }
        else if (b->IsAwake() == false)
        {
            DrawShape(f, xf, b2Color(0.6f, 0.6f, 0.6f));
        }
        else
        {
            DrawShape(f, xf, b2Color(0.9f, 0.7f, 0.7f));
        }
    }
    glPopMatrix();
}