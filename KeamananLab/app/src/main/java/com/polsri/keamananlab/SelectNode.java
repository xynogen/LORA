package com.polsri.keamananlab;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import java.util.NoSuchElementException;


public class SelectNode extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_node);
        setTitle(getString(R.string.title_name));
    }

    public void node1Btn(View view) {
        Intent intent = new Intent(SelectNode.this, Node1.class);
        startActivity(intent);
    }

    public void node2Btn(View view) {
        Intent intent = new Intent(SelectNode.this, Node2.class);
        startActivity(intent);
    }


}