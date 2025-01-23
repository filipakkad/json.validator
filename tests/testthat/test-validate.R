describe("validate_json_schema", {

  valid_json <- readLines("./data/example_valid_json.json")
  invalid_json <- readLines("./data/example_invalid_json.json")
  schema_json <- readLines("./data/example_json_schema.json")

  it("validates data" , {
    result <- json.validator::validate_json_schema(valid_json, schema_json)
    expect_true(result)
  })

  it("throws error if broken" , {
    expect_error(
      json.validator::validate_json_schema(invalid_json, schema_json)
    )
  })
})
