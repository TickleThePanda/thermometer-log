module.exports = (req, res) => {
  const data = req.body;
  console.log("temperature: " + data.temperature);
  res.status(204).send();
}
