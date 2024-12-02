import time
import serial
import requests

symbolLookup = {
    "clearsky": 2,
    "cloudy": 0,
    "fair": 2,
    "fog": 0,
    "heavyrain": 1,
    "heavyrainandthunder": 1,
    "heavyrainshowers": 1,
    "heavyrainshowersandthunder": 1,
    "heavysleet": 1,
    "heavysleetandthunder": 1,
    "heavysleetshowers": 1,
    "heavysleetshowersandthunder": 1,
    "heavysnow": 1,
    "heavysnowandthunder": 1,
    "heavysnowshowers": 1,
    "heavysnowshowersandthunder": 1,
    "lightrain": 1,
    "lightrainandthunder": 1,
    "lightrainshowers": 1,
    "lightrainshowersandthunder": 1,
    "lightsleet": 1,
    "lightsleetandthunder": 1,
    "lightsleetshowers": 1,
    "lightsnow": 1,
    "lightsnowandthunder": 1,
    "lightsnowshowers": 1,
    "lightssleetshowersandthunder": 1,
    "lightssnowshowersandthunder": 1,
    "partlycloudy": 0,
    "rain": 1,
    "rainandthunder": 1,
    "rainshowers": 1,
    "rainshowersandthunder": 1,
    "sleet": 1,
    "sleetandthunder": 1,
    "sleetshowers": 1,
    "sleetshowersandthunder": 1,
    "snow": 1,
    "snowandthunder": 1,
    "snowshowers": 1,
    "snowshowersandthunder": 1,
}

place_index = 1
places = [
    {
        "key": 0,
        "url": "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=35.70&lon=139.70",
    },
    {
        "key": 1,
        "url": "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=34.68&lon=135.52",
    },
    {
        "key": 2,
        "url": "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=43.06&lon=141.34",
    },
    {
        "key": 3,
        "url": "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=26.35&lon=127.80",
    },
]

arduino = serial.Serial("/dev/ttyACM0", baudrate=9600)


time.sleep(2)


def updateLCD(place, temp, humid, symb):
    cmd = "p{}t{}h{}s{}\n".format(place, temp, humid, symb)
    print(cmd)
    arduino.write(bytes(cmd, "ascii"))


def updateWeather():
    resp = requests.get(
        places[place_index]["url"],
        headers={
            "User-Agent": "curl/8.2.1",
            "Accept": "application/json",
        },
    )
    print(resp)
    resp_dict = resp.json()

    data = resp_dict["properties"]["timeseries"][0]["data"]
    temp = data["instant"]["details"]["air_temperature"]
    hum = data["instant"]["details"]["relative_humidity"]
    symbol = data["next_1_hours"]["summary"]["symbol_code"].split("_")[0]
    symbol_code = symbolLookup[symbol]

    updateLCD(
        place=places[place_index]["key"],
        temp=int(temp),
        humid=int(hum),
        symb=symbol_code,
    )


updateWeather()

while True:
    line = arduino.readline()
    print(line)

    if line == b"up\r\n":
        place_index = (place_index + 1) % 4
        updateWeather()
    elif line == b"down\r\n":
        place_index = (place_index - 1) % 4
        updateWeather()
