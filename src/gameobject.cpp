//
//  gameobject.cpp
//  minimalist01
//
//  Created by Robert Dionne on 2/12/13.
//
//

#include <list>

#include "circle.h"
#include "gameobject.h"
#include "ofMain.h"
#include "triangle.h"
#include "square.h"

constexpr unsigned int GameObject::kMaxPopulation;
constexpr float GameObject::kMaxSize;
constexpr float GameObject::kChildScaleFactor;
constexpr float GameObject::kDrag;
constexpr float GameObject::kGrowthRate;
constexpr float GameObject::kLineWidthScaleFactor;
constexpr float GameObject::kMaxComponentOfVelocity;

GameObject::GameObject(
    bool player, float mass, float size, float orientation, ofVec2f position, ofVec2f velocity)
: player(player), neighbors(), food(0.8), mass(mass), size(size), orientation(orientation), position(position), velocity(velocity), force() {}

GameObject::~GameObject() {}

void GameObject::Draw() const {
  ofPushMatrix();
  ofTranslate(position);
  ofScale(size, size);
  ofRotateZ(ofRadToDeg(orientation));
  ofPushStyle();
  if (food > 0) {
    ofEnableAlphaBlending();
    ofSetColor(ofColor(128, 0, 128).getLerped(ofColor(0, 0, 255), (size - 10) / 10.0), 255 * food);
    ofFill();
    ofSetLineWidth(0);
    DrawInternal();
    ofDisableAlphaBlending();
  }
  ofSetColor(color() / (21.0 / size));
  ofNoFill();
  ofSetLineWidth(size * kLineWidthScaleFactor);
  DrawInternal();
  ofPopStyle();
  ofPopMatrix();
}

void GameObject::MaybeReproduce(
    std::list<GameObject *> &triangles,
    std::list<GameObject *> &circles,
    std::list<GameObject *> &squares, unsigned int reproduce_type) {
  if (ofRandomuf() < reproductivity()) {
    size *= kChildScaleFactor;
    velocity = size * ofVec2f(
        kMaxComponentOfVelocity * ofRandomf(), kMaxComponentOfVelocity * ofRandomf());
    if (triangles.size() + circles.size() + squares.size() < kMaxPopulation) {
      if (reproduce_type == 0) {
        triangles.push_back(new Triangle(player, mass, size, orientation, position, -velocity));
      } else if (reproduce_type == 1) {
        circles.push_back(new Circle(player, mass, size, orientation, position, -velocity));
      } else {
        squares.push_back(new Square(player, mass, size, orientation, position, -velocity));
      }
    }
  }
}

void GameObject::Update(float dt) {
  UpdateInternal(dt);
  position += velocity * dt;
  velocity += force / mass * dt;
  force = ofVec2f();
  if (position.x < 0) {
    position.x += ofGetWidth();
  }
  if (position.x >= ofGetWidth()) {
    position.x -= ofGetWidth();
  }
  if (position.y < 0) {
    position.y += ofGetHeight();
  }
  if (position.y >= ofGetHeight()) {
    position.y -= ofGetHeight();
  }
}

void GameObject::UpdateInternal(float dt) {
  orientation += ofDegToRad(ofRandomf());
  if (size < kMaxSize) {
    size += kGrowthRate * ofRandomuf();
  }
  force += -kDrag * velocity;
}
