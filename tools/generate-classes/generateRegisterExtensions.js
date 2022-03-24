const fs = require("fs");
const path = require("path");

const getNameFromTitle = require("./getNameFromTitle");
const unindent = require("./unindent");

function generateRegisterExtensions(options) {
  const {
    readerOutputDir,
    writerOutputDir,
    config,
    namespace,
    readerNamespace,
    writerNamespace,
    rootSchema,
    extensions,
  } = options;

  const classNames = [];
  const extensionClassNames = [];
  const registrations = [];

  for (const title in extensions) {
    const className = getNameFromTitle(config, title);
    classNames.push(className);

    const extensionsList = extensions[title];
    for (const extension of extensionsList) {
      const extensionClassName = extension.className;
      extensionClassNames.push(extensionClassName);
      registrations.push({
        className,
        extensionClassName,
      });
    }
  }

  const includePath = namespace.split("::").join("/");

  const rootSchemaName = getNameFromTitle(config, rootSchema.title);

  const filename = "registerExtensions";

  const readerHeader = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        #pragma once
        
        namespace CesiumJsonReader {
        class ExtensionReaderContext;
        } // namespace CesiumJsonReader
        
        namespace ${readerNamespace} {
        void registerExtensions(CesiumJsonReader::ExtensionReaderContext& context);
        }
  `;

  const readerImplementation = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        
        #include "${filename}.h"
        
        #include <CesiumJsonReader/ExtensionReaderContext.h>

        ${classNames
          .map((className) => {
            return `#include <${includePath}/${className}.h>`;
          })
          .join("\n")}
        
        ${extensionClassNames
          .map((extensionClassName) => {
            return `#include "${extensionClassName}JsonHandler.h"`;
          })
          .join("\n")}  

        namespace ${readerNamespace} {
        
        void registerExtensions(CesiumJsonReader::ExtensionReaderContext& context) {
          (void)context;
          ${registrations
            .map((registration) => {
              return `context.registerExtension<${namespace}::${registration.className}, ${registration.extensionClassName}JsonHandler>();`;
            })
            .join("\n")}
        }
        } // namespace ${readerNamespace}
  `;

  const writerHeader = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        #pragma once
        
        namespace CesiumJsonWriter {
        class ExtensionWriterContext;
        } // namespace CesiumJsonWriter
        
        namespace ${writerNamespace} {
        void registerExtensions(CesiumJsonWriter::ExtensionWriterContext& context);
        }
  `;

  const writerImplementation = `
        // This file was generated by generate-classes.
        // DO NOT EDIT THIS FILE!
        
        #include "${filename}.h"
        
        #include "${rootSchemaName}JsonWriter.h"

        #include <CesiumJsonWriter/ExtensionWriterContext.h>

        ${classNames
          .map((className) => {
            return `#include <${includePath}/${className}.h>`;
          })
          .join("\n")}
        
        ${extensionClassNames
          .map((extensionClassName) => {
            return `#include <${includePath}/${extensionClassName}.h>`;
          })
          .join("\n")}  
  
        namespace ${writerNamespace} {
        
        void registerExtensions(CesiumJsonWriter::ExtensionWriterContext& context) {
          (void)context;
          ${registrations
            .map((registration) => {
              return `context.registerExtension<${namespace}::${registration.className}, ${registration.extensionClassName}JsonWriter>();`;
            })
            .join("\n")}
        }
        } // namespace ${writerNamespace}
  `;

  const readerHeaderOutputDir = path.join(readerOutputDir, "generated", "src");
  fs.mkdirSync(readerHeaderOutputDir, { recursive: true });
  const readerHeaderOutputPath = path.join(
    readerHeaderOutputDir,
    `${filename}.h`
  );
  const readerImplementationOutputPath = path.join(
    readerHeaderOutputDir,
    `${filename}.cpp`
  );
  fs.writeFileSync(readerHeaderOutputPath, unindent(readerHeader), "utf-8");
  fs.writeFileSync(
    readerImplementationOutputPath,
    unindent(readerImplementation),
    "utf-8"
  );

  const writerHeaderOutputDir = path.join(writerOutputDir, "generated", "src");
  fs.mkdirSync(writerHeaderOutputDir, { recursive: true });
  const writerHeaderOutputPath = path.join(
    writerHeaderOutputDir,
    `${filename}.h`
  );
  const writerImplementationOutputPath = path.join(
    writerHeaderOutputDir,
    `${filename}.cpp`
  );
  fs.writeFileSync(writerHeaderOutputPath, unindent(writerHeader), "utf-8");
  fs.writeFileSync(
    writerImplementationOutputPath,
    unindent(writerImplementation),
    "utf-8"
  );
}

module.exports = generateRegisterExtensions;
