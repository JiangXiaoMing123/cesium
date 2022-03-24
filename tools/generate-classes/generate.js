const fs = require("fs");
const getNameFromTitle = require("./getNameFromTitle");
const indent = require("./indent");
const lodash = require("lodash");
const NameFormatters = require("./NameFormatters");
const path = require("path");
const resolveProperty = require("./resolveProperty");
const unindent = require("./unindent");

function generate(options, schema, writers) {
  const {
    schemaCache,
    outputDir,
    readerOutputDir,
    config,
    namespace,
    readerNamespace,
    writerNamespace,
    extensions,
  } = options;

  const baseName = getNameFromTitle(config, schema.title);
  const prefix =
    schema.prefix && schema.prefix !== baseName ? schema.prefix : "";
  const name = prefix + baseName;
  const thisConfig = config.classes[schema.title] || {};

  console.log(`Generating ${name}`);

  schemaCache.pushContext(schema);

  let base = "CesiumUtility::ExtensibleObject";
  let baseSchemaRef;
  let baseSchema;
  if (schema.$ref) {
    baseSchemaRef = schema.$ref;
  } else if (schema.allOf && schema.allOf.length > 0 && schema.allOf[0].$ref) {
    baseSchemaRef = schema.allOf[0].$ref;
  }
  if (baseSchemaRef) {
    baseSchema = schemaCache.load(baseSchemaRef);
    base = getNameFromTitle(config, baseSchema.title);
  }

  const required = schema.required || [];

  let properties = Object.keys(schema.properties)
    .map((key) =>
      resolveProperty(
        schemaCache,
        config,
        schema,
        name,
        key,
        schema.properties[key],
        required,
        namespace,
        readerNamespace,
        writerNamespace
      )
    )
    .filter((property) => property !== undefined);

  // Filter out derived properties
  // TODO: this causes differences in MAXAR_mesh_variants, but perhaps good ones
  const derivedProperties = [];
  properties = properties.filter((property) => {
    if (baseSchema && baseSchema.properties[property.name]) {
      derivedProperties.push(property);
      return false;
    }
    return true;
  });

  const localTypes = lodash.uniq(
    lodash.flatten(properties.map((property) => property.localTypes))
  );

  schemaCache.popContext();

  const libraryName = `${namespace.replaceAll("::", "_").toUpperCase()}_API`;
  const includePath = namespace.split("::").join("/");

  let headers = lodash.uniq([
    `"${includePath}/Library.h"`,
    NameFormatters.getIncludeFromName(base, namespace),
    ...lodash.flatten(properties.map((property) => property.headers)),
  ]);

  // Prevent header from including itself for recursive types like Tile
  headers = headers.filter((header) => {
    return header !== `"${includePath}/${name}.h"`;
  });

  headers.sort();

  // prettier-ignore
  const header = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        #pragma once

        ${headers.map((header) => `#include ${header}`).join("\n")}

        namespace ${namespace} {
            /**
             * @brief ${schema.description ? schema.description : schema.title}
             */
            struct ${libraryName} ${name}${thisConfig.toBeInherited ? "Spec" : (thisConfig.isBaseClass ? "" : " final")} : public ${base} {
                static inline constexpr const char* TypeName = "${name}";
                ${thisConfig.extensionName ? `static inline constexpr const char* ExtensionName = "${thisConfig.extensionName}";` : ""}

                ${indent(localTypes.join("\n\n"), 16)}

                ${indent(
                  properties
                    .map((property) => formatProperty(property))
                    .filter(propertyText => propertyText !== undefined)
                    .join("\n\n"),
                  16
                )}
                ${createConstructor(name, derivedProperties, thisConfig, schema)}
            };
        }
    `;

  const headerOutputDir = path.join(
    outputDir,
    "generated",
    "include",
    includePath
  );
  fs.mkdirSync(headerOutputDir, { recursive: true });
  const headerOutputPath = path.join(
    headerOutputDir,
    `${name}${thisConfig.toBeInherited ? "Spec" : ""}.h`
  );
  fs.writeFileSync(headerOutputPath, unindent(header), "utf-8");

  let readerHeaders = lodash.uniq([
    NameFormatters.getReaderIncludeFromName(base, readerNamespace),
    `<${includePath}/${name}.h>`,
    ...lodash.flatten(properties.map((property) => property.readerHeaders)),
  ]);

  // Prevent header from including itself for recursive types like Tile
  readerHeaders = readerHeaders.filter((readerHeader) => {
    return readerHeader !== `"${name}JsonHandler.h"`;
  });

  readerHeaders.sort();

  const readerLocalTypes = lodash.uniq(
    lodash.flatten(properties.map((property) => property.readerLocalTypes))
  );

  const baseReader = NameFormatters.getReaderName(base, readerNamespace);

  // prettier-ignore
  const readerHeader = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        #pragma once

        ${readerHeaders.map((header) => `#include ${header}`).join("\n")}

        namespace CesiumJsonReader {
          class ExtensionReaderContext;
        }

        namespace ${readerNamespace} {
          class ${name}JsonHandler : public ${baseReader}${thisConfig.extensionName ? `, public CesiumJsonReader::IExtensionJsonHandler` : ""} {
          public:
            using ValueType = ${namespace}::${name};

            ${thisConfig.extensionName ? `static inline constexpr const char* ExtensionName = "${thisConfig.extensionName}";` : ""}

            ${name}JsonHandler(const CesiumJsonReader::ExtensionReaderContext& context) noexcept;
            void reset(IJsonHandler* pParentHandler, ${namespace}::${name}* pObject);

            virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

            ${thisConfig.extensionName ? `
            virtual void reset(IJsonHandler* pParentHandler, CesiumUtility::ExtensibleObject& o, const std::string_view& extensionName) override;

            virtual IJsonHandler* readNull() override {
              return ${baseReader}::readNull();
            };
            virtual IJsonHandler* readBool(bool b) override {
              return ${baseReader}::readBool(b);
            }
            virtual IJsonHandler* readInt32(int32_t i) override {
              return ${baseReader}::readInt32(i);
            }
            virtual IJsonHandler* readUint32(uint32_t i) override {
              return ${baseReader}::readUint32(i);
            }
            virtual IJsonHandler* readInt64(int64_t i) override {
              return ${baseReader}::readInt64(i);
            }
            virtual IJsonHandler* readUint64(uint64_t i) override {
              return ${baseReader}::readUint64(i);
            }
            virtual IJsonHandler* readDouble(double d) override {
              return ${baseReader}::readDouble(d);
            }
            virtual IJsonHandler* readString(const std::string_view& str) override {
              return ${baseReader}::readString(str);
            }
            virtual IJsonHandler* readObjectStart() override {
              return ${baseReader}::readObjectStart();
            }
            virtual IJsonHandler* readObjectEnd() override {
              return ${baseReader}::readObjectEnd();
            }
            virtual IJsonHandler* readArrayStart() override {
              return ${baseReader}::readArrayStart();
            }
            virtual IJsonHandler* readArrayEnd() override {
              return ${baseReader}::readArrayEnd();
            }
            virtual void reportWarning(const std::string& warning, std::vector<std::string>&& context = std::vector<std::string>()) override {
              ${baseReader}::reportWarning(warning, std::move(context));
            }
            ` : ""}

          protected:
            IJsonHandler* readObjectKey${name}(const std::string& objectType, const std::string_view& str, ${namespace}::${name}& o);

          private:
            ${indent(readerLocalTypes.join("\n\n"), 12)}
            ${namespace}::${name}* _pObject = nullptr;
            ${indent(
              properties
                .map((property) => formatReaderProperty(property))
                .join("\n"),
              12
            )}
          };
        }
  `;

  const readerHeaderOutputDir = path.join(readerOutputDir, "generated", "src");
  fs.mkdirSync(readerHeaderOutputDir, { recursive: true });

  const readerHeaderOutputPath = path.join(
    readerHeaderOutputDir,
    name + "JsonHandler.h"
  );
  fs.writeFileSync(readerHeaderOutputPath, unindent(readerHeader), "utf-8");

  const readerLocalTypesImpl = lodash.uniq(
    lodash.flatten(properties.map((property) => property.readerLocalTypesImpl))
  );

  const readerHeadersImpl = lodash.uniq([
    ...lodash.flatten(properties.map((property) => property.readerHeadersImpl)),
  ]);
  readerHeadersImpl.sort();

  function generateReaderOptionsInitializerList(properties, varName) {
    const initializerList = properties
      .filter((p) => p.readerType.toLowerCase().indexOf("jsonhandler") != -1)
      .map(
        (p) =>
          `_${p.cppSafeName}(${
            p.schemas && p.schemas.length > 0 ? varName : ""
          })`
      )
      .join(", ");
    return initializerList == "" ? "" : ", " + initializerList;
  }
  // prettier-ignore
  const readerImpl = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        #include "${name}JsonHandler.h"

        #include <${includePath}/${name}.h>
        ${readerHeadersImpl.map((header) => `#include ${header}`).join("\n")}
        #include <cassert>
        #include <string>

        namespace ${readerNamespace} {

        ${name}JsonHandler::${name}JsonHandler(const CesiumJsonReader::ExtensionReaderContext& context) noexcept : ${baseReader}(context)${generateReaderOptionsInitializerList(properties, 'context')} {}

        void ${name}JsonHandler::reset(CesiumJsonReader::IJsonHandler* pParentHandler, ${namespace}::${name}* pObject) {
          ${baseReader}::reset(pParentHandler, pObject);
          this->_pObject = pObject;
        }

        CesiumJsonReader::IJsonHandler* ${name}JsonHandler::readObjectKey(const std::string_view& str) {
          assert(this->_pObject);
          return this->readObjectKey${name}(${namespace}::${name}::TypeName, str, *this->_pObject);
        }

        ${thisConfig.extensionName ? `
        void ${name}JsonHandler::reset(CesiumJsonReader::IJsonHandler* pParentHandler, CesiumUtility::ExtensibleObject& o, const std::string_view& extensionName) {
          std::any& value =
              o.extensions.emplace(extensionName, ${namespace}::${name}())
                  .first->second;
          this->reset(
              pParentHandler,
              &std::any_cast<${namespace}::${name}&>(value));
        }
        ` : ""}

        CesiumJsonReader::IJsonHandler* ${name}JsonHandler::readObjectKey${name}(const std::string& objectType, const std::string_view& str, ${namespace}::${name}& o) {
          using namespace std::string_literals;

          ${properties.length > 0 ? `
          ${indent(
            properties
              .map((property) => formatReaderPropertyImpl(property))
              .join("\n"),
            10
          )}` : `(void)o;`}

          return this->readObjectKey${NameFormatters.removeNamespace(base)}(objectType, str, *this->_pObject);
        }

        ${indent(readerLocalTypesImpl.join("\n\n"), 8)}
        } // namespace ${readerNamespace}
  `;

  const writeForwardDeclaration = `struct ${name};`;

  const writeInclude = `#include <${includePath}/${name}.h>`;

  const writeDeclaration = `
        struct ${name}JsonWriter {
          using ValueType = ${namespace}::${name};

          ${
            thisConfig.extensionName
              ? `static inline constexpr const char* ExtensionName = "${thisConfig.extensionName}";`
              : ""
          }

          static void write(
              const ${namespace}::${name}& obj,
              CesiumJsonWriter::JsonWriter& jsonWriter,
              const CesiumJsonWriter::ExtensionWriterContext& context);
        };
  `;

  const writeJsonDeclaration = `
        void writeJson(
            const ${namespace}::${name}& obj,
            CesiumJsonWriter::JsonWriter& jsonWriter,
            const CesiumJsonWriter::ExtensionWriterContext& context);
  `;

  const writeDefinition = `
        void ${name}JsonWriter::write(
            const ${namespace}::${name}& obj,
            CesiumJsonWriter::JsonWriter& jsonWriter,
            const CesiumJsonWriter::ExtensionWriterContext& context) {
          writeJson(obj, jsonWriter, context);
        }
  `;

  let writeBaseJsonDefinition;
  let writeJsonDefinition;

  if (thisConfig.isBaseClass) {
    writeBaseJsonDefinition = `
        template <typename T>
        void write${NameFormatters.getWriterName(name)}(
            const T& obj,
            CesiumJsonWriter::JsonWriter& jsonWriter,
            const CesiumJsonWriter::ExtensionWriterContext& context) {

          ${indent(
            properties
              .map((property) => formatWriterPropertyImpl(property))
              .join("\n\n"),
            10
          )}

          write${NameFormatters.getWriterName(base)}(obj, jsonWriter, context);
        }
    `;

    writeJsonDefinition = `
        void writeJson(
            const ${namespace}::${name}& obj,
            CesiumJsonWriter::JsonWriter& jsonWriter,
            const CesiumJsonWriter::ExtensionWriterContext& context) {
          jsonWriter.StartObject();

          write${NameFormatters.getWriterName(name)}(obj, jsonWriter, context);

          jsonWriter.EndObject();
        }
    `;
  } else {
    writeJsonDefinition = `
        void writeJson(
            const ${namespace}::${name}& obj,
            CesiumJsonWriter::JsonWriter& jsonWriter,
            const CesiumJsonWriter::ExtensionWriterContext& context) {
          jsonWriter.StartObject();

          ${indent(
            properties
              .map((property) => formatWriterPropertyImpl(property))
              .join("\n\n"),
            10
          )}

          write${NameFormatters.getWriterName(base)}(obj, jsonWriter, context);

          jsonWriter.EndObject();
        }
    `;
  }

  const writeExtensionsRegistration = `
        ${
          extensions[schema.title]
            ? extensions[schema.title]
                .map((extension) => {
                  return `context.registerExtension<${namespace}::${name}, ${extension.className}JsonWriter>();`;
                })
                .join("\n")
            : ""
        }
  `;

  writers.push({
    writeInclude,
    writeForwardDeclaration,
    writeDeclaration,
    writeJsonDeclaration,
    writeDefinition,
    writeJsonDefinition,
    writeBaseJsonDefinition,
    writeExtensionsRegistration,
  });

  if (options.oneHandlerFile) {
    const readerSourceOutputPath = path.join(
      readerHeaderOutputDir,
      "GeneratedJsonHandlers.cpp"
    );
    fs.appendFileSync(readerSourceOutputPath, unindent(readerImpl), "utf-8");
  } else {
    const readerSourceOutputPath = path.join(
      readerHeaderOutputDir,
      name + "JsonHandler.cpp"
    );
    fs.writeFileSync(readerSourceOutputPath, unindent(readerImpl), "utf-8");
  }

  const schemas = lodash.flatten(
    properties.map((property) => property.schemas)
  );
  if (baseSchema && !base.includes("::")) {
    schemas.push(baseSchema);
  }

  return lodash.uniq(schemas);
}

