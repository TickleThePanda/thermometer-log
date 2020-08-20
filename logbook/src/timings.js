module.exports = class ServerTimings {

  constructor() {
    this.timings = {};
  }

  start(id, name) {
    this.timings[id] = {
      name: name,
      start: Date.now()
    }
  }

  end(id) {
    this.timings[id].end = Date.now();
  }

  addToResponse(res) {
    const timings = Object
        .entries(this.timings)
        .map(([key, value]) => key + ';desc="' + value.name + '";dur=' + (value.end - value.start))
        .join(', ');

    res.setHeader('Server-Timing', timings);
  }

}


