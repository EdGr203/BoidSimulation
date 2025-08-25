#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <SFML/Graphics.hpp>
#include <array>

#include "boid.hpp"

class Obstacle : public Boid {
 public:
  Obstacle(sf::Vector2f position, float size);
  ~Obstacle() override = default;

  //------Overridden functions-------
  void Update_Position() override;
  void SpeedChange(sf::Vector2f changedSpeed) override;

  //------Getters-------
  const sf::RectangleShape &GetShape() const;
  sf::FloatRect GetBounds() const;
  static bool GetEvasionState();

  //------Collisions functions-------
  bool CollisionResponse(Boid &boid);
  static void AlterEvasionState();
  sf::Vector2f RepelBoid(const Boid &boid, float obstacleSize) const;
  static void setCompleteEvasion(bool value);

 private:
  sf::RectangleShape shape;
  static bool completeEvasion;
  static float radius;
  static int sides;
};

#endif
