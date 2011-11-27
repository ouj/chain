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
#include "kinect.h"
#include "array.h"

static b2World world(b2Vec2(0.0f, -10.0f));
static const int32 velocityIterations = 6; 
static const int32 positionIterations = 2;
int stepCount = 0;

b2Body* leftArm = 0;
b2Body* rightArm = 0;
b2Body* worldBox = 0;
b2Body* ball = 0;
b2MouseJoint* leftHandJoint = 0;
b2MouseJoint* rightHandJoint = 0;
b2MouseJoint* leftElbowJoint = 0;
b2MouseJoint* rightElbowJoint = 0;

sarray2<b2Body*> bricks;


#define WALL_CATEGORY   1
#define WALL_MASK       2

#define BALL_CATEGORY   2
#define BALL_MASK       0xffff

#define ARM_CATEGORY    4
#define ARM_MASK        0xffff

#define BRICK_CATEGORY    8
#define BRICK_MASK        0xffff

DebugDraw debugDraw;

class BrickContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {}
    
    void EndContact(b2Contact* contact) {
        message("contact");
        //check if fixture A was a ball
        b2Body *brick = 0;
        if (contact->GetFixtureA()->GetBody() == ball) {
            brick = contact->GetFixtureB()->GetBody();
        } else if (contact->GetFixtureB()->GetBody() == ball) {
            brick = contact->GetFixtureA()->GetBody();
        }
        
        void* bodyUserData = brick->GetUserData();
        if (bodyUserData) {
            brick->SetTransform(b2Vec2(-10, -10), 0);
        }
    }
};

BrickContactListener brickContactListenerInstance;

void createWorldBox() {
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
    bd.userData = 0;
    worldBox = world.CreateBody(&bd);
    
    {
        b2FixtureDef fd;
        fd.friction = 10.0f;
        fd.restitution = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = false;
        fd.filter.categoryBits = WALL_CATEGORY;
        fd.filter.maskBits = WALL_MASK;
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(WORLD_WIDTH, WORLD_HEIGHT);
        shape.m_vertex2.Set(WORLD_WIDTH, WORLD_BOTTOM);
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
        fd.filter.categoryBits = WALL_CATEGORY;
        fd.filter.maskBits = WALL_MASK;
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(0.0f, WORLD_HEIGHT);
        shape.m_vertex2.Set(0.0f, WORLD_BOTTOM);
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
        fd.filter.categoryBits = WALL_CATEGORY;
        fd.filter.maskBits = WALL_MASK;
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
        fd.filter.categoryBits = WALL_CATEGORY;
        fd.filter.maskBits = WALL_MASK;
        fd.filter.groupIndex = int16(0);
        b2EdgeShape shape;
        shape.m_radius = 0.01f;
        shape.m_vertex0.Set(0.0f, 0.0f);
        shape.m_vertex1.Set(0.0f, WORLD_BOTTOM);
        shape.m_vertex2.Set(WORLD_WIDTH, WORLD_BOTTOM);
        shape.m_vertex3.Set(0.0f, 0.0f);
        shape.m_hasVertex0 = bool(0);
        shape.m_hasVertex3 = bool(0);
        
        fd.shape = &shape;
        
        worldBox->CreateFixture(&fd);
    }
}



void createArms() {
    {
        b2BodyDef leftArmDef;
        leftArmDef.type = b2_dynamicBody; 
        leftArmDef.position.Set(1.0f, 2.0f);
        leftArmDef.gravityScale = 1.0f;
        leftArmDef.userData = 0;
        leftArm = world.CreateBody(&leftArmDef);
        b2PolygonShape dynamicBox; 
        dynamicBox.SetAsBox(0.8f, 0.05f);
        b2FixtureDef fixtureDef; 
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 10.0f; 
        fixtureDef.friction = 0.0f;
        fixtureDef.filter.categoryBits = ARM_CATEGORY;
        fixtureDef.filter.maskBits = ARM_MASK;
        leftArm->CreateFixture(&fixtureDef);
        
        b2MouseJointDef md;
		md.bodyA = worldBox;
		md.bodyB = leftArm;
		md.maxForce = 1000.0f * leftArm->GetMass();
        md.target = leftArmDef.position + b2Vec2(-0.4f, 0);
		leftHandJoint = (b2MouseJoint*)world.CreateJoint(&md);
        md.target = leftArmDef.position + b2Vec2(0.4f, 0);;
        leftElbowJoint = (b2MouseJoint*)world.CreateJoint(&md);
    }
    {
        b2BodyDef rightArmDef;
        rightArmDef.type = b2_dynamicBody; 
        rightArmDef.position.Set(4.0f, 2.0f);
        rightArmDef.gravityScale = 1.0f;
        rightArmDef.userData = 0;
        rightArm = world.CreateBody(&rightArmDef);
        b2PolygonShape dynamicBox; 
        dynamicBox.SetAsBox(0.8f, 0.05f);
        b2FixtureDef fixtureDef; 
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 10.0f; 
        fixtureDef.friction = 0.0f;
        fixtureDef.filter.categoryBits = ARM_CATEGORY;
        fixtureDef.filter.maskBits = ARM_MASK;
        rightArm->CreateFixture(&fixtureDef);
        
        b2MouseJointDef md;
		md.bodyA = worldBox;
		md.bodyB = rightArm;
		md.maxForce = 1000.0f * rightArm->GetMass();
        md.target = rightArmDef.position + b2Vec2(0.4f, 0);
		rightHandJoint = (b2MouseJoint*)world.CreateJoint(&md);
        md.target = rightArmDef.position + b2Vec2(-0.4f, 0);
        rightElbowJoint = (b2MouseJoint*)world.CreateJoint(&md);
    }
}

