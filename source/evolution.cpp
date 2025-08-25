#include "evolution.hpp"

void Evolution(Boid &boid, const std::vector<Boid *> &neighbors,
               std::vector<Obstacle *> &obstacles, float &maxX, float &maxY,
               float &Radius, const BehaviorWeights &weights,
               bool mouseFollowMode, sf::Vector2f mousePos) {
  // standard accelerations values
  sf::Vector2f alignment = AlnSpeed(&boid, neighbors);
  sf::Vector2f separation = SepSpeed(&boid, neighbors);
  sf::Vector2f cohesion = CohSpeed(&boid, neighbors);
  sf::Vector2f steeringForce = weights.separation * separation +
                               weights.alignment * alignment +
                               weights.cohesion * cohesion;

  // keep boid still until the collision has had effect
  if (boid.GetHitStatus()) {
    steeringForce = {0.f, 0.f};
  }

  // additional force at complete evasion activated
  if (Obstacle::GetEvasionState()) {
    for (const auto *obs : obstacles) {
      steeringForce +=
          weights.evasion * obs->RepelBoid(boid, 20.f);  // tweak size as needed
    }
  }

  // additional force at arrow following activated
  if (mouseFollowMode) {
    sf::Vector2f toMouse = mousePos - boid.GetPosition();
    float dist = Norm(toMouse);
    assert(dist >= 0.f);
    if (dist > 0.01f) {
      steeringForce += (toMouse / dist) * 0.1f;
    }
    if (boid.GetHitStatus()) {
    steeringForce = {0.f, 0.f};
  }
  }

  // position & velocity after update
  sf::Vector2f pos = boid.GetPosition();
  sf::Vector2f vel = boid.GetVelocity();

  // apply complete steering
  boid.SpeedChange(boid.GetVelocity() + steeringForce);
  boid.Update_Position();

  assert(!std::isnan(pos.x) && !std::isnan(pos.y));
  assert(!std::isnan(vel.x) && !std::isnan(vel.y));

  //  Pac-Man wrapping effect at borders
  if (pos.x > maxX + Radius) {
    boid.SetPosition({-Radius, pos.y});
  } else if (pos.x < -Radius) {
    boid.SetPosition({maxX + Radius, pos.y});
  }

  if (pos.y > maxY + Radius) {
    boid.SetPosition({pos.x, -Radius});
  } else if (pos.y < -Radius) {
    boid.SetPosition({pos.x, maxY + Radius});
  }
}
