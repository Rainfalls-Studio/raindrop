#pragma once

#include <unordered_map>
#include <fmt/base.h>
#include <semver/semver.hpp>

namespace Raindrop{
    using Version = semver::version;

    class VersionConstraint {
        public:
            enum class Operator {
                EQUAL,              // ==
                NOT_EQUAL,          // !=
                GREATER,            // >
                GREATER_OR_EQUAL,   // >= (or =>)
                LESS,               // <
                LESS_OR_EQUAL,      // <=
                TILDE,              // ~ (compatible)
                CARET,              // ^ (major locked)
                WILDCARD,           // * or x
                LOGICAL_OR          // || (multiple constraints)
            };
            
        private:
            struct BaseConstraint {
                virtual ~BaseConstraint() = default;
                virtual bool satisfies(const Version& version) const = 0;
                virtual std::string toString() const = 0;
            };
            
            // Single version constraint (==1.2.3, >=1.0.0, etc.)
            struct SimpleConstraint : BaseConstraint {
                Operator op;
                Version version;
                
                SimpleConstraint(Operator op, const Version& version);
                
                bool satisfies(const Version& v) const override;
                bool satisfiesTilde(const Version& v) const;
                bool satisfiesCaret(const Version& v) const;
                std::string toString() const override;
            };
            
            // Range constraint (>=1.0.0 <2.0.0)
            struct RangeConstraint : BaseConstraint {
                std::vector<std::unique_ptr<BaseConstraint>> constraints;
                
                bool satisfies(const Version& version) const override;
                std::string toString() const override;
            };
            
            // OR constraint (1.2.3 || 2.0.0)
            struct OrConstraint : BaseConstraint {
                std::vector<std::unique_ptr<BaseConstraint>> constraints;
                
                bool satisfies(const Version& version) const override;
                std::string toString() const override;
            };
            
            // Wildcard constraint (1.2.*, 1.*, *)
            struct WildcardConstraint : BaseConstraint {
                uint32_t major{0};
                uint32_t minor{0};
                bool majorWildcard{false};
                bool minorWildcard{false};
                bool patchWildcard{false};
                
                bool satisfies(const Version& version) const override;
                std::string toString() const override;
            };
            
            std::unique_ptr<BaseConstraint> constraint_;
            
        public:
            VersionConstraint() = default;
            VersionConstraint(const VersionConstraint& other);
            
            // Parse from string
            static VersionConstraint Parse(const std::string& constraint_str);
            
            // Check if a version satisfies this constraint
            bool satisfies(const Version& version) const;
            bool satisfies(const std::string& version_str) const;
            std::string toString() const;
            
        private:
            static std::unique_ptr<BaseConstraint> ParseConstraint(const std::string& str);
            static std::unique_ptr<BaseConstraint> ParseSimpleConstraint(const std::string& str);
            static std::unique_ptr<BaseConstraint> ParseWildcardConstraint(const std::string& str);
            
            // Helper functions
            static std::string Trim(const std::string& str);
            static std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
    };
}

namespace fmt {
    template<>
    struct formatter<Raindrop::Version> : formatter<string_view> {
        template<typename FormatContext>
        auto format(const Raindrop::Version& version, FormatContext& ctx) const -> decltype(ctx.out()) {
            return fmt::format_to(ctx.out(), "{}", version.str());
        }
    };
}
