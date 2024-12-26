#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ToolUtil.hpp"

/**
 * @brief Writes everything to the files.
 */
void defineAst(const std::string&, const std::string&, const std::vector<std::string_view>&, const std::vector<std::string_view>& = std::vector<std::string_view>());

/**
 * @brief Writes a class of a given type.
 */
void defineType(std::ofstream&, std::string_view, std::string_view, std::string_view);

/**
 * @brief Write the visitor class.
 */
void defineVisitor(std::ofstream&, std::string_view, const std::vector<std::string_view>&);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: generate_ast <output_directory>" << std::endl;
        exit(64);
    }

    std::string outputDir = argv[1];

    // Generate expression code
    std::vector<std::string_view> exprTypes{
        "Assign   : Token name, Expr* value",
        "Binary   : Expr* left, Token oper, Expr* right",
        "Call     : Expr* callee, Token paren, vector<Expr*> arguments",
        "Get      : Expr* object, Token name",
        "Grouping : Expr* expression",
        "Literal  : Object value",
        "Logical  : Expr* left, Token oper, Expr* right",
        "Set      : Expr* object, Token name, Expr* value",
        "Super    : Token keyword, Token method",
        "This     : Token keyword",
        "Unary    : Token oper, Expr* right",
        "Variable : Token name",
    };
    defineAst(outputDir, "Expr", exprTypes);

    // Generate statement code
    std::vector<std::string_view> stmtTypes{
        "Block      : vector<Stmt*> statements",
        "Class      : Token name, Variable* superclass, vector<Function*> methods",
        "Expression : Expr* expression",
        "Function   : Token name, vector<Token> params, vector<Stmt*> body",
        "If         : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
        "Print      : Expr* expression",
        "Return     : Token keyword, Expr* value",
        "Var        : Token name, Expr* initializer",
        "While      : Expr* condition, Stmt* body",
    };
    std::vector<std::string_view> stmtIncludes{
        "\"Expr.hpp\"",
    };
    defineAst(outputDir, "Stmt", stmtTypes, stmtIncludes);
}

/**
 * @brief Fixes the type of the given field. Ex: Expr* becomes std::shared_ptr<Expr>, Object becomes std::any.
 */
std::string fixType(std::string_view field, bool isParam = false) {
    std::ostringstream oss;

    std::string_view type = split(field, " ")[0];
    std::string_view name = split(field, " ")[1];

    bool addRef = false;

    if (type.find("vector") != std::string_view::npos) {
        if (isParam) {
            oss << "const ";
            addRef = true;
        }
        oss << "std::";
    }

    size_t pos;
    if ((pos = type.find('*')) != std::string_view::npos) {
        // Of the form Type*
        if (pos == type.size() - 1) {
            type.remove_suffix(1);
            oss << "std::shared_ptr<" << type << ">";
        }
        // Of the form some_template<Type*>
        else {
            size_t oPos = pos;
            size_t len = 0;
            while (pos > 1 && type[pos - 1] != '<') {
                --pos;
                ++len;
            }
            oss << type.substr(0, pos) << "std::shared_ptr<" << type.substr(pos, len) << ">" << type.substr(oPos + 1, std::string_view::npos);
        }
    }
    else if (type == "Object") {
        oss << "std::any";
    }
    else if (type == "Token" && isParam) {
        oss << "const " << type;
        addRef = true;
    }
    else {
        oss << type;
    }

    if (addRef) {
        oss << "&";
    }

    oss << " " << name;

    return oss.str();
}

void defineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string_view>& types,
               const std::vector<std::string_view>& extraIncludes) {
    std::string path = outputDir + "/" + baseName + ".hpp";

    std::ofstream writer(path);

    // Header guard
    std::string headerGuard = "CPPLOX_" + toUpper(baseName) + "_HPP";
    writer << "#ifndef " << headerGuard << "\n";
    writer << "#define " << headerGuard << "\n\n";

    // Includes
    writer << "#include <any>\n"
              "#include <memory>\n"
              "#include <vector>\n"
              "#include \"../include/Token.hpp\"\n";
    for (std::string_view incl : extraIncludes) {
        writer << "#include " << incl << "\n";
    }
    writer << "\n";

    // Class forward declarations so pointers to them can be used
    for (std::string_view type : types) {
        std::string_view className = trim(split(type, ":")[0]);
        writer << "class " << className << ";\n";
    }
    writer << '\n';

    // Visitor class
    defineVisitor(writer, baseName, types);

    // Base class
    writer << "class " << baseName << " {\n";
    writer << "\tpublic:\n";
    writer << "\t virtual std::any accept(" << baseName << "Visitor& visitor) = 0;\n};\n\n";

    // Derived classes
    for (auto type : types) {
        std::string_view className = trim(split(type, ":")[0]);
        std::string_view fields = trim(split(type, ":")[1]);
        defineType(writer, baseName, className, fields);
    }

    writer << "#endif" << std::endl;
}

void defineVisitor(std::ofstream& writer, std::string_view baseName, const std::vector<std::string_view>& types) {
    writer << "struct " << baseName << "Visitor {\n";

    for (auto type : types) {
        auto typeName = trim(split(type, ":")[0]);
        writer << "\tvirtual std::any visit" << typeName << baseName << "(std::shared_ptr<" << typeName << "> "
               << toLower(baseName) << ") = 0;\n";
    }

    // Virtual destructor
    writer << "\tvirtual ~" << baseName << "Visitor() = default;\n";

    writer << "};\n\n";
}

void defineType(std::ofstream& writer, std::string_view baseName, std::string_view className, std::string_view fieldList) {
    writer << "class " << className << " : public " << baseName << ", public std::enable_shared_from_this<" << className << "> {\n";

    writer << "\tpublic:\n";

    // Constructor
    writer << "\t" << className << "(";

    auto fields = split(fieldList, ", ");
    writer << fixType(fields[0], true);

    for (size_t i = 1; i < fields.size(); ++i) {
        writer << ", " << fixType(fields[i], true);
    }
    writer << ") : ";

    // Member initializer list
    std::string_view name = split(fields[0], " ")[1];
    writer << name << "(" << name << ")";
    for (size_t i = 1; i < fields.size(); ++i) {
        name = split(fields[i], " ")[1];
        writer << ", " << name << "(" << name << ")";
    }
    writer << "{}" << "\n\n";

    // Visitor pattern implementation
    writer << "\tstd::any accept(" << baseName << "Visitor& visitor) override {\n";
    writer << "\t\treturn visitor.visit" << className << baseName << "(shared_from_this());\n";
    writer << "\t}\n\n";

    // writer << "\tprivate:\n";

    // Fields
    for (auto field : fields) {
        writer << "\tconst " << fixType(field) << ";\n";
    }

    writer << "};\n\n";
}
