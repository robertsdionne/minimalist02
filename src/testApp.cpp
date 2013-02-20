#include "circle.h"
#include "ex.h"
#include "gameobject.h"
#include "square.h"
#include "testApp.h"
#include "triangle.h"

constexpr unsigned int testApp::kNumGameObjects;

//--------------------------------------------------------------
void testApp::setup() {
  ofEnableSmoothing();
  reproduce_type = 0;
  for (unsigned int i = 0; i < kNumGameObjects; ++i) {
    CreateShape(ofVec2f(ofRandomWidth(), ofRandomHeight()));
  }
}

//--------------------------------------------------------------
void testApp::update() {
  UpdateGroup(circles, circle_key_down);
  RemoveDeadIndividuals(circles);
}

void testApp::UpdateGroup(std::list<GameObject *> &group, bool move) {
  if (move) {
    SteerGroup(group, mouse_position);
  }
  Collide(circles);
  for (auto individual : group) {
    individual->Update(1.0 / ofGetFrameRate());
  }
}

void testApp::Collide(std::list<GameObject *> &group) {
  std::list<GameObject *>::iterator i, j;
  for (i = group.begin(); i != group.end(); ++i) {
    j = i;
    GameObject *individual0 = *i;
    for (std::advance(j, 1); j != group.end(); ++j) {
      GameObject *individual1 = *j;
      const ofVec2f r = individual1->position - individual0->position;
      const float actual_distance = r.length();
      const float colliding_distance = individual0->size + individual1->size;
      if (actual_distance < colliding_distance) {
        individual0->force -= 10.0 * r.normalized() * pow(colliding_distance - actual_distance, 1);
        individual1->force += 10.0 * r.normalized() * pow(colliding_distance - actual_distance, 1);
      }
    }
  }
}

void testApp::RemoveDeadIndividuals(std::list<GameObject *> &group) {
  group.remove_if([] (const GameObject *const individual) -> bool {
    if (individual->size <= 0) {
      delete individual;
      return true;
    } else {
      return false;
    }
  });
}

//--------------------------------------------------------------
void testApp::draw() {
  ofBackground(0.0, 0.0, 0.0);
  DrawGroup(circles);
}

void testApp::DrawGroup(std::list<GameObject *> &group) {
  for (auto individual : group) {
    individual->Draw();
  }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
  switch (key) {
    case 'w':
      triangle_key_down = true;
      reproduce_type = 0;
      break;
    case 'd':
      circle_key_down = true;
      reproduce_type = 1;
      break;
    case 'a':
      square_key_down = true;
      reproduce_type = 2;
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
  switch (key) {
    case 'w':
      triangle_key_down = false;
      break;
    case 'd':
      circle_key_down = false;
      break;
    case 'a':
      square_key_down = false;
      break;
    default:
      break;
  }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
  mouse_position = ofVec2f(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
  
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
  mouse_down = true;
}

void testApp::SteerGroup(std::list<GameObject *> &group, ofVec2f target) {
  std::for_each(group.begin(), group.end(), [target] (GameObject *const individual) {
    if (individual->player) {
      ofVec2f desired_velocity = target - individual->position;
      if (desired_velocity.length() < 1000.0) {
        desired_velocity.scale(1000.0);
      }
      individual->force += desired_velocity - individual->velocity;
    }
  });
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
  mouse_down = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int width, int height) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) { 

}

void testApp::CreateShape(ofVec2f at) {
  constexpr float mass = 1.0;
  const float size = ofRandom(5.0, 15.0);
  const float orientation = 2.0 * M_PI * ofRandomuf();
  const ofVec2f velocity = ofVec2f();
  circles.push_back(new Circle(true, mass, size, orientation, at, velocity));
}
