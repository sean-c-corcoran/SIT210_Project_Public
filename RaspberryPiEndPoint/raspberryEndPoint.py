import sys
import time
import mysql.connector
from sseclient import SSEClient
import requests
import threading
import board
import adafruit_dht
import urllib3

# Main Function


def main():
    # Using threading to complete two different functions in parallel
    # setup the event notification / database lookup function on one thread
    thread1 = threading.Thread(target=notificationModule)
    # setup the temp humidity publishing function on one thread
    thread2 = threading.Thread(target=tempHumdSensorModule)
    # start both threads
    thread1.start()
    thread2.start()

# function reads the temp and humidity from the DHT22 sensor and sends it to the thingspeak channel


def tempHumdSensorModule():

    http = urllib3.PoolManager()
    myAPI = "ThingSpeak Write API Key"

    # Initial the dht device, with data pin connected to:
    dhtDevice = adafruit_dht.DHT22(board.D4)
    # set the URL with the API key
    baseURL = 'https://api.thingspeak.com/update?api_key=%s' % myAPI
    # keep running and posting sensor data
    while True:
        # error handling - if anything goes wrong print error and keep going
        try:
            # Print the values to the serial port
            temperature_c = dhtDevice.temperature  # Read temp from sensor
            humidity = dhtDevice.humidity  # Read Humidity
            """ print(
                "Temp: {:.1f} C    Humidity: {}% ".format(
                    temperature_c, humidity
                )
            ) #print to console """
            r = http.request('GET', baseURL + "&field1=%s&field2=%s" %
                             (humidity, temperature_c))  # Send the data to thingspeak
            # print(r.status) #print web request status
            r.close()  # close the web connection

        except RuntimeError as error:
            # Errors happen fairly often, DHT's are hard to read, just keep going
            print(error.args[0])

        time.sleep(2.0)  # sleep for 2 seconds before taking another reading

# Listen and act on events from the Particle Cloud


def notificationModule():
    # listen for message event at the following url from the particle cloud
    messages = SSEClient(
        'https://api.particle.io/v1/devices/events/WaterLow?access_token=Your Particle Access Token')

    # Keep listening and acting on events
    while True:
        # error handling - if anything goes wrong print error and keep going
        try:
            # for each message that comes in extact the device ID from the event data
            for msg in messages:
                print(msg)
                # convert the message to a string
                msgString = str(msg)
                # find the message length
                msgStrLength = len(msgString)

                # if there is a event message then continue
                if msgStrLength > 1:
                    # split up the data (the device ID sits in the 14th split, the data in the 4th)
                    messageList = msgString.split('"')
                    # look up the device ID in the database and see if it has a friendly name to send the notifcation with the waterlevel
                    DBlookUpandPost(messageList[13], messageList[3])
        except RuntimeError as error:
            print(error.args[0])

# setup connection to the database


def connectToDB():
    mySQLconnectionDetails = mysql.connector.connect(
        host="localhost",
        user="user",
        passwd="password",
        database="myTestDB"
    )
    # Output the connection object
    return mySQLconnectionDetails

# function that takes in the passed deviceID to lookup the friendly name in the database
# To which is then sent to the ifttt platform as a value with the waterlevel if no friendly name is found then send the device ID


def DBlookUpandPost(deviceNo, waterLevel):

    mydb = connectToDB()  # connect to the database
    mycursor = mydb.cursor()
    # find the friendly name if exists in the database given the device ID
    sql = ("SELECT friendlyName FROM devicesTest WHERE deviceID =" +
           "'" + deviceNo + "'")
    # execute the given query on the DB
    mycursor.execute(sql)

    # set the result to the returned data
    myresult = mycursor.fetchall()

    # if there isn't a result from the query then push the device id with a message
    if(len(myresult) == 0):  # To find the length of the dictionary and see if it contains data
        # send a webhook message to the ifttt platform stating there is an unknown device
        requests.post('https://maker.ifttt.com/trigger/water_low/with/key/your IFTTT webhook key', params={
                      "value1": "unnamed plant with device ID: " + deviceNo, "value2": waterLevel, "value3": "none"})
    # Push the friendly name to the ifttt
    else:
        # for each result in the list of results find the friendly name given
        for x in myresult:
            # get the name from the first object in the list
            friendlyName = x[0]
            # push the friendly name to the ifttt service
            requests.post('https://maker.ifttt.com/trigger/water_low/with/key/your IFTTT webhook key',
                          params={"value1": friendlyName, "value2": waterLevel, "value3": "none"})

    # close the connection to the database
    mydb.close()


# Start Main function
if __name__ == "__main__":
    # Wait for system boot to finish (used when installed in crontab)
    time.sleep(30.0)
    main()
