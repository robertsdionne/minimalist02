//
//  circle.cpp
//  minimalist01
//
//  Created by Robert Dionne on 2/12/13.
//
//

#include "circle.h"
#include "ofMain.h"

const ofColor Circle::kColor = ofColor(255.0, 102.0, 102.0);

Circle::Circle(bool player, float mass, float size, float orientation, ofVec2f position, ofVec2f velocity)
: GameObject(player, mass, size, orientation, position, velocity) {}

Circle::~Circle() {}

ofColor Circle::color() const {
  return kColor;
}

float Circle::reproductivity() const {
  return kReproductivity;
}

void Circle::DrawInternal() const {
  ofBeginShape();
  for (unsigned int i = 0; i < 26; ++i) {
    ofVertex(cos(i * 2.0 *  M_PI / 25.0), sin(i * 2.0 * M_PI / 25.0));
  }
  ofEndShape();
}
