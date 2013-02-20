//
//  square.cpp
//  minimalist01
//
//  Created by Robert Dionne on 2/12/13.
//
//

#include "ofMain.h"
#include "square.h"

const ofColor Square::kColor = ofColor(255.0, 105.0, 248.0);

Square::Square(bool player, float mass, float size, float orientation, ofVec2f position, ofVec2f velocity)
: GameObject(player, mass, size, orientation, position, velocity) {}

Square::~Square() {}

ofColor Square::color() const {
  return kColor;
}

float Square::reproductivity() const {
  return kReproductivity;
}

void Square::DrawInternal() const {
  ofBeginShape();
  for (unsigned int i = 0; i < 5; ++i) {
    ofVertex(cos(i * 2.0 *  M_PI / 4.0), sin(i * 2.0 * M_PI / 4.0));
  }
  ofEndShape();
}
