#include "../include/Scanner.hpp"

#include "../include/Error.hpp"
#include "../include/Token.hpp"

Scanner::Scanner(const std::string& s) : source(s), sourceLen(s.size()) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::LOX_EOF, "", nullptr, line);
    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        // Single-character tokens
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case '*':
            addToken(TokenType::STAR);
            break;
        // Possibly double-character tokens
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                singleLineComment();
            }
            else if (match('*')) {
                blockComment();
            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
        // Whitespace
        case ' ':
        case '\r':
        case '\t':
            // Ignore
            break;
        // Newline
        case '\n':
            ++line;
            break;
        // String literals
        case '"':
            string();
            break;
        default:
            // Number literals
            if (isdigit(c)) {
                number();
            }
            // Identifiers
            else if (isAlphaOrUscore(c)) {
                identifier();
            }
            // Error
            else {
                error(line, "Unexpected character.");
            }
            break;
    }
}

void Scanner::string() {
    // Read until end of string or file is hit
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            ++line;
        }
        advance();
    }

    // Handle unterminated string error
    if (isAtEnd()) {
        error(line, "Unterminated string.");
    }

    // Advance past closing "
    advance();

    addToken(TokenType::STRING, source.substr(start + 1, current - 2 - start));
}

void Scanner::number() {
    // Advance through the number
    while (isdigit(peek())) {
        advance();
    }

    // Check for fractional part
    if (peek() == '.' && isdigit(peekNext())) {
        // Skip the dot
        advance();

        // Advance through fractional part
        while (isdigit(peek())) {
            advance();
        }
    }

    addToken(TokenType::NUMBER, stod(source.substr(start, current)));
}

void Scanner::identifier() {
    // Advance through identifier
    while (isValidIdentifierChar(peek())) {
        advance();
    }

    // Default to be a user-defined identifier
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords.find(source.substr(start, current - start));
    // Identifier was found to be a keyword
    if (it != keywords.end()) {
        type = it->second;
    }
    addToken(type);
}

inline void Scanner::singleLineComment() {
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
}

void Scanner::blockComment() {
    // Advance until block comment is closed or end of source is hit
    while (!isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            break;
        }
        else if (peek() == '\n') {
            ++line;
        }
        advance();
    }
    // Handle unterminated block comment error
    if (isAtEnd()) {
        error(line, "Unterminated block comment.");
    }
}

inline char Scanner::advance() { return source[current++]; }

inline bool Scanner::isAtEnd() { return current >= sourceLen; }

inline void Scanner::addToken(TokenType type) { addToken(type, nullptr); }

inline void Scanner::addToken(TokenType type, std::any literal) {
    tokens.emplace_back(type, source.substr(start, current - start), literal, line);
}

bool Scanner::match(char c) {
    if (isAtEnd() || source[current] != c) {
        return false;
    }
    ++current;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) {
        return '\0';
    }
    return source[current];
}

char Scanner::peekNext() {
    if (current + 1 > sourceLen) {
        return '\0';
    }
    return source[current + 1];
}

const std::map<std::string, TokenType> Scanner::keywords = {{"and", TokenType::AND},
                                                            {"class", TokenType::CLASS},
                                                            {"else", TokenType::ELSE},
                                                            {"false", TokenType::FALSE},
                                                            {"fun", TokenType::FUN},
                                                            {"for", TokenType::FOR},
                                                            {"if", TokenType::IF},
                                                            {"nil", TokenType::NIL},
                                                            {"or", TokenType::OR},
                                                            {"print", TokenType::PRINT},
                                                            {"return", TokenType::RETURN},
                                                            {"super", TokenType::SUPER},
                                                            {"this", TokenType::THIS},
                                                            {"true", TokenType::TRUE},
                                                            {"var", TokenType::VAR},
                                                            {"while", TokenType::WHILE}};