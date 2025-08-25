#include "obstacle.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

bool Obstacle::completeEvasion =
    false;  // standard value for complete evasion mode

float Obstacle::radius{40.f};
int Obstacle::sides{4};

Obstacle::Obstacle(sf::Vector2f position, float size)
    : Boid(position, sf::Vector2f(0.f, 0.f)) {
  //------size defiintion and positivity check-----
  assert(size > 0.f && "Obstacle size must be positive");
  shape.setSize({size, size});
  shape.setFillColor(sf::Color::Blue);

  //------origin at center positioning and check----
  shape.setOrigin(size / 2.f, size / 2.f);
  assert(shape.getOrigin().x == size / 2.f &&
         shape.getOrigin().y == size / 2.f);
  shape.setPosition(position);
}
const sf::RectangleShape &Obstacle::GetShape() const { return shape; }
sf::FloatRect Obstacle::GetBounds() const { return shape.getGlobalBounds(); }
void Obstacle::Update_Position() {
  // do not update
}
void Obstacle::SpeedChange(sf::Vector2f changedSpeed) {
  // forced 0 speed
  (void)changedSpeed;
  this->Velocity = {0.f, 0.f};
}
bool Obstacle::GetEvasionState() { return completeEvasion; }

//--------Collision functions---------
void Obstacle::AlterEvasionState() { completeEvasion = !completeEvasion; }
bool Obstacle::CollisionResponse(Boid &boid) {
  if (completeEvasion) {
    return false;
  };  // no collision, just evasion

  sf::FloatRect bounds = this->GetBounds();
  sf::Vector2f pos = boid.GetPosition();
  assert(bounds.width > 0 && bounds.height > 0);

  float closestX = std::clamp(pos.x, bounds.left, bounds.left + bounds.width);
  float closestY = std::clamp(pos.y, bounds.top, bounds.top + bounds.height);
  sf::Vector2f closestPoint(closestX, closestY);

  sf::Vector2f diff = pos - closestPoint;
  float distance = Norm(diff);
  assert(!std::isnan(distance));

  if (distance < boid.GetRadius()) {
    if (boid.GetTimer() <= 0.f) {
      boid.SetHitStatus(false);
      float d2 = diff.x * diff.x + diff.y * diff.y;
      // initial non-infinite speed to go away from the obstacle
      float d = (std::sqrt(d2) + boid.GetRadius());
      assert(!std::isnan(d));
      boid.SpeedChange((diff / d) * (1.f / d));
      boid.SetTimer(1.f);
    } else {
      // timer begin, hit effects and standby state to show effect of collision
      boid.MarkHit();
      boid.SpeedChange({0.f, 0.f});
    }

    return true;
  }

  return false;
}

// this function will be called only at completeEvasion == true
sf::Vector2f Obstacle::RepelBoid(const Boid &boid, float obstacleSize) const {
  sf::Vector2f pos = boid.GetPosition();
  sf::FloatRect bounds = GetBounds();

  float closestX = std::clamp(pos.x, bounds.left, bounds.left + bounds.width);
  float closestY = std::clamp(pos.y, bounds.top, bounds.top + bounds.height);
  sf::Vector2f closestPoint(closestX, closestY);

  sf::Vector2f diff = pos - closestPoint;
  float d2 = diff.x * diff.x + diff.y * diff.y;

  float safety = boid.GetRadius() + obstacleSize;
  float safety2 = safety * safety;

  if (d2 < safety2 && d2 > 0.f) {
    // simple normalized repulsion force
    float d = std::sqrt(d2);
    return (diff / d);
  }
  return {0.f, 0.f};
}

void Obstacle::setCompleteEvasion(bool value) { completeEvasion = value; }