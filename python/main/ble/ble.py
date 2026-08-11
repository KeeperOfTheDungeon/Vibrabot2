import asyncio
import threading

from bleak import BleakScanner, BleakClient

class Ble:
    def __init__(self,rx_queue,tx_queue):
        self.callback  = self.notification_handler
        self.rx_queue = rx_queue
        self.tx_queue = tx_queue
        self.start()

    def start(self):
        self.thread = threading.Thread(
            target=self.run,
            daemon=True
        )
        self.thread.start()

    def run(self):
        self.loop = asyncio.new_event_loop()
        asyncio.set_event_loop(self.loop)

        try:
            self.loop.run_until_complete(self.connect(
                "VibraBot",
                "8b7c0002-1234-4abc-8def-123456789abc",
                "8b7c0002-1234-4abc-8def-123456789abc"
            ))
        except Exception as e:
            print("BLE ERROR:", repr(e))
        finally:
            self.loop.close()
        
        self.loop.run_forever()
   
    async def connect(self, device_name, TX_UUID, RX_UUID):

        print("=== CONNECT START ===")

        print("Scanning for VibraBot...")

        devices = await BleakScanner.discover()

        device = None

        for d in devices:
            print(d.name, d.address)

            if d.name == device_name:
                device = d
                break

        if device is None:
            print("VibraBot not found")
            return

        print(f"Found {device.name}: {device.address}")

        try:
                async with BleakClient(
                    device,
                    disconnected_callback=self.disconnected_callback
                ) as client:

                
                    print("Connected")

                    await client.start_notify(
                        TX_UUID,
                        self.notification_handler
                    )

                    print("Waiting for data...")
                    print("Press Ctrl+C to stop")

                    while client.is_connected:
                        print("connected =", client.is_connected)
                        await asyncio.sleep(1)

                    print("Connection lost")

        except Exception as e:
            print("BLE CONNECT ERROR:", repr(e))

            print("connect() finished")


    def notification_handler(self, sender, data):
        self.rx_queue.put(bytes(data))
        print(f"RX {len(data)} bytes:")


    def disconnected_callback(self,client):
        print("!!! BLE DISCONNECTED !!!")
    


    # If you want the data as a Python bytes object:
    # data is already bytes