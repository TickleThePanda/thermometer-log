package uk.co.ticklethepanda.thermometer.log.data;

import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;

import java.time.LocalDateTime;
import java.util.List;

public interface EntryRepository extends MongoRepository<Entry, EntryKey> {

    @Query("{'_id.location': { $eq: ?0 }, '_id.time': { $gte: ?1, $lte: ?2 }}")
    List<Entry> findByLocationAndTimeBetween(String location, LocalDateTime start, LocalDateTime end, Sort sort);

}
