import serial
import wave
import struct


PORT = "COM19"
BAUD_RATE = 460800

SAMPLE_RATE = 16000
BLOCK_SIZE = 256
BLOCK_BYTES = BLOCK_SIZE * 2

DURATION = 10
OUTPUT_FILE = "recording.wav"

HEADER = b"\x55\xAA"


def find_header(ser):
    previous = b""

    while True:
        byte = ser.read(1)

        if not byte:
            raise RuntimeError("Serial timeout while waiting for header")

        previous += byte

        if len(previous) > 2:
            previous = previous[-2:]

        if previous == HEADER:
            return


def read_exact(ser, size):
    data = bytearray()

    while len(data) < size:
        chunk = ser.read(size - len(data))

        if not chunk:
            raise RuntimeError("Serial timeout while reading audio data")

        data.extend(chunk)

    return data


def main():
    ser = serial.Serial(
        PORT,
        BAUD_RATE,
        timeout=1
    )

    ser.reset_input_buffer()

    total_blocks = (SAMPLE_RATE * DURATION) // BLOCK_SIZE

    print("Waiting for audio...")

    with wave.open(OUTPUT_FILE, "wb") as wav:

        wav.setnchannels(1)
        wav.setsampwidth(2)
        wav.setframerate(SAMPLE_RATE)

        for block in range(total_blocks):

            find_header(ser)

            raw = read_exact(
                ser,
                BLOCK_BYTES
            )

            # Raw data is already little-endian signed 16-bit PCM.
            wav.writeframes(raw)

            if block % 10 == 0:
                elapsed = block * BLOCK_SIZE / SAMPLE_RATE
                print(f"{elapsed:.2f} s", end="\r")

    ser.close()

    print()
    print("Saved:", OUTPUT_FILE)


if __name__ == "__main__":
    main()