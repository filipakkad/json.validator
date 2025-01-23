#include <Rcpp.h>
#include <nlohmann/json.hpp>
#include "include/nhlomann/json-schema.hpp"

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

    nlohmann::json_schema::json_validator validator;
    validator.set_root_schema(schema_obj);
    validator.validate(json_obj);

    return true;
  } catch (const std::exception &e) {
    if (throw_error) {
      Rcpp::stop(std::string("Validation error: ") + e.what());
    }
    return false;
  }
}