#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

struct User{
    int id_user;
    string username;
    string password;
    string role;
};

struct Jenis_Barang{
    string kategori;
};

struct Barang{
    string id_barang;
    string nama_barang;
    int harga_barang;
    int stok;
    Jenis_Barang jenis_barang;
};

struct Date{
    int hari;
    int bulan;
    int tahun;
};

struct Laporan_Transaksi{
    int id_laporan;
    string id_transaksi;
    Date tanggal_transaksi;
    string nama_kasir;
    string nama_pembeli;
    int Total_belanja;
};

struct Laporan_Transaksi_Kasir{
    string id_transaksi;
    Date tanggal_transaksi;
    string nama_pembeli;
    int Total_belanja;
};

struct Detail_Transaksi{
    int id_detail_transaksi;
    string id_transaksi;
    string id_barang;
    string nama_barang;
    int harga_barang;
    int jumlah_beli;
    int subtotal;
};

User users[100];
Barang barang[100];
Laporan_Transaksi laporan_admin[100];
Laporan_Transaksi_Kasir laporan_kasir[100];
Detail_Transaksi detail_transaksi[100];

int jumlah_barang = 0;
int jumlah_user = 0;
int jumlah_laporan_admin = 0;
int jumlah_laporan_kasir = 0;
int jumlah_detail_transaksi = 0;
string username_login = "";

int pilihMenu(const vector<string>& menu, string judul, bool tampil_laporan_kasir = false, bool tampil_detail_transaksi = false){
    int posisi = 0;
    char tombol;
    string format_harga(int harga);

    while(true){
        system("cls");

        if(tampil_laporan_kasir){

            cout << "\nDATA LAPORAN TRANSAKSI KASIR\n";
            cout << "-----------------------------------------------------------------\n";

            cout << left
                << setw(20) << "ID Transaksi"
                << setw(20) << "Tanggal"
                << setw(20) << "Nama Pembeli"
                << setw(20) << "Total Belanja (Rp)" << endl;

            cout << "-----------------------------------------------------------------\n";

            for(int i = 0; i < jumlah_laporan_kasir; i++){

            string tanggal =
                to_string(laporan_kasir[i].tanggal_transaksi.hari) + "-" +
                to_string(laporan_kasir[i].tanggal_transaksi.bulan) + "-" +
                to_string(laporan_kasir[i].tanggal_transaksi.tahun);

            cout << left
                << setw(20) << laporan_kasir[i].id_transaksi
                << setw(20) << tanggal
                << setw(20) << laporan_kasir[i].nama_pembeli
                << setw(20) << format_harga(laporan_kasir[i].Total_belanja)
                << endl;
            }

            cout << endl;
        }

        if(tampil_detail_transaksi){

            cout << "\nDETAIL TRANSAKSI PEMBELI\n";

            cout << "---------------------------------------------------------------------------------------------------\n";

            cout << left
                 << setw(10) << "ID Detail"
                 << setw(20) << "ID Transaksi"
                 << setw(15) << "ID Barang"
                 << setw(20) << "Nama Barang"
                 << setw(15) << "Harga (Rp)"
                 << setw(15) << "Jumlah"
                 << setw(15) << "Subtotal (Rp)" << endl;

            cout << "---------------------------------------------------------------------------------------------------\n";

            int total_belanja_tampil = 0;

            for(int i = 0; i < jumlah_detail_transaksi; i++){

                cout << left
                     << setw(10) << detail_transaksi[i].id_detail_transaksi
                     << setw(20) << detail_transaksi[i].id_transaksi
                     << setw(15) << detail_transaksi[i].id_barang
                     << setw(20) << detail_transaksi[i].nama_barang
                     << setw(15) << format_harga(detail_transaksi[i].harga_barang)
                     << setw(15) << detail_transaksi[i].jumlah_beli
                     << setw(15) << format_harga(detail_transaksi[i].subtotal)
                     << endl;

                total_belanja_tampil += detail_transaksi[i].subtotal;
            }

            cout << "---------------------------------------------------------------------------------------------------\n";

            cout << "Total Belanja : Rp."
                 << format_harga(total_belanja_tampil) << endl << endl;
        }

        cout << judul << endl;
        cout << "Gunakan tombol panah atas (untuk ke atas) dan panah bawah (untuk ke bawah) lalu ENTER untuk masuk ke menu\n\n";

        for(int i = 0; i < menu.size(); i++){
            if(i == posisi){
                cout << ">> " << menu[i] << endl;
            } else {
                cout << "   " << menu[i] << endl;
            }
        }

        tombol = _getch();

        if(tombol == 72){
            posisi--;
            if(posisi < 0) posisi = menu.size() - 1;
        }
        else if(tombol == 80){
            posisi++;
            if(posisi >= menu.size()) posisi = 0;
        }
        else if(tombol == 13){
            return posisi;
        }
    }
}

