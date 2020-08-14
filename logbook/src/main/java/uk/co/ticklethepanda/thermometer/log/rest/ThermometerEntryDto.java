package uk.co.ticklethepanda.thermometer.log.rest;

import java.time.LocalDateTime;

public class ThermometerEntryDto {
    private LocalDateTime time;
    private String location;
    private int readingCount;
    private double averageTemperature;

    public ThermometerEntryDto() {
    }

    public ThermometerEntryDto(LocalDateTime time, String location, int readingCount, double averageTemperature) {
        this.time = time;
        this.location = location;
        this.readingCount = readingCount;
        this.averageTemperature = averageTemperature;
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

    public int getReadingCount() {
        return readingCount;
    }

    public void setReadingCount(int readingCount) {
        this.readingCount = readingCount;
    }

    public double getAverageTemperature() {
        return averageTemperature;
    }

    public void setAverageTemperature(double averageTemperature) {
        this.averageTemperature = averageTemperature;
    }
}
