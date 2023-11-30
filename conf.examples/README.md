# WebSocket Server / Repeater

Configuration (examples)

- config.json (main)
  - wsblack.json (black-list of clients)

- data format
  - JSON


config.json

```    
{ "ID":"WsRepeaterA", 
  "Port":8100,
  "ConnMax":30,
  "ConnPerCli":3,
  "ConnLifeTime":28800,
  "UseLog":0,
  "WsRepeater":"ws://at09.ddns.net:8102",
  "Black":"E:\\logger\\wsblack.json",
  "Log":"E:\\logger\\server-repeater.log"
}
````

| "ID" | ID for this WebSocket Server / Repeater | STRING |  |
| "Port" | shared net-port for this WebSocket Server / Repeater | NUMBER |  |
| "ConnMax" | maximum number of connections (ws-clients) for this server | NUMBER | 0...250 |
| "ConnPerCli" | maximum number of connections for one ws-client | NUMBER | 0...250 |
| "ConnLifeTime" | connection duration, sec | NUMBER | -1 - unlimit, 0...65000 |
| "UseLog" | permission to write log-file | NUMBER | 0 - off, 1 - on |
| "WsRepeater" | URL of other WebSocket Server / Repeater B | STRING | ws://IP-or-HostName:NetPort |
| "Black" | path to black-list of ws-clients | STRING |  |
| "Log" | path to log-file | STRING |  |


wsblack.json
```
[ "192.168.1.77:12345",
  "127.0.0.1:6789"
]
