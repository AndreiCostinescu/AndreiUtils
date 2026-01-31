//
// Created by Andrei on 31.01.26.
//

#ifndef ANDREIUTILS_TEMPLATEPARSER_H
#define ANDREIUTILS_TEMPLATEPARSER_H

#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace AndreiUtils {
    // Represents a parsed template type
    struct TemplateType {
        std::string baseType;                    // The base type name (e.g., "std::vector")
        std::vector<TemplateType> templateArgs;  // Template arguments (can be nested)
        bool typeDefinedWithTemplateChars;       // Helps disambiguate between "Type" and "Type<>"

        // Convert back to string representation
        [[nodiscard]] std::string toString() const;

        // Check if this is a template type (has template arguments)
        [[nodiscard]] bool isTemplate() const;
    };

    class TemplateParser {
    public:
        // Parse a template type string
        static TemplateType parse(std::string const &typeStr, std::string const &templateCharacters = "<>");

    private:
        explicit TemplateParser(std::string str, char const &startTemplateCharacter, char const &endTemplateCharacter);

        TemplateType parseType();

        std::vector<TemplateType> parseTemplateArgs();

        void skipWhitespace();

        // looks at the next character without moving on
        [[nodiscard]] char peek() const;

        // moves on to next character
        char consume();

        // peeks and if the character matches, moves on to next character
        bool expect(char c);

        [[nodiscard]] bool isValidCharacterOfName(char c) const;

        std::string input;
        size_t pos;
        char startTemplateCharacter, endTemplateCharacter;  // default '<' and '>'
    };
} // AndreiUtils

#endif //ANDREIUTILS_TEMPLATEPARSER_H
