import asyncio
import websockets
import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

async def handle_client(websocket):
    client_address = websocket.remote_address
    logger.info(f"New client connected: {client_address}")
    
    try:
        async for message in websocket:
            logger.info(f"Received from {client_address}: {message}")
            await websocket.send(message)
            logger.info(f"Echoed back to {client_address}: {message}")
    except websockets.exceptions.ConnectionClosed:
        logger.info(f"Client disconnected: {client_address}")
    except Exception as e:
        logger.error(f"Error handling client {client_address}: {e}")

async def main():
    host = "0.0.0.0"
    port = 8765
    
    logger.info(f"Starting WebSocket server on {host}:{port}")
    
    async with websockets.serve(handle_client, host, port):
        logger.info("WebSocket server is running. Press Ctrl+C to stop.")
        await asyncio.Future()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        logger.info("Server stopped by user")