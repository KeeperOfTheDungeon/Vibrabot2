import asyncio
from bleak import BleakScanner, BleakClient

DEVICE_NAME = "VibraBot"

TX_UUID = "8b7c0002-1234-4abc-8def-123456789abc"


def notification_handler(sender, data):
    print(f"RX {len(data)} bytes:")
    print(" ".join(f"{b:02X}" for b in data))

    # If you want the data as a Python bytes object:
    # data is already bytes


async def main():

    print("Scanning for VibraBot...")

    devices = await BleakScanner.discover()

    device = None

    for d in devices:
        print(d.name, d.address)

        if d.name == DEVICE_NAME:
            device = d
            break

    if device is None:
        print("VibraBot not found")
        return

    print(f"Found {device.name}: {device.address}")

    async with BleakClient(device) as client:

        print("Connected")

        await client.start_notify(
            TX_UUID,
            notification_handler
        )

        print("Waiting for data...")
        print("Press Ctrl+C to stop")

        while True:
            await asyncio.sleep(1)


asyncio.run(main())