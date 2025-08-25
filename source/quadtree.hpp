#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "boid.hpp"

class Quadtree {
 public:
  //-----Quadtree general variables----
  sf::FloatRect boundary;
  int capacity;
  std::vector<Boid *> points;
  bool divided = false;

  //-----Four Section Pointers-----

  std::unique_ptr<Quadtree> northeast = nullptr;
  std::unique_ptr<Quadtree> northwest = nullptr;
  std::unique_ptr<Quadtree> southeast = nullptr;
  std::unique_ptr<Quadtree> southwest = nullptr;

  //--------------------------------

  Quadtree(float x, float y, float width, float height, int cap);

  //-----Section functions-------

  void subdivide();
  bool insert(Boid *boid);
  void query(const sf::FloatRect &range, std::vector<Boid *> &found);
  void clear();
  void draw(sf::RenderWindow &window) const;
};

#endif
