#ifndef EVOLUTION_HPP
#define EVOLUTION_HPP

#include <iostream>
#include <random>
#include <vector>

#include "flock.hpp"
#include "obstacle.hpp"

// these are default values for the weights or multiplying factors for each
// force
struct BehaviorWeights {
  float alignment = 1e-4f;
  float separation = 3e-4f;
  float cohesion = 1e-4f;
  float evasion = 5e-1f;
  // last one refers to the separation force from the obstacles
};

// this function manages the majority of the boid interactions
void Evolution(Boid &boid, const std::vector<Boid *> &neighbors,
               std::vector<Obstacle *> &obstacles, float &maxX, float &maxY,
               float &Radius, const BehaviorWeights &weights,
               bool mouseFollowMode = false,
               sf::Vector2f mousePos = {0.f, 0.f});

#endif
