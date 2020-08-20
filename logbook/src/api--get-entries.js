import moment from 'moment';

import isAuthorised from './auth.js';
import ThermometerLog from './log-store.js';

import allowCors from './allow-cors.js';

import ServerTimings from './timings.js';

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

    const timeAsString = time.format();

    aggregatedEntries.push({
      time: timeAsString,
      count: count,
      temperature: avg
    });
  }

  return aggregatedEntries;
}

module.exports = allowCors(async (req, res) => {
  const timings = new ServerTimings();
  try {
    if(!isAuthorised(req)) {
      res.status(401).send();
      return;
    }

    const date = req.query.date;
    const roomId = req.query.roomId;
    const period = req.query.period;

    timings.start('db', 'Database');

    const entries = await log.getEntries({
      date,
      roomId
    });

    timings.end('db');

    if (period !== undefined) {
      timings.start('agg', 'Process aggregates');
      const aggregated = aggregateToPeriod(entries, period);
      timings.end('agg');
      timings.addToResponse(res);
      res.status(200).json(aggregated);
    } else { 
      res.status(200).json(entries);
    }
  } catch (error) {
    console.log("Error serving " + req.url, error);
    res.status(500).send();
  }
});

