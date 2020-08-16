import AWS from 'aws-sdk';

const AWS_ACCESS_KEY = process.env.THERMOMETER_AWS_ACCESS_KEY;
const AWS_SECRET_KEY = process.env.THERMOMETER_AWS_SECRET_KEY;
const AWS_REGION = process.env.THERMOMETER_AWS_REGION;

AWS.config.update({
  'accessKeyId': AWS_ACCESS_KEY,
  'secretAccessKey': AWS_SECRET_KEY,
  'region': AWS_REGION
})

const tableName = "ThermometerLog";
const db = new AWS.DynamoDB.DocumentClient();

function extractTime(entry) {
  const date = entry.RoomDay.substring(entry.RoomDay.length - 10);
  const time = entry.Time;

  return date + 'T' + time;
}

module.exports = class ThermometerLog {

  async updateEntry({ room, date, time, temperature }) {

    console.log("update:", room, date, time, temperature);

    const item = Object.assign({
      "RoomDay": room + "-" + date,
      "Time": time
    }, {
      "Temperature": temperature
    });

    return await db.put({
      "TableName": tableName,
      "Item": item
    }).promise();
  }

  async getEntries({
    roomId, date
  }) {

		const roomDay = roomId + '-' + date;

    const result = await db.query({
      "TableName": tableName,
      "KeyConditionExpression": "#RoomDay = :roomDay",
      "ExpressionAttributeNames": {
        "#RoomDay": "RoomDay"
      },
      "ExpressionAttributeValues": {
        ":roomDay": roomDay
      }
    }).promise();

    return result.Items.map(i => ({
        temperature: i.Temperature,
        time: extractTime(i)
      })
    );

  }

}


