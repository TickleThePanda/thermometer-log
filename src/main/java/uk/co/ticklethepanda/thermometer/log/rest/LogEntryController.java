package uk.co.ticklethepanda.thermometer.log.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Sort;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import uk.co.ticklethepanda.thermometer.log.data.Entry;
import uk.co.ticklethepanda.thermometer.log.data.EntryKey;
import uk.co.ticklethepanda.thermometer.log.data.EntryRepository;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Controller
@RequestMapping("/thermometers")
public class LogEntryController {

    private EntryRepository repository;

    @Autowired
    public LogEntryController(EntryRepository repository) {
        this.repository = repository;
    }

    @PostMapping
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public void create(@RequestBody ThermometerEntryDto dto) {
        Entry entry = new Entry(
                dto.getTime(),
                dto.getLocation(),
                dto.getReadingCount(),
                dto.getAverageTemperature());

        repository.save(entry);
    }

    @PostMapping(path = "/rooms/{room}/entries", consumes = "text/csv")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public void importCsv(
            @PathVariable String room,
            @RequestBody String csv
    ) throws IOException {

        List<Entry> entries = convertCsvToEntries(room, csv);

        repository.save(entries);
    }

    private List<Entry> convertCsvToEntries(@PathVariable String room, @RequestBody String csv) throws IOException {
        List<Entry> entries = new ArrayList<>();

        BufferedReader reader = new BufferedReader(new StringReader(csv));
        String line;
        while ((line = reader.readLine()) != null) {
            String[] values = line.split(",");

            String date = values[0];
            String nReadings = values[1];
            String temperature = values[2];

            entries.add(new Entry(
                    LocalDateTime.parse(date, DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")),
                    room,
                    Integer.parseInt(nReadings),
                    Float.parseFloat(temperature)
            ));
        }
        return entries;
    }

    @GetMapping("/rooms/{room}/entries/{date}/{time}")
    @ResponseBody
    public ThermometerEntryDto getEntryForDateTime(
            @PathVariable("room") String room,
            @PathVariable("date") @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) LocalDate date,
            @PathVariable("time") @DateTimeFormat(iso = DateTimeFormat.ISO.TIME) LocalTime time
    ) {

        Entry e = repository.findOne(new EntryKey(date.atTime(time), room));

        if(e == null) {
            throw new EntryNotFoundException();
        }

        return convertEntryToDto(e);

    }

    @GetMapping("/rooms/{room}/entries/{date}")
    @ResponseBody
    public List<ThermometerEntryDto> getEntriesForDate(
            @PathVariable("room") String room,
            @PathVariable("date") @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) LocalDate date
    ) {

        List<Entry> entries = repository.findByLocationAndTimeBetween(
                room, date.atStartOfDay(), date.atTime(LocalTime.MAX),
                new Sort(Sort.Direction.ASC, "_id.time")
        );

        return entries.stream()
                .map(this::convertEntryToDto)
                .collect(Collectors.toList());
    }

    private ThermometerEntryDto convertEntryToDto(Entry e) {
        return new ThermometerEntryDto(
                e.getTime(),
                e.getLocation(),
                e.getReadingCount(),
                e.getTemperature()
        );
    }
}
