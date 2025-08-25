#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "evolution.hpp"
#include "quadtree.hpp"

static constexpr float EPS = 1e-4f;

TEST_CASE("Boid Position and Velocity Initialization") {
  sf::Vector2f position(100.f, 150.f);
  sf::Vector2f velocity(0.01f, 0.02f);
  Boid boid(position, velocity);

  CHECK(boid.GetPosition() == position);
  CHECK(boid.GetVelocity() == velocity);
}

TEST_CASE("SpeedChange respects max speed limit") {
  Boid::SetRadii(10.f, 3.f, 5.f, 8.f);  // also sets radius
  Boid boid({0.f, 0.f}, {0.0f, 0.0f});
  boid.SetMaxSpeed(0.03f);

  sf::Vector2f fastVec = {1.f, 1.f};  // Clearly faster than _maxSpeed
  boid.SpeedChange(fastVec);

  float speed = Norm(boid.GetVelocity());
  CHECK(speed <= 0.03f);
}

TEST_CASE("Norm and Dot Product utilities work correctly") {
  sf::Vector2f a = {3.f, 4.f};
  sf::Vector2f b = {1.f, 2.f};

  CHECK(Norm(a) == doctest::Approx(5.f));
  CHECK((a * b) == doctest::Approx(11.f));
}

TEST_CASE("Boid SetRadii applies correct radii") {
  Boid::SetRadii(10.f, 2.f, 4.f, 6.f);

  Boid boid({0.f, 0.f}, {0.f, 0.f});
  CHECK(boid.GetRadius() == doctest::Approx(10.f));
  CHECK(boid.GetRadiusSep() == doctest::Approx(20.f));
  CHECK(boid.GetRadiusCoh() == doctest::Approx(40.f));
  CHECK(boid.GetRadiusAlg() == doctest::Approx(60.f));
}

TEST_CASE("Boid Damage and Hit Logic") {
  Boid boid({0.f, 0.f}, {0.01f, 0.01f});
}  // and ??? how do i check this???

TEST_CASE("Separation vector behaves correctly") {
  Boid center({0.f, 0.f}, {0.f, 0.f});
  Boid close1({5.f, 0.f}, {0.f, 0.f});
  Boid close2({-5.f, 0.f}, {0.f, 0.f});
  Boid far({100.f, 100.f}, {0.f, 0.f});

  std::vector<Boid *> boids = {&center, &close1, &close2, &far};
  Boid::SetRadii(10.f, 2.f, 3.f,
                 4.f);  // make sure sep radius covers close1 and close2

  auto sep = SepSpeed(&center, boids);
  CHECK(sep.x == doctest::Approx(0.0f));
  CHECK(sep.y == doctest::Approx(0.0f));
}

TEST_CASE("Obstacle bounds and initial state") {
  Obstacle o({50.f, 50.f}, 20.f);
  auto b = o.GetBounds();
  CHECK(b.width == doctest::Approx(20.f));
  CHECK(b.height == doctest::Approx(20.f));
  // center should be at position {50,50}
  CHECK(o.GetShape().getPosition().x == doctest::Approx(50.f));
  CHECK(o.GetShape().getPosition().y == doctest::Approx(50.f));
}

TEST_CASE("CollisionResponse detects overlap") {
  Boid b({100.f, 100.f}, {0.f, 0.f});
  Obstacle o({100.f, 100.f}, 30.f);  // perfectly overlapping center
  CHECK(o.CollisionResponse(b) == true);
  CHECK(b.GetHitStatus() == true);
}

TEST_CASE("CollisionResponse respects evasion mode") {
  Boid b({150.f, 150.f}, {0.f, 0.f});
  Obstacle::AlterEvasionState();  // turn evasion ON
  Obstacle o({150.f, 150.f}, 30.f);
  CHECK(o.CollisionResponse(b) == false);
  // reset for other tests
  Obstacle::AlterEvasionState();
}

TEST_CASE("CollisionResponse no-collision when far") {
  Boid b({0.f, 0.f}, {0.f, 0.f});
  Obstacle o({100.f, 100.f}, 20.f);
  CHECK(o.CollisionResponse(b) == false);
}

