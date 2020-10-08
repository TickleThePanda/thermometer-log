const allowlist = [
  /^https?:\/\/localhost(:\d*)?$/,
  /^https:\/\/(\w*\.)?ticklethepanda.co.uk$/,
  /^https:\/\/(\w*\.)?ticklethepanda.dev$/,
  /^https:\/\/(\w*\.)?ticklethepanda.netlify.com$/
];

module.exports = fn => async (req, res) => {
  const origin = req.headers.origin;

  if (allowlist.some(r => origin.match(r))) {
    res.setHeader('Access-Control-Allow-Credentials', true);
    res.setHeader('Access-Control-Allow-Origin', origin);
    res.setHeader('Access-Control-Allow-Methods', 'GET,OPTIONS');
    res.setHeader('Timing-Allow-Origin', origin);
    res.setHeader('Access-Control-Max-Age', '86400');
    res.setHeader(
      'Access-Control-Allow-Headers',
      'X-CSRF-Token, X-Requested-With, Accept, Accept-Version, Content-Length, Content-MD5, Content-Type, Date, X-Api-Version, Authorization'
    );
    if (req.method === 'OPTIONS') {
      res.status(200).end();
      return;
    }

  }

  return await fn(req, res);
}

