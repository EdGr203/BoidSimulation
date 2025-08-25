#include "flock.hpp"

#include <array>
#include <cmath>
#include <iostream>

//------accelerations list-------

sf::Vector2f SepSpeed(Boid *boid1, const std::vector<Boid *> &boid_list) {
  float sep2 = boid1->GetRadiusSep() * boid1->GetRadiusSep();
  sf::Vector2f diff{0.f, 0.f};

  for (Boid *boid0 : boid_list) {
    if (boid0 != boid1 &&
        DistSqr(boid0->GetPosition(), boid1->GetPosition()) <= sep2) {
      float dnorm = Norm(boid0->GetPosition() - boid1->GetPosition());
      if (dnorm != 0)
        diff += (boid1->GetPosition() - boid0->GetPosition()) / dnorm;
    }
  }
  return (diff != sf::Vector2f{0.f, 0.f}) ? diff / Norm(diff)
                                          : sf::Vector2f{0.f, 0.f};
}

sf::Vector2f CohSpeed(Boid *boid, const std::vector<Boid *> &boid_list) {
  float coh2 = boid->GetRadiusCoh() * boid->GetRadiusCoh();
  sf::Vector2f sum_p{0.f, 0.f};
  int counter = 0;
  for (const Boid *boid0 : boid_list) {
    if (boid0 != boid &&
        DistSqr(boid0->GetPosition(), boid->GetPosition()) <= coh2) {
      sum_p += boid0->GetPosition();
      counter++;
    }
  }
  if (counter == 0) return {0.f, 0.f};

  sf::Vector2f vcoh =
      (sum_p / static_cast<float>(counter)) - boid->GetPosition();
  return (Norm(vcoh) != 0) ? vcoh / Norm(vcoh) - boid->GetVelocity()
                           : sf::Vector2f{0.f, 0.f};
}

sf::Vector2f AlnSpeed(Boid *boid1, const std::vector<Boid *> &boid_list) {
  float alg2 = boid1->GetRadiusAlg() * boid1->GetRadiusAlg();
  sf::Vector2f sum_v{0.f, 0.f};
  int counter = 0;
  for (const Boid *boid0 : boid_list) {
    if (boid0 != boid1 &&
        DistSqr(boid0->GetPosition(), boid1->GetPosition()) <= alg2) {
      sum_v += boid0->GetVelocity();
      counter++;
    }
  }
  if (counter == 0) return {0.f, 0.f};

  sf::Vector2f valg = (sum_v / static_cast<float>(counter));
  return (Norm(valg) != 0) ? valg / Norm(valg) - boid1->GetVelocity()
                           : sf::Vector2f{0.f, 0.f};
}