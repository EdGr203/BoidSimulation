#include <iostream>

#include "evolution.hpp"
#include "menu.hpp"
#include "quadtree.hpp"

int main() {
  // --- window  ---
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Boids Simulation");
  window.setFramerateLimit(120);
  sf::Vector2u windowlimits = window.getSize();
  float minX = 0.0f;
  float maxX = windowlimits.x;
  float minY = 0.0f;
  float maxY = windowlimits.y;

  // --- various menus creation ---
  sf::Font font;
  if (!font.loadFromFile("OpenSans-Regular.ttf")) {
    std::cerr << "Failed to load font.\n";
    return 1;
  }
  Menu mainMenu(window, font, MenuType::Main);
  Menu weightsMenu(window, font, MenuType::Radii);
  Menu modalitiesMenu(window, font, MenuType::Modalities);
  Menu *activeMenu = &mainMenu;

  // --- default boid values ---
  float Radius = 5.f;
  float defaultRad1 = 5.f;   // this is the standard multiplier
  float defaultRad2 = 10.f;  // this is the standard multiplier
  float defaultRad3 = 30.f;  // this is the standard multiplier
  BehaviorWeights weights;
  weights.separation = 0.4f;
  weights.cohesion = 0.1f;
  weights.alignment = 0.1f;
  int spawnedBoids{2};  // random default for initially spawned boids

  // --- imposed limits (no criteria other than calculation time was considered)
  // ---
  const int maxObstacles{10};
  const int maxBoids{300};

  // --- definition and standard setting for the arrow following mode ---
  bool mouseFollowMode = false;

  // --- random generators: device, engines and distribiutions ---
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> present_dist(20, 300);
  std::uniform_real_distribution<float> positionX_dist(minX + Radius,
                                                       maxX - Radius);
  std::uniform_real_distribution<float> positionY_dist(minY + Radius,
                                                       maxY - Radius);
  std::uniform_real_distribution<float> speedX_dist(-2, 2);
  std::uniform_real_distribution<float> speedY_dist(-2, 2);

  // --- main game loop ---
  while (window.isOpen()) {
    // --- menus loop ---
    while (window.isOpen() && !activeMenu->startState()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        activeMenu->handleEvent(event);
      }
      // --- main menu UI ---
      if (activeMenu == &mainMenu) {
        spawnedBoids =
            static_cast<int>(activeMenu->wasSliderMoved("Spawned Boids"));

        if (activeMenu->wasButtonClicked("Boids Radii")) {
          activeMenu = &weightsMenu;
        } else if (activeMenu->wasButtonClicked("Modes")) {
          activeMenu = &modalitiesMenu;
        } else if (activeMenu->wasButtonClicked("Start")) {
          activeMenu->setGameState(true);
        }
      }
      // --- radii menu UI ---
      else if (activeMenu == &weightsMenu) {
        //  can make radii shorter or bigger by multiplication factors
        float factor0 = activeMenu->wasSliderMoved("Boid Size");
        float factor1 = activeMenu->wasSliderMoved("Separation Radius");
        float factor2 = activeMenu->wasSliderMoved("Cohesion Radius");
        float factor3 = activeMenu->wasSliderMoved("Alignment Radius");

        Boid::SetRadii(Radius * factor0, defaultRad1 * factor1,
                       defaultRad2 * factor2, defaultRad3 * factor3);
      }
      // --- modalities menu UI ---
      else if (activeMenu == &modalitiesMenu) {
        Boid::setGradualDamage(
            activeMenu->wasCheckBoxChecked("Gradual Damage"));
        Obstacle::setCompleteEvasion(
            activeMenu->wasCheckBoxChecked("Complete Evasion"));
        mouseFollowMode = activeMenu->wasCheckBoxChecked("Arrow Following");
      }

      if (activeMenu->wasButtonClicked("Back")) {
        activeMenu = &mainMenu;
      }

      window.clear();
      activeMenu->draw();
      window.display();
    }

    Notification notification;  // for error messages or in-game warnings
    std::vector<Boid> boids;
    std::vector<Obstacle> obstacles;
    bool obstacleMode = false;  // for obstacles generation

    // initial spawned boids vector filling
    for (int i{1}; i <= spawnedBoids; i++) {
      sf::Vector2f position{positionX_dist(e1), positionY_dist(e1)};
      sf::Vector2f velocity{speedX_dist(e1), speedY_dist(e1)};
      Boid new_Boid(position, velocity);
      boids.push_back(std::move(new_Boid));
    }

    // first standard tree creation
    Quadtree tree(0.f, 0.f, static_cast<float>(windowlimits.x),
                  static_cast<float>(windowlimits.y), 4);

    // clock for collisions timers
    sf::Clock deltaClock;

    // --- boid simulation loop / game loop ---
    while (window.isOpen() && activeMenu->startState()) {
      sf::Event event;
      float dt = deltaClock.restart().asSeconds();

      // --- events list ---
      while (window.pollEvent(event)) {
        switch (event.type) {
          case sf::Event::Closed:
            window.close();
            break;

          case sf::Event::MouseButtonPressed:
            // --- boid and obstacle creation from mouse up to fixed limits
            if (event.mouseButton.button == sf::Mouse::Left) {
              sf::Vector2f position(static_cast<float>(event.mouseButton.x),
                                    static_cast<float>(event.mouseButton.y));

              if (obstacleMode) {
                if (obstacles.size() < maxObstacles) {
                  float obstacleSide = 40.f;
                  obstacles.emplace_back(position, obstacleSide);
                } else {
                  notification.show("Max obstacles reached!", font,
                                    {20.f, 20.f});
                }
              } else {
                if (boids.size() < maxBoids) {
                  sf::Vector2f velocity(speedX_dist(e1), speedY_dist(e1));
                  boids.emplace_back(position, velocity);
                } else {
                  notification.show("Max boids reached!", font, {20.f, 50.f});
                }
              }
            }

            break;
          case sf::Event::KeyPressed:
            // --- shortcuts for modes changing + back to menu key ---
            if (event.key.code == sf::Keyboard::F) {
              mouseFollowMode = !mouseFollowMode;
              if (mouseFollowMode) {
                notification.show("Arrow Following Enabled", font,
                                  {20.f, 20.f});
              } else {
                notification.show("Arrow Following Disabled", font,
                                  {20.f, 20.f});
              }
            }
            if (event.key.code == sf::Keyboard::O) {
              obstacleMode = !obstacleMode;
              if (obstacleMode) {
                notification.show("Obstacle Creation Enabled", font,
                                  {20.f, 20.f});
              } else {
                notification.show("Obstacle Creation Disabled", font,
                                  {20.f, 20.f});
              }
            }
            if (event.key.code == sf::Keyboard::E) {
              Obstacle::AlterEvasionState();
              if (Obstacle::GetEvasionState()) {
                notification.show("Obstacle Evasion Enabled", font,
                                  {20.f, 20.f});
              } else {
                notification.show("Obstacle Evasion Disabled", font,
                                  {20.f, 20.f});
              }
            }
            if (event.key.code == sf::Keyboard::A) {
              std::cout << "set false " << std::endl;
              activeMenu->setGameState(false);
              mainMenu.setGameState(false);
              weightsMenu.setGameState(false);
              modalitiesMenu.setGameState(false);
              activeMenu = &mainMenu;
            }
            break;

          default:
            break;
        }
      }

      window.clear();
      tree.clear();

      notification.draw(window);  // this can be positioned also elsewhere, it
                                  // suffices after event list

      // insertion of boids into quadtree
      for (Boid &boid : boids) {
        tree.insert(&boid);
      }

      // --- mouse following data ---
      sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
      sf::Vector2f mousePos(static_cast<float>(mousePixel.x),
                            static_cast<float>(mousePixel.y));

      // --- obstacles loop ---
      std::vector<Obstacle *> obstacle_ptrs;
      obstacle_ptrs.reserve(obstacles.size());
      for (Obstacle &obs : obstacles) {
        obstacle_ptrs.push_back(&obs);
        window.draw(obs.GetShape());
      }

      // --- boids main loop ---
      for (Boid &boid : boids) {
        float maxRadius = std::max(
            {boid.GetRadiusSep(), boid.GetRadiusCoh(), boid.GetRadiusAlg()});
        sf::Vector2f pos = boid.GetPosition();
        sf::FloatRect queryRange(pos.x - maxRadius, pos.y - maxRadius,
                                 2 * maxRadius, 2 * maxRadius);

        std::vector<Boid *> neighbors;
        tree.query(queryRange,
                   neighbors);  // restriction to closer boids through quadtree

        Evolution(boid, neighbors, obstacle_ptrs, maxX, maxY, Radius, weights,
                  mouseFollowMode, mousePos);

        window.draw(boid.GetShape());
      }

      // ------ collision loops -------
      for (auto boidIt = boids.begin(); boidIt != boids.end();) {
        bool collided = false;

        for (Obstacle &obstacle : obstacles) {
          if (obstacle.CollisionResponse(*boidIt)) {
            collided = true;
            break;
          }
        }

        if (collided) {
          bool shouldDestroy = boidIt->UpdateHit(dt);

          if (shouldDestroy) {
            boidIt = boids.erase(boidIt);
            continue;
          }
        }

        ++boidIt;
      }

      window.display();
    }
  }
}