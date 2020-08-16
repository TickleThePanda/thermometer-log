import moment from 'moment';

import isAuthorised from './auth.js'
import ThermometerLog from './log-store.js'

const log = new ThermometerLog();

module.exports = async (req, res) => {
  try {
    if(!isAuthorised(req)) {
      res.status(401).send();
      return;
    }

    const temperature = req.body.temperature;
    const room = req.query.roomId;

    console.log("temperature in " + room + ": " + temperature);

    const now = moment();

    const date = now.format("YYYY-MM-DD");
    const time = now.format("HH:mm:ss");

    await log.updateEntry({
      room, temperature,
      date, time
    })

    res.status(204).send();
  } catch (error) {
    console.log("Error serving " + req.url, error);
    res.status(500).send();
  }
}

