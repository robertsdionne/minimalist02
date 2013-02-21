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
  UpdateGroup(circles, mean_overlap < 0.5 || circle_key_down);
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
  float total_overlap = 0;
  std::for_each(group.begin(), group.end(), [&total_overlap, group] (GameObject *const individual0) {
    std::list<GameObject *> overlapping;
    std::for_each(group.begin(), group.end(), [&total_overlap, individual0, &overlapping] (GameObject *const individual1) {
      if (individual0 != individual1) {
        const ofVec2f r = individual1->position - individual0->position;
        const float actual_distance = r.length();
        const float colliding_distance = individual0->size + individual1->size;
        if (actual_distance < colliding_distance) {
          overlapping.push_back(individual1);
          const float overlap = colliding_distance - actual_distance;
          total_overlap += overlap / 2;
          individual0->force -= 5.0 * r.normalized() * pow(overlap, 1);
          individual1->force += 5.0 * r.normalized() * pow(overlap, 1);
        }
      }
    });
    const float size_diffusion_amount = 0.01 * overlapping.size();
    individual0->neighbors = overlapping.size();
    std::for_each(overlapping.begin(), overlapping.end(), [individual0, size_diffusion_amount] (GameObject *const individual1) {
      if (ofRandomuf() < 0.1 && individual0->size > 2 && individual1->size < 20) {
        individual0->size -= size_diffusion_amount;
        individual1->size += size_diffusion_amount;
      }
    });
  });
  mean_overlap = total_overlap / group.size();
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
  std::stringstream overlap;
  overlap << mean_overlap;
  ofDrawBitmapString(overlap.str(), 10, 10);
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
    case ' ':
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
    case ' ':
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
  const float size = 10;
  const float orientation = 2.0 * M_PI * ofRandomuf();
  const ofVec2f velocity = ofVec2f();
  circles.push_back(new Circle(true, mass, size, orientation, at, velocity));
}
