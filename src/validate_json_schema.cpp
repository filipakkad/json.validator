#include <Rcpp.h>
#include <nlohmann/json.hpp>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

// [[Rcpp::export]]
bool validate_json_schema(
  Rcpp::StringVector json_str,
  Rcpp::StringVector schema_str,
  bool throw_error = true
) {
  if (json_str.size() == 0 || schema_str.size() == 0) {
    if (throw_error) {
      Rcpp::stop("Error: Empty input string");
    }
    return false;
  }

  try {
    // Concatenate JSON strings
    std::string json_string;
    std::string schema_string;
    for (int i = 0; i < json_str.size(); ++i) {
      json_string += Rcpp::as<std::string>(json_str[i]);
    }

    for (int i = 0; i < schema_str.size(); ++i) {
      schema_string += Rcpp::as<std::string>(schema_str[i]);
    }

    nlohmann::json json_obj = nlohmann::json::parse(json_string);
    nlohmann::json schema_obj = nlohmann::json::parse(schema_string);

    // Create Valijson schema
    valijson::Schema schema;
    valijson::SchemaParser parser;
    valijson::adapters::NlohmannJsonAdapter schemaAdapter(schema_obj);
    parser.populateSchema(schemaAdapter, schema);

    // Validate JSON against schema
    valijson::Validator validator;
    valijson::adapters::NlohmannJsonAdapter targetAdapter(json_obj);

    valijson::ValidationResults results;
    if (!validator.validate(schema, targetAdapter, &results)) {
      if (throw_error) {
        std::string error_message = "Validation failed. Errors:\n";
        valijson::ValidationResults::Error error;
        unsigned int error_num = 1;
        while (results.popError(error)) {
          error_message += "  " + std::to_string(error_num) + ": " + error.description + "\n";
          ++error_num;
        }
        Rcpp::stop(error_message);
      }
      return false;
    }

    return true;
  } catch (const std::exception &e) {
    if (throw_error) {
      Rcpp::stop(std::string("Validation error: ") + e.what());
    }
    return false;
  }
}