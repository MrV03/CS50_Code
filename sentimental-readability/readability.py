from cs50 import get_string


def main():
    text = get_string("Text: ")

    letters = sum(c.isalpha() for c in text)
    words = len(text.split())
    sentences = text.count(".") + text.count("!") + text.count("?")

    avg_letters_100 = (letters / words) * 100
    avg_sentences_100 = (sentences / words) * 100

    index = round(0.0588 * avg_letters_100 - 0.296 * avg_sentences_100 - 15.8)

    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()
