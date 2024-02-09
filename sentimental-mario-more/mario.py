while True:
    user = input("Height: ")
    if user.isdigit():
        height = int(user)
        if 1 <= height <= 8:
            break

for x in range(height):
    print(" " * (height - x - 1), end="")

    print("#" * (x + 1), end="")

    print("  ", end="")

    print("#" * (x + 1))
