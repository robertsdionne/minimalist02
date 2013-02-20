//
//  ex.cpp
//  minimalist01
//
//  Created by Robert Dionne on 2/13/13.
//
//

#include "ex.h"
#include "ofMain.h"

const ofColor Ex::kColor = ofColor(124.0, 178.0, 232.0);

Ex::Ex(bool player, float mass, float size, float orientation, ofVec2f position, ofVec2f velocity)
: GameObject(player, mass, size, orientation, position, velocity) {}

Ex::~Ex() {}

ofColor Ex::color() const {
  return kColor;
}

float Ex::reproductivity() const {
  return kReproductivity;
}

void Ex::DrawInternal() const {
  ofBeginShape();
  for (unsigned int i = 0; i < 4; i += 2) {
    ofVertex(cos(i * 2.0 *  M_PI / 4.0), sin(i * 2.0 * M_PI / 4.0));
  }
  ofEndShape();
  ofBeginShape();
  for (unsigned int i = 1; i < 4; i += 2) {
    ofVertex(cos(i * 2.0 *  M_PI / 4.0), sin(i * 2.0 * M_PI / 4.0));
  }
  ofEndShape();
}
