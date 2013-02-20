//
//  ex.h
//  minimalist01
//
//  Created by Robert Dionne on 2/13/13.
//
//

#ifndef __minimalist01__ex__
#define __minimalist01__ex__

#include <iostream>
#include <list>

#include "gameobject.h"
#include "ofMain.h"

class Ex : public GameObject {
public:
  Ex(bool player, float mass, float size, float orientation, ofVec2f position, ofVec2f velocity);
  
  virtual ~Ex();
  
  virtual ofColor color() const;
  
  virtual float reproductivity() const;
  
  virtual void DrawInternal() const;
  
private:
  static constexpr float kReproductivity = 0.001;
  static const ofColor kColor;
};

#endif /* defined(__minimalist01__ex__) */
