package com.polsri.keamananlab;

public class DataModel {
    String tanggal, waktu, keterangan, node;

    public String getTanggal() {
        return tanggal;
    }

    public String getWaktu() {
        return waktu;
    }

    public String getKeterangan() {
        if (keterangan.equals("1")) {
            return "Ada Api Terdeteksi";
        } else {
            return  "Ada Gerakan Terdeteksi";
        }
    }

    public String getNode() {
        return node;
    }
}
