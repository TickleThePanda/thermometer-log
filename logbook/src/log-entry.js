import AWS from 'aws-sdk';
import moment from 'moment';

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

async function updateEntry({ room, date, time, temperature }) {

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

module.exports = async (req, res) => {
  const temperature = req.body.temperature;
  const room = req.query.roomId;
  console.log(req.query)
  console.log("temperature in " + room + ": " + temperature);
  
  const now = moment();

  const date = now.format("YYYY-MM-DD");
  const time = now.format("HH:mm:ss");

	await updateEntry({
    room, temperature,
    date, time
  })

  res.status(204).send();
}