void createBall() {
    b2BodyDef bd;
    bd.position.Set(1.0f, 2.0f);
    bd.type = b2_dynamicBody;
    //bd.bullet = true;
    bd.bullet = false;
    bd.userData = 0;
    ball = world.CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_radius = 0.1f;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 0.0f;
    fd.filter.categoryBits = BALL_CATEGORY;
    fd.restitution = 0.9f;
    fd.filter.maskBits = BALL_MASK;
    ball->CreateFixture(&fd);
}

void createBricks() {
    float bwidth = WORLD_WIDTH / 10;
    float bheight = bwidth * 0.5f;
    
    bricks.resize(8, 5);
    for (int j = 0; j < bricks.height(); j++) {
        for (int i = 0; i < bricks.width(); i++) {
            b2BodyDef bd;
            bd.type = b2_staticBody;
            bd.position.Set(bwidth * 1.5f + i * bwidth, (WORLD_HEIGHT - (bheight * 1.5f + j * bheight)));
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
            bd.userData = (void*)1;
            bricks.at(i, j) = world.CreateBody(&bd);
            b2PolygonShape brickBox; 
            brickBox.SetAsBox(bwidth / 2, bheight / 2);
            b2FixtureDef fd; 
            fd.shape = &brickBox;
            fd.density = 10.0f; 
            fd.friction = 0.0f;
            fd.filter.categoryBits = BRICK_CATEGORY;
            fd.filter.maskBits = BRICK_MASK;
            bricks.at(i, j)->CreateFixture(&fd);
        }
    }
}

bool setupPhysics() {
    //world.SetAllowSleeping(true);
    world.SetDebugDraw(&debugDraw);
    world.SetContactListener(&brickContactListenerInstance);
    uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDraw.SetFlags(flags);
    createWorldBox();
    createArms();
    createBall();
    createBricks();
    return true;
}


void simulate() {
    float32 timeStep = 1.0f / 60.0f;
    world.Step(timeStep, velocityIterations, positionIterations); 
    if (timeStep > 0.0f)
		++stepCount;
}

void DrawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color) {
	switch (fixture->GetType()) {
        case b2Shape::e_circle: {
			b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();
            
			b2Vec2 center = b2Mul(xf, circle->m_p);
			float32 radius = circle->m_radius;
			b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));
            
			debugDraw.DrawSolidCircle(center, radius, axis, color);
		} break;
        case b2Shape::e_edge: {
			b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
			b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
			b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
			debugDraw.DrawSegment(v1, v2, color);
		} break;
        case b2Shape::e_chain: {
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
		} break;
        case b2Shape::e_polygon: {
			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			int32 vertexCount = poly->m_vertexCount;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];
            
			for (int32 i = 0; i < vertexCount; ++i) {
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}
			debugDraw.DrawSolidPolygon(vertices, vertexCount, color);
		} break;
        default: break;
	}
}

void drawBody(b2Body *b, b2Color c) {
    const b2Transform& xf = b->GetTransform();
    for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
        DrawShape(f, xf, c);
    }
}

void drawWorldBox() {
    glLineWidth(4.0f);
    drawBody(worldBox, b2Color(0.5f, 0.5f, 0.3f));
}

void drawArms() {
    glLineWidth(1.0f);
    drawBody(leftArm, b2Color(0.1f, 0.5f, 0.3f));
    drawBody(rightArm, b2Color(0.1f, 0.5f, 0.8f));
}

void drawBall() {
    glLineWidth(1.0f);
    drawBody(ball, b2Color(0.6, 0.6, 0.2));
}

static b2Vec2 convert(const XnPoint3D& pt) {
    float scaleX = WORLD_WIDTH / KINECT_WIDTH * 1.5f;
    float scaleY = WORLD_WIDTH / KINECT_WIDTH * 1.5f;
    return b2Vec2(pt.X * scaleX, (KINECT_HEIGHT - pt.Y) * scaleY);
}

void drawWorld() {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-.2, WORLD_WIDTH+.2, -.2, WORLD_HEIGHT+.2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
//    drawWorldBox();
//    drawArms();
//    drawBall();
    
    KinectUser& user = getKinectUser();
    if (user.tracking) {
        b2Vec2 leftElbow, rightElbow;
        b2Vec2 leftHand, rightHand;
        leftElbow = convert(user.leftLowerArm.posSrn[0]);
        leftHand = convert(user.leftLowerArm.posSrn[1]);
        rightElbow = convert(user.rightLowerArm.posSrn[0]);
        rightHand = convert(user.rightLowerArm.posSrn[1]);
        b2Vec2 leftArmPos = (leftElbow + leftHand);
        leftArmPos *= 0.5f;
        b2Vec2 rightArmPos = rightElbow + rightHand;
        rightArmPos *= 0.5f;
//        leftArm->SetTransform(leftArmPos, 0.0f);
//        rightArm->SetTransform(rightArmPos, 0.0f);
        leftElbowJoint->SetTarget(leftElbow);
        leftHandJoint->SetTarget(leftHand);
        rightElbowJoint->SetTarget(rightElbow);
        rightHandJoint->SetTarget(rightHand);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex2f(leftArmPos.x, leftArmPos.y);
        glVertex2f(rightArmPos.x, rightArmPos.y);
//        glVertex2f(leftElbow.x, leftElbow.y);
//        glVertex2f(leftHand.x, leftHand.y);
//        glVertex2f(rightElbow.x, rightElbow.y);
//        glVertex2f(rightHand.x, rightHand.y);
        glEnd();
    }
    world.DrawDebugData();
    
    glPopMatrix();
}