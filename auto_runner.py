import asyncio
import sys


from misc.constants import Constants

Constants = Constants()


async def echo():
    ws =  create_connection("ws://echo.websocket.events/")
    for x in range(Constants.RANGE):




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
