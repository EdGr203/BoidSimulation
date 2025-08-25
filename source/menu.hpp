#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <functional>
#include <string>

// ---- UI COMPONENTS ---

struct Slider {
  sf::RectangleShape track;
  sf::CircleShape knob;
  sf::Text label;
  float min, max, value;
  bool dragging = false;

  Slider(const std::string& labelText, float minVal, float maxVal,
         float defaultVal, const sf::Font& font, sf::Vector2f pos);
  void draw(sf::RenderWindow& target) const;
  void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
  float getValue() const;
};

struct CheckBox {
  sf::RectangleShape box;
  sf::Text label;
  bool checked = false;

  CheckBox(const std::string& labelText, const sf::Font& font,
           sf::Vector2f pos);
  void draw(sf::RenderWindow& target) const;
  void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
  bool isChecked();
};

struct Button {
  sf::RectangleShape shape;
  sf::Text text;
  bool clicked = false;

  Button(const std::string& label, const sf::Font& font, sf::Vector2f pos,
         sf::Vector2f size);
  Button(const std::string& label, const sf::Font& font, sf::Vector2f pos,
         sf::Vector2f size, sf::Color colour);
  void draw(sf::RenderTarget& target) const;
  void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
  bool isClicked();
  void unClicked();
};

struct Notification {
  sf::Text text;
  sf::Clock clock;
  float duration = 2.0f;  // default value for message timer
  bool active = false;

  void show(const std::string& message, const sf::Font& font, sf::Vector2f pos);
  void draw(sf::RenderWindow& window);
};

// ---- MENU ---

// main menu and sub-menus
enum class MenuType { Main, Radii, Modalities };

class Menu {
 public:
  Menu(sf::RenderWindow& window, sf::Font& font, MenuType type);

  void handleEvent(const sf::Event& event);
  void draw();

  bool startState();
  bool wasButtonClicked(const std::string& label);
  bool wasCheckBoxChecked(const std::string& label);
  float wasSliderMoved(const std::string& label);
  void setGameState(bool state);

 private:
  void generateLayout();

  sf::RenderWindow& window;
  sf::Font& font;
  MenuType type;

  //---- menu UI elements ----
  std::vector<Slider> sliders;
  std::vector<CheckBox> checkboxes;
  std::vector<Button> buttons;

  bool startSimulation = false;
};

#endif  // MENU_HPP
