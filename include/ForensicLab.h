#ifndef FORENSIC_LAB_H
#define FORENSIC_LAB_H

#include "Evidence.h"
#include <string>
#include <memory>
#include <map>
#include <vector>

// Encapsulation of forensic assay results
struct ForensicReport {
    std::string evidenceId;
    std::string analyzerName;
    std::string chemicalFormula;
    std::string microscopicObservation;
    std::string secretRevealed;
    bool isDecisiveProof;
    int forensicPoints;

    std::string formatCertificate() const;
};

// Abstract Base Class: Demonstrating the Template Method Pattern & Polymorphism
class ForensicAnalyzer {
protected:
    std::string m_name;
    std::string m_specialty;

    // Pure virtual steps for Template Method
    virtual void prepareReagents() = 0;
    virtual std::string executeAssay(const Evidence& evidence) = 0;
    virtual ForensicReport synthesizeReport(const Evidence& evidence, const std::string& assayData) = 0;

public:
    ForensicAnalyzer(const std::string& name, const std::string& specialty);
    virtual ~ForensicAnalyzer() = default;

    std::string getName() const { return m_name; }
    std::string getSpecialty() const { return m_specialty; }

    // Template Method: Skeleton of the forensic chemical/microscopic algorithm
    ForensicReport analyze(const Evidence& evidence);
};

// Concrete Analyzer 1: Chemical Reagent Chromatography (Acid/Reagent Assay)
class ChemicalReagentAnalyzer : public ForensicAnalyzer {
protected:
    void prepareReagents() override;
    std::string executeAssay(const Evidence& evidence) override;
    ForensicReport synthesizeReport(const Evidence& evidence, const std::string& assayData) override;

public:
    ChemicalReagentAnalyzer();
};

// Concrete Analyzer 2: Microscopic Calligraphy & Toolmark Spectroscopy
class SpectroscopyAnalyzer : public ForensicAnalyzer {
protected:
    void prepareReagents() override;
    std::string executeAssay(const Evidence& evidence) override;
    ForensicReport synthesizeReport(const Evidence& evidence, const std::string& assayData) override;

public:
    SpectroscopyAnalyzer();
};

// Concrete Analyzer 3: Ballistics & Metallurgical Stress Analyzer
class BallisticsMetallurgyAnalyzer : public ForensicAnalyzer {
protected:
    void prepareReagents() override;
    std::string executeAssay(const Evidence& evidence) override;
    ForensicReport synthesizeReport(const Evidence& evidence, const std::string& assayData) override;

public:
    BallisticsMetallurgyAnalyzer();
};

// ForensicLab Class: Demonstrates Composition & Strategy Pattern
class ForensicLab {
private:
    std::map<std::string, std::unique_ptr<ForensicAnalyzer>> m_analyzers;
    std::map<std::string, ForensicReport> m_completedReports; // key: evidenceId + "_" + analyzerName

public:
    ForensicLab();

    // Strategy registration & execution
    void registerAnalyzer(std::unique_ptr<ForensicAnalyzer> analyzer);
    bool runAnalysis(const std::string& analyzerName, const Evidence& evidence, ForensicReport& outReport);
    bool hasReport(const std::string& evidenceId, const std::string& analyzerName) const;
    const std::map<std::string, ForensicReport>& getAllReports() const { return m_completedReports; }
    std::vector<std::string> getAvailableAnalyzerNames() const;
};

#endif // FORENSIC_LAB_H