TEST_CASE("CollisionResponse algorithm integrity") {
  // place boid exactly at edge: should still collide if within radius
  float size = 20.f;
  Obstacle o({200.f, 200.f}, size);
  Boid b({200.f + size / 2.f + o.GetRadius() - 0.1f, 200.f}, {0.f, 0.f});
  // distance = (size/2 + radius - 0.1) -> just under radius
  CHECK(o.CollisionResponse(b) == true);
}

TEST_CASE("Insert and query basic") {
  Quadtree qt(0, 0, 100, 100, 2);
  Boid b1({10, 10}, {0, 0}), b2({20, 20}, {0, 0});
  CHECK(qt.insert(&b1));
  CHECK(qt.insert(&b2));

  std::vector<Boid *> found;
  qt.query({0, 0, 15, 15}, found);
  CHECK(found.size() == 1);
  CHECK(found[0] == &b1);
}

TEST_CASE("Obstacle construction with positive size") {
  Obstacle o({50, 50}, 20.f);
  CHECK(o.GetBounds().width == doctest::Approx(20.f));
  CHECK(o.GetBounds().height == doctest::Approx(20.f));
}

TEST_CASE("Obstacle origin is centered") {
  Obstacle o({0, 0}, 10.f);
  auto origin = o.GetShape().getOrigin();
  CHECK(origin.x == doctest::Approx(5.f));
  CHECK(origin.y == doctest::Approx(5.f));
}

TEST_CASE("CollisionResponse returns false when evasion ON") {
  Obstacle::AlterEvasionState();  // turn ON
  Boid b({5, 5}, {0, 0});
  Obstacle o({0, 0}, 10.f);
  CHECK(o.CollisionResponse(b) == false);
  Obstacle::AlterEvasionState();  // reset
}

TEST_CASE("CollisionResponse detects overlap") {
  Boid b({2, 2}, {0, 0});
  Obstacle o({0, 0}, 5.f);
  CHECK(o.CollisionResponse(b) == true);
}

TEST_CASE("CollisionResponse misses when outside") {
  Boid b({100, 100}, {0, 0});
  Obstacle o({0, 0}, 5.f);
  CHECK(o.CollisionResponse(b) == false);
}

TEST_CASE("Alignment: boid aligns with average neighbor velocity") {
  // Boid at rest
  Boid boid({0.f, 0.f}, {0.f, 0.f});
  // Two neighbors moving along +X and +Y
  Boid n1({0.f, 0.f}, {2.f, 0.f});
  Boid n2({0.f, 0.f}, {0.f, 4.f});
  std::vector<Boid *> neighbors = {&n1, &n2};

  auto result = AlnSpeed(&boid, neighbors);
  // average velocity = (1,2) → normalized = (1/√5, 2/√5)
  float len = std::sqrt(5.f);
  CHECK(result.x == doctest::Approx(1.f / len).epsilon(EPS));
  CHECK(result.y == doctest::Approx(2.f / len).epsilon(EPS));
}

TEST_CASE("Separation: boid repels from a too-close neighbor") {
  // Boid at origin, neighbor to its right
  Boid boid({0.f, 0.f}, {0.f, 0.f});
  Boid n1({1.f, 0.f}, {0.f, 0.f});
  std::vector<Boid *> neighbors = {&n1};

  auto sep = SepSpeed(&boid, neighbors);
  // vector = boid.pos - neighbor.pos = (-1,0) → normalized = (-1,0)
  CHECK(sep.x == doctest::Approx(-1.f).epsilon(EPS));
  CHECK(sep.y == doctest::Approx(0.f).epsilon(EPS));
}

TEST_CASE("Obstacle::RepelBoid produces correct repulsion strength") {
  // Create a 2×2 obstacle centered at (1,0)
  Obstacle obs({1.f, 0.f}, 2.f);
  // A boid just outside its right edge: pos = (2.5, 0)
  Boid boid({2.5f, 0.f}, {0.f, 0.f});
  float obsSize = obs.GetBounds().width;  // = 2

  auto repel = obs.RepelBoid(boid, obsSize);
  CHECK(repel.x == doctest::Approx(1.f).epsilon(EPS));
  CHECK(repel.y == doctest::Approx(0.f).epsilon(EPS));
}
