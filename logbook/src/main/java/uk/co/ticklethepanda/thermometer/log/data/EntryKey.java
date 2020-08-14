package uk.co.ticklethepanda.thermometer.log.data;

import java.io.Serializable;
import java.time.LocalDateTime;

public class EntryKey implements Serializable {
    private LocalDateTime time;
    private String location;

    public EntryKey(LocalDateTime time, String location) {
        this.time = time;
        this.location = location;
    }

    public LocalDateTime getTime() {
        return time;
    }

    public void setTime(LocalDateTime time) {
        this.time = time;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }
}
