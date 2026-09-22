import serial
import time

def main():
    try:
        ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    except serial.SerialException as e:
        print(f"Could not open serial port: {e}")
        return

    log_file = "fuzz_log.txt"
    print(f"Logging started → {log_file} (Press Ctrl+C to stop)\n")

    with open(log_file, "a") as f:
        try:
            while True:
                line = ser.readline().decode(errors='replace').strip()
                if line:
                    timestamp = time.strftime("%H:%M:%S")
                    log_entry = f"[{timestamp}] {line}"
                    print(log_entry)
                    f.write(log_entry + "\n")
        except KeyboardInterrupt:
            print("\n Logging stopped by user.")
        finally:
            ser.close()

if __name__ == "__main__":
    main()
