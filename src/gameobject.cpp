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

constexpr unsigned int GameObject::kMaxPopulation;
constexpr float GameObject::kMinSize;
constexpr float GameObject::kBreederSize;
constexpr float GameObject::kMaxSize;
constexpr float GameObject::kChildScaleFactor;
constexpr float GameObject::kDrag;
constexpr float GameObject::kGrowthRate;
constexpr float GameObject::kLineWidthScaleFactor;
constexpr float GameObject::kMaxComponentOfVelocity;

GameObject::GameObject(
    bool player, float mass, float size, float food, float orientation, ofVec2f position, ofVec2f velocity)
: player(player), neighbors(), food(food), mass(mass), size(size), poison(0), age(0), orientation(orientation), position(position), velocity(velocity), force() {}

GameObject::~GameObject() {}

void GameObject::Draw() const {
  ofPushMatrix();
  ofTranslate(position);
  ofScale(size, size);
  ofRotateZ(ofRadToDeg(orientation));
  ofPushStyle();
  if (food > 0) {
    ofEnableAlphaBlending();
    ofSetColor(interior_cell_color().getLerped(wall_cell_color(), (size - 10) / 10.0), 255 * food);
    ofFill();
    ofSetLineWidth(0);
    DrawInternal();
    ofDisableAlphaBlending();
  }
  ofColor membrane = membrane_color();
  membrane.setSaturation(255 * (1-age));
  ofSetColor(membrane / (21.0 / size));
  ofNoFill();
  ofSetLineWidth(size * kLineWidthScaleFactor);
  DrawInternal();
  ofPopStyle();
  ofPopMatrix();
}

void GameObject::MaybeReproduce(std::list<GameObject *> &group) {
  if (size <= kBreederSize && food >= 0.5 && ofRandomuf() < reproductivity() && group.size() < kMaxPopulation) {
    size *= kChildScaleFactor;
    food -= 0.5;
    age = 0;
    const ofVec2f epsilon = ofVec2f(0.1, 0.1);
    group.push_back(new Circle(player, mass, size, 0, orientation, position + epsilon, velocity));
  }
  const float cell_mortality = size <= kBreederSize ? mortality() : kWallMortality;
  if (ofRandomuf() < cell_mortality * age) {
    size = 0;
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
  if (size < kBreederSize - kGrowthRate) {
    size += kGrowthRate * ofRandomuf();
  }
  if (age <= 1.0 - kAgeRate) {
    age += kAgeRate * ofRandomuf();
  }
  force += -kDrag * velocity;
}
