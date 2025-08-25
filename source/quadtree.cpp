#include "quadtree.hpp"

Quadtree::Quadtree(float x, float y, float width, float height, int cap)
    : boundary(x, y, width, height), capacity(cap) {
  assert(cap > 0 && "Quadtree capacity must be positive");
}

void Quadtree::subdivide() {
  float x = boundary.left;
  float y = boundary.top;
  float w = boundary.width / 2;
  float h = boundary.height / 2;

  northeast = std::make_unique<Quadtree>(x + w, y, w, h, capacity);
  northwest = std::make_unique<Quadtree>(x, y, w, h, capacity);
  southeast = std::make_unique<Quadtree>(x + w, y + h, w, h, capacity);
  southwest = std::make_unique<Quadtree>(x, y + h, w, h, capacity);

  divided = true;
}

bool Quadtree::insert(Boid *boid) {
  assert(boid != nullptr);

  if (!boundary.contains(boid->GetPosition())) return false;

  if (points.size() < static_cast<std::size_t>(capacity)) {
    points.push_back(boid);
    return true;
  }

  if (!divided) subdivide();

  return (northeast->insert(boid) || northwest->insert(boid) ||
          southeast->insert(boid) || southwest->insert(boid));
}

void Quadtree::query(const sf::FloatRect &range, std::vector<Boid *> &found) {
  assert(range.width >= 0 && range.height >= 0);

  if (!boundary.intersects(range)) return;

  for (Boid *b : points) {
    if (range.contains(b->GetPosition())) found.push_back(b);
  }

  if (divided) {
    northeast->query(range, found);
    northwest->query(range, found);
    southeast->query(range, found);
    southwest->query(range, found);
  }
}

void Quadtree::clear() {
  points.clear();

  if (divided) {
    northeast->clear();
    northwest->clear();
    southeast->clear();
    southwest->clear();

    northeast.reset();
    northwest.reset();
    southeast.reset();
    southwest.reset();
    divided = false;
  }
}

void Quadtree::draw(sf::RenderWindow &window) const {
  sf::RectangleShape rectShape;
  rectShape.setPosition(sf::Vector2f(boundary.left, boundary.top));
  rectShape.setSize({boundary.width, boundary.height});
  rectShape.setFillColor(sf::Color::Transparent);
  rectShape.setOutlineThickness(1.f);
  rectShape.setOutlineColor(sf::Color::Green);
  window.draw(rectShape);

  if (divided) {
    northeast->draw(window);
    northwest->draw(window);
    southeast->draw(window);
    southwest->draw(window);
  }
}
