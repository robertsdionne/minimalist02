//
//  triangle.cpp
//  minimalist01
//
//  Created by Robert Dionne on 2/12/13.
//
//

#include <list>

#include "ofMain.h"
#include "triangle.h"

const ofColor Triangle::kColor = ofColor(64.0, 226.0, 160.0);

Triangle::Triangle(bool player, float mass, float size, float orientation, ofVec2f position, ofVec2f velocity)
: GameObject(player, mass, size, orientation, position, velocity) {}

Triangle::~Triangle() {}

ofColor Triangle::color() const {
  return kColor;
}

float Triangle::reproductivity() const {
  return kReproductivity;
}

void Triangle::DrawInternal() const {
  ofBeginShape();
  for (unsigned int i = 0; i < 4; ++i) {
    ofVertex(cos(i * 2.0 *  M_PI / 3.0), sin(i * 2.0 * M_PI / 3.0));
  }
  ofEndShape();
}
