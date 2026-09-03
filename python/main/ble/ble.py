import asyncio
import threading
import time

from bleak import BleakScanner, BleakClient


class Ble:
    def __init__(self, rx_queue, tx_queue):
        self.callback = self.notification_handler
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
                    "8b7c0003-1234-4abc-8def-123456789abc"
            ))
            
        except Exception as e:
            print("BLE ERROR:", repr(e))
        finally:
            self.loop.close()



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

        async with BleakClient(
            device,
            disconnected_callback=self.disconnected_callback
        ) as client:

            self.client = client

            print("Connected")

            await client.start_notify(
                TX_UUID,
                self.notification_handler
            )

            # Start sending task
            send_task = asyncio.create_task(
                self.send_loop(client, RX_UUID)
            )

            try:
                while client.is_connected:
                    await asyncio.sleep(1)

            finally:
                send_task.cancel()

                try:
                    await send_task
                except asyncio.CancelledError:
                    pass

        self.client = None

    async def send_loop(self, client, RX_UUID):

        while client.is_connected:

            try:
                # Wait for data from the queue
                value = await asyncio.to_thread(
                    self.tx_queue.get
                )

                print("Sending:", value)
                print("Queue size:", self.tx_queue.qsize())

                await client.write_gatt_char(
                    RX_UUID,
                    value
                )

                self.tx_queue.task_done()

            except Exception as e:
                print("TX ERROR:", repr(e))
                break

            
        print("connest end tx")


    def notification_handler(self, sender, data):
        self.rx_queue.put(bytes(data))

    def disconnected_callback(self, client):
        print("!!! BLE DISCONNECTED !!!")