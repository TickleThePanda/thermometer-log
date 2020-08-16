import moment from 'moment';

import isAuthorised from './auth.js';
import ThermometerLog from './log-store.js';

import allowCors from './allow-cors.js';

const log = new ThermometerLog();

function aggregateToPeriod(entries, period) {
  const buckets = {};

  for (let entry of entries) {
    const bucket = Math.floor(moment(entry.time).unix() / period) * period;
    if (buckets[bucket] === undefined) {
      buckets[bucket] = [];
    }
    buckets[bucket].push(entry);
  }

  const aggregatedEntries = [];
  for (let [bucketTime, bucketEntries] of Object.entries(buckets)) {
    const time = moment.unix(bucketTime);
    const sum = bucketEntries.reduce((acc, curr) => acc + curr.temperature, 0);
    const count = bucketEntries.length;
    const avg = sum / count;
    aggregatedEntries.push({
      time: time.format(),
      count: count,
      temperature: avg
    });
  }

  return aggregatedEntries;
}

module.exports = allowCors(async (req, res) => {
  try {
    if(!isAuthorised(req)) {
      res.status(401).send();
      return;
    }

    const date = req.query.date;
    const roomId = req.query.roomId;
    const period = req.query.period;

    const entries = await log.getEntries({
      date,
      roomId
    });

    const aggregated = aggregateToPeriod(entries, period);

    res.status(200).json(aggregated);
  } catch (error) {
    console.log("Error serving " + req.url, error);
    res.status(500).send();
  }
});

