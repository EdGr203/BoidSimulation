#include "boid.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

//------mode variables---------
bool Boid::gradualDamage = false;

//------shape variables--------
float Boid::radius = 5.0f;  // Default value for radii
std::size_t Boid::sides = 3;        // Default triangular shape
float Boid::r1 = 20.0f;     // Default value for separation radius
float Boid::r2 = 50.0f;     // Default value for coehesion radius
float Boid::r3 = 150.0f;    // Default value for alignement radius

//------limit variables--------
float Boid::_maxSpeed = 3E-1f;

//------constrctors and destructors-------
Boid::Boid() : Velocity{0, 0}, Position{0, 0} {}
Boid::Boid(sf::Vector2f position, sf::Vector2f velocity)
    : Velocity{velocity}, Position{position} {
  assert(sides >= 3);  // default triangle check
  b_bird.setPointCount(sides);
  SetSize(radius);
  b_bird.setPosition(sf::Vector2f(Position.x, Position.y));
}

//------getters-------
sf::Vector2f Boid::GetPosition() const { return {Position.x, Position.y}; }
sf::Vector2f Boid::GetVelocity() const { return {Velocity.x, Velocity.y}; }
float Boid::GetRadius() const { return radius; }
float Boid::GetRadiusSep() const { return r1; }
float Boid::GetRadiusCoh() const { return r2; }
float Boid::GetRadiusAlg() const { return r3; }
const sf::ConvexShape& Boid::GetShape() const { return b_bird; }
int Boid::GetDamage() const { return damage; }
bool Boid::GetDamageType() const { return gradualDamage; }
bool Boid::GetHitStatus() const { return isHit; }
float Boid::GetTimer() const { return hitTimer; }

//------setters-------
void Boid::SetRadii(float baseSize, float factor1, float factor2,
                    float factor3) {
  assert(baseSize > 0 && factor1 > 0 && factor2 > 0 && factor3 > 0);
  radius = baseSize;
  r1 = baseSize * factor1;  // separation
  r2 = baseSize * factor2;  // cohesion
  r3 = baseSize * factor3;  // alignment
}
void Boid::SetSize(float rad) {
  for (std::size_t i = 0; i < sides; ++i) {
    float angle = static_cast<float>(i * 2 * 3.14159f / sides - 3.14159f / 2.0f);
    float x = rad * std::cos(angle);
    float y = rad * std::sin(angle);
    b_bird.setPoint(i, sf::Vector2f(x, y));
  }
}
void Boid::SetHitStatus(bool hit) { this->isHit = hit; }
void Boid::SetMaxSpeed(float maxspeed) { this->_maxSpeed = maxspeed; }
void Boid::SetTimer(float time) { this->hitTimer = time; }
void Boid::setGradualDamage(bool value) { gradualDamage = value; }

//------direct variables modifiers-------
void Boid::SpeedChange(sf::Vector2f changedSpeed) {
  // the function changes the velocity vector instantly
  if (Norm(changedSpeed) > _maxSpeed) {
    this->Velocity = (changedSpeed / Norm(changedSpeed)) * _maxSpeed;
  } else {
    this->Velocity = changedSpeed;
  }
}
void Boid::SetPosition(sf::Vector2f pos) { this->Position = pos; }
void Boid::ColorChange(sf::Color newColor) {
  // the function changes the boid's colour instantly
  b_bird.setFillColor(newColor);
}

//------evolution functions------
void Boid::Update_Position() {
  Position += Velocity;
  b_bird.setPosition(Position);

  if (Norm(Velocity) > 0.001f) {
    float angle = std::atan2(Velocity.y, Velocity.x) * 180.0f / 3.14159f;
    // adjustment of triangle pointing
    b_bird.setRotation(angle + 90.0f);
  }
}
void Boid::MarkHit() {
  // marks the begin of the "hit status" and orders the "damage colour change"
  if (isHit) {
    hitTimer = std::max(hitTimer, 0.f);  // exclude negative timer values
  } else {
    isHit = true;
    ApplyDamage(gradualDamage ? 1 : 4);
  }
}
bool Boid::UpdateHit(float deltaTime) {
  // tells whether the boid should be destroyed or not and in which situations
  if (!isHit) return false;

  hitTimer -= deltaTime;
  if (hitTimer > 0.f) {
    return false;  // still frozen
  }

  if (damage >= 4) {
    return true;  // should be destroyed
  }

  return false;  // default not destroyed
}
void Boid::ApplyDamage(int level) {
  // applies the colour change out of three possibilities
  if (hitColorChanged) return;
  damage += level;

  if (damage >= 3) {
    ColorChange(sf::Color::Red);
  } else if (damage == 2) {
    ColorChange(sf::Color(255, 165, 0));
  } else if (damage == 1) {
    ColorChange(sf::Color::Yellow);
  }
}

//------mathematical operators-------
float operator*(sf::Vector2f vel1, sf::Vector2f vel2) {
  // dot product
  return vel1.x * vel2.x + vel1.y * vel2.y;
}
float Norm(const sf::Vector2f& vec) {
  // vector norm
  assert(!std::isnan(vec.x) && !std::isnan(vec.y));
  float product = static_cast<float>(vec*vec); 
  return sqrt(product);
}
float DistSqr(const sf::Vector2f& vec1, const sf::Vector2f& vec2) {
  // vector difference squared
  return ((vec2 - vec1) * (vec2 - vec1));
}
