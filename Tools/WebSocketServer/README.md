# WebSocket Echo Server

Simple WebSocket server that echoes back received messages.

## Setup

This project uses `uv` for Python dependency management.

### Install Dependencies

```bash
uv sync
```

## Starting the Server

```bash
uv run python server.py
```

The server will start on `ws://0.0.0.0:8765`.

## Features

- Accepts WebSocket connections on port 8765
- Echoes back received messages (text/binary)
- Supports multiple simultaneous client connections
- Logs all connections and messages to console
- Safe shutdown with Ctrl+C

## Server Specifications

- **Host**: 0.0.0.0 (listens on all network interfaces)
- **Port**: 8765
- **Protocol**: WebSocket (ws://)
- **Message Format**: Supports both text and binary