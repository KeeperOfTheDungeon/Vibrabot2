import asyncio
from bleak import BleakScanner, BleakClient

class Ble:
    def __init__(self):



    async def connect(self, device_name,TX_UUID, RX_UUID)
        pass:



    def notification_handler(sender, data):
        print(f"RX {len(data)} bytes:")
        print(" ".join(f"{b:02X}" for b in data))

    # If you want the data as a Python bytes object:
    # data is already bytes