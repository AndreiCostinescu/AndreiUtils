//
// Created by Andrei on 31.01.26.
//

#include <AndreiUtils/classes/TemplateParser.h>
#include <AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsVector.hpp>
#include <sstream>

using namespace AndreiUtils;

std::string TemplateType::toString() const {
    if (!this->isTemplate()) {
        return this->baseType;
    }
    return this->baseType + "<" + AndreiUtils::printVectorToString<TemplateType>(
            this->templateArgs, [](auto const &elem) { return elem.toString(); }) + ">";
}

bool TemplateType::isTemplate() const { return this->typeDefinedWithTemplateChars; }

TemplateType TemplateParser::parse(std::string const &typeStr, std::string const &templateCharacters) {
    if (typeStr.empty()) {
        throw std::invalid_argument("Empty type string");
    }
    if (templateCharacters.size() != 2) {
        throw std::invalid_argument(
                "Invalid amount of template characters: " + std::to_string(templateCharacters.size()) +
                ". Expected 2 to interpret as the starting and closing template character.");
    }

    TemplateParser parser(typeStr, templateCharacters[0], templateCharacters[1]);
    TemplateType result = parser.parseType();

    parser.skipWhitespace();
    if (parser.pos < parser.input.length()) {
        throw std::runtime_error(
                "Unexpected characters at position " + std::to_string(parser.pos) + ": '" +
                parser.input.substr(parser.pos) + "'");
    }

    return result;
}

TemplateParser::TemplateParser(std::string str, char const &startTemplateCharacter, char const &endTemplateCharacter) :
        input(std::move(str)), pos(0), startTemplateCharacter(startTemplateCharacter),
        endTemplateCharacter(endTemplateCharacter) {}

TemplateType TemplateParser::parseType() {
    TemplateType result;
    this->skipWhitespace();

    // Parse base type name
    std::ostringstream baseName;
    while (this->pos < this->input.length()) {
        char c = peek();

        // Valid characters in type name: alphanumeric, underscore, colon (for ::), space
        if (this->isValidCharacterOfName(c)) {
            baseName << consume();
        } else if (std::isspace(c)) {
            // Handle spaces in type names (e.g., "unsigned int")
            this->skipWhitespace();  // consumes multiple spaces

            // Check if next character is part of the type name or a delimiter
            if (this->pos < this->input.length()) {
                char next = this->peek();
                if (this->isValidCharacterOfName(next)) {
                    // Space is part of the type name
                    baseName << ' ';
                } else {
                    // Space before delimiter, stop parsing base name
                    break;
                }
            }
        } else {
            break;
        }
    }

    result.baseType = baseName.str();

    // Trim trailing whitespace from base_type
    result.baseType = AndreiUtils::trimEnd(result.baseType, " \t\n\r");
    if (result.baseType.empty()) {
        throw std::runtime_error("Expected type name at position " + std::to_string(this->pos));
    }

    this->skipWhitespace();

    // Check for template arguments
    result.typeDefinedWithTemplateChars = this->peek() == this->startTemplateCharacter;
    if (result.typeDefinedWithTemplateChars) {
        result.templateArgs = this->parseTemplateArgs();
    }

    return result;
}

std::vector<TemplateType> TemplateParser::parseTemplateArgs() {
    std::vector<TemplateType> args;

    if (!this->expect(this->startTemplateCharacter)) {
        throw std::runtime_error("Expected '" + std::string(1, this->startTemplateCharacter) + "' at position " +
                                 std::to_string(this->pos));
    }

    this->skipWhitespace();

    // Handle empty template arguments (edge case)
    if (this->peek() == this->endTemplateCharacter) {
        this->consume();
        return args;
    }

    // Parse template arguments separated by commas
    while (this->pos < this->input.length()) {
        args.push_back(this->parseType());
        this->skipWhitespace();

        char c = peek();
        if (c == ',') {
            this->consume();
            this->skipWhitespace();
        } else if (c == this->endTemplateCharacter) {
            break;
        } else {
            throw std::runtime_error(
                    "Expected ',' or '" + std::string(1, this->endTemplateCharacter) + "' at position " +
                    std::to_string(this->pos) + ", got '" + std::string(1, c) + "'");
        }
    }

    if (this->peek() == '\0' && this->pos >= this->input.length()) {
        // Reached end without closing endTemplateCharacter
        throw std::runtime_error("Unclosed template argument list");
    }
    this->consume();  // move past the endTemplateCharacter

    return args;
}

void TemplateParser::skipWhitespace() {
    while (this->pos < this->input.length() && std::isspace(this->input[this->pos])) {
        ++this->pos;
    }
}

char TemplateParser::peek() const {
    if (this->pos >= this->input.length()) {
        return '\0';
    }
    return this->input[this->pos];
}

char TemplateParser::consume() {
    if (this->pos >= this->input.length()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return this->input[this->pos++];
}

bool TemplateParser::expect(char const c) {
    if (this->peek() == c) {
        this->consume();
        return true;
    }
    return false;
}

bool TemplateParser::isValidCharacterOfName(char const c) const {
    return std::isalnum(c) || c == '_' || c == ':';
}
