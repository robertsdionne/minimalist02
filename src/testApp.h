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
  void Collide(std::list<GameObject *> &group, float &mean_group_overlap, float &mean_group_food);
  ofVec2f FindCenterOfMass(std::list<GameObject *> &group);
  void CreateShape(std::list<GameObject *> &group, bool player, ofVec2f at);
  void DrawGroup(std::list<GameObject *> &group);
  void RemoveDeadIndividuals(std::list<GameObject *> &group);
  void SteerGroup(std::list<GameObject *> &group, ofVec2f target);
  void UpdateGroup(std::list<GameObject *> &group, float &mean_group_overlap, float &mean_group_food, ofVec2f target, bool move, bool player);
  void Launch(std::list<GameObject *> &group);
  void Wrap(ofVec2f &position);
  
private:
  static const unsigned int kNumGameObjects = 50;
  
  std::list<GameObject *> circles;
  std::list<GameObject *> enemy_circles;
  
  float enemy_target_angle;
  ofVec2f mouse_position;
  ofVec2f enemy_target;
  ofVec2f enemy_center_of_mass;
  unsigned int reproduce_type;
  
  float mean_overlap;
  float mean_enemy_overlap;
  float mean_food;
  float mean_enemy_food;
  
  bool mouse_down;
  bool old_circle_key_down;
  bool circle_key_down;
  bool shift_key_down;
  bool square_key_down;
};
