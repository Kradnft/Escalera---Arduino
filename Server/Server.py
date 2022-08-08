import asyncio
import websockets
import json
import secrets

PUERTO = 8001
FICHAS = {}

async def broadcast(event):
    for key in FICHAS:
        await FICHAS[key].send(json.dumps(event))

async def handler(websocket):
    join_key = secrets.token_urlsafe(12)
    FICHAS[join_key] = websocket
    event = {
        "type":0,
        "codigo":join_key
    }
    await websocket.send(json.dumps(event))
    try:
        async for message in websocket:
            try:
                event = json.loads(message)
            except ValueError as error:
                event = {
                    "type": -1,
                    "mensaje":"Formato incorrecto!"
                }
                await websocket.send(json.dumps(event))
                continue
            event = {
                "type": 1,
                "mensaje":message
            }
            await broadcast(event)
    finally:
        del FICHAS[join_key]
        print("FICHAS: ",FICHAS)

async def main():
    async with websockets.serve(handler, "", PUERTO):
        await asyncio.Future()

if __name__ == "__main__":
    asyncio.run(main())