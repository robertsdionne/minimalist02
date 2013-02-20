#pragma once

#include <list>

#include "gameobject.h"
#include "ofMain.h"

class testApp : public ofBaseApp {
public:
  void setup();
  
  void update();
  
  void draw();
  
  void keyPressed(int key);
  
  void keyReleased(int key);
  
  void mouseMoved(int x, int y);
  
  void mouseDragged(int x, int y, int button);
  
  void mousePressed(int x, int y, int button);
  
  void mouseReleased(int x, int y, int button);
  
  void windowResized(int width, int height);
  
  void dragEvent(ofDragInfo dragInfo);
  
  void gotMessage(ofMessage msg);
  
private:
  void CollideGroups(std::list<GameObject *> &winners, std::list<GameObject *> &losers);
  void CreateRandomShape(ofVec2f at, bool player);
  void DrawGroup(std::list<GameObject *> &group);
  void RemoveDeadIndividuals(std::list<GameObject *> &group);
  void SteerGroup(std::list<GameObject *> &group, ofVec2f target);
  void UpdateGroup(std::list<GameObject *> &group, bool move);
  
private:
  static const unsigned int kNumEnemies;
  static const unsigned int kNumGameObjects;
  
  std::list<GameObject *> triangles;
  std::list<GameObject *> circles;
  std::list<GameObject *> squares;
  
  std::list<GameObject *> enemy_triangles;
  std::list<GameObject *> enemy_circles;
  std::list<GameObject *> enemy_squares;
  
  ofVec2f mouse_position;
  unsigned int reproduce_type;
  
  bool mouse_down;
  bool triangle_key_down;
  bool circle_key_down;
  bool square_key_down;
};
