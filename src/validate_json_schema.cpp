#include <Rcpp.h>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>

// [[Rcpp::export]]
bool validate_json_schema(std::string json_str, std::string schema_str) {

  try {
    nlohmann::json json_obj = nlohmann::json::parse(json_str);
    nlohmann::json schema_obj = nlohmann::json::parse(schema_str);

    nlohmann::json_schema::json_validator validator;
    validator.set_root_schema(schema_obj); // Load schema

    validator.validate(json_obj); // Validate JSON
    return true; // Validation successful
  } catch (const std::exception &e) {
    Rcpp::Rcerr << "Validation error: " << e.what() << std::endl;
    return false; // Validation failed
  }
}
