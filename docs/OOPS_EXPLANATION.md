# Object-Oriented Programming (OOP) Concepts & Viva Voce Guide

This document is prepared specifically for second-year Computer Science & Engineering (CSE) academic examinations, project reviews, and viva voce evaluations based on **CASE CLOSED — Detective Mystery Simulator**.

---

## 1. The Four Pillars of OOP in Case Closed

### A. Encapsulation
**Concept:** Bundling data members and the member functions that manipulate them together inside a class, while restricting direct access to the internal state using access specifiers (`private` / `protected`).

**Code Demonstration:**
- In [`Evidence.h`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/include/Evidence.h):
  ```cpp
  class Evidence {
  private:
      std::string m_id;
      std::string m_name;
      EvidenceCategory m_category;
      EvidenceRelevance m_relevance;
      bool m_isDiscovered;
      bool m_isKeyEvidence;
  public:
      std::string getId() const { return m_id; }
      bool isDiscovered() const { return m_isDiscovered; }
      void setDiscovered(bool discovered) { m_isDiscovered = discovered; }
  };
  ```
- **Rationale:** `GameEngine` or external callers cannot mutate an evidence card's discovery state or change its category directly; all state changes must pass through validated methods.

---

### B. Abstraction
**Concept:** Hiding complex internal logic and presenting only essential interface contracts to client code.

**Code Demonstration:**
- In [`Character.h`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/include/Character.h):
  ```cpp
  class Character {
  protected:
      std::string m_id;
      std::string m_name;
      std::string m_occupation;
  public:
      virtual ~Character() = default;
      virtual std::string getRole() const = 0;       // Pure virtual
      virtual void displayProfile() const = 0;       // Pure virtual
      virtual std::string interact(const std::string& topic) = 0; // Pure virtual
  };
  ```
- **Rationale:** `Character` is an **Abstract Base Class**. Callers only need to know that *any* character can be interacted with via `interact(topic)` or displayed via `displayProfile()`. How a suspect evaluates hostility or how a witness recalls times is hidden inside derived classes.

---

### C. Inheritance ("IS-A" Hierarchy)
**Concept:** A derived class inherits state and behavior from a base class, establishing an "IS-A" relationship and eliminating redundant boilerplate.

**Code Demonstration:**
- Derived classes [`Detective`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/include/Detective.h), [`Suspect`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/include/Suspect.h), and [`Witness`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/include/Witness.h):
  ```cpp
  class Suspect : public Character {
  private:
      std::string m_alibi;
      std::string m_motive;
      bool m_alibiBroken;
      std::vector<Contradiction> m_contradictions;
  public:
      std::string getRole() const override;
      void displayProfile() const override;
      std::string interact(const std::string& topic) override;
  };
  ```
- **Rationale:** Common character attributes (`m_id`, `m_name`, `m_occupation`, `m_greeting`) are defined once in `Character` and reused across all roles.

---

### D. Runtime Polymorphism (Dynamic Binding)
**Concept:** Resolving function calls at runtime based on the actual object type rather than the pointer type, facilitated via the compiler's Virtual Table (`vtable`).

