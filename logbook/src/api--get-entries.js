import moment from 'moment';

import isAuthorised from './auth.js';
import ThermometerLog from './log-store.js';

import allowCors from './allow-cors.js';

const log = new ThermometerLog();

module.exports = allowCors(async (req, res) => {
  try {
    if(!isAuthorised(req)) {
      res.status(401).send();
      return;
    }

    const date = req.query.date;
    const roomId = req.query.roomId;

    const entries = await log.getEntries({
      date,
      roomId
    });

    res.status(200).json(entries);
  } catch (error) {
    console.log("Error serving " + req.url, error);
    res.status(500).send();
  }
});

