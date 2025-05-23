## Client/Server for assets

Asset management for the Splinter game engine.

### Server

Optimized to index, search through, and serve large amounts of textures/models/etc.

Splinter Engine connects to the asset server.

### Client

Standalone client GUI:

- Asset browser
- built-in 3d engine to view assets
- apply materials (for level creation)
- manipulate/create materials

### Notes

Bad code, undocumented, and work-in-progress.

`sudo apt install -y libpng pkg-config cmake qt6-base-dev qt6-multimedia-dev qt6-websockets-dev qt6-httpserver-dev zip unzip`

#### systemd service

```
[Unit]
Description=Persistent asset service
After=network.target

[Service]
User=assets
Restart=on-failure
RestartSec=20
WorkingDirectory=/home/assets/splinter_engine_asset_server
ExecStart=bash /home/assets/splinter_engine_asset_server/bin/server
 
[Install]
WantedBy=multi-user.target
```