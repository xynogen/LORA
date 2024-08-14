from flask import Flask
from flask_restful import Api, Resource, reqparse
from firebase_admin import credentials, db, initialize_app, delete_app, storage
import requests
import time
import json 
from datetime import datetime


flask_app = Flask(__name__)
api = Api(flask_app)

class firebase_api:
    def __init__(self, tanggal:str, waktu:str, keterangan:str, node:str):
        self.tanggal = tanggal
        self.waktu = waktu
        self.keterangan = keterangan
        self.node = node

    def addData(self):
        # untuk keterangan 0 untuk orang dan 1 untuk api 
        # untuk node itu 1 dan 2
        new_data = {"tanggal":self.tanggal, "waktu":self.waktu, "keterangan":self.keterangan, "node":self.node}

        with open("./DataDB.json",'r+') as file:
            file_data = json.load(file)
            file_data.append(new_data)
            file.seek(0)
            json.dump(file_data, file, indent = 4)

    def sendData(self):
        self.cred = credentials.Certificate("./polsrilora-firebase-adminsdk.json")
        self.app = initialize_app(self.cred, {
            "storageBucket": "polsrilora.appspot.com"
        })
        ref = db.reference("/", self.app, "https://polsrilora-default-rtdb.asia-southeast1.firebasedatabase.app/")
        
        with open("./DataDB.json", "r") as infile:    
            mydata = json.load(infile)

        # upload data
        ref.set(mydata) 
        pass

    def sendImage(self, Node :str):
        if Node == "1":
            req = requests.get("http://192.168.43.101/capture")
            time.sleep(6)
            req = requests.get("http://192.168.43.101/saved-photo")
            if req.status_code == 200:
                with open(f"CaptureNode{Node}.jpeg", "wb") as file:
                    file.write(req.content)
            del req
        elif Node == "2":
            req = requests.get("http://192.168.43.102/capture")
            time.sleep(6)
            req = requests.get("http://192.168.43.102/saved-photo")
            if req.status_code == 200:
                with open(f"CaptureNode{Node}.jpeg", "wb") as file:
                    file.write(req.content)
            del req

        bucket = storage.bucket(app=self.app)
        blob = bucket.blob(f"Node{Node}/CapturNode{Node}.jpg")
        fileUpload=f"CaptureNode{Node}.jpeg"
        blob.upload_from_filename(fileUpload)
        delete_app(self.app)

        pass

class MyAPI(Resource):
    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument("keterangan")
        parser.add_argument("node")
        params = parser.parse_args()

        now = datetime.now()
        tanggal = now.strftime("%d/%m/%Y")
        waktu = now.strftime("%H:%M:%S")

        fire = firebase_api(tanggal, waktu, params["keterangan"], params["node"])
        fire.addData()
        fire.sendData()
        fire.sendImage(params["node"])

        return 201

api.add_resource(MyAPI, "/data/")

    
if __name__ == "__main__":
   flask_app.run(host="0.0.0.0",debug=True)