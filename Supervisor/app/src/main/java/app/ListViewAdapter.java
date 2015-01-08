package app;

import java.util.ArrayList;
import java.util.HashMap;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.gcm.R;

/**
 * Created by Thomas on 06/01/2015.
 */
public class ListViewAdapter extends BaseAdapter {

    // Declare Variables
    Context _context;
    LayoutInflater inflater;
    ArrayList<HashMap<String, String>> data;
    HashMap<String, String> resultp = new HashMap<String, String>();

    public ListViewAdapter(Context context,ArrayList<HashMap<String, String>> arraylist) {
        _context = context;
        data = arraylist;
    }

    @Override
    public int getCount() {
        return data.size();
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    public View getView(final int position, View convertView, ViewGroup parent) {
        TextView _type;
        TextView _date;
        TextView _level;

        inflater = (LayoutInflater) _context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View itemView = inflater.inflate(R.layout.listview_item, parent, false);

        // Get the position
        resultp = data.get(position);

        // Locate the TextViews in listview_item.xml
        _type = (TextView) itemView.findViewById(R.id.type);
        _level = (TextView) itemView.findViewById(R.id.level);
        _date = (TextView) itemView.findViewById(R.id.date);

        //Alert level
        String _alert = resultp.get(Historique.LEVEL);

        Resources res = _context.getResources();

        if(_alert.equals("1")){
            _level.setText("Infos");
            _level.setTextColor(res.getColor(R.color.Jaune));
        }else if(_alert.equals("2")){
            _level.setText("Sérieux");
            _level.setTextColor(res.getColor(R.color.Orange));
        }else if(_alert.equals("3")){
            _level.setText("Urgent");
            _level.setTextColor(res.getColor(R.color.Rouge));
        }

        // Alert type
        String type = resultp.get(Historique.TYPE);
        if(type.equals("1")){
            _type.setText("Médicaments non pris");
        }else if(type.equals("2")){
            _type.setText("Chute de la personne.");
        }else if(type.equals("3")){
            _type.setText("Demande de l'utilisateur");
        }

        // Alert type
        String date = resultp.get(Historique.DATE);
        String an = date.substring(0, 4);
        String mois = date.substring(5, 7);
        String jour = date.substring(9, 10);

        if(mois.equals("01")){
            mois = "Janvier";
        }else if(mois.equals("02")){
            mois = "Fevrier";
        }else if(mois.equals("03")){
            mois = "Mars";
        }else if(mois.equals("04")){
            mois = "Avril";
        }else if(mois.equals("05")){
            mois = "Mai";
        }else if(mois.equals("06")){
            mois = "Juin";
        }else if(mois.equals("07")){
            mois = "Juillet";
        }else if(mois.equals("08")){
            mois = "Aout";
        }else if(mois.equals("09")){
            mois = "Septembre";
        }else if(mois.equals("10")){
            mois = "Octobre";
        }else if(mois.equals("11")){
            mois = "Novembre";
        }else if(mois.equals("12")){
            mois = "Decembre";
        }

        String hour = resultp.get(Historique.HOUR);
        String hou = hour.substring(0, 2);
        String minute = hour.substring(3, 5);

        _date.setText("Le "+ jour +" "+ mois +" "+an +" à "+hou+ "h"+minute );



        return itemView;
    }
}