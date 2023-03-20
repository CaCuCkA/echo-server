import asyncio
import sys

import websockets

from misc.constants import Constants

Constants = Constants()


async def echo():
    for x in range(Constants.RANGE):
        async with websockets.connect(Constants.URL) as websocket:
            await websocket.send(str())
            name = await websocket.recv()
            print(name, end=" ", flush=True)
            sys.stdout.flush()


async def main():
    await asyncio.wait([
        echo(),
        echo(),
        echo(),
        echo(),
        echo(),
        echo(),
        echo(),
        echo(),
        echo(),
        echo()
    ])


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
