#include "ForensicLab.h"
#include <sstream>
#include <iostream>

std::string ForensicReport::formatCertificate() const {
    std::ostringstream oss;
    oss << "========================================================\n"
        << "  SCOTLAND YARD METROPOLITAN FORENSIC CERTIFICATE\n"
        << "========================================================\n"
        << "Target Specimen: " << evidenceId << "\n"
        << "Assay Method:    " << analyzerName << "\n"
        << "Reagent Formula: " << chemicalFormula << "\n"
        << "Observation:     " << microscopicObservation << "\n"
        << "Secret Revealed: " << secretRevealed << "\n"
        << "Evidentiary Merit: " << (isDecisiveProof ? "DECISIVE COURTROOM PROOF" : "CORROBORATING LEAD") << "\n"
        << "Forensic Merit Points: +" << forensicPoints << "\n"
        << "========================================================";
    return oss.str();
}

ForensicAnalyzer::ForensicAnalyzer(const std::string& name, const std::string& specialty)
    : m_name(name), m_specialty(specialty) {}

ForensicReport ForensicAnalyzer::analyze(const Evidence& evidence) {
    // Template Method Pattern Execution
    prepareReagents();
    std::string assayData = executeAssay(evidence);
    return synthesizeReport(evidence, assayData);
}

// -----------------------------------------------------------------------------
// ChemicalReagentAnalyzer
// -----------------------------------------------------------------------------
ChemicalReagentAnalyzer::ChemicalReagentAnalyzer()
    : ForensicAnalyzer("Chemical Reagent Assay", "Cyanide, Nitrates, and Blood Luminol Testing") {}

void ChemicalReagentAnalyzer::prepareReagents() {
    // Reagent preparation simulation
}

std::string ChemicalReagentAnalyzer::executeAssay(const Evidence& evidence) {
    std::string id = evidence.getId();
    if (id == "E01" || id == "E04") {
        return "PRUSSIAN_BLUE_REACTION: Deep Prussian blue precipitate formed within 4 seconds.";
    } else if (id == "E03" || id == "E07") {
        return "SILVER_NITRATE_HALIDE: Dense white silver halide curds exposed on mechanical contact points.";
    }
    return "ORGANIC_RESIDUE_TRACE: Mild organic vegetable tannin reaction detected.";
}

ForensicReport ChemicalReagentAnalyzer::synthesizeReport(const Evidence& evidence, const std::string& assayData) {
    ForensicReport r;
    r.evidenceId = evidence.getId();
    r.analyzerName = m_name;
    r.chemicalFormula = "Fe4[Fe(CN)6]3 + AgNO3";
    r.microscopicObservation = assayData;

    if (assayData.find("PRUSSIAN_BLUE") != std::string::npos) {
        r.secretRevealed = "Hydrocyanic acid residue verified. Ingestion occurred exactly 12 minutes prior to respiratory collapse.";
        r.isDecisiveProof = true;
        r.forensicPoints = 40;
    } else if (assayData.find("SILVER_NITRATE") != std::string::npos) {
        r.secretRevealed = "Metallic silver halide staining proves deliberate bypass using chloride-doped jumper wire.";
        r.isDecisiveProof = true;
        r.forensicPoints = 35;
    } else {
        r.secretRevealed = "Common soot, mineral dust, and Victorian city fog deposits.";
        r.isDecisiveProof = false;
        r.forensicPoints = 15;
    }
    return r;
}

// -----------------------------------------------------------------------------
// SpectroscopyAnalyzer
// -----------------------------------------------------------------------------
SpectroscopyAnalyzer::SpectroscopyAnalyzer()
    : ForensicAnalyzer("Microscopic Spectroscopy", "Ultraviolet Diffraction & Gemological Refraction") {}

void SpectroscopyAnalyzer::prepareReagents() {
    // Mercury arc lamp ultraviolet excitation
}

std::string SpectroscopyAnalyzer::executeAssay(const Evidence& evidence) {
    std::string id = evidence.getId();
    if (id == "E01" || id == "E09") {
        return "UV_FLUORESCENCE_POLARIZED: Refractive index 1.77. Micro-engraved 8-character coordinate grid detected.";
    } else if (id == "E02" || id == "E05") {
        return "SPECTRO_INK_ABSORPTION: Synthetic aniline dye signature with 420nm blue-violet absorption notch.";
    }
    return "DIFFUSE_SURFACE_SCATTER: Standard calcite and steel reflection spectrum.";
}

