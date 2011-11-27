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
#include "func.h"
#include "game.h"

#define OBJ_NUM 8
#define CHAIN_LEN 20

static b2World world(b2Vec2(0.0f, -3.0f));
static const int32 velocityIterations = 12; 
static const int32 positionIterations = 6;
int stepCount = 0;

b2Body* leftHand = 0;
b2Body* rightHand = 0;
b2Body* leftShoulder = 0;
b2Body* rightShoulder = 0;

b2Body* worldWall = 0;
b2Body* worldFloor = 0;
b2Body* ball = 0;
b2MouseJoint* leftHandJoint = 0;
b2MouseJoint* rightHandJoint = 0;
b2MouseJoint* leftShoulderJoint = 0;
b2MouseJoint* rightShoulderJoint = 0;

b2Body* pointObjects[OBJ_NUM];

#define WALL_CATEGORY   1
#define WALL_MASK       2

#define FLOOR_CATEGORY  16
#define FLOOR_MASK      0xffff

#define BALL_CATEGORY   2
#define BALL_MASK       0xffff

#define ARM_CATEGORY    4
#define ARM_MASK        19

#define POINT_CATEGORY    8
#define POINT_MASK        2

DebugDraw debugDraw;

class BrickContactListener : public b2ContactListener
{
public:
    BrickContactListener() { nHit = 0; dead = false; }
    void BeginContact(b2Contact* contact) {}
    
    void EndContact(b2Contact* contact) {
        if (contact->GetFixtureA()->GetBody() == ball && 
            contact->GetFixtureB()->GetBody() == worldFloor) {
            dead = true;
        } else if (contact->GetFixtureB()->GetBody() == ball && 
                   contact->GetFixtureA()->GetBody() == worldFloor) {
            dead = true;
        }
                   
        //check if fixture A was a ball
        b2Body *hit = 0;
        if (contact->GetFixtureA()->GetBody() == ball) {
            hit = contact->GetFixtureB()->GetBody();
        } else if (contact->GetFixtureB()->GetBody() == ball) {
            hit = contact->GetFixtureA()->GetBody();
        } else {
            return;
        }
        
        void* bodyUserData = hit->GetUserData();
        if (bodyUserData) {
            bodies[nHit] = hit;
            nHit++;
        }
    }
    bool    dead;
    int     nHit;
    b2Body* bodies[32];
};

BrickContactListener brickContactListenerInstance;

void createWorldWall() {
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
    worldWall = world.CreateBody(&bd);
    
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
        
        worldWall->CreateFixture(&fd);
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
        
        worldWall->CreateFixture(&fd);
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
        
        worldWall->CreateFixture(&fd);
    }
}

void createWorldFloor() {
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
    worldFloor = world.CreateBody(&bd);
    {
        b2FixtureDef fd;
        fd.friction = 10.0f;
        fd.restitution = 0.0f;
        fd.density = 0.0f;
        fd.isSensor = bool(0);
        fd.filter.categoryBits = FLOOR_CATEGORY;
        fd.filter.maskBits = FLOOR_MASK;
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
        worldFloor->CreateFixture(&fd);
    }
}

