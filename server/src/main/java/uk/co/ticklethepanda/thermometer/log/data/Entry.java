package uk.co.ticklethepanda.thermometer.log.data;

import org.springframework.data.annotation.Id;

import java.time.LocalDateTime;

public class Entry {

    @Id
    private EntryKey id;

    private int readingCount;
    private double temperature;

    public Entry() {

    }

    public Entry(LocalDateTime time, String location, int readingCount, double temperature) {
        this.id = new EntryKey(time, location);
        this.readingCount = readingCount;
        this.temperature = temperature;
    }

    public LocalDateTime getTime() {
        return id.getTime();
    }

    public void setTime(LocalDateTime time) {
        id.setTime(time);
    }

    public String getLocation() {
        return id.getLocation();
    }

    public void setLocation(String location) {
        id.setLocation(location);
    }

    public int getReadingCount() {
        return readingCount;
    }

    public void setReadingCount(int readingCount) {
        this.readingCount = readingCount;
    }

    public double getTemperature() {
        return temperature;
    }

    public void setTemperature(double temperature) {
        this.temperature = temperature;
    }
}
