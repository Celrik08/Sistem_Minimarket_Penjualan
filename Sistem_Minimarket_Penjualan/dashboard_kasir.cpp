void lihat_laporan_transaksi_kasir(Laporan_Transaksi_Kasir *laporan_kasir, int &jumlah_laporan_kasir){

    load_laporan_transaksi_kasir(laporan_kasir, jumlah_laporan_kasir);
    
    string format_harga(int harga);

    if(jumlah_laporan_kasir == 0){

        cout << "Data laporan transaksi kasir belum ada!" << endl;
        return;
    }

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
}

void laporan_transaksi_kasir () {

    lihat_laporan_transaksi_kasir(laporan_kasir, jumlah_laporan_kasir);

    if(jumlah_laporan_kasir == 0){

        cout << "\nTekan Enter untuk kembali...";
        cin.get();
        return;
    }

    vector<string> daftarTransaksi;

    for(int i = 0; i < jumlah_laporan_kasir; i++){

        daftarTransaksi.push_back(laporan_kasir[i].id_transaksi);
    }

    int pilih = pilihMenu(daftarTransaksi, "PILIH ID TRANSAKSI YANG INGIN DIKONFIRMASI", true);

    cout << "DETAIL LAPORAN TRANSAKSI\n\n";

    cout << "ID Transaksi   : " << laporan_kasir[pilih].id_transaksi << endl;

    cout << "Tanggal        : "
         << laporan_kasir[pilih].tanggal_transaksi.hari << "-"
         << laporan_kasir[pilih].tanggal_transaksi.bulan << "-"
         << laporan_kasir[pilih].tanggal_transaksi.tahun << endl;

    cout << "Nama Pembeli   : " << laporan_kasir[pilih].nama_pembeli << endl;

    cout << "Total Belanja (Rp) : " << format_harga(laporan_kasir[pilih].Total_belanja) << endl;

    char konfirmasi;

    while(true){

        cout << "Apakah anda yakin ingin mengonfirmasi transaksi ini? (y/n): ";
        cin >> konfirmasi;

        if(konfirmasi == 'n' || konfirmasi == 'N'){

            cin.ignore();

            cout << "Pengonfirmasi transaksi dibatalkan!" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();

            return;
        }

        else if(konfirmasi == 'y' || konfirmasi == 'Y'){

            break;
        }

        else{

            cin.ignore();

            cout << "Input konfirmasi tidak valid!" << endl;
        }
    }

    ifstream infile("Database/laporan_transaksi.json");

    vector<string> data_lama;

    int id_terakhir = 0;
    string isi_lama = "";
    string baris;

    if(infile.is_open()){

        while(getline(infile, baris)){

            isi_lama += baris + "\n";

            if(baris.find("\"id_laporan\"") != string::npos){

                int pos = baris.find(":") + 1;

                id_terakhir = stoi(baris.substr(pos));
            }
        }

        infile.close();
    }

    string cek_isi = isi_lama;

    cek_isi.erase(remove(cek_isi.begin(), cek_isi.end(), ' '), cek_isi.end());

    cek_isi.erase(remove(cek_isi.begin(), cek_isi.end(), '\n'), cek_isi.end());

    ofstream simpan("Database/laporan_transaksi.json");

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int hari = ltm->tm_mday;
    int bulan = 1 + ltm->tm_mon;
    int tahun = 1900 + ltm->tm_year;

    if(isi_lama.empty() || cek_isi == "[]"){

        simpan << "[" << endl;
    }

    else{

        while(!isi_lama.empty() &&
            (isi_lama.back() == '\n' ||
            isi_lama.back() == ' ')){

            isi_lama.pop_back();
        }

        if(!isi_lama.empty() &&
            isi_lama.back() == ']'){

            isi_lama.pop_back();
        }

        simpan << isi_lama;

        simpan << "," << endl;
    }

    simpan << "    {" << endl;
    simpan << "        \"id_laporan\": " << id_terakhir + 1 << "," << endl;
    simpan << "        \"id_transaksi\": \"" << laporan_kasir[pilih].id_transaksi << "\"," << endl;
    simpan << "        \"tanggal_transaksi\": \"" << hari << "-" << bulan << "-" << tahun << "\"," << endl;
    simpan << "        \"nama_kasir\": \"" << username_login << "\"," << endl;
    simpan << "        \"nama_pembeli\": \"" << laporan_kasir[pilih].nama_pembeli << "\"," << endl;
    simpan << "        \"total_pemasukan \": " << laporan_kasir[pilih].Total_belanja << endl;
    simpan << "    }" << endl;

    simpan << "]";

    simpan.close();
        
    ofstream hapus("Database/laporan_transaksi_kasir.json");

    hapus << "[\n";

    bool pertama = true;

    for(int i = 0; i < jumlah_laporan_kasir; i++){

        if(i != pilih){

            if(!pertama){
                hapus << ",\n";
            }

            hapus << "    {\n";

            hapus << "        \"id_transaksi\": \"" 
                        << laporan_kasir[i].id_transaksi << "\",\n";

            hapus << "        \"tanggal_transaksi\": \""
                    << laporan_kasir[i].tanggal_transaksi.hari << "-"
                    << laporan_kasir[i].tanggal_transaksi.bulan << "-"
                    << laporan_kasir[i].tanggal_transaksi.tahun << "\",\n";

            hapus << "        \"nama_pembeli\": \"" 
                    << laporan_kasir[i].nama_pembeli << "\",\n";

            hapus << "        \"total_belanja\": " 
                    << laporan_kasir[i].Total_belanja << "\n";

            hapus << "    }";

            pertama = false;
        }
    }

    hapus << "\n]";

    hapus.close();

    cout << "\nLaporan transaksi berhasil dikonfirmasi!" << endl;

    cin.ignore();
    cout << "\nTekan Enter untuk kembali...";
    cin.get();
}

void menu_kasir(Barang *barang, int &jumlah_barang, User *users, int &jumlah_user){

    load_laporan_transaksi_kasir(laporan_kasir, jumlah_laporan_kasir);

    system("cls");
    int posisi;

    while(true){
        vector<string> daftarMenu = {
            "Menu Lihat Barang",
            "Menu Laporan Transaksi Kasir",
            "Keluar"
        };

        posisi = pilihMenu(daftarMenu, "SELEMAT DATANG DI MINIMARKET MUHAMMAD FAJAR (MENU KASIR)") + 1;

        if(posisi == 1){
            lihat_barang(barang,jumlah_barang);
            cout << "\nTekan Enter untuk kembali...";
            cin.get();
        }
        else if(posisi == 2){
            laporan_transaksi_kasir();
        }
        else if(posisi == 3){
            cout<<"Anda Keluar dari MENU MINIMARKET MUHAMMAD FAJAR\n";
            cout << "\nTekan Enter untuk kembali...";
            cin.get();
            break;
        }
    }

}