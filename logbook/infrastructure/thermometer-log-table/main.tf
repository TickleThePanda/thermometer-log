resource "aws_dynamodb_table" "thermometer-log" {
  name = "ThermometerLog"
  billing_mode = "PROVISIONED"
  read_capacity = "5"
  write_capacity = "5"
  stream_enabled = false

  hash_key = "RoomDay"
  range_key = "Time"

  attribute {
    name = "RoomDay"
    type = "S"
  }

  attribute {
    name = "Time"
    type = "S"
  }
}

output "table-arn" {
  value = aws_dynamodb_table.thermometer-log.arn
}

