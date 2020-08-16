import { JWT, JWK } from 'jose';

const AUTH_KEY = JWK.asKey(process.env.THERMOMETER_AUTH_SECRET);

module.exports = function isAuthorised(req) {
  const authValue = req.headers.authorization;
  if (authValue === undefined || !authValue.startsWith('Bearer ')) {
    return false;
  }

  const token = authValue.substring(7);

  return JWT.verify(token, AUTH_KEY);
}