void createArms() {
    {
        b2BodyDef bd;
        bd.type = b2_dynamicBody; 
        bd.position.Set(2.0f, 4.0f);
        bd.gravityScale = 1.0f;
        bd.userData = 0;
        leftHand = world.CreateBody(&bd);
        b2PolygonShape dynamicBox; 
        dynamicBox.SetAsBox(0.05f, 0.05f);
        b2FixtureDef fd; 
        fd.shape = &dynamicBox;
        fd.density = 10.0f; 
        fd.friction = 1.0f;
        fd.filter.categoryBits = ARM_CATEGORY;
        fd.filter.maskBits = ARM_MASK;
        leftHand->CreateFixture(&fd);
        
        b2MouseJointDef md;
		md.bodyA = worldWall;
		md.bodyB = leftHand;
		md.maxForce = 2000.0f * leftHand->GetMass();
        md.target = bd.position;
		leftHandJoint = (b2MouseJoint*)world.CreateJoint(&md);
    }
    {
        b2BodyDef bd;
        bd.type = b2_dynamicBody; 
        bd.position.Set(5.0f, 4.0f);
        bd.gravityScale = 1.0f;
        bd.userData = 0;
        rightHand = world.CreateBody(&bd);
        b2PolygonShape dynamicBox; 
        dynamicBox.SetAsBox(0.05f, 0.05f);
        b2FixtureDef fd; 
        fd.shape = &dynamicBox;
        fd.density = 10.0f; 
        fd.friction = 1.0f;
        fd.filter.categoryBits = ARM_CATEGORY;
        fd.filter.maskBits = ARM_MASK;
        rightHand->CreateFixture(&fd);
        
        b2MouseJointDef md;
		md.bodyA = worldWall;
		md.bodyB = rightHand;
		md.maxForce = 2000.0f * rightHand->GetMass();
        md.target = bd.position;
		rightHandJoint = (b2MouseJoint*)world.CreateJoint(&md);
    }
    /* {
        b2BodyDef bd;
        bd.type = b2_dynamicBody; 
        bd.position.Set(3.0f, 4.0f);
        bd.gravityScale = 1.0f;
        bd.userData = 0;
        leftShoulder = world.CreateBody(&bd);
        b2PolygonShape dynamicBox; 
        dynamicBox.SetAsBox(0.05f, 0.05f);
        b2FixtureDef fd; 
        fd.shape = &dynamicBox;
        fd.density = 10.0f; 
        fd.friction = 1.0f;
        fd.filter.categoryBits = ARM_CATEGORY;
        fd.filter.maskBits = ARM_MASK;
        leftShoulder->CreateFixture(&fd);
        
        b2MouseJointDef md;
		md.bodyA = worldBox;
		md.bodyB = leftShoulder;
		md.maxForce = 2000.0f * leftShoulder->GetMass();
        md.target = bd.position;
		leftShoulderJoint = (b2MouseJoint*)world.CreateJoint(&md);
    }
    {
        b2BodyDef bd;
        bd.type = b2_dynamicBody; 
        bd.position.Set(3.0f, 4.0f);
        bd.gravityScale = 1.0f;
        bd.userData = 0;
        rightShoulder = world.CreateBody(&bd);
        b2PolygonShape dynamicBox; 
        dynamicBox.SetAsBox(0.05f, 0.05f);
        b2FixtureDef fd; 
        fd.shape = &dynamicBox;
        fd.density = 10.0f; 
        fd.friction = 1.0f;
        fd.filter.categoryBits = ARM_CATEGORY;
        fd.filter.maskBits = ARM_MASK;
        rightShoulder->CreateFixture(&fd);
        
        b2MouseJointDef md;
		md.bodyA = worldBox;
		md.bodyB = rightShoulder;
		md.maxForce = 2000.0f * rightShoulder->GetMass();
        md.target = bd.position;
		rightShoulderJoint = (b2MouseJoint*)world.CreateJoint(&md);
    } */
}

void createBall() {
    b2BodyDef bd;
    bd.position.Set(WORLD_WIDTH/2, 6.0f);
    bd.type = b2_dynamicBody;
    bd.bullet = true;
    bd.userData = 0;
    ball = world.CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_radius = 0.1f;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 0.0f;
    fd.filter.categoryBits = BALL_CATEGORY;
    fd.restitution = 1.0f;
    fd.filter.maskBits = BALL_MASK;
    ball->CreateFixture(&fd);
}

void createChain(b2Body* begin, b2Body *end, int count) {
    const float cwidth = 0.1f;
    const float cheight = 0.05f;
    b2PolygonShape shape;
    shape.SetAsBox(cwidth, cheight);
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 0.2f;
    fd.filter.categoryBits = ARM_CATEGORY;
    fd.filter.maskBits = ARM_MASK;
    
    b2Vec2 p = begin->GetPosition();
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(p.x + cwidth, p.y);
    b2Body* sb = world.CreateBody(&bd);
    sb->CreateFixture(&fd);
    
    b2RevoluteJointDef jd;
    jd.collideConnected = false;
    jd.enableLimit = true;
    jd.maxMotorTorque = 5.0f;
    jd.motorSpeed = 0.0f;
    jd.enableMotor = true;
    
    jd.bodyA = begin;
    jd.bodyB = sb;
    jd.localAnchorA = b2Vec2(0.0f, 0.0f);
    jd.localAnchorB = b2Vec2(-cwidth * 0.6f, 0.0f);
    world.CreateJoint(&jd);
    
    b2Body* prevBody = sb;
    
    for (int i = 0; i < count; i++) {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(p.x + cwidth * i, p.y);
        b2Body* body = world.CreateBody(&bd);
        body->CreateFixture(&fd);
        
        b2RevoluteJointDef jd;
        jd.collideConnected = false;
        jd.bodyA = prevBody;
        jd.bodyB = body;
        jd.localAnchorA = b2Vec2(cwidth * 0.6f, 0.0f);
        jd.localAnchorB = b2Vec2(-cwidth * 0.6f, 0.0f);
        world.CreateJoint(&jd);
        prevBody = body;
    }
    
    jd.collideConnected = false;
    jd.bodyA = prevBody;
    jd.bodyB = end;
    jd.localAnchorA = b2Vec2(cwidth * 0.6f, 0.0f);
    jd.localAnchorB = b2Vec2(0.0f, 0.0f);
    world.CreateJoint(&jd);
}


