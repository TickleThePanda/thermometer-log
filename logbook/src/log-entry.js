import AWS from 'aws-sdk';
import moment from 'moment';

import { JWT, JWK } from 'jose';

const AWS_ACCESS_KEY = process.env.THERMOMETER_AWS_ACCESS_KEY;
const AWS_SECRET_KEY = process.env.THERMOMETER_AWS_SECRET_KEY;
const AWS_REGION = process.env.THERMOMETER_AWS_REGION;

const AUTH_KEY = JWK.asKey(process.env.THERMOMETER_AUTH_SECRET);

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

function isAuthed(req) {
  const authValue = req.headers.authorization;
  if (authValue === undefined || !authValue.startsWith('Bearer ')) {
    return false;
  }

  const token = authValue.substring(7);

  return JWT.verify(token, AUTH_KEY);
}

module.exports = async (req, res) => {
  try {
    if(!isAuthed(req)) {
      res.status(401).send();
      return;
    }

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
  } catch {
    res.status(500).send();
  }
}

