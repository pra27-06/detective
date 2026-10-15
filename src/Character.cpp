#include "Character.h"

Character::Character(const std::string& id, const std::string& name,
                     const std::string& occupation, const std::string& personality,
                     const std::string& greeting)
    : m_id(id), m_name(name), m_occupation(occupation),
      m_personality(personality), m_greeting(greeting) {}

std::string Character::getId() const { return m_id; }
std::string Character::getName() const { return m_name; }
std::string Character::getOccupation() const { return m_occupation; }
std::string Character::getPersonality() const { return m_personality; }
std::string Character::getGreeting() const { return m_greeting; }
