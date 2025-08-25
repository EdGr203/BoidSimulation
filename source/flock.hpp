#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <array>

#include "boid.hpp"

//---- global accelleration constants list------
extern std::array<float, 3> constant_list;

//---- accelerations list------
sf::Vector2f SepSpeed(Boid *boid1, const std::vector<Boid *> &boid_list);
sf::Vector2f AlnSpeed(Boid *boid2, const std::vector<Boid *> &boid_list);
sf::Vector2f CohSpeed(Boid *boid1, const std::vector<Boid *> &boid_list);

#endif