function formatProperty(property) {
  if (!property.type) {
    return undefined;
  }

  let result = "";

  result += `/**\n * @brief ${property.briefDoc || property.cppSafeName}\n`;
  if (property.fullDoc) {
    result += ` *\n * ${property.fullDoc.split("\n").join("\n * ")}\n`;
  }

  result += ` */\n`;

  result += `${property.type} ${property.cppSafeName}`;

  if (property.defaultValue !== undefined) {
    result += " = " + property.defaultValue;
  } else if (property.needsInitialization) {
    result += " = " + property.type + "()";
  }

  result += ";";

  return result;
}

function formatReaderProperty(property) {
  return `${property.readerType} _${property.cppSafeName};`;
}

function formatReaderPropertyImpl(property) {
  return `if ("${property.name}"s == str) return property("${property.name}", this->_${property.cppSafeName}, o.${property.cppSafeName});`;
}

function formatWriterPropertyImpl(property) {
  let result = "";

  const type = property.type;
  const defaultValue = property.defaultValueWriter || property.defaultValue;

  const isId = property.requiredId !== undefined;
  const isRequiredEnum = property.requiredEnum === true;
  const isVector = type.startsWith("std::vector");
  const isMap = type.startsWith("std::unordered_map");
  const isOptional = type.startsWith("std::optional");

  // Somewhat opinionated but it's helpful to see byteOffset: 0 in accessors and bufferViews
  const requiredPropertyOverride = ["byteOffset"];

  const hasDefaultValueGuard =
    !isId &&
    !isRequiredEnum &&
    !requiredPropertyOverride.includes(property.cppSafeName) &&
    defaultValue !== undefined;
  const hasDefaultVectorGuard = hasDefaultValueGuard && isVector;
  const hasEmptyGuard = isVector || isMap;
  const hasOptionalGuard = isOptional;
  const hasNegativeIndexGuard = isId;
  const hasGuard =
    hasDefaultValueGuard ||
    hasEmptyGuard ||
    hasOptionalGuard ||
    hasNegativeIndexGuard;

  if (hasDefaultVectorGuard) {
    result += `static const ${type} ${property.cppSafeName}Default = ${defaultValue};\n`;
    result += `if (obj.${property.cppSafeName} != ${property.cppSafeName}Default) {\n`;
  } else if (hasDefaultValueGuard) {
    result += `if (obj.${property.cppSafeName} != ${defaultValue}) {\n`;
  } else if (hasEmptyGuard) {
    result += `if (!obj.${property.cppSafeName}.empty()) {\n`;
  } else if (hasNegativeIndexGuard) {
    result += `if (obj.${property.cppSafeName} > -1) {\n`;
  } else if (hasOptionalGuard) {
    result += `if (obj.${property.cppSafeName}.has_value()) {\n`;
  }

  result += `jsonWriter.Key("${property.name}");\n`;
  result += `writeJson(obj.${property.cppSafeName}, jsonWriter, context);\n`;

  if (hasGuard) {
    result += "}\n";
  }

  return result;
}