ForensicReport SpectroscopyAnalyzer::synthesizeReport(const Evidence& evidence, const std::string& assayData) {
    ForensicReport r;
    r.evidenceId = evidence.getId();
    r.analyzerName = m_name;
    r.chemicalFormula = "UV Lambda = 365nm Polarized Field";
    r.microscopicObservation = assayData;

    if (assayData.find("UV_FLUORESCENCE") != std::string::npos) {
        r.secretRevealed = "Micro-etched astronomical coordinates confirm direct link to the Syndicate of the Obsidian Hourglass!";
        r.isDecisiveProof = true;
        r.forensicPoints = 50;
    } else if (assayData.find("SPECTRO_INK") != std::string::npos) {
        r.secretRevealed = "Aniline ballpoint ink formulation proves the note was written in 1894, not authentic vintage quill ink.";
        r.isDecisiveProof = true;
        r.forensicPoints = 40;
    } else {
        r.secretRevealed = "Standard surface refraction with no anomalous luminescence.";
        r.isDecisiveProof = false;
        r.forensicPoints = 15;
    }
    return r;
}

// -----------------------------------------------------------------------------
// BallisticsMetallurgyAnalyzer
// -----------------------------------------------------------------------------
BallisticsMetallurgyAnalyzer::BallisticsMetallurgyAnalyzer()
    : ForensicAnalyzer("Metallurgical Toolmark Assay", "Striation Micro-Grooves & Mechanical Shearing Stress") {}

void BallisticsMetallurgyAnalyzer::prepareReagents() {
    // Optical comparator calibration
}

std::string BallisticsMetallurgyAnalyzer::executeAssay(const Evidence& evidence) {
    std::string id = evidence.getId();
    if (id == "E01" || id == "E03" || id == "E06") {
        return "SHEAR_STRIATION_MATCH: 14 parallel microscopic grooves matching a 0.2mm tungsten pick wire.";
    }
    return "STANDARD_MACHINING_GROOVES: Industrial lathe turning marks with no intentional tampering.";
}

ForensicReport BallisticsMetallurgyAnalyzer::synthesizeReport(const Evidence& evidence, const std::string& assayData) {
    ForensicReport r;
    r.evidenceId = evidence.getId();
    r.analyzerName = m_name;
    r.chemicalFormula = "Tungsten Carbide Micro-Striation Matrix";
    r.microscopicObservation = assayData;

    if (assayData.find("SHEAR_STRIATION") != std::string::npos) {
        r.secretRevealed = "Toolmark grooves precisely match the specialty lockpick set carried by the syndicate operative.";
        r.isDecisiveProof = true;
        r.forensicPoints = 45;
    } else {
        r.secretRevealed = "Uniform tool wear consistent with factory fabrication.";
        r.isDecisiveProof = false;
        r.forensicPoints = 15;
    }
    return r;
}

// -----------------------------------------------------------------------------
// ForensicLab
// -----------------------------------------------------------------------------
ForensicLab::ForensicLab() {
    registerAnalyzer(std::make_unique<ChemicalReagentAnalyzer>());
    registerAnalyzer(std::make_unique<SpectroscopyAnalyzer>());
    registerAnalyzer(std::make_unique<BallisticsMetallurgyAnalyzer>());
}

void ForensicLab::registerAnalyzer(std::unique_ptr<ForensicAnalyzer> analyzer) {
    if (analyzer) {
        m_analyzers[analyzer->getName()] = std::move(analyzer);
    }
}

bool ForensicLab::runAnalysis(const std::string& analyzerName, const Evidence& evidence, ForensicReport& outReport) {
    auto it = m_analyzers.find(analyzerName);
    if (it == m_analyzers.end()) {
        return false;
    }

    outReport = it->second->analyze(evidence);
    std::string key = evidence.getId() + "_" + analyzerName;
    m_completedReports[key] = outReport;
    return true;
}

bool ForensicLab::hasReport(const std::string& evidenceId, const std::string& analyzerName) const {
    std::string key = evidenceId + "_" + analyzerName;
    return m_completedReports.find(key) != m_completedReports.end();
}

std::vector<std::string> ForensicLab::getAvailableAnalyzerNames() const {
    std::vector<std::string> names;
    for (const auto& pair : m_analyzers) {
        names.push_back(pair.first);
    }
    return names;
}
