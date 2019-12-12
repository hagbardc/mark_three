## Mark Three
The third iteration of our pancel control software.  This one is actually pretty cool.

For command line interaction with the platformui controls, read the documentation online.  

The tl/dr is:

To compile the code for one of our devices:
```
pio run
```

To get a list of attached devices
```
pio device list
```

To upload the project to a device:
```
pio run --target upload  --upload-port /dev/ttyACM1

To actually monitor one of our devices:
```
pio device monitor -b 19200 -p /dev/ttyACM1
```