function gatherInitializers(name, property, initializers) {
  if (property.default !== undefined) {
    initializers.push(
      `${name} = ${
        typeof property.default === "string"
          ? `"${property.default.toString()}"`
          : property.default
      };`
    );
    return;
  }

  const nestedProperties = property.properties;
  for (const nestedName in nestedProperties) {
    const nestedProperty = nestedProperties[nestedName];
    gatherInitializers(`${name}.${nestedName}`, nestedProperty, initializers);
  }
}

function createConstructor(name, derivedProperties, thisConfig, schema) {
  const className = thisConfig.toBeInherited ? `${name}Spec` : name;

  let constructor = "";

  if (derivedProperties.length > 0) {
    // The derived class's constructor initializes member variables of the base case for derived properties
    constructor = `
    ${className}() {
      ${indent(
        derivedProperties
          .map((derivedProperty) => {
            const initializers = [];
            gatherInitializers(
              derivedProperty.name,
              schema.properties[derivedProperty.name],
              initializers
            );
            return initializers.join("\n");
          })
          .join("\n"),
        16
      )}
    }
  `;
  } else if (thisConfig.toBeInherited) {
    constructor = `${className}() = default;`;
  }

  if (thisConfig.toBeInherited) {
    return `
    private:
      /**
       * @brief This class is not meant to be instantiated directly. Use {@link ${name}} instead.
       */
      ${constructor}
      friend struct ${name};
  `;
  }

  return constructor;
}

module.exports = generate;
