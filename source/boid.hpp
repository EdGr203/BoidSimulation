#ifndef BOID_HPP
#define BOID_HPP

#include <vector>

#include "SFML/Graphics.hpp"

class Boid {
 public:
  //------constrctors and destructors-------
  Boid();
  Boid(sf::Vector2f position, sf::Vector2f velocity);
  virtual ~Boid() = default;
  //------getters-------
  sf::Vector2f GetPosition() const;
  sf::Vector2f GetVelocity() const;
  const sf::ConvexShape &GetShape() const;
  float GetRadius() const;
  float GetRadiusCoh() const;
  float GetRadiusSep() const;
  float GetRadiusAlg() const;
  float GetTimer() const;
  bool GetHitStatus() const;
  bool GetDamageType() const;
  int GetDamage() const;
  //------setters-------
  static void SetRadii(float baseSize, float factor1, float factor2,
                       float factor3);
  void SetSize(float average);
  void SetMaxSpeed(float maxspeed);
  void SetTimer(float time);
  void SetHitStatus(bool hit);
  static void setGradualDamage(bool value);
  //------direct variables modifiers-----
  virtual void SpeedChange(sf::Vector2f changedSpeed);
  void ColorChange(sf::Color newColor);
  void SetPosition(sf::Vector2f pos);
  //------evolution functions------
  virtual void Update_Position();
  void MarkHit();
  bool UpdateHit(float deltaTime);
  void ApplyDamage(int level);

 protected:
  sf::Vector2f Velocity;
  sf::Vector2f Position;

 private:
  //------limit variables--------
  static float _maxSpeed;
  //------shape variables--------
  sf::ConvexShape b_bird;
  static float radius;
  static std::size_t sides;
  static float r1;
  static float r2;
  static float r3;
  //------impact variables-------
  int damage = 0;
  float hitTimer = 1.f;
  bool isHit = false;
  bool hitColorChanged = false;
  //------mode variables---------
  static bool gradualDamage;
};

//------mathematical operators-------

float operator*(sf::Vector2f vel1, sf::Vector2f vel2);
float Norm(const sf::Vector2f &vec);
float DistSqr(const sf::Vector2f &vec1, const sf::Vector2f &vec2);
#endif