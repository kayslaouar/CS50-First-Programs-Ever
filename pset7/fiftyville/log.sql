--Chamberlin St, 7/28/2020, THEFT

SELECT * FROM "crime_scene_reports" WHERE day = 28 AND month = 7 AND year = 2020 AND street = "Chamberlin Street";

--courthouse, 3 interviews, 10:15 AM

SELECT * FROM "interviews" WHERE day = 28 AND month = 7 AND year = 2020 AND transcript LIKE '%courthouse%';

-- left parking lot between 10:15 AM and 10:25 AM
-- ATM withdrawl before theft by thief on Fifer St
-- earliest flight out of fiftyville 7/29/2020
-- less than 1 minute call
-- thief asked other person to purchase flight tickets

SELECT * FROM "airports" WHERE full_name LIKE '%Fiftyville%';

-- Fiftyville airport id = 8:20

SELECT * FROM "flights" WHERE origin_airport_id = '8' AND day = 29 AND month = 7 AND year = 2020 ORDER BY hour, minute;

-- earliest flight from Fiftyville at 8:30 AM, destination airport id = 4, flight id = 36

SELECT * FROM airports WHERE id = 4;

-- destination = London Heathrow Airport

SELECT * FROM flights JOIN passengers ON id = flight_id WHERE id = 36;

-- flight had 8 passengers, 4 passengers were in seats that were next to each other.
-- Thief and accomplice were likely sitting next to each other seats (6C and 6D) OR (7A and 7B)
-- check passports (8294398571 and 1988161715) OR (9878712108 and 8496433585)

SELECT * FROM people JOIN passengers ON people.passport_number = passengers.passport_number WHERE people.passport_number IN
(SELECT passport_number FROM flights JOIN passengers ON id = flight_id WHERE id = 36);

-- Gets personal information of everyone on the flight
-- FACTS:
-- DATE: 7/28/2020
-- location: Fiftyville Courthouse, Chamberlin St
-- FLIGHT-ID: 36
-- DESTINATION: LONDON
--

SELECT * FROM "atm_transactions" WHERE day = 28 AND month = 7 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw';

SELECT * FROM "phone_calls" WHERE day = 28 AND month = 7 AND year = 2020 AND duration < 60;

SELECT * FROM "courthouse_security_logs" WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute < 25 AND minute > 15;

SELECT * FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE
day = 28 AND month = 7 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'));

SELECT * FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'))
INTERSECT
SELECT * FROM people WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute < 25 AND minute > 15)
INTERSECT
SELECT * FROM people WHERE phone_number IN
(SELECT phone_number FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2020 AND duration < 60)
INTERSECT
SELECT * FROM people WHERE passport_number IN
(SELECT passport_number FROM flights JOIN passengers ON id = flight_id WHERE id = 36);

-- gives us two suspects:
-- 1) 467400, Danielle, (389) 555-5198, 8496433585, 4328GD8
-- 2) 686048, Ernest, (367) 555-5533, 5773159633, 94KL13X

-- At this point, the last query found us two suspects based on ALL the information we were given in the interviews. 
-- The only thing we can do from here is figure out if the recepient of the phone call was also on the plane. 
-- Also, two conjectures can be drawn from data we've already seen so far: 
-- 1) it is likely that the thief and accomplice traveled to the same destination, 
-- which would mean Ernest is the thief because Daniella is the only person from flight 36 to take another flight
-- 2) it is likely that the thief sat next to the accomplice on the plane, 
-- which would mean Daniella is the thief because she is the only suspect that was sitting next to someone on flight 36

SELECT * FROM people WHERE phone_number IN 
(SELECT receiver FROM phone_calls WHERE caller IN 
(SELECT phone_number FROM people WHERE name = "Ernest") AND day = 28 AND month = 7 AND year = 2020 AND duration < 60);

-- query tells us that 
-- 864400	Berthold	(375) 555-8161	NULL	4V16VO0
-- is the only receiver of a phone call where the date, time, and duration of the call are the same as what we are looking for
-- HOWEVER, Berthold does not have a passport number, whereas ALL people on the plane do. So, Berthold is not who we are looking for...

SELECT * FROM people WHERE phone_number IN 
(SELECT receiver FROM phone_calls WHERE caller IN 
(SELECT phone_number FROM people WHERE name = "Danielle") AND day = 28 AND month = 7 AND year = 2020 AND duration < 60);

-- Danielle did not make ANY outgoing phone calls on 7/28/2020. 
-- Therefore, Daneille does not check out as a suspect given the information we are given

-- Berthold could have been on the plane secretly or something. 
-- After rereading the instructions, nothing is said about the accomplice being in the same plane as the thief. 
-- Ernest is our thief!