**Code Demonstration:**
- In [`CaseFile.h`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/include/CaseFile.h) & [`GameEngine.cpp`](file:///C:/Users/Prachi/OneDrive/Desktop/detective/src/GameEngine.cpp):
  ```cpp
  std::vector<std::shared_ptr<Character>> m_characters;

  // In interrogation loop:
  std::shared_ptr<Character> target = characters[choice - 1];
  target->displayProfile(); // Dynamic dispatch via vtable!
  std::string response = target->interact(topic); // Executes Suspect or Witness logic!
  ```
- **Rationale:** The interrogation loop does not require bulky `switch-case` statements or type flags to determine how to interact with characters. Adding new character types requires zero modifications to existing loop logic (Open-Closed Principle).

---

## 2. Additional Core Design Principles Demonstrated

1. **Composition & Aggregation ("HAS-A" Architecture):**
   - `GameEngine` *owns* `Detective`, `CaseFile`, and `Notebook` via `std::shared_ptr` and `std::unique_ptr`.
   - `CaseFile` *owns* `CrimeScene` objects, `Evidence` maps, and the `ClueBoard`.
   - `CrimeScene` *aggregates* `Hotspot` structures.
2. **Resource Acquisition Is Initialization (RAII):**
   - Memory is managed with `std::unique_ptr` and `std::shared_ptr`. Zero manual `new` or `delete` operators are used, preventing memory leaks and dangling pointers.
   - File streams (`std::ifstream`, `std::ofstream`) close automatically upon scope exit.
3. **Const Correctness:**
   - Inspection methods are marked `const` (e.g. `isDiscovered() const`, `getRole() const`, `isAlibiBroken() const`) ensuring compile-time immutability.
4. **Safe Downcasting:**
   - `std::dynamic_pointer_cast<Suspect>(character)` safely checks if a base-class character pointer points to a derived `Suspect` before calling suspect-specific methods.

---

## 3. Top 12 College CSE Viva Voce Questions & Answers

### Q1: What makes `Character` an abstract class?
**Answer:** A class is abstract in C++ if it declares at least one **pure virtual function** (using `= 0`). In `Character.h`, `getRole()`, `displayProfile()`, and `interact()` are pure virtual. Abstract classes cannot be instantiated directly and must be implemented by derived classes.

### Q2: Why is the destructor in `Character` declared as `virtual ~Character() = default;`?
**Answer:** When deleting a derived class object (like `Suspect` or `Detective`) through a base-class pointer (`std::shared_ptr<Character>`), a virtual destructor guarantees that the derived class destructor executes first, followed by the base class destructor. Without a virtual destructor, only the base destructor runs, causing resource leaks.

### Q3: What is the Virtual Method Table (`vtable`) and how does it enable polymorphism?
**Answer:** For any class containing virtual functions, the compiler generates a static table of function pointers called the `vtable`. Each object instance holds an internal pointer (`vptr`) pointing to its class's `vtable`. When a virtual function is called through a base pointer, the runtime looks up the `vptr`, indexes the function in the `vtable`, and jumps to the derived implementation.

### Q4: Why did you choose `std::unique_ptr` for `m_activeCase` in `GameEngine`?
**Answer:** `std::unique_ptr` represents exclusive ownership. Only one `CaseFile` is active in the game engine at any time. When a case ends or a new game begins, the previous case file is automatically freed with zero memory leaks and zero reference-counting overhead.

### Q5: How does `std::dynamic_pointer_cast` work in your interrogation system?
**Answer:** `std::dynamic_pointer_cast<Suspect>(character)` uses Run-Time Type Information (RTTI) to safely cast a `std::shared_ptr<Character>` to a `std::shared_ptr<Suspect>`. If the object is not a `Suspect` (e.g., if it is a `Witness`), it returns `nullptr`, allowing safe branching without unsafe raw C-style casting.

### Q6: How does the contradiction detection mechanism operate under OOP principles?
**Answer:** Contradictions are encapsulated within the `Suspect` class as `Contradiction` structures containing a statement quote, a conflicting evidence ID, and an admission resolution. Calling `Suspect::testContradiction(evidenceId, outQuote, outResolution)` compares the player's presented evidence against the suspect's lies. If valid, it updates the suspect's internal state (`m_alibiBroken = true`), recording the admission in the interview transcript.

### Q7: Explain the difference between Composition and Aggregation in your project.
**Answer:**
- **Composition (Strong Ownership):** `CaseFile` owns `CrimeScene` objects by value in `std::vector<CrimeScene>`. When `CaseFile` is destroyed, its scenes are destroyed simultaneously.
- **Aggregation / Polymorphic Aggregation:** `CaseFile` holds `std::vector<std::shared_ptr<Character>>`. The characters can outlive the container if referenced elsewhere.

### Q8: How did you prevent infinite loops when standard input is redirected from a file or stream?
**Answer:** In `TerminalUI.cpp`, `std::getline(std::cin, input)` checks `std::cin.eof()`. If the end-of-file is encountered during stream redirection, the program prints a clean session termination notice and exits gracefully (`std::exit(0)`) rather than looping infinitely on failed input.

### Q9: What STL containers were used and why?
**Answer:**
- `std::vector`: For dynamic sequential lists of scenes, characters, objectives, and timeline events.
- `std::map`: For key-value associations of dialogue topics and fast evidence lookup by ID (`m_allEvidence["E01"]`).
- `std::set`: In `ClueBoard` (`m_pinnedEvidenceIds`) to guarantee unique evidence pins with zero duplicates and logarithmic search complexity.
- `std::pair`: For connecting two clues (`std::pair<std::string, std::string>`) and storing Q&A transcripts.

### Q10: How does your Save/Load system demonstrate file handling?
**Answer:** `SaveManager` uses RAII-compliant `std::ofstream` to serialize case ID, difficulty, detective score, discovered evidence IDs, and broken alibis into a tagged, human-readable text format. `std::ifstream` parses each section sequentially, rebuilding the exact case state upon reload.

### Q11: What is the Single Responsibility Principle (SRP) and how is it reflected here?
**Answer:**
- `TerminalUI`: Responsible strictly for terminal presentation, ASCII drawing, and input validation.
- `SaveManager`: Responsible strictly for file serialization and deserialization.
- `CaseFile`: Responsible strictly for domain case data, logic, and accusation evaluation.
- `GameEngine`: Responsible strictly for the game state machine and menu coordination.

### Q12: Why are functions like `toUpper()` and `trim()` placed in the `UI` namespace?
**Answer:** Namespaces prevent global name collisions and group related utility functions logically under a common scope (`UI::toUpper`, `UI::promptInt`), adhering to modular software engineering practices.
