#include "circle.h"
#include "ex.h"
#include "gameobject.h"
#include "square.h"
#include "testApp.h"
#include "triangle.h"

const unsigned int testApp::kNumEnemies = 10;
const unsigned int testApp::kNumGameObjects = 10;

//--------------------------------------------------------------
void testApp::setup() {
  ofEnableSmoothing();
  reproduce_type = 0;
  for (unsigned int i = 0; i < kNumGameObjects; ++i) {
    CreateRandomShape(ofVec2f(ofRandomWidth(), ofRandomHeight()), true);
  }
  for (unsigned int i = 0; i < kNumEnemies; ++i) {
    CreateRandomShape(ofVec2f(ofRandomWidth(), ofRandomHeight()), false);
  }
}

//--------------------------------------------------------------
void testApp::update() {
  UpdateGroup(triangles, triangle_key_down);
  UpdateGroup(circles, circle_key_down);
  UpdateGroup(squares, square_key_down);
  
  UpdateGroup(enemy_triangles, false);
  UpdateGroup(enemy_circles, false);
  UpdateGroup(enemy_squares, false);
  
  CollideGroups(triangles, enemy_circles);
  CollideGroups(circles, enemy_squares);
  CollideGroups(squares, enemy_triangles);
  
  CollideGroups(enemy_triangles, circles);
  CollideGroups(enemy_circles, squares);
  CollideGroups(enemy_squares, triangles);
  
  RemoveDeadIndividuals(triangles);
  RemoveDeadIndividuals(circles);
  RemoveDeadIndividuals(squares);
  
  RemoveDeadIndividuals(enemy_triangles);
  RemoveDeadIndividuals(enemy_circles);
  RemoveDeadIndividuals(enemy_squares);
}

void testApp::UpdateGroup(std::list<GameObject *> &group, bool move) {
  if (move) {
    SteerGroup(group, mouse_position);
  }
  for (auto individual : group) {
    individual->Update(1.0 / ofGetFrameRate());
    if (individual->player) {
      individual->MaybeReproduce(triangles, circles, squares, reproduce_type);
    } else {
      unsigned int enemy_reproduce_type = 0;
      const float choice = ofRandomuf();
      if (choice < 1.0 / 3.0) {
        enemy_reproduce_type = 1;
      } else if (choice < 2.0 / 3.0) {
        enemy_reproduce_type = 2;
      }
      individual->MaybeReproduce(enemy_triangles, enemy_circles, enemy_squares, enemy_reproduce_type);
    }
  }
}

void testApp::CollideGroups(std::list<GameObject *> &winners, std::list<GameObject *> &losers) {
  std::for_each(winners.begin(), winners.end(), [losers] (GameObject *const winner) {
    std::for_each(losers.begin(), losers.end(), [winner] (GameObject *const loser) {
      const float colliding_distance = winner->size + loser->size;
      if (winner->position.distanceSquared(loser->position) < colliding_distance * colliding_distance) {
        winner->size = std::min(winner->size + loser->size, GameObject::kMaxSize);
        loser->size = 0;
      }
    });
  });
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
  DrawGroup(enemy_triangles);
  DrawGroup(enemy_circles);
  DrawGroup(enemy_squares);
  
  DrawGroup(triangles);
  DrawGroup(circles);
  DrawGroup(squares);
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

void testApp::CreateRandomShape(ofVec2f at, bool player) {
  constexpr float mass = 1.0;
  const float choice = ofRandomuf();
  const float size = ofRandom(5.0, 15.0);
  const float orientation = 2.0 * M_PI * ofRandomuf();
  const ofVec2f velocity = ofVec2f();
  if (choice < 1.0 / 3.0) {
    if (player) {
      triangles.push_back(new Triangle(player, mass, size, orientation, at, velocity));
    } else {
      enemy_triangles.push_back(new Triangle(player, mass, size, orientation, at, velocity));
    }
  } else if (choice < 2.0 / 3.0) {
    if (player) {
      squares.push_back(new Square(player, mass, size, orientation, at, velocity));
    } else {
      enemy_squares.push_back(new Square(player, mass, size, orientation, at, velocity));
    }
  } else {
    if (player) {
      circles.push_back(new Circle(player, mass, size, orientation, at, velocity));
    } else {
      enemy_circles.push_back(new Circle(player, mass, size, orientation, at, velocity));
    }
  }
}
