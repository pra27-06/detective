#include "DialogueSystem.h"
#include <algorithm>

DialogueSystem::DialogueSystem()
    : m_currentIndex(0), m_charsRevealed(0.0f), m_typingSpeed(45.0f),
      m_isTyping(false), m_isComplete(false), m_isEnded(true) {
}

void DialogueSystem::clear() {
    m_entries.clear();
    m_idToIndex.clear();
    m_currentIndex = 0;
    m_charsRevealed = 0.0f;
    m_isTyping = false;
    m_isComplete = false;
    m_isEnded = true;
}

void DialogueSystem::addEntry(const DialogueEntry& entry) {
    if (!entry.id.empty()) {
        m_idToIndex[entry.id] = m_entries.size();
    }
    m_entries.push_back(entry);
}

void DialogueSystem::setEntries(const std::vector<DialogueEntry>& entries) {
    clear();
    for (const auto& e : entries) {
        addEntry(e);
    }
}

void DialogueSystem::start(size_t startIndex) {
    if (m_entries.empty() || startIndex >= m_entries.size()) {
        m_isEnded = true;
        m_isTyping = false;
        m_isComplete = false;
        return;
    }

    m_currentIndex = startIndex;
    m_charsRevealed = 0.0f;
    m_isTyping = true;
    m_isComplete = false;
    m_isEnded = false;

    if (m_entries[m_currentIndex].onShow) {
        m_entries[m_currentIndex].onShow();
    }
}

bool DialogueSystem::jumpTo(const std::string& entryId) {
    auto it = m_idToIndex.find(entryId);
    if (it != m_idToIndex.end()) {
        start(it->second);
        return true;
    }
    return false;
}

void DialogueSystem::update(float dt) {
    if (m_isEnded || !m_isTyping) return;

    const DialogueEntry& entry = m_entries[m_currentIndex];
    m_charsRevealed += m_typingSpeed * dt;

    if (m_charsRevealed >= (float)entry.text.length()) {
        m_charsRevealed = (float)entry.text.length();
        m_isTyping = false;
        m_isComplete = true;
    }
}

void DialogueSystem::finishTyping() {
    if (m_isEnded || !m_isTyping) return;

    const DialogueEntry& entry = m_entries[m_currentIndex];
    m_charsRevealed = (float)entry.text.length();
    m_isTyping = false;
    m_isComplete = true;
}

bool DialogueSystem::advance() {
    if (m_isEnded) return false;

    // Click 1: if currently typing, finish typing instantly!
    if (m_isTyping) {
        finishTyping();
        return true;
    }

    // Click 2: if already complete and waiting, move to next line
    if (m_isComplete) {
        if (hasChoices()) {
            return true; // Wait for player to click a choice
        }

        const DialogueEntry& cur = m_entries[m_currentIndex];
        if (!cur.nextId.empty()) {
            return jumpTo(cur.nextId);
        }

        if (m_currentIndex + 1 < m_entries.size()) {
            start(m_currentIndex + 1);
            return true;
        } else {
            m_isEnded = true;
            return false;
        }
    }

    return false;
}

bool DialogueSystem::hasChoices() const {
    if (m_isEnded || m_currentIndex >= m_entries.size()) return false;
    return !m_entries[m_currentIndex].choices.empty() && m_isComplete;
}

const std::vector<DialogueChoice>& DialogueSystem::getChoices() const {
    static std::vector<DialogueChoice> s_empty;
    if (m_isEnded || m_currentIndex >= m_entries.size()) return s_empty;
    return m_entries[m_currentIndex].choices;
}

bool DialogueSystem::selectChoice(size_t index) {
    if (!hasChoices()) return false;
    const auto& choices = m_entries[m_currentIndex].choices;
    if (index >= choices.size()) return false;

    const DialogueChoice& chosen = choices[index];
    if (chosen.onSelected) {
        chosen.onSelected();
    }

    if (!chosen.nextId.empty()) {
        return jumpTo(chosen.nextId);
    } else {
        return advance();
    }
}

const DialogueEntry* DialogueSystem::getCurrentEntry() const {
    if (m_isEnded || m_currentIndex >= m_entries.size()) return nullptr;
    return &m_entries[m_currentIndex];
}

std::string DialogueSystem::getDisplayedText() const {
    if (m_isEnded || m_currentIndex >= m_entries.size()) return "";
    const std::string& full = m_entries[m_currentIndex].text;
    size_t count = (size_t)std::min((float)full.length(), m_charsRevealed);
    return full.substr(0, count);
}
