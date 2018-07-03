# Temperature sensor server for Orange Pi (Lite)

This service reads temperature and humidity data from the popular *DHT22* (or DHT11) and *DS18B20* sensors connected to an OrangePi board. The measured data is then accessible as JSON string by querying the embedded HTTP server.

You should modify the source code of this server to suit your needs as the current configuration is tailored to my specific needs.

## Requirements

* libevhtp-dev
* libevent-dev
* wiringOP (https://github.com/zhaolei/WiringOP)

You should be install the libraries using apt-get eg. (`apt-get install libevhtp-dev`), check the link for wiringOP.

## Sensors

I'm using the DS18B20 for outdoor and the DHT22 for indoor measurements. If you are using only one sensor then you should modify `main.c` to suite your needs (json_response, update_thread, etc).

### DHT22

Connect the sensor to the board, in my case I used Pin 7 (PA6) on the GPIO header for the data line. Modify `DHT_PIN` in `dht22.c` if you are using a different port.

### DS18B20

Enable 1-wire support on the board using `armbian-config` if you are using Armbian. Connect the sensor to Pin 37 (PA20) on OrangePi Lite (check the corrent port for other variants). Don't forget to add `param_w1_pin=PA20` to `/boot/armbianEnv.txt`!
After reboot you should list the recognized 1-wire devices with `ls /sys/bus/w1/devices/` and update `w1_sensor_id` in `main.c`.

## Building

Use `make`. If it succeeds then the binary will be in the `.build` folder.

## Running

Run `temp-montor` as root (with sudo).

## Querying

Send an HTTP GET request to `http://IP:8012/sensor`. For example use curl:

> `curl http://127.0.0.1:8012/sensor`

If the server is working correctly then it should respond with a JSON resonse like this:

```javascript
{
  "sensors": {
    "dht22": {
      "temperature": 25.1,
      "humidity": 44.5
    },
    "ds18b20": {
      "temperature": 19.2
    }
  },
  "updated": "2018-07-03T23:08:40Z"
}
```

Values are in Celsius.

## License

MIT.
