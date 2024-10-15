import random

if __name__ == "__main__":
    for _ in range(10):
        noise = noise_generator = random.gauss(0, 0.25)
        x = random.uniform(0, 1)
        y = x ** 2 + 0.1 * x + noise
        print("{},{}".format(x, y))