int total_stok(Barang *barang, int index){
    if(index < 0){
        return 0;
    }
    return (barang+index)->stok + total_stok(barang,index-1);
}

void Output(string pesan){
    cout<<pesan<<endl;
}

void Output(string pesan, int nilai){
    cout<<pesan<<nilai<<endl;
}

void load_user(User *users, int &jumlah_user){
    ifstream file("Database/user.json");

    jumlah_user = 0;

    if(file.is_open()){
        string baris;
        int idx = 0;

        while(getline(file, baris)){

            if(baris.find("\"id_user\"") != string::npos){
                int pos = baris.find(":") + 1;
                users[idx].id_user = stoi(baris.substr(pos));
            }

            if(baris.find("\"username\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                users[idx].username = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"password\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                users[idx].password = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"role\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                users[idx].role = baris.substr(pos1, pos2 - pos1);

                idx++;
            }
        }

        jumlah_user = idx;
        file.close();
    }
}

void load_barang(Barang *barang, int &jumlah_barang){
    ifstream file("Database/barang.json");

    jumlah_barang = 0;

    if(file.is_open()){
        string baris;
        int idx = 0;

        while(getline(file, baris)){

            if(baris.find("\"id_barang\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                barang[idx].id_barang = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"nama_barang\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                barang[idx].nama_barang = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"kategori\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                barang[idx].jenis_barang.kategori = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"harga\"") != string::npos){
                int pos = baris.find(":") + 1;
                barang[idx].harga_barang = stoi(baris.substr(pos));
            }

            if(baris.find("\"stok\"") != string::npos){
                int pos = baris.find(":") + 1;
                barang[idx].stok = stoi(baris.substr(pos));
                idx++;
            }
        }

        jumlah_barang = idx;
        file.close();
    }
}

void load_laporan_transaksi(Laporan_Transaksi *laporan_admin, int &jumlah_laporan_admin){

    ifstream file("Database/laporan_transaksi.json");

    jumlah_laporan_admin = 0;

    if(file.is_open()){

        string baris;
        int idx = 0;

        while(getline(file, baris)){

            if(baris.find("\"id_laporan\"") != string::npos){

                int pos = baris.find(":") + 1;

                laporan_admin[idx].id_laporan = stoi(baris.substr(pos));
            }

            if(baris.find("\"id_transaksi\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                laporan_admin[idx].id_transaksi = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"tanggal_transaksi\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                string tanggal = baris.substr(pos1, pos2 - pos1);

                int pos_strip1 = tanggal.find("-");
                int pos_strip2 = tanggal.find("-", pos_strip1 + 1);

                laporan_admin[idx].tanggal_transaksi.hari =
                    stoi(tanggal.substr(0, pos_strip1));

                laporan_admin[idx].tanggal_transaksi.bulan =
                    stoi(tanggal.substr(pos_strip1 + 1,
                    pos_strip2 - pos_strip1 - 1));

                laporan_admin[idx].tanggal_transaksi.tahun =
                    stoi(tanggal.substr(pos_strip2 + 1));
            }

            if(baris.find("\"nama_kasir\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                laporan_admin[idx].nama_kasir = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"nama_pembeli\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                laporan_admin[idx].nama_pembeli = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"total_pemasukan\"") != string::npos){

                int pos = baris.find(":") + 1;

                laporan_admin[idx].Total_belanja = stoi(baris.substr(pos));

                idx++;
            }
        }

        jumlah_laporan_admin = idx;

        file.close();
    }
}

void load_laporan_transaksi_kasir(Laporan_Transaksi_Kasir *laporan_kasir, int &jumlah_laporan_kasir){

    ifstream file("Database/laporan_transaksi_kasir.json");

    jumlah_laporan_kasir = 0;

    if(file.is_open()){

        string baris;
        int idx = 0;

        while(getline(file, baris)){

            if(baris.find("\"id_transaksi\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                laporan_kasir[idx].id_transaksi = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"tanggal_transaksi\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                string tanggal = baris.substr(pos1, pos2 - pos1);

                int pos_strip1 = tanggal.find("-");
                int pos_strip2 = tanggal.find("-", pos_strip1 + 1);

                laporan_kasir[idx].tanggal_transaksi.hari =
                    stoi(tanggal.substr(0, pos_strip1));

                laporan_kasir[idx].tanggal_transaksi.bulan =
                    stoi(tanggal.substr(pos_strip1 + 1,
                    pos_strip2 - pos_strip1 - 1));

                laporan_kasir[idx].tanggal_transaksi.tahun =
                    stoi(tanggal.substr(pos_strip2 + 1));
            }

            if(baris.find("\"nama_pembeli\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                laporan_kasir[idx].nama_pembeli = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"total_belanja\"") != string::npos){

                int pos = baris.find(":") + 1;

                laporan_kasir[idx].Total_belanja = stoi(baris.substr(pos));

                idx++;
            }
        }

        jumlah_laporan_kasir = idx;

        file.close();
    }
}

void load_detail_transaksi(Detail_Transaksi *detail_transaksi, int &jumlah_detail_transaksi){

    ifstream file("Database/detail_transaksi.json");

    jumlah_detail_transaksi = 0;

    if(file.is_open()){

        string baris;
        int idx = 0;

        while(getline(file, baris)){

            if(baris.find("\"id_transaksi\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                detail_transaksi[idx].id_transaksi =
                    baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"id_barang\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                detail_transaksi[idx].id_barang =
                    baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"nama_barang\"") != string::npos){

                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);

                detail_transaksi[idx].nama_barang =
                    baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"harga_barang\"") != string::npos){

                int pos = baris.find(":") + 1;

                detail_transaksi[idx].harga_barang =
                    stoi(baris.substr(pos));
            }

            if(baris.find("\"jumlah_beli\"") != string::npos){

                int pos = baris.find(":") + 1;

                detail_transaksi[idx].jumlah_beli =
                    stoi(baris.substr(pos));
            }

            if(baris.find("\"subtotal\"") != string::npos){

                int pos = baris.find(":") + 1;

                detail_transaksi[idx].subtotal =
                    stoi(baris.substr(pos));

                idx++;
            }
        }

        jumlah_detail_transaksi = idx;

        file.close();
    }
}

bool cek_user(User *users, int &jumlah_user, string username, string password){

    ifstream file("Database/user.json");
    if(file.is_open()){
        string baris;

        string u = "", p = "";
        bool insideObject = false;

        while(getline(file, baris)){

            if(baris.find("{") != string::npos){
                insideObject = true;
                u = "";
                p = "";
            }

            if(insideObject && baris.find("\"username\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                u = baris.substr(pos1, pos2 - pos1);
            }

            if(insideObject && baris.find("\"password\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                p = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("}") != string::npos){
                if(username == u && password == p){
                    file.close();
                    return true;
                }
                insideObject = false;
            }
        }

        file.close();
    }

    for(int i=0;i<jumlah_user;i++){
        if(username == (users+i)->username && password == (users+i)->password){
            return true;
        }
    }

    return false;
}

void User_json(User user){
    ifstream infile("Database/user.json");
    string isi_lama = "";
    string baris;

    if(infile.is_open()){
        while(getline(infile, baris)){
            isi_lama += baris + "\n";
        }
        infile.close();
    }

    ofstream file("Database/user.json");

    if(file.is_open()){

        if(isi_lama.empty()){
            file << "[\n";
            file << "   {\n";
            file << "        \"id_user\": " << user.id_user << ",\n";
            file << "        \"username\": \"" << user.username << "\",\n";
            file << "        \"password\": \"" << user.password << "\",\n";
            file << "        \"role\": \"" << user.role << "\"\n";
            file << "   }\n";
            file << "]\n";
        }
        else{

            while(!isi_lama.empty() && (isi_lama.back() == '\n' || isi_lama.back() == ' ')){
                isi_lama.pop_back();
            }
            if(!isi_lama.empty() && isi_lama.back() == ']'){
                isi_lama.pop_back();
            }

            file << isi_lama << ",\n";

            file << "   {\n";
            file << "        \"id_user\": " << user.id_user << ",\n";
            file << "        \"username\": \"" << user.username << "\",\n";
            file << "        \"password\": \"" << user.password << "\",\n";
            file << "        \"role\": \"" << user.role << "\"\n";
            file << "   }\n";
            file << "]\n";
        }

        file.close();
    } else {
        cout<<"Gagal membuka file user.json\n";
    }
}

void Barang_json(Barang barang){
    ifstream infile("Database/barang.json");
    string isi_lama = "";
    string baris;

    if(infile.is_open()){
        while(getline(infile, baris)){
            isi_lama += baris + "\n";
        }
        infile.close();
    }

    string cek_isi = isi_lama;

    cek_isi.erase(remove(cek_isi.begin(), cek_isi.end(), ' '), cek_isi.end());
    cek_isi.erase(remove(cek_isi.begin(), cek_isi.end(), '\n'), cek_isi.end());

    ofstream file("Database/barang.json");

    if(file.is_open()){

        if(isi_lama.empty() || cek_isi == "[]"){
            file << "[\n";
            file << "   {\n";
            file << "        \"id_barang\": \"" << barang.id_barang << "\",\n";
            file << "        \"nama_barang\": \"" << barang.nama_barang << "\",\n";
            file << "        \"kategori\": \"" << barang.jenis_barang.kategori << "\",\n";
            file << "        \"harga\": " << barang.harga_barang << ",\n";
            file << "        \"stok\": " << barang.stok << "\n";
            file << "   }\n";
            file << "]\n";
        }
        else{

            while(!isi_lama.empty() && (isi_lama.back() == '\n' || isi_lama.back() == ' ')){
                isi_lama.pop_back();
            }
            if(!isi_lama.empty() && isi_lama.back() == ']'){
                isi_lama.pop_back();
            }

            file << isi_lama << ",\n";

            file << "   {\n";
            file << "        \"id_barang\": \"" << barang.id_barang << "\",\n";
            file << "        \"nama_barang\": \"" << barang.nama_barang << "\",\n";
            file << "        \"kategori\": \"" << barang.jenis_barang.kategori << "\",\n";
            file << "        \"harga\": " << barang.harga_barang << ",\n";
            file << "        \"stok\": " << barang.stok << "\n";
            file << "   }\n";
            file << "]\n";
        }

        file.close();
    } else {
        cout<<"Gagal membuka file barang.json\n";
    }
}

void register_user(User *users, int &jumlah_user){

    cout<<"\nMENU REGISTER PEMBELI\n";

    users[jumlah_user].id_user = jumlah_user + 1;
    users[jumlah_user].role = "pembeli";

    cout<<"Username : ";

    while(true){

        getline(cin, users[jumlah_user].username);

        if(users[jumlah_user].username.empty()){
            cout << "Error: Username tidak boleh kosong!" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        bool username_sama = false;

        for(int i = 0; i < jumlah_user; i++){

            if(users[i].username == users[jumlah_user].username){
                username_sama = true;
                break;
            }
        }

        if(username_sama){
            cout << "Error: Username sudah digunakan, silahkan input username lain!\n";
            cout << "Username : ";
        }
        else{
            break;
        }
    }

    cout<<"Password : ";
    getline(cin, users[jumlah_user].password);

    User_json(users[jumlah_user]);

    jumlah_user++;

    Output("Registrasi berhasil ditambahkan, silahkan anda login kembali");
    cout << "Tekan Enter untuk kembali ke menu...";
    cin.get();
}

string format_harga(int harga){

    string harga_str = to_string(harga);
    string hasil = "";
    int hitung = 0;

    for(int i = harga_str.length() - 1; i >= 0; i--){

        hasil = harga_str[i] + hasil;
        hitung++;

        if(hitung == 3 && i != 0){
            hasil = "." + hasil;
            hitung = 0;
        }
    }

    return hasil;
}

void lihat_barang(Barang *barang, int &jumlah_barang){

    ifstream file("Database/barang.json");

    if(file.is_open()){

        for(int i = 0; i < 100; i++){
            barang[i].id_barang = "";
            barang[i].nama_barang = "";
            barang[i].jenis_barang.kategori = "";
            barang[i].harga_barang = 0;
            barang[i].stok = 0;
        }

        string baris;
        int idx = 0;

        while(getline(file, baris)){

            if(baris.find("\"id_barang\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                barang[idx].id_barang = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"nama_barang\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                barang[idx].nama_barang = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"kategori\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                barang[idx].jenis_barang.kategori = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"harga\"") != string::npos){
                int pos = baris.find(":") + 1;
                barang[idx].harga_barang = stoi(baris.substr(pos));
            }

            if(baris.find("\"stok\"") != string::npos){
                int pos = baris.find(":") + 1;
                barang[idx].stok = stoi(baris.substr(pos));

                idx++;
            }
        }

        jumlah_barang = idx;

        file.close();
    }

    if(jumlah_barang == 0){
        cout << "Data barang belum diisi!" << endl;
        return;
    }

    cout<<"\nDATA BARANG\n";
    cout<<"-------------------------------------------------------------\n";
    cout<<left<<setw(10)<<"ID"
        <<setw(15)<<"Nama Barang"
        <<setw(15)<<"Jenis Barang"
        <<setw(15)<<"Harga (Rp)"
        <<setw(10)<<"Stok"<<endl;
    cout<<"-------------------------------------------------------------\n";

    for(int i=0;i<jumlah_barang;i++){
        cout<<left<<setw(10)<<(barang+i)->id_barang
            <<setw(15)<<(barang+i)->nama_barang
            <<setw(15)<<(barang+i)->jenis_barang.kategori
            <<setw(15)<<format_harga((barang+i)->harga_barang)
            <<setw(10)<<(barang+i)->stok<<endl;
    }

    Output("Total stok barang : ", total_stok(barang,jumlah_barang-1));
}

#include "dashboard_admin.cpp"
#include "dashboard_kasir.cpp"
#include "dashboard_pembeli.cpp"

void login(User *users, int &jumlah_user, Barang *barang, int &jumlah_barang){

    system("cls");
    int jumlah_login = 3;
    string username,password;
    bool status_login = false;

    cout<<"\nMENU LOGIN MINIMARKET MUHAMMAD FAJAR\n";

    while(jumlah_login > 0 && status_login == false){

        cout<<"Username : ";
        getline(cin,username);

        cout<<"Password : ";
        getline(cin,password);

        if(cek_user(users,jumlah_user,username,password)){

            status_login = true;
            username_login = username;

            for(int i = 0; i < jumlah_user; i++){

                if(users[i].username == username && users[i].password == password){

                    if(users[i].role == "admin"){

                        cout<<"Login berhasil, selamat datang admin\n";
                        cout << "\nTekan Enter untuk kembali ke menu...";
                        cin.get();

                        menu_admin(barang,jumlah_barang);
                    }

                    else if(users[i].role == "kasir"){

                        username_login = users[i].username;

                        cout<<"Login berhasil, selamat datang kasir\n";
                        cout << "\nTekan Enter untuk kembali ke menu...";
                        cin.get();

                        menu_kasir(barang,jumlah_barang,users,jumlah_user);
                    }

                    else if(users[i].role == "pembeli"){

                        cout<<"Login berhasil, selamat datang pembeli\n";
                        cout << "\nTekan Enter untuk kembali ke menu...";
                        cin.get();

                        menu_pembeli(barang,jumlah_barang,users,jumlah_user);
                    }

                    return;
                }
            }
        }

        jumlah_login--;

        cout << "Username atau password salah, kesempatan login tersisa : " << jumlah_login << endl;
    }

    if (status_login == false) {
        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.get();
        cout << "Username dan Password Anda salah sebanyak 3 kali. Kembali ke Menu MINIMARKET MUHAMMAD FAJAR" << endl;
    }
}

int main(){

    load_barang(barang, jumlah_barang);

    load_user(users, jumlah_user);

    int posisi;

    while(true){
        vector<string> menuAwal = {
            "Register",
            "Login",
            "Keluar"
        };

        posisi = pilihMenu(menuAwal, "SELEMAT DATANG DI MINIMARKET MUHAMMAD FAJAR") + 1;

        if(posisi == 1){
            register_user(users,jumlah_user);
        }
        else if(posisi == 2){
            login(users,jumlah_user,barang,jumlah_barang);
        }
        else if(posisi == 3){
            cout<<"Terima kasih telah menggunakan SISTEM MINIMARKET MUHAMMAD FAJAR\n";
            break;
        }
    }
}