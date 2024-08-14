package com.polsri.keamananlab;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import java.util.List;

public class DataAdapter extends RecyclerView.Adapter<DataAdapter.ViewHolder> {
    Context context;
    List<DataModel> data_list;

    public DataAdapter(Context context, List<DataModel> data_list) {
        this.context = context;
        this.data_list = data_list;
    }

    @NonNull
    @Override
    public DataAdapter.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
         View view = LayoutInflater.from(context).inflate(R.layout.data_layout, parent, false);
         return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        if (data_list != null & data_list.size() > 0) {
            DataModel model = data_list.get(position);
            holder.tanggal_tv.setText(model.getTanggal());
            holder.waktu_tv.setText(model.getWaktu());
            holder.keterangan_tv.setText((model.getKeterangan()));
        } else {
            return;
        }

    }

    @Override
    public int getItemCount() {
        return data_list.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        TextView tanggal_tv, waktu_tv, keterangan_tv;
        public ViewHolder(@NonNull View itemView){
            super(itemView);
            tanggal_tv = itemView.findViewById(R.id.tanggal_tv);
            waktu_tv = itemView.findViewById(R.id.waktu_tv);
            keterangan_tv = itemView.findViewById(R.id.keterangan_tv);

        }
    }
}
