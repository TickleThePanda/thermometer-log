terraform {
  required_providers {
    aws = {
      source = "hashicorp/aws"
    }
  }
}

provider "aws" {
  profile = "terraform"
  region = "eu-west-2"
}

resource "aws_dynamodb_table" "thermometer-log" {
  name = "ThermometerLog"
  billing_mode = "PROVISIONED"
  read_capacity = "5"
  write_capacity = "5"

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

resource "aws_iam_user" "ThermometerLogger" {
  name = "ThermometerLogger"
}

resource "aws_iam_access_key" "ThermometerLogger" {
  user = aws_iam_user.ThermometerLogger.name
}

data "aws_iam_policy_document" "ThermometerLogger" {
  statement {
		actions = [
			"dynamodb:BatchGetItem",
			"dynamodb:BatchWriteItem",
			"dynamodb:PutItem",
			"dynamodb:DescribeTable",
			"dynamodb:DeleteItem",
			"dynamodb:GetItem",
			"dynamodb:Scan",
			"dynamodb:Query",
			"dynamodb:UpdateItem"
    ]

		resources = [
		  aws_dynamodb_table.thermometer-log.arn
		]
  }
}

resource "aws_iam_policy" "ThermometerLogger" {
	name = "thermometer_logger"
	policy = data.aws_iam_policy_document.ThermometerLogger.json
}

resource "aws_iam_user_policy_attachment" "ThermometerLogger" {
  user       = aws_iam_user.ThermometerLogger.name
  policy_arn = aws_iam_policy.ThermometerLogger.arn
}

