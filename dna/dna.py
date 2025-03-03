import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    STRs = []
    profiles = []

    with open(sys.argv[1], mode="r") as database:
        reader = csv.DictReader(database)
        STRs = reader.fieldnames[1:]
        for row in reader:
            profiles.append(row)

    # TODO: Read DNA sequence file into a variable
    seq_str_count = {STR: 0 for STR in STRs}

    with open(sys.argv[2], mode="r") as sequence_file:
        sequence = sequence_file.readline()
        for STR in STRs:
            # TODO: Find longest match of each STR in DNA sequence
            seq_str_count[STR] = longest_match(sequence, STR)

    # TODO: Check database for matching profiles
    match_found = False
    for profile in profiles:
        if all(int(profile[STR]) == seq_str_count[STR] for STR in STRs):
            print(profile["name"])
            match_found = True
            break

    if not match_found:
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
