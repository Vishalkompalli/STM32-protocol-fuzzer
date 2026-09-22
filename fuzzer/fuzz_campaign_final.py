import serial
import time
import random
from pathlib import Path


# ==================================================
# CONFIGURATION
# ==================================================

PORT = "COM7"
BAUDRATE = 9600
TOTAL_TESTS = 1000

FAILURE_DIR = Path("failures")
FAILURE_DIR.mkdir(exist_ok=True)


# ==================================================
# SERIAL COMMUNICATION
# ==================================================

def send_frame(ser, frame):
    ser.reset_input_buffer()

    ser.write(frame)
    ser.flush()

    # STM32 responses end with \r\n,
    # so readline() waits for the complete response.
    return ser.readline()


# ==================================================
# FAILURE HANDLING
# ==================================================

def save_failure(frame, response, reason, test_number):

    filename = FAILURE_DIR / f"failure_{test_number:04d}.bin"

    filename.write_bytes(frame)

    print("\n!!! FAILURE !!!")
    print("Test:", test_number)
    print("Reason:", reason)
    print("Frame:", frame.hex(" "))
    print("Response:", response.hex(" "))
    print("Saved:", filename)


# ==================================================
# TEST CASE GENERATOR
# ==================================================

def generate_case_with_type():

    test_type = random.choice([
        "VALID",
        "UNKNOWN_COMMAND",
        "INVALID_LENGTH",
        "BAD_START",
        "MUTATED_DATA"
        ])


    # ------------------------------------------------
    # VALID FRAME
    # ------------------------------------------------

    if test_type == "VALID":

        length = random.randint(0, 32)

        # Zero-length STATUS command
        if length == 0:

            return test_type, bytes([
                0xAA,
                0x00,
                0x02
            ])

        data = bytes(
            random.randint(0, 255)
            for _ in range(length)
        )

        frame = bytes([
            0xAA,
            length,
            0x01
        ]) + data

        return test_type, frame


    # ------------------------------------------------
    # UNKNOWN COMMAND
    # ------------------------------------------------

    if test_type == "UNKNOWN_COMMAND":

        length = random.randint(1, 32)

        data = bytes(
            random.randint(0, 255)
            for _ in range(length)
        )

        command = random.choice([
            0x00,
            0x03,
            0x10,
            0x7F,
            0xFF
        ])

        frame = bytes([
            0xAA,
            length,
            command
        ]) + data

        return test_type, frame


    # ------------------------------------------------
    # INVALID LENGTH
    # ------------------------------------------------

    if test_type == "INVALID_LENGTH":

        length = random.randint(33, 255)

        frame = bytes([
            0xAA,
            length,
            random.randint(0, 255)
        ])

        return test_type, frame


    # ------------------------------------------------
    # BAD START BYTE
    # ------------------------------------------------

    if test_type == "BAD_START":

        length = random.randint(1, 32)

        # Deliberately exclude 0xAA from the data.
        # Otherwise a random AA could accidentally
        # become a legitimate frame start.
        data = bytes(
            random.choice([
                0x00,
                0x01,
                0x02,
                0x10,
                0x20,
                0x30,
                0x40,
                0x55,
                0xFE,
                0xFF
            ])
            for _ in range(length)
        )

        bad_start = random.choice([
            0x00,
            0x01,
            0x55,
            0xFF
        ])

        frame = bytes([
            bad_start,
            length,
            0x01
        ]) + data

        return test_type, frame


    # ------------------------------------------------
    # MUTATED DATA
    # ------------------------------------------------

    if test_type == "MUTATED_DATA":

        length = random.randint(1, 32)

        data = bytearray(
            random.randint(0, 255)
            for _ in range(length)
        )

        index = random.randrange(length)

        data[index] = random.randint(0, 255)

        frame = bytes([
            0xAA,
            length,
            0x01
        ]) + bytes(data)

        return test_type, frame
    raise RuntimeError("Unknown test type generated")


