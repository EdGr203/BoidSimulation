#include "menu.hpp"  // assuming this is where the struct is declared

#include <iostream>

//-------Slider-------

Slider::Slider(const std::string& labelText, float minVal, float maxVal,
               float defaultVal, const sf::Font& font, sf::Vector2f pos)
    : min(minVal), max(maxVal), value(defaultVal) {
  // track
  track.setSize(sf::Vector2f(300.f, 4.f));
  track.setFillColor(sf::Color(150, 150, 150));
  track.setPosition(pos);

  // knob
  knob.setRadius(8.f);
  knob.setFillColor(sf::Color(100, 100, 255));
  knob.setOrigin(knob.getRadius(), knob.getRadius());

  // label
  label.setFont(font);
  label.setCharacterSize(14);
  label.setFillColor(sf::Color::White);
  label.setString(labelText);
  sf::FloatRect bounds = label.getLocalBounds();
  label.setOrigin(bounds.left + bounds.width / 2.f,
                  bounds.top + bounds.height / 2.f);
  label.setPosition(pos.x + track.getSize().x / 2.f, pos.y - 24.f);

  // set knob position
  float x = pos.x + ((value - min) / (max - min)) * track.getSize().x;
  float y = pos.y + track.getSize().y / 2.f;
  knob.setPosition(x, y);
}

void Slider::draw(sf::RenderWindow& target) const {
  target.draw(label);
  target.draw(track);
  target.draw(knob);
}

void Slider::handleEvent(const sf::Event& event,
                         const sf::RenderWindow& window) {
  sf::Vector2f mousePos =
      window.mapPixelToCoords(sf::Mouse::getPosition(window));

  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (knob.getGlobalBounds().contains(mousePos)) {
        dragging = true;
      }
    }
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      dragging = false;
    }
  }

  if (event.type == sf::Event::MouseMoved) {
    if (dragging) {
      float x = mousePos.x;
      float trackStart = track.getPosition().x;
      float trackEnd = trackStart + track.getSize().x;

      // x-axis slider borders limits
      x = std::max(trackStart, std::min(trackEnd, x));

      // update knob position
      knob.setPosition(x, track.getPosition().y + track.getSize().y / 2.f);

      // update given value
      float t = (x - trackStart) / track.getSize().x;
      value = min + t * (max - min);
    }
  }
}

float Slider::getValue() const { return value; }

//------CheckBox--------

CheckBox::CheckBox(const std::string& labelText, const sf::Font& font,
                   sf::Vector2f pos) {
  // box setup
  box.setSize({20.f, 20.f});
  box.setFillColor(sf::Color::White);
  box.setOutlineColor(sf::Color::Black);
  box.setOutlineThickness(2.f);
  box.setPosition(pos);

  // label
  label.setFont(font);
  label.setCharacterSize(16);
  label.setFillColor(sf::Color::White);
  label.setString(labelText);
  label.setPosition(pos.x + box.getSize().x + 10.f,
                    pos.y - 2.f);  // small vertical alignment
}

void CheckBox::draw(sf::RenderWindow& target) const {
  target.draw(box);

  if (checked) {
    sf::RectangleShape checkMark;
    checkMark.setSize({12.f, 12.f});
    checkMark.setFillColor(sf::Color::Red);
    checkMark.setPosition(box.getPosition().x + 4.f, box.getPosition().y + 4.f);
    target.draw(checkMark);
  }

  target.draw(label);
}

void CheckBox::handleEvent(const sf::Event& event,
                           const sf::RenderWindow& window) {
  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (box.getGlobalBounds().contains(mousePos)) {
      checked = !checked;
    }
  }
}

bool CheckBox::isChecked() { return checked; }

//------Button-------

// default button generator
Button::Button(const std::string& label, const sf::Font& font, sf::Vector2f pos,
               sf::Vector2f size) {
  // shape
  shape.setSize(size);
  shape.setPosition(pos);
  shape.setFillColor(sf::Color(100, 100, 255));
  shape.setOutlineColor(sf::Color::Black);
  shape.setOutlineThickness(2.f);

  // text
  text.setFont(font);
  text.setString(label);
  text.setCharacterSize(16);
  text.setFillColor(sf::Color::White);

  // centering
  sf::FloatRect textBounds = text.getLocalBounds();
  text.setOrigin(textBounds.left + textBounds.width / 2.f,
                 textBounds.top + textBounds.height / 2.f);
  text.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
}

