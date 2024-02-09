-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Query 1: Crime scene reports around time of theft
SELECT *
FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- 28/07/2021, 10:15am, 3 witnesses.

-- Query 2: Interviews
SELECT *
FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;
--Ruth. 10 minutes, exit
--Eugen. Drawing money at Leggett Street
--Raymond. Phonecall for less than a minute and ticket for TOMORROW.

--Query 3:Who left the parking lot?
SELECT *
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour >= 10 AND hour < 11;
-- License plates of cars leaving within 10 minutes of the crime
-- 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55.

-- Query 4:Find names using plates
SELECT name, license_plate
FROM people
WHERE license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');
-- The people who left the car park were:
-- Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey and Bruce.

-- Query 5: ATM transactions at leggett street
SELECT *
FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location LIKE '%Leggett Street%';
-- Acc numbers: 28500762, 28296815, 76054385, 49610011, 16153065,86363979, 25506511, 81061156, 26013199.

-- Query 6: Use account numbers to find names
SELECT p.name, b.account_number
FROM people p
JOIN bank_accounts b ON p.id = b.person_id
WHERE b.account_number IN ('28500762', '28296815', '76054385', '49610011', '16153065', '86363979', '25506511', '81061156', '26013199');
-- Transactions done by Bruce, Kaelyn, Diana, Brooke, Kenny, Iman, Luca, Taylor and Benista.
-- This means that the people who transacted and left the car park are Iman, Luca, Diana and Bruce.

-- Query 7: Find their phone numbers
SELECT name, phone_number
FROM people
WHERE name IN ('Iman', 'Luca', 'Diana', 'Bruce');
-- The Numbers are: Iman = (829) 555-5269, Luca = (389) 555-5198, Diana = (770) 555-1861, Bruce = (367) 555-5533.

-- Query 8: Who did these numbers call on that day?
SELECT id, caller, receiver, year, month, day, duration
FROM phone_calls
WHERE caller IN ('(829) 555-5269', '(389) 555-5198', '(770) 555-1861', '(367) 555-5533');
--Bruce made 4 calls to: (375) 555-8161, (344) 555-9601, (022) 555-4052, (704) 555-5790
--Diana made one to: (725) 555-3243

-- Query 9: Names of people that they called
SELECT name, phone_number
FROM people
WHERE phone_number IN ('(375) 555-8161', '(344) 555-9601', '(022) 555-4052', '(704) 555-5790', '(725) 555-3243');
-- The names and numbers are :
-- Gregory: (022) 555-4052, Carl: (704) 555-5790, Philip: (725) 555-3243, Robin: (375) 555-8161, Deborah: (344) 555-9601.
-- However it was Robin who only spoke to him for a minute

-- Query 10: Using names find flights.Include Bruce and Diana
SELECT p.name, p.passport_number, pl.flight_id, pl.seat
FROM passengers pl
JOIN people p ON pl.passport_number = p.passport_number
WHERE p.name IN ('Diana', 'Bruce');
-- Flights that contain these names are 18, 24, 54 and 36.

-- Query 11: Use flight ID's and cross reference flights during that day
SELECT f.id AS flight_id, f.origin_airport_id, f.destination_airport_id, f.year, f.month, f.day, f.hour, f.minute
FROM flights f
WHERE f.id IN (18, 24, 54, 36);
-- Only one flight left the earliest the next day at 08:20, that was flight_id 36 to dest_airport 4.
-- This is Bruces

-- Query 12: Find Destination
SELECT airports.city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE flights.id = 36
AND flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND flights.hour = 8 AND flights.minute = 20;
-- New York City. This means that Bruce is in New York with the help of Robin

