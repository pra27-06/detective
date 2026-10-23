#ifndef DIALOGUE_SYSTEM_H
#define DIALOGUE_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <functional>

struct DialogueChoice {
    std::string text;
    std::string nextId;
    std::function<void()> onSelected;

    DialogueChoice() : text(""), nextId(""), onSelected(nullptr) {}
    DialogueChoice(const std::string& t, const std::string& nId = "", std::function<void()> cb = nullptr)
        : text(t), nextId(nId), onSelected(cb) {}
};

struct DialogueEntry {
    std::string id;
    std::string speaker;
    std::string characterId;
    std::string expression;
    std::string text;
    std::string background;
    std::string nextId;
    std::vector<DialogueChoice> choices;
    std::function<void()> onShow;

    DialogueEntry()
        : id(""), speaker(""), characterId(""), expression("neutral"),
          text(""), background(""), nextId(""), onShow(nullptr) {}

    DialogueEntry(const std::string& id_, const std::string& spk, const std::string& charId,
                  const std::string& expr, const std::string& txt, const std::string& bg = "",
                  const std::string& next_ = "", std::function<void()> cb = nullptr)
        : id(id_), speaker(spk), characterId(charId), expression(expr),
          text(txt), background(bg), nextId(next_), onShow(cb) {}
};

class DialogueSystem {
private:
    std::vector<DialogueEntry> m_entries;
    std::map<std::string, size_t> m_idToIndex;
    size_t m_currentIndex;

    float m_charsRevealed;
    float m_typingSpeed; // characters per second
    bool m_isTyping;
    bool m_isComplete;
    bool m_isEnded;

public:
    DialogueSystem();

    void clear();
    void addEntry(const DialogueEntry& entry);
    void setEntries(const std::vector<DialogueEntry>& entries);

    void start(size_t startIndex = 0);
    bool jumpTo(const std::string& entryId);

    void update(float dt);

    void finishTyping();
    bool advance(); // Returns false if reached end of script

    bool isTyping() const { return m_isTyping; }
    bool isComplete() const { return m_isComplete; }
    bool isEnded() const { return m_isEnded; }

    bool hasChoices() const;
    const std::vector<DialogueChoice>& getChoices() const;
    bool selectChoice(size_t index);

    const DialogueEntry* getCurrentEntry() const;
    std::string getDisplayedText() const;

    void setTypingSpeed(float charsPerSec) { m_typingSpeed = charsPerSec; }
    float getTypingSpeed() const { return m_typingSpeed; }
};

#endif // DIALOGUE_SYSTEM_H