# ==================================================
# RESPONSE CLASSIFICATION
# ==================================================

def classify(response, test_type):

    # ------------------------------------------------
    # BAD START
    # ------------------------------------------------

    if test_type == "BAD_START":

        # A bad start byte should be ignored.
        # Therefore no response is the expected behavior.

        if not response:
            return "EXPECTED TIMEOUT"

        return "UNEXPECTED RESPONSE"


    # ------------------------------------------------
    # NO RESPONSE FOR OTHER TEST TYPES
    # ------------------------------------------------

    if not response:
        return "TIMEOUT"


    # ------------------------------------------------
    # VALID FRAME
    # ------------------------------------------------

    if test_type == "VALID":

        if (
            response.startswith(b"ECHO:")
            or response.startswith(b"STATUS:")
        ):
            return "PASS"


    # ------------------------------------------------
    # UNKNOWN COMMAND
    # ------------------------------------------------

    elif test_type == "UNKNOWN_COMMAND":

        if response.startswith(
            b"ERROR: UNKNOWN COMMAND"
        ):
            return "EXPECTED ERROR"


    # ------------------------------------------------
    # INVALID LENGTH
    # ------------------------------------------------

    elif test_type == "INVALID_LENGTH":

        if response.startswith(
            b"ERROR: LENGTH TOO LARGE"
        ):
            return "EXPECTED ERROR"


    # ------------------------------------------------
    # MUTATED DATA
    # ------------------------------------------------

    elif test_type == "MUTATED_DATA":

        if response.startswith(b"ECHO:"):
            return "PASS"


    return "UNEXPECTED RESPONSE"


# ==================================================
# START SERIAL CONNECTION
# ==================================================

ser = serial.Serial(
    port=PORT,
    baudrate=BAUDRATE,
    timeout=2
)

print("Serial port opened")
print("Waiting for STM32...")

time.sleep(2)


# ==================================================
# STATISTICS
# ==================================================

passed = 0
expected_errors = 0
expected_timeouts = 0
timeouts = 0
unexpected = 0


# ==================================================
# START FUZZ CAMPAIGN
# ==================================================

print()
print("================================")
print("STARTING FUZZ CAMPAIGN")
print("Tests:", TOTAL_TESTS)
print("================================")


for test_number in range(1, TOTAL_TESTS + 1):

    # Generate test case
    test_type, frame = generate_case_with_type()

    # Send to STM32
    response = send_frame(
        ser,
        frame
    )

    # Classify response
    result = classify(
        response,
        test_type
    )


    # ------------------------------------------------
    # UPDATE STATISTICS
    # ------------------------------------------------

    if result == "PASS":

        passed += 1


    elif result == "EXPECTED ERROR":

        expected_errors += 1


    elif result == "EXPECTED TIMEOUT":

        expected_timeouts += 1


    elif result == "TIMEOUT":

        timeouts += 1

        save_failure(
            frame,
            response,
            "TIMEOUT",
            test_number
        )


    else:

        unexpected += 1

        save_failure(
            frame,
            response,
            "UNEXPECTED RESPONSE",
            test_number
        )


    # ------------------------------------------------
    # PROGRESS
    # ------------------------------------------------

    if test_number % 100 == 0:

        print(
            f"Progress: "
            f"{test_number}/{TOTAL_TESTS}"
        )


# ==================================================
# CLEAN UP
# ==================================================

ser.close()


# ==================================================
# FINAL SUMMARY
# ==================================================

print()
print("================================")
print("FUZZING COMPLETE")
print("================================")

print(
    f"Total tests:       {TOTAL_TESTS}"
)

print(
    f"Passes:            {passed}"
)

print(
    f"Expected errors:   {expected_errors}"
)

print(
    f"Expected timeouts: {expected_timeouts}"
)

print(
    f"Unexpected timeout:{timeouts}"
)

print(
    f"Unexpected output: {unexpected}"
)

print(
    f"Failures saved:    {timeouts + unexpected}"
)

print("================================")