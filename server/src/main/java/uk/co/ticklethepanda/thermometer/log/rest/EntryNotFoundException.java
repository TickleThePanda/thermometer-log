package uk.co.ticklethepanda.thermometer.log.rest;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(HttpStatus.NOT_FOUND)
public class EntryNotFoundException extends RuntimeException {

    public EntryNotFoundException() {
    }

    public EntryNotFoundException(String message) {
        super(message);
    }

}
