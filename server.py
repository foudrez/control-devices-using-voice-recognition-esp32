#export library
from flask import Flask, render_template, request, jsonify, redirect, url_for, session
import requests
from urllib.parse import urlencode
import os


local_audio_path = 'audio/Mot Nha.mp3'

app = Flask(__name__)

# ESP32 Configuration
ESP32_IP = "192.168.xx.xx"  #type your esp32's IP
ESP32_PORT = 80 #default port for esp32            


# Devices and commands
devices = {'light':['light','lights',"đèn","led"],
           'buzzer':['buzzer','còi'],
           'servo':['servo','sơ vô','sơ vồ',],
           }




commands = {
    'on':['on','bật'],
    'off':['off','tắt'],
    'play':['play','chơi','phát'],
    'rotate':['quay','rotate'],
    'blink':['nháy','blink'],
    }




#webpage
@app.route('/')
def index():
    print("Rendering the main page.")
    return render_template('index.html')

@app.route('/process-speech', methods=['POST'])
def process_speech():
    """
    Process speech command with translation and flexible command recognition.
    """
    print("Processing speech command...")
    speech_text = request.json.get('speech', '')
    print(f"Speech text received: {speech_text}")



    # Check for commands and devices in the translated text
    matched_command = None
    matched_device = None
    translated=speech_text.lower()
    #command
    for command in commands.values():
        for x in command:
            if x in translated:
                com=list(commands.keys())[list(commands.values()).index(command)]
                matched_command = com.upper() # Convert to command format ("ON", "OFF")
                print(matched_command)
                break
    #device
    for device in devices.values():
        for x in device:
            if x in translated:
                dev=list(devices.keys())[list(devices.values()).index(device)]
                matched_device = dev # Convert to command format ("ON", "OFF")
                print(matched_device)
                break

    if matched_command=='ROTATE' and matched_device=='servo':
        import re
        # Extract the number (angle) from the speech text
        match = re.search(r'(\d+)', speech_text)
        if match:
            degree = int(match.group(1))
            if 0 <= degree <= 180:  # Validate the angle range
                return send_command_to_esp32(f"SERVO_{degree}")
            else:
                return jsonify({"status": "error", "message": "Servo angle must be between 0 and 180."}), 400
        else:
            return jsonify({"status": "error", "message": "No valid servo angle found in speech."}), 400

    elif matched_command=='BLINK' :
        import re
        # Extract the number (angle) from the speech text
        match = re.search(r'(\d+)', speech_text)
        if match:
            times = int(match.group(1))
            if times >0 :
                return send_command_to_esp32(f"lightblink_{times}")
            else:
                return jsonify({"status": "error", "message": "blink times must >=1 !"}), 400
        else:
            return send_command_to_esp32(f"lightblink_{1}")

    elif matched_command=='ON' and matched_device=='light':
        import re
        # Extract the number (angle) from the speech text
        match = re.search(r'(\d+)', speech_text)
        if match:
            times = int(match.group(1))
            if times >0 :
                return send_command_to_esp32(f"ONlight{times}")
            else:
                return jsonify({"status": "error", "message": "light index must >=1 !"}), 400
        else:
            return send_command_to_esp32(f"ONlight")

    elif matched_command=='OFF' and matched_device=='light':
        import re
        # Extract the number (angle) from the speech text
        match = re.search(r'(\d+)', speech_text)
        if match:
            times = int(match.group(1))
            if times >0 :
                return send_command_to_esp32(f"OFFlight{times}")
            else:
                return jsonify({"status": "error", "message": "light index must >=1 !"}), 400
        else:
            return send_command_to_esp32(f"OFFlight")


    elif matched_command and matched_device:
        print(f"Matched command: {matched_command}, Matched device: {matched_device}")
        return send_command_to_esp32(matched_command+matched_device)
    else:
        print("No valid command or device found.")
        return jsonify({"status": "error", "message": "Unknown command"}), 400
#button control read
@app.route('/manual-control', methods=['POST'])
def manual_control():
    command = request.json.get('command', '')
    if command.startswith("SERVO_"):
        try:
            degree = int(command.split("_")[1])
            if 0 <= degree <= 180:
                return send_command_to_esp32(command)
            else:
                return jsonify({"status": "error", "message": "Degree out of range (0-180)"}), 400
        except ValueError:
            return jsonify({"status": "error", "message": "Invalid servo command"}), 400
    elif command not in ["ON", "OFF"]:
        return jsonify({"status": "error", "message": "Invalid command"}), 400
    return send_command_to_esp32(command + "light")
#send command to esp32
def send_command_to_esp32(command, audio_url=None):
    """Send command to ESP32."""
    print(f"Sending command to ESP32: {command}")
    try:
        url = f"http://{ESP32_IP}:{ESP32_PORT}/control"
        data = {"command": command}
        if audio_url:
            data["audio_url"] = audio_url
        response = requests.post(url, json=data)
        if response.status_code == 200:
            print(f"Command {command} successfully sent to ESP32.")
            return response.json()
        else:
            print(f"Failed to send command to ESP32. Status code: {response.status_code}")
            return {"status": "error", "message": "Failed to send command to ESP32"}
    except Exception as e:
        print(f"Error sending command to ESP32: {str(e)}")
        return {"status": "error", "message": str(e)}


if __name__ == '__main__':
    print("Starting Flask server...")
    app.run(host='0.0.0.0', port=5000, debug=True,)