// important buttons generator
Button::Button(const std::string& label, const sf::Font& font, sf::Vector2f pos,
               sf::Vector2f size, sf::Color colour) {
  // shape
  shape.setSize(size);
  shape.setPosition(pos);
  shape.setFillColor(colour);
  shape.setOutlineColor(sf::Color::Black);
  shape.setOutlineThickness(2.f);

  // text
  text.setFont(font);
  text.setString(label);
  text.setCharacterSize(16);
  text.setFillColor(sf::Color::Black);

  // centering
  sf::FloatRect textBounds = text.getLocalBounds();
  text.setOrigin(textBounds.left + textBounds.width / 2.f,
                 textBounds.top + textBounds.height / 2.f);
  text.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
}

void Button::draw(sf::RenderTarget& target) const {
  target.draw(shape);
  target.draw(text);
}

void Button::handleEvent(const sf::Event& event,
                         const sf::RenderWindow& window) {
  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (shape.getGlobalBounds().contains(mousePos)) {
      clicked = true;
    }
  }
}

bool Button::isClicked() { return clicked; }

void Button::unClicked() { clicked = false; }

//------Notifications------

void Notification::show(const std::string& message, const sf::Font& font,
                        sf::Vector2f pos) {
  text.setFont(font);
  text.setString(message);
  text.setCharacterSize(18);
  text.setFillColor(sf::Color::Red);
  text.setPosition(pos);
  clock.restart();
  active = true;
}

void Notification::draw(sf::RenderWindow& window) {
  if (active) {
    if (clock.getElapsedTime().asSeconds() < duration) {
      window.draw(text);
    } else {
      active = false;  // auto-hide
    }
  }
}

//-------MENU---------

Menu::Menu(sf::RenderWindow& window, sf::Font& font, MenuType type)
    : _window(window), _font(font), _type(type) {
  generateLayout();  // it generates the specific layout
}

void Menu::handleEvent(const sf::Event& event) {
  for (auto& slider : sliders) slider.handleEvent(event, _window);

  for (auto& checkbox : checkboxes) checkbox.handleEvent(event, _window);

  for (auto& button : buttons) {
    button.handleEvent(event, _window);
  }
}

void Menu::draw() {
  _window.clear(sf::Color(30, 30, 30));  // grayish background color

  for (const auto& slider : sliders) slider.draw(_window);

  for (const auto& checkbox : checkboxes) checkbox.draw(_window);

  for (const auto& button : buttons) button.draw(_window);
}

void Menu::generateLayout() {
  sliders.clear();
  checkboxes.clear();
  buttons.clear();

  if (_type == MenuType::Main) {
    sliders.emplace_back("Spawned Boids", 20, 300, 160, _font,
                         sf::Vector2f(250, 100));
    buttons.emplace_back("Start", _font, sf::Vector2f(325, 400),
                         sf::Vector2f(150, 40), sf::Color::Yellow);
    buttons.emplace_back("Boids Radii", _font, sf::Vector2f(175, 250),
                         sf::Vector2f(150, 40));
    buttons.emplace_back("Modes", _font, sf::Vector2f(475, 250),
                         sf::Vector2f(150, 40));
  } else if (_type == MenuType::Radii) {
    sliders.emplace_back("Boid Size", 0.1f, 1.9f, 1.f, _font,
                         sf::Vector2f(250, 100));
    sliders.emplace_back("Separation Radius", 0.6f, 1.4f, 1.f, _font,
                         sf::Vector2f(250, 150));
    sliders.emplace_back("Cohesion Radius", 0.6f, 1.4f, 1.f, _font,
                         sf::Vector2f(250, 200));
    sliders.emplace_back("Alignment Radius", 0.6f, 1.4f, 1.f, _font,
                         sf::Vector2f(250, 250));
    buttons.emplace_back("Back", _font, sf::Vector2f(325, 350),
                         sf::Vector2f(150, 40));
  } else if (_type == MenuType::Modalities) {
    checkboxes.emplace_back("Gradual Damage", _font, sf::Vector2f(325, 100));
    checkboxes.emplace_back("Complete Evasion", _font, sf::Vector2f(325, 150));
    checkboxes.emplace_back("Arrow Following", _font, sf::Vector2f(325, 200));
    buttons.emplace_back("Back", _font, sf::Vector2f(325, 300),
                         sf::Vector2f(150, 40));
  }
}

bool Menu::startState() { return startSimulation; }

bool Menu::wasButtonClicked(const std::string& label) {
  for (auto& button : buttons) {
    if (button.text.getString() == label && button.isClicked()) {
      button.unClicked();
      return true;
    }
  }
  return false;
}

bool Menu::wasCheckBoxChecked(const std::string& label) {
  for (auto& checkbox : checkboxes) {
    if (checkbox.label.getString() == label) {
      return checkbox.isChecked();
    }
  }
  return false;
}

float Menu::wasSliderMoved(const std::string& label) {
  for (auto& slider : sliders) {
    if (slider.label.getString() == label) {
      return slider.getValue();
    }
  }
  return 1.f;
}

void Menu::setGameState(bool state) { this->startSimulation = state; }