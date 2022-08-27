terraform {
  cloud {
    organization = "TickleThePanda"
    workspaces {
      name = "thermometer-log"
    }
  }
  required_providers {
    aws = {
      source = "hashicorp/aws"
    }
  }
}

provider "aws" {
  default_tags {
    tags = {
      TerraformManaged = "True"
    }
  }
  region = "eu-west-2"
}

provider "aws" {
  default_tags {
    tags = {
      TerraformManaged = "True"
    }
  }
  region = "us-west-1"
  alias = "us-west-1"
}

module "thermometer-log-table-eu-west-2" {
  source = "./thermometer-log-table"

  # use default provider
}

module "thermometer-log-table-us-west-1" {
  source = "./thermometer-log-table"

  providers = {
    aws = aws.us-west-1
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
		  module.thermometer-log-table-eu-west-2.table-arn,
		  module.thermometer-log-table-us-west-1.table-arn
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

