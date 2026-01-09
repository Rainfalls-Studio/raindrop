#include "Raindrop/Version.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop{
                
    VersionConstraint::SimpleConstraint::SimpleConstraint(Operator op_, const Version& version_) : op(op_), version(version_) {}

    bool VersionConstraint::SimpleConstraint::satisfies(const Version& v) const{
        switch (op) {
            case Operator::EQUAL: return v == version;
            case Operator::NOT_EQUAL: return !(v == version);
            case Operator::GREATER: return v > version;
            case Operator::GREATER_OR_EQUAL: return v >= version;
            case Operator::LESS: return v < version;
            case Operator::LESS_OR_EQUAL: return v <= version;
            case Operator::TILDE: return satisfiesTilde(v);
            case Operator::CARET: return satisfiesCaret(v);
            default: return false;
        }
    }

    bool VersionConstraint::SimpleConstraint::satisfiesTilde(const Version& v) const {
        // ~1.2.3 means >=1.2.3 <1.3.0
        // ~1.2 means >=1.2.0 <1.3.0
        // ~1 means >=1.0.0 <2.0.0
        if (v.major() != version.major()) return false;
        if (v.minor() < version.minor()) return false;
        if (version.minor() == version.minor() && v.patch() < version.patch()) return false;
        return v.major() == version.major() && 
            (v.minor() == version.minor() || v.minor() == version.minor() + 1);
    }

    bool VersionConstraint::SimpleConstraint::satisfiesCaret(const Version& v) const {
        // ^1.2.3 means >=1.2.3 <2.0.0
        // ^0.2.3 means >=0.2.3 <0.3.0
        // ^0.0.3 means >=0.0.3 <0.0.4
        if (v.major() != version.major()) return false;
        if (version.major() > 0) {
            return v >= version && v.major() == version.major();
        } else if (version.minor() > 0) {
            return v >= version && v.minor() == version.minor();
        } else {
            return v >= version && v.patch() == version.patch();
        }
    }

    std::string VersionConstraint::SimpleConstraint::toString() const {
        static const std::unordered_map<Operator, std::string> opToString = {
            {Operator::EQUAL, "=="},
            {Operator::NOT_EQUAL, "!="},
            {Operator::GREATER, ">"},
            {Operator::GREATER_OR_EQUAL, ">="},
            {Operator::LESS, "<"},
            {Operator::LESS_OR_EQUAL, "<="},
            {Operator::TILDE, "~"},
            {Operator::CARET, "^"}
        };
        
        auto it = opToString.find(op);
        if (it != opToString.end()) {
            return it->second + version.str();
        }
        return "??" + version.str();
    }


    // ============= RangeConstraint

    bool VersionConstraint::RangeConstraint::satisfies(const Version& version) const {
        for (const auto& constraint : constraints) {
            if (!constraint->satisfies(version)) {
                return false;
            }
        }
        return true;
    }

    std::string VersionConstraint::RangeConstraint::toString() const {
        std::string result;
        for (size_t i = 0; i < constraints.size(); ++i) {
            if (i > 0) result += " ";
            result += constraints[i]->toString();
        }
        return result;
    }


    // ============= OrConstraint


    bool VersionConstraint::OrConstraint::satisfies(const Version& version) const {
        for (const auto& constraint : constraints) {
            if (constraint->satisfies(version)) {
                return true;
            }
        }
        return false;
    }
                
    std::string VersionConstraint::OrConstraint::toString() const {
        std::string result;
        for (size_t i = 0; i < constraints.size(); ++i) {
            if (i > 0) result += " || ";
            result += constraints[i]->toString();
        }
        return result;
    }



    // ============= WildcardConstraint


    bool VersionConstraint::WildcardConstraint::satisfies(const Version& version) const {
        if (majorWildcard) return true;  // * matches everything
        if (version.major() != major) return false;
        if (minorWildcard) return true;  // 1.* matches 1.x.x
        if (version.minor() != minor) return false;
        if (patchWildcard) return true;  // 1.2.* matches 1.2.x
        return version.patch() != 0;  // Actually this needs more logic
    }
    
    std::string VersionConstraint::WildcardConstraint::toString() const {
        if (majorWildcard) return "*";
        if (minorWildcard) return std::to_string(major) + ".*";
        if (patchWildcard) return std::to_string(major) + "." + std::to_string(minor) + ".*";
        return "*";
    }



    // ============= VersionConstraint

    VersionConstraint::VersionConstraint(const VersionConstraint& other){

        // Very ugly indeed, yet works
        constraint_ = std::move(Parse(other.toString()).constraint_);
    }



    VersionConstraint VersionConstraint::Parse(const std::string& constraint_str) {
        VersionConstraint vc;
        vc.constraint_ = ParseConstraint(constraint_str);
        return vc;
    }

    VersionConstraint VersionConstraint::Any(){
        VersionConstraint vc;

        auto constraint = std::make_unique<WildcardConstraint>();
        constraint->majorWildcard = true;

        vc.constraint_ = std::move(constraint);
        return vc;
    }
    
    // Check if a version satisfies this constraint
    bool VersionConstraint::satisfies(const Version& version) const {
        if (!constraint_) return true; // empty acts as contraint *.*.*
        return constraint_->satisfies(version);
    }
    
    bool VersionConstraint::satisfies(const std::string& version_str) const {
        try {
            Version version = Version::parse(version_str);
            return satisfies(version);
        } catch (...) {
            return false;
        }
    }
    
    std::string VersionConstraint::toString() const {
        if (!constraint_) return "none";
        return constraint_->toString();
    }


    std::unique_ptr<VersionConstraint::BaseConstraint> VersionConstraint::ParseConstraint(const std::string& str) {
        std::string trimmed = Trim(str);
        
        // Check for OR constraints
        if (trimmed.find("||") != std::string::npos) {
            auto orConstraint = std::make_unique<OrConstraint>();
            std::vector<std::string> parts = Split(trimmed, "||");
            for (const auto& part : parts) {
                orConstraint->constraints.push_back(ParseSimpleConstraint(Trim(part)));
            }
            return orConstraint;
        }
        
        // Check for range constraints (multiple simple constraints)
        if (trimmed.find(' ') != std::string::npos) {
            auto rangeConstraint = std::make_unique<RangeConstraint>();
            std::vector<std::string> parts = Split(trimmed, " ");
            for (const auto& part : parts) {
                rangeConstraint->constraints.push_back(ParseSimpleConstraint(Trim(part)));
            }
            return rangeConstraint;
        }
        
        // Simple constraint
        return ParseSimpleConstraint(trimmed);
    }

    std::unique_ptr<VersionConstraint::BaseConstraint> VersionConstraint::ParseSimpleConstraint(const std::string& str) {
        if (str.empty()) return nullptr;
        
        // Check for wildcards
        if (str.find('*') != std::string::npos || 
            str.find('x') != std::string::npos || 
            str.find('X') != std::string::npos) {
            return ParseWildcardConstraint(str);
        }
        
        // Parse operator and version
        Operator op = Operator::EQUAL;
        std::string version_str;
        
        if (str.substr(0, 2) == "==") {
            op = Operator::EQUAL;
            version_str = str.substr(2);
        } else if (str.substr(0, 2) == "!=") {
            op = Operator::NOT_EQUAL;
            version_str = str.substr(2);
        } else if (str.substr(0, 2) == ">=" || str.substr(0, 2) == "=>") {
            op = Operator::GREATER_OR_EQUAL;
            version_str = str.substr(2);
        } else if (str.substr(0, 2) == "<=") {
            op = Operator::LESS_OR_EQUAL;
            version_str = str.substr(2);
        } else if (str[0] == '>') {
            op = Operator::GREATER;
            version_str = str.substr(1);
        } else if (str[0] == '<') {
            op = Operator::LESS;
            version_str = str.substr(1);
        } else if (str[0] == '~') {
            op = Operator::TILDE;
            version_str = str.substr(1);
        } else if (str[0] == '^') {
            op = Operator::CARET;
            version_str = str.substr(1);
        } else {
            // Default to equality
            op = Operator::EQUAL;
            version_str = str;
        }
        
        try {
            Version version = Version::parse(version_str);
            return std::make_unique<SimpleConstraint>(op, version);
        } catch (const std::exception&) {
            // If parsing fails, try as wildcard
            return ParseWildcardConstraint(str);
        }
    }

    std::unique_ptr<VersionConstraint::BaseConstraint> VersionConstraint::ParseWildcardConstraint(const std::string& str) {
        auto wc = std::make_unique<WildcardConstraint>();
        
        if (str == "*" || str == "x" || str == "X") {
            wc->majorWildcard = true;
            return wc;
        }
        
        // Parse patterns like 1.*, 1.2.*
        std::vector<std::string> parts = Split(str, ".");
        if (parts.size() >= 1) {
            try {
                wc->major = static_cast<uint32_t>(std::stoul(parts[0]));
                if (parts.size() >= 2 && (parts[1] == "*" || parts[1] == "x" || parts[1] == "X")) {
                    wc->minorWildcard = true;
                } else if (parts.size() >= 2) {
                    wc->minor = static_cast<uint32_t>(std::stoul(parts[1]));
                    if (parts.size() >= 3 && (parts[2] == "*" || parts[2] == "x" || parts[2] == "X")) {
                        wc->patchWildcard = true;
                    }
                }
            } catch (...) {
                // Invalid
            }
        }
        
        return wc;
    }

    std::string VersionConstraint::Trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    std::vector<std::string> VersionConstraint::Split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = str.find(delimiter, start)) != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }
}