void createPoint() {
    b2BodyDef bd;
    
    float x = (rand() / (float)INT_MAX) * WORLD_WIDTH;
    float y = WORLD_HEIGHT / 2.0f + ((rand() / (float)INT_MAX) * WORLD_HEIGHT / 2.0f);
    
    bd.position.Set(x, y);
    bd.type = b2_staticBody;
    bd.bullet = false;
    bd.userData = (void*)1;
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
    
    b2Body* body = world.CreateBody(&bd);
    
    b2CircleShape shape;
    int level = 0;
    float r = (rand() / (float)INT_MAX);
    if (r < 0.5f) {
        shape.m_radius = 0.5f;
        level = 1;
    } else if (r < 0.8f) {
        shape.m_radius = 0.3f;
        level = 2;
    } else {
        shape.m_radius = 0.2f;
        level = 3;
    }
    
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 0.0f;
    fd.filter.categoryBits = POINT_CATEGORY;
    fd.restitution = 0.5f;
    fd.filter.maskBits = POINT_MASK;
    body->CreateFixture(&fd); 
    body->SetUserData((void*)level);
}

void addBalls() {
    createBall();
    for (int i = 0; i < OBJ_NUM; i++) {
        createPoint();
    }
}

bool setupPhysics() {
    //world.SetAllowSleeping(true);
    world.SetDebugDraw(&debugDraw);
    world.SetContactListener(&brickContactListenerInstance);
    uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
	debugDraw.SetFlags(flags);
    createWorldWall();
    createWorldFloor();
    createArms();
    createChain(leftHand, rightHand, CHAIN_LEN);
    return true;
}

static b2Vec2 convert(const XnPoint3D& pt) {
    float scaleX = WORLD_WIDTH / KINECT_WIDTH;
    float scaleY = WORLD_WIDTH / KINECT_WIDTH * 0.5f;
    b2Vec2 v = b2Vec2(pt.X * scaleX, (KINECT_HEIGHT - pt.Y) * scaleY);
    v.x = (v.x * 2) - WORLD_WIDTH / 2;
    //v.y = (v.y * 2) - WORLD_WIDTH / 2;
    return v;
}


void simulate() {
    if (gameState() == GS_BEGIN) {
        addBalls();
        gameState() = GS_RUNNING;
    }
    float32 timeStep = 1.0f / 60.0f;
    world.Step(timeStep, velocityIterations, positionIterations); 
    if (timeStep > 0.0f)
        ++stepCount;
    // handle collision
    if (brickContactListenerInstance.nHit > 0) {
        for (int i = 0; i < brickContactListenerInstance.nHit; i++) {
            b2Body* b = brickContactListenerInstance.bodies[i];
            world.DestroyBody(b);
            int level = (int)(intptr_t)b->GetUserData();
            printf("%d\n", level);
            addScore(level);
            createPoint();
        }
        brickContactListenerInstance.nHit = 0;
    }
    
    if (brickContactListenerInstance.dead) {
        brickContactListenerInstance.dead = false;
        world.DestroyBody(ball);
        reduceScore();
        createBall();
    }
    
    if (gameState() == GS_RUNNING) {
        KinectUser& user = getKinectUser();
        if (user.tracking) {
            b2Vec2 leftHandPos, rightHandPos, leftShoulderPos, rightShoulderPos;
            leftHandPos = convert(user.leftLowerArm.posSrn[1]);
            leftShoulderPos = convert(user.leftShoulder.posSrn[1]);
            rightHandPos = convert(user.rightLowerArm.posSrn[1]);
            rightShoulderPos = convert(user.rightShoulder.posSrn[1]);
            leftHandJoint->SetTarget(leftHandPos);
            rightHandJoint->SetTarget(rightHandPos);
            //        leftShoulderJoint->SetTarget(leftShoulderPos);
            //        rightShoulderJoint->SetTarget(rightShoulderPos);
        }  
    }
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
    drawBody(worldWall, b2Color(0.5f, 0.5f, 0.3f));
}

void drawArms() {
    glLineWidth(1.0f);
    drawBody(leftHand, b2Color(0.1f, 0.5f, 0.3f));
    drawBody(rightHand, b2Color(0.1f, 0.5f, 0.8f));
}

void drawBall() {
    glLineWidth(1.0f);
    drawBody(ball, b2Color(0.6, 0.6, 0.2));
}

void drawWorld() {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WORLD_WIDTH, 0, WORLD_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
//    drawWorldBox();
//    drawArms();
//    drawBall();
    

    glColor3b(1, 0, 0);
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glVertex2f(rightHandJoint->GetTarget().x, rightHandJoint->GetTarget().y);
    glVertex2f(leftHandJoint->GetTarget().x, leftHandJoint->GetTarget().y);
//    glVertex2f(leftShoulderJoint->GetTarget().x, leftShoulderJoint->GetTarget().y);
//    glVertex2f(rightShoulderJoint->GetTarget().x, rightShoulderJoint->GetTarget().y);
    glEnd();
    world.DrawDebugData();
    
    glPopMatrix();
}