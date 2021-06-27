package com.polsri.keamananlab;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import java.util.ArrayList;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;

public class Node2 extends AppCompatActivity {

    RecyclerView recyclerView;
    DatabaseReference database;
    DataAdapter myAdapter;
    ArrayList<DataModel> list;
    Query query;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_node2);

        recyclerView = findViewById(R.id.recycler_view);
        database = FirebaseDatabase.getInstance().getReference("/");
        query = database.orderByChild("node").equalTo("2");

        recyclerView.setHasFixedSize(true);
        recyclerView.setLayoutManager(new LinearLayoutManager(Node2.this));

        list = new ArrayList<>();
        myAdapter = new DataAdapter(Node2.this,list);
        recyclerView.setAdapter(myAdapter);

        query.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                list.clear();
                for (DataSnapshot dataSnapshot : snapshot.getChildren()){
                    DataModel user = dataSnapshot.getValue(DataModel.class);
                    list.add(user);
                }
                myAdapter.notifyDataSetChanged();
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(getBaseContext(), "Data Canceled" , Toast.LENGTH_SHORT ).show();
            }
        });


    }
    public void CameraBtn2(View view) {
        Intent intent = new Intent(Node2.this, CameraImageView2.class);
        startActivity(intent);
    }

}