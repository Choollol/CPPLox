#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ToolUtil.hpp"

/**
 * @brief Writes everything to the file.
 */
void defineAst(const std::string&, const std::string&, const std::vector<std::string_view>&);

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

    std::vector<std::string_view> types{
        "Binary   : Expr* left, Token oper, Expr* right",
        "Grouping : Expr* expression",
        "Literal  : Object value",
        "Unary    : Token oper, Expr* right",
    };
    defineAst(outputDir, "Expr", types);
}

/**
 * @brief Fixes the type of the given field. Ex: Expr* becomes std::shared_ptr<Expr>, Object becomes std::any.
 */
std::string fixType(std::string_view field, bool isParam = false) {
    std::string out;

    std::string_view type = split(field, " ")[0];
    std::string_view name = split(field, " ")[1];

    if (type.back() == '*') {
        type.remove_suffix(1);
        out += "std::shared_ptr<";
        out += type;
        out += ">";
    }
    else if (type == "Object") {
        out += "std::any";
    }
    else if (type == "Token" && isParam) {
        out += "const ";
        out += type;
        out += "&";
    }
    else {
        out += type;
    }

    out += " ";
    out += name;

    return out;
}

void defineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string_view>& types) {
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
              "#include \"../include/Token.hpp\"\n\n";

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
    writer << ") :\n";

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
