import paho.mqtt.client as mqtt

# MQTT settings
BROKER = "broker.emqx.io"
TOPIC = "esp8266/chats"

# Callback when a message is received
def on_message(client, userdata, message):
    print(f"📩 Received message: {message.payload.decode()} on topic {message.topic}")

# Setup client
client = mqtt.Client()
client.on_message = on_message

# Connect and subscribe
client.connect(BROKER, 1883, 60)
client.subscribe(TOPIC)

# Keep listening
print("🔍 Listening for messages...")
client.loop_forever()